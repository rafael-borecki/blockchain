#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "Block.h"
#include "Worker.h"
#include "SharedState.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "KafkaThreads.h"
#include "KafkaHelpers.h"

class Conductor {
  public:
    Conductor(std::string filename, int num_workers, uint32_t max_height);

    // Public method to start the entire process
    void run();

  private:
    std::string filename_;
    uint32_t max_height_;
    int num_workers_;

    SharedState shared_state_;
    std::vector<Block> blockchain_;
    std::vector<Worker> workers_;

    KafkaProducer block_producer_;

    // Helper methods to keep the run() method clean.
    void init_genesis_block();
    void mining_loop();

    // Methods to load and save stuff to the bin file.
    // Despite being methods of conductor, their definition
    // is in Filesystem.cpp
    void loadBlockchain(std::string filename);
    bool saveBlockchain(std::string filename, Block block);
    void writeHeader(std::fstream& out_file, std::string height, char status);
    void readBlock(std::stringstream& block_stream);
};

#endif // CONDUCTOR_H
