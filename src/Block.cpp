#include "Block.h"
#include "HashUtils.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>


std::string Block::blockStream() {
  std::ostringstream blockStream;
  blockStream << this->height << this->timestamp << this->hashMeta << this->prevBlockHash << this->data;
  return blockStream.str();
}

std::string Block::blockStreamHash() {
  return HashUtils::SHA256(blockStream());
}

void Block::debugBlock() {
  std::cout<<"---- BLOCK START ----" << std::endl;
  std::cout<<"Height: " << height <<  std::endl;
  std::cout<<"Timestamp: " <<  timestamp <<  std::endl;
  std::cout<<"hashMeta: " <<  hashMeta <<  std::endl;
  std::cout<<"prevBlockHash: " <<  prevBlockHash <<  std::endl;
  std::cout<<"data: " <<  data <<  std::endl;
  std::cout<<"blockHash: " <<  blockHash <<  std::endl;
  std::cout<<"nonce: " <<  nonce <<  std::endl;
  std::cout<<"---- BLOCK   END ----\n" << std::endl;
}
