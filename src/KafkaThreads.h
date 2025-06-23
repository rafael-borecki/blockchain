#pragma once
#include "KafkaHelpers.h"
#include "SharedState.h"

void kafkaConsumerToQueueThread(const std::string& brokers,
                                const std::string& group,
                                const std::string& topic,
                                SharedState& state);
