#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(10, 30, "OASIS");
  u8g2.drawStr(15, 50, "Ready!");
  u8g2.sendBuffer();
  
  Serial.println("OASIS Display test started");
  delay(2000);
}

void loop() {
  static int moisture = 20;
  moisture = (moisture + 5) % 100;
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 15, "OASIS Sensor");
  u8g2.drawStr(0, 30, "Soil Moisture:");
  
  u8g2.setFont(u8g2_font_ncenB14_tr);
  char buffer[10];
  sprintf(buffer, "%d%%", moisture);
  u8g2.drawStr(30, 50, buffer);
  
  u8g2.sendBuffer();
  
  Serial.print("Soil Moisture: ");
  Serial.print(moisture);
  Serial.println("%");
  
  delay(1000);
}
