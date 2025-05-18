#include <Arduino.h>
#include <SoftwareSerial.h>

#define UART
// #define UART2

#ifdef UART
SoftwareSerial uart(8, 7);
#endif

#ifdef UART2
SoftwareSerial uart2(3, 2);
#endif

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
#ifdef UART
  uart.begin(9600, SERIAL_8N1, true);
#endif

#ifdef UART2
  uart2.begin(9600, SERIAL_8N1, false);
#endif

}

void loop() {
  // Serial1.print("H");
  uart.print("H");

#ifdef UART
  if (uart.available()) {
    int byte_val = uart.read();
    Serial.print("Byte value: ");
    Serial.println(byte_val, HEX);
  }
#endif

#ifdef UART2
  if (uart2.available()) {
    Serial.print("uart2 - ");
    Serial.print("Bytes available: ");
    Serial.print(uart2.available());
    Serial.println("");
    int byte_val = uart2.read();
    Serial.println((char)byte_val);
  }
#endif

  if (Serial1.available()) {
    Serial.print("Serial1 - ");
    Serial.print("Bytes available: ");
    Serial.print(Serial1.available());
    Serial.println("");
    int byte_val = Serial1.read();
    Serial.println((char)byte_val);
  }
  delay(50);
  // put your main code here, to run repeatedly:
}