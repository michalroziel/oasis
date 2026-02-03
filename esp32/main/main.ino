  // main.ino single entry point for the whole sketch

  #define PIN_RELAY 13  // relay pin (used by pump.ino and mqtt.ino)

  bool getSimState();

  void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println("\n=== OASIS System Starting... ===\n");

    wifi_init();
    mqtt_init();

    moisture_init();
    tl231_init();

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
