#include <U8g2lib.h>
#include <Wire.h>

#define SOIL_PIN 32

const int dry = 4095;
const int wet = 400;

// Display initialisieren
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);

unsigned long startTime = 0;  // Zeit, seit ESP32 gestartet wurde
unsigned long lastMeasureTime = 0;

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  // Display initialisieren
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 20, "OASIS");
  u8g2.drawStr(0, 40, "Starting...");
  u8g2.sendBuffer();
  
  startTime = millis();  // Startzeit speichern
  
  Serial.println("\n=== OASIS Soil Moisture Sensor ===");
  delay(2000);
}

void loop() {
  int sensorValue = analogRead(SOIL_PIN);
  int moisturePercent = map(sensorValue, wet, dry, 100, 0);
  
  // Zeit seit Start (in Sekunden)
  unsigned long elapsedTime = (millis() - startTime) / 1000;
  unsigned long minutes = elapsedTime / 60;
  unsigned long seconds = elapsedTime % 60;
  
  // Serial Ausgabe
  Serial.print("Time: ");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");  // Führende Null
  Serial.print(seconds);
  Serial.print(" | Raw: ");
  Serial.print(sensorValue);
  Serial.print(" | Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  // Display Update
  u8g2.clearBuffer();
  
  // Titel
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 12, "OASIS Sensor");
  
  // Zeit
  u8g2.setFont(u8g2_font_ncenB08_tr);
  char timeStr[20];
  sprintf(timeStr, "Time: %lu:%02lu", minutes, seconds);
  u8g2.drawStr(0, 25, timeStr);
  
  // Feuchte
  u8g2.setFont(u8g2_font_ncenB10_tr);
  char moistStr[30];
  sprintf(moistStr, "Moisture: %d%%", moisturePercent);
  u8g2.drawStr(0, 45, moistStr);
  
  // Raw Value
  u8g2.setFont(u8g2_font_ncenB08_tr);
  char rawStr[20];
  sprintf(rawStr, "Raw: %d", sensorValue);
  u8g2.drawStr(0, 60, rawStr);
  
  u8g2.sendBuffer();
  
  delay(1000);
}
