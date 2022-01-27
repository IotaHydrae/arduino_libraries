#include <Arduino.h>
#line 1 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino"
// #include <Wire.h>
#include "SSD1306.h"

#line 4 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino"
void setup();
#line 8 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino"
void loop();
#line 4 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino"
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
