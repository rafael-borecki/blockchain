#include "KafkaThreads.h"
#include <iostream>
#include <thread>

void kafkaConsumerToQueueThread(const std::string& brokers,
                                const std::string& group,
                                const std::string& topic,
                                SharedState& state) {
  KafkaSimpleConsumer cons(brokers, group, topic);
  {
    std::lock_guard<std::mutex> lk(state.kafka_mutex);
    state.kafka_ready = true;
  }
  state.kafka_cv.notify_one();
  while (true) {
    std::string payload = cons.poll(100);
    if (payload.empty()) continue;

    {
      std::lock_guard<std::mutex> lk(state.dataMutex);
      state.data_pool.push(payload);
      std::cout << "[KafkaConsumer] ← \"" << payload << "\"\n";
    }
    state.data_semaphore.release();
  }
}
