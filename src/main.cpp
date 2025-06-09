#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <thread>
#include <mutex>
#include "Worker.h"
#include "HashUtils.h"
#include "Block.h"
#include "Time.h"
#include "DataProducer.h"
#include "Conductor.h"

int main(int argc, char* argv[]) {

  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <FILENAME> <NUM_OF_THREADS> <CHAIN_HEIGHT>\n";
    return 1;
  }
  
  std::string filename = argv[1];
  filename = filename  + ".bin";
  int num_workers = std::atoi(argv[2]);
  uint32_t max_height = std::atoi(argv[3]);

  if (num_workers <= 0 || max_height <= 0) {
    std::cerr << "Number of threads and chain height must be > 0.\n";
    return 1;
  }

  // conductor object
  Conductor conductor(filename, num_workers, max_height);

  // start
  conductor.run();
}
