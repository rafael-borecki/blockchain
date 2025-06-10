#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Block.h"
#include "Conductor.h"

// Methods to save blockchain to a binary file:
//
void Conductor::readBlock(std::stringstream& block_stream) {
  std::string temp_height, temp_timestamp, t_hashMeta, t_prevBlockHash, t_data, t_blockHash, t_nonce, t_fullBlockHash;

  std::getline(block_stream, temp_height, '\0');
  std::getline(block_stream, temp_timestamp, '\0');
  std::getline(block_stream, t_hashMeta, '\0');
  std::getline(block_stream, t_prevBlockHash, '\0');
  std::getline(block_stream, t_data, '\0');
  std::getline(block_stream, t_blockHash, '\0');
  std::getline(block_stream, t_nonce, '\0');
  std::getline(block_stream, t_fullBlockHash, '\0');
  
  if (temp_height.empty()) return;

  uint32_t t_height = static_cast<uint32_t>(std::stoul(temp_height));
  uint64_t t_timestamp = static_cast<uint64_t>(std::stoull(temp_timestamp));

  Block temp_block(t_height, t_timestamp, t_hashMeta, t_prevBlockHash, t_data);
  
  temp_block.blockHash = t_blockHash;
  temp_block.nonce = t_nonce;
  temp_block.fullBlockHash = t_fullBlockHash;

  blockchain_.push_back(temp_block);
  std::cout << "[Filesystem] Loaded block #" << temp_block.height << std::endl;
}


void Conductor::loadBlockchain(std::string filename) {
  uint32_t max_block_size = 512;
  uint32_t header_size = 16;

  std::ifstream input_file;
  input_file.open(filename, std::ios::in);

  if (!input_file.is_open()) {
    std::cerr << "[Filesystem] Failed to open file. " << filename << std::endl;
    std::exit(EXIT_FAILURE);
  }
  
  char header[header_size];
  input_file.read(header, header_size);  

  if (header[0] != '0') {
    std::cerr << "[Filesystem] File is corrupted or was not closed properly." << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string t_block_count(&header[1]);

  try {
    uint32_t block_count = 0;
    if (!t_block_count.empty()) {
      block_count = std::stoul(t_block_count);
    }

    std::cout << "[Filesystem] Loading " << block_count << " blocks from " << filename << std::endl;

    // The file pointer is already past the header.
    std::vector<char> buffer(max_block_size);
    for (uint32_t i = 0; i < block_count; ++i) {
      input_file.read(buffer.data(), max_block_size);
      if (input_file.gcount() < max_block_size && i < block_count - 1) {
        std::cerr << "[Filesystem] Error: File ended unexpectedly." << std::endl;
        throw std::runtime_error("Unexpected end of file");
      }

      std::stringstream block_stream(std::string(buffer.data(), max_block_size));
      readBlock(block_stream);
    }
  } catch (const std::exception& e) {
    std::cerr << "[Filesystem] Failed to parse blockchain file: " << e.what() << std::endl;
    blockchain_.clear(); 
    std::exit(EXIT_FAILURE);
  }
}

bool Conductor::saveBlockchain(std::string filename, Block block) {
  char null_padding = '\0';

  /** Definition of the block size,
   *  which might change later.
   */
  uint32_t max_block_size = 512; 

  std::cout << "[Filesystem] Saving block #" << block.height << " to " << filename_ << std::endl;

  // Creates the file if it doesn't exist.
  std::fstream out_file(filename, std::ios::in | std::ios::out | std::ios::binary);

  if (!out_file.is_open()) {
    std::ofstream newFile(filename, std::ios::binary);
    if (!newFile) {
      std::cerr << "[Filesystem] Failed to create or open file. " << filename << std::endl;
      return false;
    }
    newFile.close();
    out_file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  }

  if (!out_file) {
    std::cerr << "[Filesystem] Failed to open file. " << filename << std::endl;
    return false;
  }

  try {
    writeHeader(out_file, std::to_string(block.height + 1), '1');

    // Append the new block's data at the end of the file.
    out_file.seekp(0, std::ios::end);

    // Writes data into a buffer first.
    std::stringstream block_buffer;

    block_buffer << std::to_string(block.height) << null_padding;
    block_buffer << std::to_string(block.timestamp) << null_padding;
    block_buffer << block.hashMeta << null_padding;
    block_buffer << block.prevBlockHash << null_padding;
    block_buffer << block.data << null_padding;
    block_buffer << block.blockHash << null_padding;
    block_buffer << block.nonce << null_padding;
    block_buffer << block.fullBlockHash << null_padding;

    std::string block_data_str = block_buffer.str();
    out_file << block_buffer.rdbuf();

    uint32_t current_size = block_data_str.length();
    if (current_size < max_block_size) {
      for (uint32_t k = 0; k < max_block_size - current_size; k++) {
        out_file.write(&null_padding, 1);
      }
    } else {
      std::cerr << "[Filesystem] WARNING: Block data size (" << current_size
        << ") exceeds max block size (" << max_block_size << ")." << std::endl;
    }

  } catch (const std::exception& e) {
    std::cerr << "[Filesystem] An exception occurred while writing to file: " << e.what() << std::endl;
    out_file.close();
    return false;
  }

  writeHeader(out_file, std::to_string(block.height + 1), '0');
  out_file.close();
  std::cout << "[Filesystem] Data successfully saved to " << filename << std::endl;
  return true;
}

// Possibly writeHeader should initialize the file as a better practice.
void Conductor::writeHeader(std::fstream &out_file, std::string height, char mode) {
  char header[16] = {0};
  header[0] = mode;

  strncpy(&header[1], height.c_str(), 14);
  header[15] = '\0';

  out_file.seekp(0, std::ios::beg);
  out_file.write(header, 16);
}
