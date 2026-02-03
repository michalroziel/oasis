// main.ino — entry point

#include <WiFi.h>
#include <PubSubClient.h>

// --- Wi-Fi & MQTT settings ---
const char* ssid       = "TP-LINK_936A";
const char* password   = "39429964";
const char* mqtt_server = "broker.hivemq.com";
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "esp/moisture";


// --- Soil Moisture module ---
#define SOIL_PIN 32          // Capacitive sensor signal pin
const int dry = 2700;        // Calibration: sensor in air
const int wet = 800;         // Calibration: sensor in water

static unsigned long startTime   = 0;
static unsigned long lastMoistUpdate  = 0;
static const unsigned long MOIST_INTERVAL = 1000; // ms

static int lastRaw          = 0;
static int lastMoistPercent = 0;

// --- MQTT client ---
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
const unsigned long MSG_INTERVAL = 2000;

// ---------------------------
// Soil moisture functions
// ---------------------------
void moisture_init() {
  pinMode(SOIL_PIN, INPUT);
  startTime  = millis();
  lastMoistUpdate = 0;

  Serial.println("\n=== OASIS Soil Moisture Sensor (no display) ===");
}

void moisture_update() {
  unsigned long now = millis();
  if (now - lastMoistUpdate < MOIST_INTERVAL) return;
  lastMoistUpdate = now;

  lastRaw = analogRead(SOIL_PIN);
  lastMoistPercent = map(lastRaw, wet, dry, 100, 0);
  lastMoistPercent = constrain(lastMoistPercent, 0, 100);

  unsigned long elapsedSec = (now - startTime) / 1000;
  unsigned long minutes = elapsedSec / 60;
  unsigned long seconds = elapsedSec % 60;

  Serial.print("Time ");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");
  Serial.print(seconds);
  Serial.print(" | Raw: ");
  Serial.print(lastRaw);
  Serial.print(" | Moisture: ");
  Serial.print(lastMoistPercent);
  Serial.println("%");
}

// Optional getters for MQTT
int moisture_getRaw()     { return lastRaw; }
int moisture_getPercent() { return lastMoistPercent; }

// ---------------------------
// Wi-Fi & MQTT functions
// ---------------------------
void setup_wifi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) { // ~15s timeout
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Wi-Fi connected! IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wi-Fi connection failed!");
  }
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    // Use unique client ID by appending millis()
    String clientId = "OASIS_ESP32_Sensor-" + String(millis());
    if (client.connect(clientId.c_str())) {
      Serial.println("connected!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2s");
      delay(2000);
    }
  }
}

void mqtt_init() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  lastMsg = 0;
}

void mqtt_update() {
  if (!client.connected()) {
    reconnect_mqtt();
  }

  client.loop(); // maintain connection

  unsigned long now = millis();
  if (now - lastMsg < MSG_INTERVAL) return;
  lastMsg = now;

  int raw   = moisture_getRaw();
  int moist = moisture_getPercent();

  char payload[128];
  snprintf(payload, sizeof(payload),
           "{\"raw\":%d,\"moist\":%d,\"timestamp\":%lu}",
           raw, moist, now);

  // Publish with retain=true so new subscribers get last value
  if (client.publish(mqtt_topic, payload, true)) {
    Serial.print("MQTT published: ");
    Serial.println(payload);
  } else {
    Serial.println("MQTT publish failed!");
  }
}

// ---------------------------
// Arduino setup & loop
// ---------------------------
void setup() {
  Serial.begin(115200);
  delay(200);

  moisture_init();
  mqtt_init();
}

void loop() {
  moisture_update();
  mqtt_update();
}
