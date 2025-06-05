#include "Time.h"
#include "Worker.h"
#include "Block.h"
#include "HashUtils.h"
#include <string>
#include <vector>
#include <mutex>
#include <thread>

std::string Worker::findNonce(std::string prev_block_hash) {
  std::string base_hash = prev_block_hash;
  std::string current_hash = "";
  std::string full_nonce = "";
  std::string workerId = Worker::workerId;

  uint32_t nonce_iteration = 0;

  for(nonce_iteration = 0; nonce_iteration < 1000000000 /* will change this eventually*/; nonce_iteration++){
    full_nonce = workerId + std::to_string(nonce_iteration);
    current_hash = base_hash + full_nonce;
    current_hash = HashUtils::SHA256(current_hash);

    if (HASH_META > current_hash) {
      std::cout << "VALID HASH: "<< current_hash << std::endl;
      return full_nonce; // valid nonce found;
    }
  }

  return full_nonce;
}

void Worker::mine(std::vector<Block>& blockchain, std::mutex& chainMutex, int max_height) { 
  while(true) {
    Block local_block;
    std::string prev_block_hash;
    uint32_t local_height;

    {
      std::lock_guard<std::mutex> lock(chainMutex);

      if (blockchain.back().height >= max_height) {
        break; // found max_height blocks
      }
      prev_block_hash = blockchain.back().blockHash;

      local_height = blockchain.back().height + 1;
    }

    std::string data = "BLOCK DATA " + std::to_string(local_height);
    uint64_t timestamp = getTimestamp();

    local_block = Block(blockchain.size(), timestamp, HASH_META, prev_block_hash, data);
    
    std::string local_hash = local_block.blockStreamHash();
    std::string winning_nonce = "";
    std::string winning_hash  = "";
    
    uint64_t nonce_val = 0;
    for (nonce_val = 0; nonce_val < 10000000; ++nonce_val) {
      std::string current_nonce = this->workerId + std::to_string(nonce_val);
      std::string current_hash = HashUtils::SHA256(local_hash + current_nonce);

      if (HASH_META > current_hash) { 
        winning_nonce = current_nonce;
        winning_hash = current_hash;
        break;
      }
    }
/*
    if (winning_nonce.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    }
*/
    {
      std::lock_guard<std::mutex> lock(chainMutex);

      if (blockchain.back().height >= max_height) {
        break;
      }

      if (blockchain.back().blockHash == local_block.prevBlockHash) {
        local_block.setNonce(winning_nonce);
        blockchain.push_back(local_block);

        std::cout << "Block " << local_block.height << " found by Worker #" << workerId << std::endl;
        blockchain.back().debugBlock();
      } /*else {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }*/
    } // Lock released
    //std::this_thread::sleep_for(std::chrono::milliseconds(1)); 
  }
}
