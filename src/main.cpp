#include <Arduino.h>
#include "optris_ir_thermometer.hpp"

#define MAX_N_ATTEMPT 5
#define SENS_BAUDRATE 9600

// Since the IR thermometer works on RS232, an RS232-TTL converter is required
// for the voltage logic level to work with the software serial
// Hardware Serial is not being used since it does not support inverted logic
Optris_IR_Thermometer ir_thermometer_1(8, 7);
static unsigned long last_update_ms;

void setup() {

  last_update_ms = millis();
  Serial.begin(9600);
  
  // Initialize the baudrate of the IR thermometer with 9600 baudrate
  // The RS232 is also inverted
  // Sensor will attempt to initialize a few times, if failed then just continue
  size_t n_init_attempt = MAX_N_ATTEMPT;
  while (n_init_attempt > 0) {
    if (ir_thermometer_1.begin(SENS_BAUDRATE, SERIAL_8N1, true)) {
      Serial.println("IR Thermometer Serial initialized!");
    } else {
      Serial.println("Fails to initialize the IR thermometer sensor. Retrying...");
      n_init_attempt--;
      // TODO (Khalid): Figure out a reasonable delay value
      delay(1000);
    }
  }
}


void loop() {
  float data = ir_thermometer_1.request_data(CMD_READ_PROCESS_TEMP);
  Serial.println(data);
  delay(10);
}
