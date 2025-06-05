// START WORKER.CPP

#include "Time.h"
#include "Worker.h"
#include "Block.h"
#include "HashUtils.h"
#include <string>
#include <vector>
#include <mutex>
#include <thread>

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
    while (true) { // loop until a block is found or work becomes stale
      {
        std::lock_guard<std::mutex> lock(chainMutex);
        // check if another worker found a block for this height
        if (blockchain.back().height >= local_height) { 
          break; // stale condition, break loop to start over
        }
      }

      std::string current_nonce = this->workerId + std::to_string(nonce_val);
      std::string current_hash = HashUtils::SHA256(local_hash + current_nonce);

      if (HASH_META > current_hash) {
        winning_nonce = current_nonce;
        break; // found nonce
      }

      nonce_val++;

      // little sleep
      if (nonce_val % 10000 == 0) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    {
      std::lock_guard<std::mutex> lock(chainMutex);

      if (blockchain.back().height >= max_height) {
        break;
      }

      if (blockchain.back().blockHash == local_block.prevBlockHash) {
        local_block.setNonce(winning_nonce);
        blockchain.push_back(local_block);

        std::cout << "Block " << local_block.height << " found by Worker #" << this->workerId << std::endl;
        blockchain.back().debugBlock();
      } 
    } // Lock released
  }
}


// END WORKER.CPP
