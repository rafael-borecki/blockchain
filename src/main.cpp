// START MAIN.CPP

#include "Worker.h"
#include "HashUtils.h"
#include "Block.h"
#include "Time.h"
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>


int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <NUM_OF_THREADS> <CHAIN_HEIGHT>\n";
    return 1;
  }

  int num_workers = std::atoi(argv[1]);
  uint32_t max_height = std::atoi(argv[2]);

  std::vector<Block> blockchain;

  // genesis block
  blockchain.push_back(Block());
  blockchain.back().blockHash = blockchain.back().blockStreamHash();
  blockchain.back().debugBlock();

  std::mutex chainMutex;

  std::vector<Worker> worker;
  for (int i = 0; i < num_workers; ++i) {
    worker.emplace_back(std::to_string(i));
  }

  std::vector<std::thread> threads;
  threads.reserve(worker.size());
  for (size_t i = 0; i < worker.size(); ++i) {
    threads.emplace_back(
        &Worker::mine,
        &worker[i],
        std::ref(blockchain),
        std::ref(chainMutex),
        max_height
        );
  }

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  return 0; 
};


// END MAIN.CPP
