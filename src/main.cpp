#include <Arduino.h>
#include <SoftwareSerial.h>

#define MAX_N_ATTEMPT 5
#define SENS_BAUDRATE 9600

class Optris_IR_Thermometer: public SoftwareSerial {
  public:
    Optris_IR_Thermometer() = default;
    ~Optris_IR_Thermometer() = default;

    void decode_process_temperature();

    byte get_sync_byte() { return _sync_byte; }
    byte get_end_byte() { return _end_byte; }
  
  private:
    bool _initialized;
    uint8_t _rx_pin;
    uint8_t _tx_pin;
    unsigned long _baudrate;
    char _buffer[14];
    float _process_temperature;

    byte _sync_byte = 0xAA;
    byte _end_byte = 0x00;
    
    // Not implemented
    float _head_temperature;
    float _curr_target_temperature;
    float curr_ambient_temperature;
    float _curr_emmisivity;
};

// Since the IR thermometer works on RS232, an RS232-TTL converter is required
// for the voltage logic level to work with the software serial
// Hardware Serial is not being used since it does not support inverted logic
SoftwareSerial ir_thermometer_1(8, 7);

void setup() {
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
      delay(100);
    }
  }
}

void loop() {
  if (ir_thermometer_1.available()) {
    int byte_val = ir_thermometer_1.read();
    Serial.print("Byte value: ");
    Serial.println(byte_val, HEX);
  }
  delay(50);
}
