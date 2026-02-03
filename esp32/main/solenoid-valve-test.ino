// main/solenoid-valve-test.ino
// Module to test ONLY the 3-Way Valve (No Pump)

// - -  UNIQUE PIN DEFINITION - - 

const int TEST_VALVE_PIN = 14; 

// - -  UNIQUE VARIABLES - - 

static unsigned long valveTestTimer = 0;  
static bool valveTestState = false;      

void valve_only_init() {
  pinMode(TEST_VALVE_PIN, OUTPUT);
  
  // Start with Valve OFF 
  digitalWrite(TEST_VALVE_PIN, HIGH);
  
  Serial.println("\n - - 3way Valve Test Module (No Pump) - - ");
  Serial.println("Listening for a Click Soundd every 3 seconds...");
  
  valveTestTimer = millis();
}

void valve_only_update() {
  unsigned long now = millis();
  
  // Toggle every 5 seconds

  if (now - valveTestTimer >= 5000) {
    valveTestTimer = now;
    valveTestState = !valveTestState;

    if (valveTestState) {
      
      digitalWrite(TEST_VALVE_PIN, LOW); 
      Serial.println("> Valve ON - Route [B] ");

    } else {

      
      digitalWrite(TEST_VALVE_PIN, HIGH);
      Serial.println("> Valve OFF - Route [A] ");

    }
  }
}