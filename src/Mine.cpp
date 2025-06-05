#include "Mine.h"
#include "Worker.h"

void mine(Worker worker1, Block block) {
  while(1) {
    std::string non = worker1.findNonce();
    
    if(worker1.hashMeta.compare(HashUtils::SHA256(worker1.hashMeta)) < 0){
    worker1.height = worker1.height  + 1;
    } 
  }
}
