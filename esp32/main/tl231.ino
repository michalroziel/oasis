// main/tl231.ino — Water Level Sensor Module (Calibrated)

const int PIN_LEVEL_SENSOR = 34; 

// -- VALUES FOR EMPTY AND FULL --

const int LEVEL_EMPTY = 20;  
const int LEVEL_FULL  = 1000; 

static int currentLevelPercent = 0;
static int lastLevelRaw = 0;

void tl231_init() {
  pinMode(PIN_LEVEL_SENSOR, INPUT);
  Serial.println(" TL231 initilized - Starting ");
}

void tl231_update() {
  static unsigned long lastRead = 0;
  
  // Read every 500ms

  if (millis() - lastRead > 500) {
    lastRead = millis();

    // as our tank is small and we cannot amplify the 
    // signal further, we average
    // 1. Take 10 readings and average them - smooth 

    long sum = 0;

    for(int i=0; i<10; i++){

      int reading = analogRead(PIN_LEVEL_SENSOR);

      sum +=  reading;
      delay(5); 
    }

    int averageRaw = (int)(sum / 10.0);
    
    lastLevelRaw = averageRaw;

    // Just like the CMS convert into percent
    // 
    int percent = map(averageRaw, LEVEL_EMPTY, LEVEL_FULL, 0, 100);
    currentLevelPercent = constrain(percent, 0, 100);

    // Print for debugging
    Serial.print("[ TL 231 ] Raw: ");
    Serial.print(averageRaw);
    Serial.print(" | Level: ");
    Serial.print(currentLevelPercent);
    Serial.println("%");
  }
}

// Helper function for other modules to read TL231

int tl231_getLevel() {
  return currentLevelPercent;
}

int tl231_getRaw(){
  return lastLevelRaw;
}