// START TIME.CPP

#include "Time.h"
#include <iostream>
#include <chrono>
#include <cstdint>

uint64_t getTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
              ).count();
  
  uint64_t timestamp = ms;
  return timestamp;
}

// END TIME.CPP
