// wifi.ino wifi scanning module

#include <WiFi.h>

void wifi_init() {
  Serial.println("\n=== WiFi Scan ===");
  
  int networks = WiFi.scanNetworks();
  Serial.print("Networks found: ");
  Serial.println(networks);
  
  for (int i = 0; i < networks; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (RSSI: ");
    Serial.print(WiFi.RSSI(i));
    Serial.println(" dBm)");
  }
  
  Serial.println("\n=== Scan Complete ===\n");
}

void wifi_update() {
  // scanner only runs once at startup
}
