// main/pump.ino
// pump 12V 

const int PIN_MOTOR_PWM = 25; //   speed 
const int PIN_MOTOR_DIR = 27; // direction 

const int PUMP_SPEED = 255;  //max speed

void pump_init() {
  pinMode(PIN_MOTOR_DIR, OUTPUT);
  pinMode(PIN_RELAY, OUTPUT);
  
  // PWM Setup: 20kHz, 8-bit resolution
  ledcAttach(PIN_MOTOR_PWM, 20000, 8);

  digitalWrite(PIN_RELAY, HIGH);  // high meanss to start relay as off for safety
  
  digitalWrite(PIN_MOTOR_DIR, LOW); 
  ledcWrite(PIN_MOTOR_PWM, 0);

  Serial.println("[PUMP] System Initialized (Safe Mode: OFF)");
}

void pump_on() {
  digitalWrite(PIN_RELAY, LOW);   // relay on (active low
  ledcWrite(PIN_MOTOR_PWM, PUMP_SPEED);
  Serial.println("[PUMP] ON");
}

void pump_off() {
  ledcWrite(PIN_MOTOR_PWM, 0);
  digitalWrite(PIN_RELAY, HIGH);  // relay off
  Serial.println("[PUMP] OFF");
}

