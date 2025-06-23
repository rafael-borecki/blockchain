#include "KafkaHelpers.h"

#include <iostream>

KafkaProducer::KafkaProducer(const std::string& brokers,
                             const std::string& topic)
    : topic_(topic) {
    std::string err;
    auto cfg = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    cfg->set("bootstrap.servers", brokers, err);
    cfg->set("enable.idempotence", "true", err);
    producer_ = RdKafka::Producer::create(cfg, err);
    delete cfg;
}

void KafkaProducer::send(const std::string& payload) {
    producer_->produce(topic_, RdKafka::Topic::PARTITION_UA,
                       RdKafka::Producer::RK_MSG_COPY,
                       const_cast<char*>(payload.data()), payload.size(),
                       nullptr, 0, 0, nullptr, nullptr);
    producer_->poll(0);
}

KafkaSimpleConsumer::KafkaSimpleConsumer(const std::string& brokers,
                                         const std::string& group,
                                         const std::string& topic) {
    std::string err;
    auto cfg = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    cfg->set("bootstrap.servers", brokers, err);
    cfg->set("group.id", group, err);
    cfg->set("auto.offset.reset", "earliest", err);
    consumer_ = RdKafka::KafkaConsumer::create(cfg, err);
    consumer_->subscribe({topic});
    delete cfg;
}

std::string KafkaSimpleConsumer::poll(int timeout_ms) {
    auto msg = consumer_->consume(timeout_ms);
    if (!msg) return "";
    if (msg->err()) {
        delete msg;
        return "";
    }
    std::string payload(static_cast<const char*>(msg->payload()), msg->len());
    delete msg;
    return payload;
}
