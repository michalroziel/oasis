// mqtt.ino — MQTT publishing module (refactored from send-moisture.ino)

#include <WiFi.h>
#include <PubSubClient.h>


static const char* ssid       = "iPhone 14";
static const char* password   = "Laith2003";
static const char* mqtt_server = "broker.hivemq.com";
static const int   mqtt_port   = 1883;

static const char* mqtt_topic = "esp/moisture";
static const char* mqtt_topic_water = "esp/water_level";
static const char* topic_pump_cmd = "esp/water";


static WiFiClient espClient;
static PubSubClient client(espClient);

static unsigned long lastMsg = 0;
static const unsigned long MSG_INTERVAL = 2000;

static void setup_wifi() {

  WiFi.begin(ssid, password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {

    delay(500);

    attempts++;

  }
}

static void reconnect() {
  while (!client.connected()) {
    if (client.connect("OASIS_ESP32_Sensor")) {
      client.subscribe(topic_pump_cmd);
    } else {
      delay(2000);
    }
  }
}

void mqtt_init() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
  lastMsg = 0;
}

void mqtt_update() {
  if (!client.connected()) {
    reconnect();
  }

  // Needed to maintain the MQTT connection and process packets.

  client.loop(); 

  unsigned long now = millis();

  if (now - lastMsg < MSG_INTERVAL) return;

  lastMsg = now;

  // get values to send 
  int raw   = moisture_getRaw();
  int moist = moisture_getPercent();

  // set up payload to send

  char payload[128];
  
  snprintf(payload, sizeof(payload),

           "{\"raw\":%d,\"moist\":%d,\"timestamp\":%lu}",

           raw, moist, now);

  client.publish(mqtt_topic, payload);

  int waterRaw = tl231_getRaw();
  int waterPct = tl231_getLevel();
  char waterPayload[128];
  snprintf(waterPayload, sizeof(waterPayload), "{\"raw\":%d,\"percent\":%d,\"timestamp\":%lu}", waterRaw, waterPct, now);
  client.publish(mqtt_topic_water, waterPayload);
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  String msgString = String(message);
  String topicString = String(topic);

  if (topicString == topic_pump_cmd) {
    if (msgString == "ON" && getSimState()) {
      pump_on();
    } else if (msgString == "OFF") {
      pump_off();
    }
  }
}
