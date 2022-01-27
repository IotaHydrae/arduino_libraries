# 1 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino"
// #include <Wire.h>
# 3 "d:\\Softerware_Location\\Arduino\\libraries\\NixieClockLite\\examples\\Blink\\Blink.ino" 2

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
