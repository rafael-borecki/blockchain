FROM gcc:12 as builder

WORKDIR /app

COPY src/ .

RUN apt-get update && apt-get install -y \
    make \
    libssl-dev \
    librdkafka-dev \
 && rm -rf /var/lib/apt/lists/*

RUN make

FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libssl3 \
    librdkafka++1 \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/blockchain .

CMD ["bash", "-c", "\
  until echo > /dev/tcp/broker/29092; do \
    echo '[miner] Waiting for Kafka broker...'; \
    sleep 1; \
  done; \
  echo '[miner] Kafka broker is up. Starting blockchain miner...'; \
  ./blockchain output.bin 4 40"]
