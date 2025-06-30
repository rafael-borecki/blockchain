#!/bin/bash

seq 1 20 | awk '{printf "{\"sensor_id\": 1, \"value\": %d}\n", $1, $1}' | \
/opt/kafka/bin/kafka-console-producer.sh \
  --bootstrap-server broker:29092 \
  --topic raw-data
