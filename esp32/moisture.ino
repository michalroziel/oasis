#define SOIL_PIN 32  // AO Pin vom Bodenfeuchtigkeitssensor

const int dry = 2450;
const int wet = 950;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("\n=== OASIS Soil Moisture Sensor ===");
}

void loop() {
  int sensorValue = analogRead(SOIL_PIN);
  
  // Umrechnung in Prozent: 4095 = trocken, 0 = nass
  int moisturePercent = map(sensorValue, wet, dry, 100, 0);
  
  Serial.print(" Raw: ");
  Serial.println(sensorValue);
 Serial.print(" | Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  delay(1000);
}
