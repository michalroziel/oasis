#include <WiFi.h>
#include <PubSubClient.h>

// ===== SENSOR PIN =====
#define SOIL_PIN 32  // Analog pin connected to soil moisture sensor

// ===== SENSOR CALIBRATION =====
const int dry = 4095;   // When sensor is in dry soil (or air)
const int wet = 400;    // When sensor is in water

// ===== WiFi Credentials =====
const char* ssid = "Connecting...";
const char* password = "71326979";

// ===== MQTT Broker (Free Public) =====
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "esp/moisture";

// ===== Setup WiFi & MQTT =====
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
const long MSG_INTERVAL = 2000;  // Send data every 2 seconds

// ===== WiFi Connection =====
void setup_wifi() {
  Serial.println("\n=== Connecting to WiFi ===");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n✗ WiFi Failed!");
  }
}

// ===== MQTT Reconnection =====
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    
    if (client.connect("OASIS_ESP32_Sensor")) {
      Serial.println("✓ Connected!");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ===== Read Sensor Data =====
void read_and_publish_sensor() {
  // Read raw ADC value (0-4095)
  int rawValue = analogRead(SOIL_PIN);
  
  // Convert to moisture percentage (0-100%)
  // Constrain to ensure we stay within 0-100
  int moisturePercent = map(rawValue, wet, dry, 100, 0);
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  // Create JSON payload
  char payload[100];
  sprintf(payload, "{\"raw\":%d,\"moist\":%d,\"timestamp\":%lu}", 
          rawValue, moisturePercent, millis());
  
  // Print to Serial for debugging
  Serial.print("Sensor Value: raw=");
  Serial.print(rawValue);
  Serial.print(" | moisture=");
  Serial.print(moisturePercent);
  Serial.print("% | Payload: ");
  Serial.println(payload);
  
  // Publish to MQTT
  if (client.publish(mqtt_topic, payload)) {
    Serial.println("✓ Data published successfully!");
  } else {
    Serial.println("✗ Failed to publish data");
  }
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n================================");
  Serial.println("=== OASIS ESP32 Soil Sensor ===");
  Serial.println("================================");
  
  // Initialize sensor pin
  pinMode(SOIL_PIN, INPUT);
  
  // Connect to WiFi
  setup_wifi();
  
  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
  
  Serial.println("✓ Setup complete!\n");
}

// ===== Main Loop =====
void loop() {
  // Ensure MQTT connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  // Read and publish sensor data every MSG_INTERVAL (2 seconds)
  unsigned long now = millis();
  if (now - lastMsg > MSG_INTERVAL) {
    lastMsg = now;
    read_and_publish_sensor();
    Serial.println();  // blank line for readability
  }
}
