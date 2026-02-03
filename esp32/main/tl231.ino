// main/tl231.ino water level sensor module (SIMULATED FOR DEMO)

const int PIN_LEVEL_SENSOR = 34; 

bool simState = false;
static unsigned long lastToggle = 0;

// const int LEVEL_EMPTY = 20;   
// const int LEVEL_FULL = 3000; 

static int currentLevelPercent = 0;
// static int lastLevelRaw = 0;  

void tl231_init() {
  pinMode(PIN_LEVEL_SENSOR, INPUT);
  Serial.println(" TL231 (Simulated) initialized - Starting ");
}

void tl231_update() {

  //   toggle  10 sec
  if (millis() - lastToggle > 10000) {
    lastToggle = millis();
    simState = !simState; 

    if (simState) {
      currentLevelPercent = 100; 
      Serial.println("\n[SIMULATOR] Tank is now FULL (100%)");

    } else {
      currentLevelPercent = 0; 
      Serial.println("\n[SIMULATOR] Tank is now EMPTY (0%)");
    }
  }
  
// commented out the  logic for the pressure sens, kept because still usable on a larger tank
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
