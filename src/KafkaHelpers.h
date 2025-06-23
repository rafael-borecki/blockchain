#pragma once
#include <string>
#include <librdkafka/rdkafkacpp.h>

class KafkaProducer {
 public:
  KafkaProducer(const std::string& brokers, const std::string& topic);
  void send(const std::string& payload);
 private:
  RdKafka::Producer* producer_;
  std::string topic_;
};

class KafkaSimpleConsumer {
 public:
  KafkaSimpleConsumer(const std::string& brokers,
                      const std::string& group,
                      const std::string& topic);
  std::string poll(int timeout_ms = 1000);
 private:
  RdKafka::KafkaConsumer* consumer_;
};
