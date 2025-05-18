#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial uart(8, 7);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  uart.begin(9600, SERIAL_8N1, true);
}

void loop() {
  if (uart.available()) {
    int byte_val = uart.read();
    Serial.print("Byte value: ");
    Serial.println(byte_val, HEX);
  }
  delay(50);
}
