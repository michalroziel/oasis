from influxdb import InfluxDBClient
import paho.mqtt.client as mqtt
import json
import requests
from datetime import datetime, timedelta

# =============================
# MQTT CONFIG
# =============================
BROKER = "broker.hivemq.com"
PORT = 1883
SOIL_TOPIC = "esp/moisture"
CONTROL_TOPIC = "esp/water"

# =============================
# INFLUXDB CONFIG
# =============================
influx_client = InfluxDBClient(
    host="192.168.0.102",
    port=8086,
    username="admin",
    password="oasis12345",
    database="oasis"
)

# =============================
# WEATHERAPI CONFIG
# =============================
API_KEY = "4060c9375e3b4b4b9c8203235262601"
LAT, LON = 49.238, 6.997
PAST_RAIN_HOURS = 12

# =============================
# IRRIGATION LOGIC
# =============================
SOIL_MOISTURE_THRESHOLD = 30  # %

# =============================
# CHECK RAIN IN LAST 12 HOURS
# =============================
def rained_in_last_12_hours():
    try:
        url = (
            f"http://api.weatherapi.com/v1/forecast.json"
            f"?key={API_KEY}&q={LAT},{LON}&days=1"
        )
        response = requests.get(url, timeout=10)
        data = response.json()

        now = datetime.utcnow()
        past_limit = now - timedelta(hours=PAST_RAIN_HOURS)

        hours = data['forecast']['forecastday'][0]['hour']

        for hour in hours:
            hour_time = datetime.strptime(hour['time'], "%Y-%m-%d %H:%M")

            if past_limit <= hour_time <= now:
                if hour['precip_mm'] > 1:
                    print(f"Rain detected at {hour['time']} ({hour['precip_mm']} mm)")
                    return True

        print("No rain in last 12 hours")
        return False

    except Exception as e:
        print("WeatherAPI error:", e)
        return False

# =============================
# DECISION LOGIC
# =============================
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
        return False

# =============================
# MQTT CALLBACKS
# =============================
def on_connect(client, userdata, flags, rc, properties=None):
    print("Connected to MQTT broker")
    client.subscribe(SOIL_TOPIC)

def on_message(client, userdata, msg):
    try:
        payload = msg.payload.decode()
        data = json.loads(payload)

        raw = int(data["raw"])
        moist = int(data["moist"])

        print(f"Moisture received → Raw: {raw}, Moisture: {moist}%")

        # Save to InfluxDB
        influx_client.write_points([
            {
                "measurement": "soil_moisture",
                "tags": {"device": "esp32"},
                "fields": {
                    "raw": raw,
                    "percent": moist
                }
            }
        ])

        # Decision
        if should_water(moist):
            client.publish(CONTROL_TOPIC, "ON", retain=True)
            print("MQTT → PUMP ON")
        else:
            client.publish(CONTROL_TOPIC, "OFF", retain=True)
            print("MQTT → PUMP OFF")

    except Exception as e:
        print("MQTT message error:", e)

# =============================
# MQTT CLIENT SETUP
# =============================
client = mqtt.Client(client_id="Pi_Irrigation_Controller", protocol=mqtt.MQTTv5)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, 60)
client.loop_forever()
