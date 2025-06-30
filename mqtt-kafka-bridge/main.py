print("🔥 Iniciando bridge MQTT → Kafka")
import time
import paho.mqtt.client as mqtt
from kafka import KafkaProducer
from kafka.errors import NoBrokersAvailable
import socket



KAFKA_BROKER = 'broker:29092'
while True:
    try:
        producer = KafkaProducer(bootstrap_servers=KAFKA_BROKER)
        print("kafka connection active")
        break
    except NoBrokersAvailable:
        print("kafka not available. retrying in 2s...")
        time.sleep(2)


MQTT_BROKER = 'mosquitto'
MQTT_PORT = 1883
while True:
    try:
        with socket.create_connection((MQTT_BROKER, MQTT_PORT), timeout=2):
            print("mosquitto connection active")
            break
    except OSError:
        print("mosquitto not available. retrying in 2s...")
        time.sleep(2)


MQTT_TOPIC = 'sensor/+/data'
KAFKA_TOPIC = 'raw-data'

def on_connect(client, userdata, flags, reason_code, properties):
    print(f"[MQTT] Connected (code={reason_code})")
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    print(f"[Bridge] {msg.topic} → {KAFKA_TOPIC} | {msg.payload.decode()}")
    producer.send(KAFKA_TOPIC, msg.payload)

client = mqtt.Client(
    client_id="mqtt-kafka-bridge",
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2
)
client.on_connect = on_connect
client.on_message = on_message

client.connect(MQTT_BROKER, MQTT_PORT, 60)

client.loop_forever()
