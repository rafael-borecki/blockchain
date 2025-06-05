#include <queue>
#include <semaphore>
#include <string>
#include <iostream>
#include <mutex>
#include "DataProducer.h"
#include "SharedState.h"

// simulates new data, in production could be changed for any string data source
void dataProvider(SharedState& state, uint32_t max_height) { 
    for (uint32_t i = 1; i <= max_height; ++i) {
        std::string new_data = "Data for block #" + std::to_string(i);
        {
            std::lock_guard<std::mutex> lock(state.dataMutex);
            state.data_pool.push(new_data);
            std::cout << "[DataProvider] Added data to pool: \"" << new_data << "\"" << std::endl;
        }

        // signal that one item of data is now available
        state.data_semaphore.release();

        // delay before next thing comes
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
