// #include <Wire.h>
#include "SSD1306.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Uart openned.");
}
void loop() {
  Serial.println("main loop");
  ssd1306.test();
  Serial.println("after test");
  delay(500);
}