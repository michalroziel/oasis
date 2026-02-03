// diverter.ino — Solenoid diverter & Pump Safety Logic

const int PIN_DIVERTER = 14;

// If TL231 reads high % when tank is empty, set to 1
#define TL231_INVERTED 0 // [CHANGED TO 0] We are now using hardcoded 0/100, so no inversion needed

void diverter_init() {
  pinMode(PIN_DIVERTER, OUTPUT);
  digitalWrite(PIN_DIVERTER, HIGH);  // Default: Valve OFF = Route [A] = drain
  Serial.println("[DIVERTER] init (drain)");
}

void diverter_update() {
  int level = tl231_getLevel(); // Will return 0 or 100 from simulator

  // - - - - TEST 2: DIVERTER LOGIC - - - - 
  // IF Full -> Drain (Route A). IF Empty -> Fill (Route B)
  if (level >= 100) {
    digitalWrite(PIN_DIVERTER, HIGH); 
    // Serial.println("   -> Diverter: DRAIN (Tank Full)"); 
  } else {
    digitalWrite(PIN_DIVERTER, LOW);
    // Serial.println("   -> Diverter: FILLING (Tank Empty)");
  }

  // - - - - TEST 3: PUMP SAFETY LOGIC - - - - 
  // Pump should NOT run if tank is empty (cavitation protection)
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) { // Print status every 2 seconds
    lastPrint = millis();
    
    if (level <= 0) {
      Serial.println("!!! PUMP SAFETY TRIGGERED: Tank Empty -> PUMP OFF !!!");
      // digitalWrite(PIN_PUMP, LOW); // (Uncomment if you have a real pump pin)
    } else {
      Serial.println("... Pump Allowed to Run (Tank has water) ...");
      // digitalWrite(PIN_PUMP, HIGH); // (Uncomment if you have a real pump pin)
    }
  }
}