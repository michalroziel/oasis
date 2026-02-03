from influxdb import InfluxDBClient
import paho.mqtt.client as mqtt
import json

BROKER = "broker.hivemq.com"
PORT = 1883

SOIL_TOPIC = "esp/moisture"
WATER_TOPIC = "esp/water_level"

PUMP_TOPIC = "esp/water"
VALVE_TOPIC = "esp/valve"



influx_client = InfluxDBClient(
    host="192.168.0.103",
    port=8086,
    username="admin",
    password="oasis12345",
    database="oasis"
)

SOIL_MOISTURE_THRESHOLD = 60
# this is commmented out because we cant rely on weather to be dry during the demo
"""
def rained_in_last_12_hours():
    # TESTING MODE: no WeatherAPI calls
    print("TESTING: Skipping WeatherAPI, assuming no rain.")
    return False


def should_water(moisture_percent):
    if moisture_percent < SOIL_MOISTURE_THRESHOLD:
        if rained_in_last_12_hours():
            print("Soil dry BUT recent rain → DO NOT WATER")
            return False
        else:
            print("Soil dry & no recent rain → WATER")
            return True
    else:
        print("Soil moisture sufficient → DO NOT WATER")
        return False """


def on_connect(client, userdata, flags, rc, properties=None):
    print("Connected to MQTT broker, rc =", rc)
    client.subscribe([(SOIL_TOPIC, 0), (WATER_TOPIC, 0)])
    print("Subscribed to:", SOIL_TOPIC, "and", WATER_TOPIC)


def write_soil_to_influx(raw: int, moist: int):
    influx_client.write_points([{
        "measurement": "soil_moisture",
        "tags": {"device": "esp32"},
        "fields": {"raw": raw, "percent": moist}
    }])


def write_water_to_influx(raw: int, percent: int):
    influx_client.write_points([{
        "measurement": "water_level",
        "tags": {"device": "esp32", "tank": "main"},
        "fields": {"raw": raw, "percent": percent}
    }])


def control_irrigation(client, moisture_percent):
    print(f"Checking moisture: {moisture_percent}% (Threshold: {SOIL_MOISTURE_THRESHOLD}%)")
    if moisture_percent < SOIL_MOISTURE_THRESHOLD:
        print(">>> DECISION: Soil Dry -> Send ON (pump runs on ESP only if tank full)")
        client.publish(VALVE_TOPIC, "ON", retain=True)
        client.publish(PUMP_TOPIC, "ON", retain=True)
    else:
        print(">>> DECISION: Soil Sufficient -> Send OFF")
        client.publish(VALVE_TOPIC, "OFF", retain=True)
        client.publish(PUMP_TOPIC, "OFF", retain=True)

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)

        if msg.topic == SOIL_TOPIC:
            raw = int(data["raw"])
            moist = int(data["moist"])

            print(f"[SOIL] Raw: {raw}, Moisture: {moist}%")
            write_soil_to_influx(raw, moist)
            control_irrigation(client, moist)

        elif msg.topic == WATER_TOPIC:
            raw = int(data["raw"])
            percent = int(data["percent"])

            print(f"[WATER] Raw: {raw}, Level: {percent}% (simstate: full=100, empty=0)")
            if percent >= 100:
                print(">>> DECISION: Tank full -> Valve OFF (drain)")
            else:
                print(">>> DECISION: Tank not full -> Valve ON (to tank)")
            if percent <= 0:
                print(">>> DECISION: Tank EMPTY -> Send OFF so pump turns off")
                client.publish(PUMP_TOPIC, "OFF", retain=True)
            write_water_to_influx(raw, percent)

        else:
            print("[MQTT] Message on unknown topic:", msg.topic)

    except Exception as e:
        print("MQTT message error:", e)

client = mqtt.Client(client_id="Pi_Irrigation_Controller", protocol=mqtt.MQTTv5)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 60)
client.loop_forever()
