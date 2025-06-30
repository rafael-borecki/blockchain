#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include "Conductor.h"
#include "Time.h"
#include "DataProducer.h"

Conductor::Conductor(std::string filename, int num_workers, uint32_t max_height)
  : filename_(filename),
  max_height_(max_height),
  num_workers_(num_workers),
  block_producer_("broker:29092", "mined-blocks")
{
  for (int i = 0; i < num_workers_; ++i) {
    workers_.emplace_back(std::to_string(i));
  }
}

void Conductor::init_genesis_block() {
  blockchain_.emplace_back(); // constructor for genesis
  blockchain_.back().finalizeHash();
  std::cout << "[Blockchain]: Genesis Block Initialized" << std::endl;
  blockchain_.back().debugBlock();
  saveBlockchain(filename_, blockchain_.back());
}

void Conductor::mining_loop() {
  while (blockchain_.back().height < max_height_) {
    std::cout << "\n[Conductor] Waiting for data for block "
      << blockchain_.back().height + 1 << std::endl;

    //  wait for data
    shared_state_.data_semaphore.acquire();
    std::string data;
    {
      std::lock_guard<std::mutex> lock(shared_state_.dataMutex);
      if (shared_state_.data_pool.empty()) continue; // should not happen with semaphore
      data = shared_state_.data_pool.front();
      shared_state_.data_pool.pop();
    }

    // create the single candidate block for this round
    Block candidate_block(
        blockchain_.back().height + 1,
        getTimestamp(),
        HASH_META,
        blockchain_.back().blockHash,
        data
        );
    std::cout << "[Conductor] Created candidate block. Starting mining... " << std::endl;

    // reset shared state for the new mining round
    shared_state_.nonce_found_flag.store(false);
    shared_state_.winning_nonce.reset();
    shared_state_.winning_worker_id.reset();

    // launch all worker threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_workers_; ++i) {
      threads.emplace_back(
          &Worker::findNonce,
          &workers_[i],
          std::cref(candidate_block),
          std::ref(shared_state_)
          );
    }

    // for all threads to complete
    for (auto& t : threads) {
      t.join();
    }

    // nonce found, finalize the block
    if (shared_state_.winning_nonce.has_value()) {
      std::cout << "[Conductor] Block #" << candidate_block.height << " mined by Worker #"
        << shared_state_.winning_worker_id.value() << std::endl;

      candidate_block.setNonce(shared_state_.winning_nonce.value());
      candidate_block.finalizeHash();
      blockchain_.push_back(candidate_block);
      Block mined_block = blockchain_.back();
      std::thread([=]() {
          mined_block.debugBlock();
          saveBlockchain(filename_, mined_block);
          block_producer_.send(mined_block.serialize());
          std::cout << "[Conductor] Block #" << mined_block.height << " persisted and sent.\n";
      }).detach();

    } else {
      std::cerr << "[Conductor] Mining round finished with no nonce found."<< std::endl;
    }
  }
}

void Conductor::run() {
  const int num_kafka_consumers = 4; // must match the number of partitions in the topic
  const std::string brokers = "broker:29092";
  const std::string group   = "blockminer-group";
  const std::string topic   = "raw-data";

  std::vector<std::thread> kafka_threads;
  kafka_threads.reserve(num_kafka_consumers);

  std::atomic<int> ready_count{0};

  for (int i = 0; i < num_kafka_consumers; ++i) {
    kafka_threads.emplace_back(
      [&, i] {
        kafkaConsumerToQueueThread(brokers, group, topic, shared_state_);
      }
    );
  }

  {
    std::unique_lock<std::mutex> lk(shared_state_.kafka_mutex);
    shared_state_.kafka_cv.wait(lk, [&]{
      return shared_state_.kafka_ready.load();
    });
  }
  std::cout << "[Conductor] Kafka connected.\n";


  bool genesis_was_created = false;
  if (std::filesystem::exists(filename_)) {
    loadBlockchain(filename_);
  } else {
    init_genesis_block();
    genesis_was_created = true;
  }

  if (genesis_was_created) {
    block_producer_.send(blockchain_.front().serialize());
    std::cout << "[Conductor] Gênesis sent.\n";
  }


  mining_loop();

  for (auto& t : kafka_threads) {
    t.detach();
  }

  std::cout << "\n[Conductor] All blocks were mined.\n";
}
