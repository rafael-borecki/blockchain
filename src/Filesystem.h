#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Block.h"
#include <string>

char getMenuChoice();

std::vector<Block> loadBlockchainFile();

void writeToFile(std::ofstream& outFile, const std::string& str);

bool saveBlockchain(std::string filename, Block block);

#endif // FILESYSTEM_H
