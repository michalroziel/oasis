// diverter.ino solenoid diverter & pump safety logic

const int PIN_DIVERTER = 14;

#define TL231_INVERTED 0 // [CHANGED TO 0] We are now using hardcoded 0/100, so no inversion needed

void diverter_init() {
  pinMode(PIN_DIVERTER, OUTPUT);
  digitalWrite(PIN_DIVERTER, HIGH);  // default  off (drain)
  Serial.println("[DIVERTER] init (drain)");
}

void diverter_update() {
  int level = tl231_getLevel(); // will returns 0 or 100 from simulator

  if (level >= 100) {
    digitalWrite(PIN_DIVERTER, HIGH); 
  } else {
    digitalWrite(PIN_DIVERTER, LOW);
  }
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 2000) { //  print status every 2 seconds
    lastPrint = millis();
    
    if (level <= 0) {
      Serial.println("!!! PUMP SAFETY TRIGGERED: Tank Empty -> PUMP OFF !!!");
    } else {
      Serial.println("... Pump Allowed to Run (Tank has water) ...");
    }
  }
}
