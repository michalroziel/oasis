// main/tl231.ino — Water Level Sensor Module (SIMULATED FOR DEMO)

const int PIN_LEVEL_SENSOR = 34; 

bool simState = false;
static unsigned long lastToggle = 0;

// const int LEVEL_EMPTY = 20;   // [COMMENTED OUT FOR DEMO]
// const int LEVEL_FULL  = 3000; // [COMMENTED OUT FOR DEMO]

static int currentLevelPercent = 0;
// static int lastLevelRaw = 0;  // [COMMENTED OUT FOR DEMO]

void tl231_init() {
  pinMode(PIN_LEVEL_SENSOR, INPUT);
  Serial.println(" TL231 (Simulated) initialized - Starting ");
}

void tl231_update() {
  // - - - - - - - SIMULATION LOGIC START - - - - - - - - 


  // Toggle every 10 seconds (10000ms)
  if (millis() - lastToggle > 10000) {
    lastToggle = millis();
    simState = !simState; // Flip state

    if (simState) {
      currentLevelPercent = 100; // Force FULL
      Serial.println("\n[SIMULATOR] Tank is now FULL (100%)");

    } else {
      currentLevelPercent = 0;   // Force EMPTY
      Serial.println("\n[SIMULATOR] Tank is now EMPTY (0%)");
    }
  }
  // - - - - - - - SIMULATION LOGIC END - - - - - - - - 

  /* 
  static unsigned long lastRead = 0;
  if (millis() - lastRead > 500) {
    lastRead = millis();
    long sum = 0;
    lastLevelRaw = analogRead(PIN_LEVEL_SENSOR);
    int percent = map(lastLevelRaw, LEVEL_EMPTY, LEVEL_FULL, 0, 100);
    currentLevelPercent = constrain(percent, 0, 100);
    
    Serial.print("[ TL 231 ] Raw: ");
    Serial.print(lastLevelRaw);
    Serial.print(" | Level: ");
    Serial.print(currentLevelPercent);
    Serial.println("%");
  }
  */
}

bool getSimState(){
  return simState;
}

int tl231_getLevel() {
  return currentLevelPercent;
}

int tl231_getRaw(){
  return 0; // return dummy value
}