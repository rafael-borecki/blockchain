#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <queue>
#include <mutex>
#include <semaphore>
#include <atomic>
#include <string>
#include <optional>
#include "Block.h"

struct SharedState {
    // data input
    std::mutex dataMutex;
    std::queue<std::string> data_pool;
    std::counting_semaphore<> data_semaphore{0};

    // once true, all workers stop
    std::atomic<bool> nonce_found_flag{false};

    // winner thread writes the winning nonce here - mutex protects against
    // race condition
    std::mutex resultMutex;
    std::optional<std::string> winning_nonce;
    std::optional<std::string> winning_worker_id;
};

#endif // SHARED_STATE_H
