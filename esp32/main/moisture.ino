// moisture.ino — soil moisture sensor module 

#define SOIL_PIN 32          // Capacitive m-sensor signal pin
const int dry = 4095;        // sensor in air
const int wet = 400;         // sensor in water

static unsigned long startTime   = 0;
static unsigned long lastUpdate  = 0;
static const unsigned long UPDATE_INTERVAL = 1000; // ms

static int lastRaw          = 0;
static int lastMoistPercent = 0;

// - - - public API - - -

void moisture_init() {
  pinMode(SOIL_PIN, INPUT);

  startTime  = millis();
  lastUpdate = 0;

  Serial.println("\n - - OASIS Soil Moisture Sensor - - ");
}

void moisture_update() {

  unsigned long now = millis();

  if (now - lastUpdate < UPDATE_INTERVAL) {

    // update only once per second

    return; 
  }

  lastUpdate = now;

  // Read sensor (0-4095 values on ESP32 ) 
  lastRaw = analogRead(SOIL_PIN);

  // Convert to percentage by mapping values
  lastMoistPercent = map(lastRaw, wet, dry, 100, 0);
  lastMoistPercent = constrain(lastMoistPercent, 0, 100);

  // Time since start 
  unsigned long elapsedSec = (now - startTime) / 1000;

  unsigned long minutes    = elapsedSec / 60;

  unsigned long seconds    = elapsedSec % 60;

  // Serial output
  Serial.print("[MOIST SENSOR] | ");
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

//  getters for other modules 
int moisture_getRaw()     { return lastRaw; }
int moisture_getPercent() { return lastMoistPercent; }