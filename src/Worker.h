#ifndef WORKER_H
#define WORKER_H

#include <vector>
#include <mutex>
#include <cstdint>
#include "HashUtils.h"
#include "DataProducer.h"
#include "Block.h"

class Worker{
  public:
    std::string workerId;

    Worker(std::string wid)
      : workerId(std::move(wid))
    {}
    
    void findNonce(const Block& blockToMine, SharedState& state) const;

}; 
#endif // WORKER_H
