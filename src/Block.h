#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>
#include "HashUtils.h"
#include "Time.h"

#define GENESIS_HASH "0000000000000000000000000000000000000000000000000000000000000000"

class Block {
  public: 
    uint32_t height;             // height of current block
    uint64_t timestamp;          // timestamp of data
    std::string hashMeta;        // current hash difficulty
    std::string prevBlockHash;   // hash of everything in block n-1 
    std::string data;            // actual data - json or whatever
    std::string blockHash;
    std::string nonce;              // h(miner_id) + random

    // constructor
    Block() 
      : height(0), timestamp(getTimestamp()), hashMeta(HASH_META), prevBlockHash(GENESIS_HASH), data("GENESIS BLOCK"), blockHash(""), nonce("0")
    {}

    Block(uint32_t height,
        uint64_t timestamp,
        std::string hashMeta,
        std::string prevBlockHash,
        std::string data)
      : height(height),
      timestamp(timestamp),
      hashMeta(std::move(hashMeta)),
      prevBlockHash(std::move(prevBlockHash)),
      data(std::move(data)),
      blockHash(blockStreamHash()),
      nonce()
  {}

    // To convert a block data into a single stream of concatenated variables.
    std::string blockStream();

    // To convert a block data stream into a SHA-256 hash.
    std::string blockStreamHash();

    void setNonce(std::string& found_nonce);

    void finalizeHashWithNonce();

    void debugBlock();
};

#endif // BLOCK_H
