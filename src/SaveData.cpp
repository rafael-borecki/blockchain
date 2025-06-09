#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Block.h"
#include "Conductor.h"

// Method to save blockchain to a binary file:
// TODO: - define a fixed block size
//       - set padding according to fixed size 
//       - create a header for the binary file to track
//         details about the blockchain
bool Conductor::saveBlockchain(std::string filename, Block block)
{
  char null_padding = '\0';
  uint32_t block_size = sizeof(block);
  std::cout << "[Filesystem] Blocksize " << block_size << std::endl;

  std::cout << "[Filesystem] Saving to " << filename_ << std::endl;
  
  // creates the file if it doesn't exist.
  std::fstream outFile(filename, std::ios::in | std::ios::out | std::ios::binary);

  if (!outFile.is_open()) {
    std::ofstream newFile(filename, std::ios::binary);
    if (!newFile) {
      std::cerr << "[Filesystem] Failed to create or open file: " << filename << std::endl;
      return false;
    }
    newFile.close();
    outFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  }

  if (!outFile) {
    std::cerr << "[Filesystem] Failed to open file: " << filename << std::endl;
    return false;
  }

  try{
    // append the new block's data at the end of the file 
    outFile.seekp(0, std::ios::end); 

    // writes data into a buffer first
    std::stringstream blockBuffer;
    
    blockBuffer << " START_OF_BLOCK " << null_padding;
    blockBuffer << std::to_string(block.height) << null_padding;
    blockBuffer << std::to_string(block.timestamp) << null_padding;
    blockBuffer << block.hashMeta << null_padding;
    blockBuffer << block.prevBlockHash << null_padding;
    blockBuffer << block.data << null_padding;
    blockBuffer << block.blockHash << null_padding;
    blockBuffer << block.fullBlockHash << null_padding;
    
    outFile << blockBuffer.rdbuf();

  } catch (const std::exception& e) {
    std::cerr << "[Filesystem] An exception occurred while writing to file: " << e.what() << std::endl;
    outFile.close();
    return false;
  }

  outFile.close();
  std::cout << "[Filesystem] Data successfully saved to " << filename << std::endl;
  return true;
}
