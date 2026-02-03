  // main.ino — single entry point for the whole sketch

  #define PIN_RELAY 13  // relay pin (used by pump.ino and mqtt.ino)

  bool getSimState();

  void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("\n=== OASIS System Starting... ===\n");

    // 1. Init Communication first (so we can get commands)
    wifi_init();
    mqtt_init();

    // 2. Init Sensors
    moisture_init();
    tl231_init();

    // 3. Init Actuators
    pump_init();
    diverter_init();

    Serial.println("\n=== OASIS System Ready & Online ===\n");
  }

  void loop() {
    moisture_update();
    tl231_update();
    mqtt_update();
    diverter_update();
  }