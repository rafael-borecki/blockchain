#include "Time.h"
#include "Worker.h"
#include "Block.h"
#include "HashUtils.h"
#include <string>
#include <vector>
#include <mutex>
#include <thread>

#define MAX_HEIGHT 100

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

void Worker::mine(std::vector<Block>& blockchain, std::mutex& chainMutex) { 
  while(blockchain.back().height < MAX_HEIGHT) { 
    {
      std::lock_guard<std::mutex> lock(chainMutex);

      std::string prev_block_hash = blockchain.back().blockHash;
      data = "BLOCK DATA " + std::to_string(blockchain.size()); /*QUEBRADO*/

      uint64_t timestamp = getTimestamp();
      blockchain.push_back(
          Block(blockchain.size(), timestamp, HASH_META, prev_block_hash, data)
          );
    }

    std::string prev_block_hash = blockchain.back().prevBlockHash;
    std::string nonce = findNonce(prev_block_hash);

    {
      std::lock_guard<std::mutex> lock(chainMutex);
      
      std::cout << "Block " << blockchain.size() - 1 << " found by Worker #"<< workerId << std::endl;
      blockchain.back().nonce = nonce;
      blockchain.back().blockHash = blockchain.back().blockStreamHash();
      blockchain.back().debugBlock();
    }
  }
}
