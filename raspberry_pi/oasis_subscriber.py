import paho.mqtt.client as mqtt

BROKER = "broker.hivemq.com"
PORT = 1883
TOPIC = "esp/moisture"

def on_connect(client, userdata, flags, rc, properties=None):
    print("Connected with result code", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print("Topic:", msg.topic, "| Payload:", msg.payload.decode())

client = mqtt.Client(client_id="PiSubscriber", protocol=mqtt.MQTTv5)

client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, 1883, 60)
client.loop_forever()
