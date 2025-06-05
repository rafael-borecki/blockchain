#ifndef WORKER_H
#define WORKER_H

#include "HashUtils.h"
#include "Block.h"
#include <vector>
#include <mutex>

class Worker : public Block {
  public:
    std::string workerId;

    Worker(std::string wid)
      : workerId(std::move(wid))
    {}

    std::string findNonce(std::string last_hash);

    Block& latest();

    void mine(std::vector<Block>& blockchain, std::mutex& chainMutex, int max_height);
}; 
#endif // WORKER_H
