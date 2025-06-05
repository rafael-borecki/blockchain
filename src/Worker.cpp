#include "Time.h"
#include "Worker.h"
#include "Block.h"
#include "HashUtils.h"
#include <string>
#include <vector>

void Worker::addBlock(const Block& b) {
  blocks.push_back(b);
}

Block& Worker::latest() {
  return blocks.back();
}

void Worker::debugWorker() {
  Block current_block = Worker::latest();
  if(Worker::blocks.size() > 0) {
    std::cout << "WID: " << workerId << std::endl;
    current_block.debugBlock();
  }
  else {
    std::cout<< "Worker's blockchain is currently empty.";
  }
}

std::string Worker::findNonce() {
  Block current_block = Worker::latest();
  std::string hashMeta = current_block.hashMeta;
  std::string base_hash = current_block.blockStreamHash();
  std::string current_hash = "";
  std::string full_nonce = "";
  std::string workerId = Worker::workerId;

  uint32_t nonce_iteration = 0;

  for(nonce_iteration = 0; nonce_iteration < 1000000000 /* will change this eventually*/; nonce_iteration++){
    full_nonce = workerId + std::to_string(nonce_iteration);
    current_hash = base_hash + full_nonce;
    current_hash = HashUtils::SHA256(current_hash);

    if (hashMeta > current_hash) {
      return full_nonce; // valid nonce found;
    }
  }

  return full_nonce;
}

void Worker::mine(std::vector<Block> blockchain) { 
  for(auto block_id = blockchain.size(); block_id < blockchain.size() + 3 /*TESTING*/; block_id++) {
    std::string lastHash = blockchain.back().blockHash;
    data = "BLOCK DATA " + std::to_string(blockchain.size());
    uint64_t timestamp = getTimestamp();
    blockchain.push_back(
        Block(block_id, timestamp, hashMeta, lastHash, data)
        );
    // insert new block in the worker's blockchain
    addBlock(blockchain.back());

    //mine the nonce for the new block
    std::string nonce = findNonce(); 
    blocks.back().nonce = nonce;

    // append the blockhash to the block data
    blocks.back().blockHash = blockchain.back().blockStreamHash();
    debugWorker();
  }
}
