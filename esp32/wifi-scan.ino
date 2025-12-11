#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n=== WiFi Scan ===");
  
  int networks = WiFi.scanNetworks();
  Serial.print("Networks found: ");
  Serial.println(networks);
  
  for (int i = 0; i < networks; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(WiFi.SSID(i));
  }
}

void loop() {}
