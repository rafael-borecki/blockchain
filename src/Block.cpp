#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include "Block.h"
#include "HashUtils.h"

std::string Block::blockStream() const {
  std::ostringstream blockStream;
  blockStream << this->height << this->timestamp << this->hashMeta << this->prevBlockHash << this->data;
  return blockStream.str();
}

std::string Block::blockStreamHash() const {
  return HashUtils::SHA256(blockStream());
}

void Block::finalizeHash() {
  this->fullBlockHash = HashUtils::SHA256(this->blockStream() + this->nonce);
}

void Block::setNonce(std::string& found_nonce) {
  this->nonce = found_nonce;
}

void Block::debugBlock() const {
  std::cout<<"---- BLOCK START ----" << std::endl;
  std::cout<<"height:         " << height <<  std::endl;
  std::cout<<"timestamp:      " <<  timestamp <<  std::endl;
  std::cout<<"hashMeta:       " <<  hashMeta <<  std::endl;
  std::cout<<"prevBlockHash:  " <<  prevBlockHash <<  std::endl;
  std::cout<<"data:           " <<  data <<  std::endl;
  std::cout<<"blockHash:      " <<  blockHash <<  std::endl;
  std::cout<<"nonce:          " <<  nonce <<  std::endl;
  std::cout<<"fullBlockHash:  " <<  fullBlockHash <<  std::endl; //actual hash used to verify the integrity of the blockchain
                                                                 //since it also contains the nonce
  std::cout<<"---- BLOCK   END ----\n" << std::endl;
}
