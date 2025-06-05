#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include "Time.h"
#include "Worker.h"
#include "Block.h"
#include "HashUtils.h"
#include "DataProducer.h"

void Worker::findNonce(const Block& blockToMine, SharedState& state) const {
  std::string base_hash = blockToMine.blockStreamHash();
  uint64_t nonce_val = 0;

  while (!state.nonce_found_flag.load(std::memory_order_relaxed)) {
    std::string current_nonce = this->workerId + std::to_string(nonce_val);
    std::string current_hash = HashUtils::SHA256(base_hash + current_nonce);
    
    if (HASH_META > current_hash) {
      // found nonce - lock the state to save the nonce
      std::lock_guard<std::mutex> lock(state.resultMutex);
      
      // double check to see if other thread already found it 
      // while this thread was locking
      if (!state.nonce_found_flag.load()) {
        state.nonce_found_flag.store(true);
        state.winning_nonce = current_nonce;
        state.winning_worker_id = this->workerId;
      }
      // exit loop since the nonce was found
      return;
    }
    nonce_val++;
  }
}
