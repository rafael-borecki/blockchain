#ifndef WORKER_H
#define WORKER_H

#include "HashUtils.h"
#include "Block.h"
#include <vector>

class Worker : public Block {
  public:
    std::string workerId;
    std::vector<Block> blocks;

    Worker(std::string wid)
      : workerId(std::move(wid))
    {}

    Worker(std::string wid, std::vector<Block> initialBlocks) 
      : workerId(std::move(wid)),
      blocks(std::move(initialBlocks)) {}

    void addBlock(const Block& b);

    std::string findNonce();

    Block& latest();

    void debugWorker();

    void mine(std::vector<Block> blocks);
}; 
#endif // WORKER_H
