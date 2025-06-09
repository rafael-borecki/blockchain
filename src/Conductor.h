#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "Block.h"
#include "Worker.h"
#include "SharedState.h"
#include <vector>
#include <string>

class Conductor {
public:
    Conductor(std::string filename, int num_workers, uint32_t max_height);

    // public method to start the entire process
    void run();

private:
    std::string filename_;
    uint32_t max_height_;
    int num_workers_;

    SharedState shared_state_;
    std::vector<Block> blockchain_;
    std::vector<Worker> workers_;

    // helper methods to keep the run() method clean
    void init_genesis_block();
    void mining_loop();
    
    // methods to save stuff to the bin file
    // despite being methods of conductor, their definition
    // is in Filesystem.cpp
    bool saveBlockchain(std::string filename, Block block);
};

#endif // CONDUCTOR_H
