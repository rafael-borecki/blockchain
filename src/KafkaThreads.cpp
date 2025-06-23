#include "KafkaThreads.h"
#include <iostream>
#include <thread>

void kafkaConsumerToQueueThread(const std::string& brokers,
                                const std::string& group,
                                const std::string& topic,
                                SharedState& state) {
  KafkaSimpleConsumer cons(brokers, group, topic);
  while (true) {
    std::string payload = cons.poll(1000);
    if (payload.empty()) continue;

    {
      std::lock_guard<std::mutex> lk(state.dataMutex);
      state.data_pool.push(payload);
      std::cout << "[KafkaConsumer] ← \"" << payload << "\"\n";
    }
    state.data_semaphore.release();
  }
}
