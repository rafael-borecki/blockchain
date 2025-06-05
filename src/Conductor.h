#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "Block.h"
#include "Worker.h"
#include "SharedState.h"
#include <vector>
#include <string>

class Conductor {
public:
    Conductor(int num_workers, uint32_t max_height);

    // public method to start the entire process
    void run();

private:
    uint32_t max_height_;
    int num_workers_;

    SharedState shared_state_;
    std::vector<Block> blockchain_;
    std::vector<Worker> workers_;

    // helper methods to keep the run() method clean
    void init_genesis_block();
    void mining_loop();
};

#endif // CONDUCTOR_H
