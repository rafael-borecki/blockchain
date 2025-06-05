#include "Worker.h"
#include "HashUtils.h"
#include "Block.h"
#include "Time.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

#define NUM_WORKERS 8

int main() {
  std::vector<Block> blockchain;
  // genesis block
  blockchain.push_back(Block());
  blockchain.back().blockHash = blockchain.back().blockStreamHash();
  blockchain.back().debugBlock();

  std::mutex chainMutex;

  std::vector<Worker> worker;
  for (int i = 0; i < NUM_WORKERS; ++i) {
    worker.emplace_back(std::to_string(i));
  }

  std::vector<std::thread> threads;
  threads.reserve(worker.size());
  for (size_t i = 0; i < worker.size(); ++i) {
    threads.emplace_back(
        &Worker::mine,
        &worker[i],
        std::ref(blockchain),
        std::ref(chainMutex)
        );
  }

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  return 0; 
};
