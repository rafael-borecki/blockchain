#ifndef DATAPRODUCER_H
#define DATAPRODUCER_H

#include <queue>
#include <semaphore>
#include <cstdint>
#include "SharedState.h"

void dataProvider(SharedState& state, uint32_t max_height);

#endif // DATAPRODUCER_H
