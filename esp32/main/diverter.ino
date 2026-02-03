// diverter.ino — Solenoid diverter (same pin & polarity as solenoid-valve-test.ino)
// Test: HIGH = Valve OFF = Route [A],  LOW = Valve ON = Route [B]
// We use: Route [B] = to tank (fill),  Route [A] = drain

const int PIN_DIVERTER = 14;  // same as TEST_VALVE_PIN in solenoid-valve-test.ino

// If your TL231 reads high % when tank is empty, set to 1
#define TL231_INVERTED 1

void diverter_init() {
  pinMode(PIN_DIVERTER, OUTPUT);
  digitalWrite(PIN_DIVERTER, HIGH);  // Valve OFF = Route [A] = drain (same as valve_only_init)
  Serial.println("[DIVERTER] init (drain)");
}

void diverter_update() {
  int level = tl231_getLevel();
#if TL231_INVERTED
  level = 100 - level;  // invert: sensor reads 100 when empty, 0 when full
#endif

  if (level >= 100) {
    digitalWrite(PIN_DIVERTER, HIGH);  // Valve OFF = drain (tank full)
  } else {
    digitalWrite(PIN_DIVERTER, LOW);   // Valve ON = Route [B] = to tank (fill)
  }
}
