from aiokafka import AIOKafkaProducer, AIOKafkaConsumer
import asyncio, os, json

BOOTSTRAP = os.getenv("KAFKA_BOOTSTRAP", "broker:29092")
RAW_TOPIC = "raw-data"
BLOCKS_TOPIC = "mined-blocks"
GROUP_ID = "sse-consumer"

producer: AIOKafkaProducer | None = None
consumer: AIOKafkaConsumer | None = None

async def init_kafka():
    global producer, consumer
    loop = asyncio.get_running_loop()

    producer = AIOKafkaProducer(
        loop=loop, bootstrap_servers=BOOTSTRAP,
        acks="all", enable_idempotence=True
    )
    await producer.start()

    consumer = AIOKafkaConsumer(
        BLOCKS_TOPIC,
        loop=loop,
        bootstrap_servers=BOOTSTRAP,
        group_id=GROUP_ID
    )
    await consumer.start()

async def stop_kafka():
    await consumer.stop()
    await producer.stop()

async def send_raw(payload: dict):
    """Envia JSON para raw-data."""
    await producer.send_and_wait(RAW_TOPIC, str(payload).encode())

async def blocks_stream():
    """Async generator que gera eventos SSE a cada novo bloco."""
    async for msg in consumer:
        yield f"data: {msg.value.decode()}\n\n"
