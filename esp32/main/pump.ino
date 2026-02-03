// main/pump.ino
// Simple Variable Speed Pump & Relay
// the pump we are using is 12V DC

// and : using MAX14870 Polulu Motor Driver

// - -  PIN DEFINITIONS - - 
// PIN_RELAY is defined in main.ino

const int PIN_MOTOR_PWM = 25; // Speed : MAX14870 PWM
const int PIN_MOTOR_DIR = 27; // Direction  : MAX14870 DIR

// - - set the  pump - - 
// Speed: 0 (Halt) to 255 (100 %)

const int PUMP_SPEED = 255; 

void pump_init() {
  // Setup Pins
  pinMode(PIN_MOTOR_DIR, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  
  // PWM Setup: 20kHz, 8-bit resolution
  ledcAttach(PIN_MOTOR_PWM, 20000, 8);

  // --- SAFETY INITIALIZATION ---
  // Since your relay is Active LOW (LOW = ON), we write HIGH to start it OFF.
  digitalWrite(PIN_RELAY, HIGH); 
  
  // Ensure Motor Driver is also 0
  digitalWrite(PIN_MOTOR_DIR, LOW); 
  ledcWrite(PIN_MOTOR_PWM, 0);

  Serial.println("[PUMP] System Initialized (Safe Mode: OFF)");
}

void pump_on() {
  digitalWrite(PIN_RELAY, LOW);   // relay ON (active LOW)
  ledcWrite(PIN_MOTOR_PWM, PUMP_SPEED);
  Serial.println("[PUMP] ON");
}

void pump_off() {
  ledcWrite(PIN_MOTOR_PWM, 0);
  digitalWrite(PIN_RELAY, HIGH);  // relay OFF
  Serial.println("[PUMP] OFF");
}

