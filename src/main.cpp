#include "Worker.h"
#include "HashUtils.h"
#include "Block.h"
#include "Time.h"
#include <iostream>
#include <vector>
#include <thread>

int main() {
  std::vector<Block> blockchain;
  // genesis block
  blockchain.push_back(Block());
  blockchain.back().blockHash = blockchain.back().blockStreamHash();
  blockchain.back().debugBlock();

  std::vector<Worker> worker;

  for(int i = 0; i < 7; i ++) {
    std::string wid = std::to_string(i);
    Worker temp_worker(wid);
    worker.push_back(temp_worker);
  }

  std::thread t1(&Worker::mine, &worker[0], blockchain);
  std::thread t2(&Worker::mine, &worker[1], blockchain);
  std::thread t3(&Worker::mine, &worker[2], blockchain);
  t1.join();
  t2.join();
  t3.join();

  return 0; 
};
