#include <Arduino.h>
#include <SoftwareSerial.h>

#define MAX_N_ATTEMPT 5
#define SENS_BAUDRATE 9600

class Optris_IR_Thermometer: public SoftwareSerial {
  public:
    Optris_IR_Thermometer() = default;
    ~Optris_IR_Thermometer() = default;

    void decode_process_temperature();

    static const byte sync_byte = 0xAA;
    static const byte end_byte = 0x00;

    // Frame size which contains useful data consists of 14 bytes with another 4 bytes being
    // sync frame and end frame with 2 bytes for each frame
    static const size_t expected_frame_size = 18;
    static const size_t expected_data_frame_size = 14;
 
  private:
    bool _initialized;
    bool _read_frame;
    uint8_t _rx_pin;
    uint8_t _tx_pin;
    unsigned long _baudrate;
    

    char _buffer[expected_data_frame_size];
    float _process_temperature;
    
    // Not implemented
    float _head_temperature;
    float _curr_target_temperature;
    float curr_ambient_temperature;
    float _curr_emmisivity;
};

// Shortening the alias to the Optris class
using Thermometer = Optris_IR_Thermometer;

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
  if (ir_thermometer_1.available() > Thermometer::expected_frame_size) {
    // We would only want to read when we receive the sync bytes 0xAAAA
    bool read_frame = false;
    int sync_frame_count = 2;
    if (ir_thermometer_1.peek() == 0xAA) {
      // Remove the sync byte frame from the buffer by reading it
      while (sync_frame_count > 0) {
        if (ir_thermometer_1.read() == 0xAA) {
          // Decrement the count if we found the sync frame
          sync_frame_count--;
        } else {
          // If the byte found is not the sync frame, then assume framing error 
          break;
        }
        
        if (sync_frame_count == 0) {
          // Once the sync frame reaches zero, simply lock the loop to read the buffer
          read_frame = true;
          // Break out of the while loop to start reading the data
          break;
        }
      }

      byte data_buf[14];
      size_t n = 0;
      while (read_frame) {
        // TODO (Khalid): It is likely that one of the bytes are actually 0x00, so make sure
        // to check two bytes of 0x00
        if (ir_thermometer_1.peek() == 0x00) {
          read_frame = false;
          // Just break out of the loop if we know it is the end of the frame
          break;
        }

        // Fill a temporary buffer with the received data
        // There should be a total of 14 bytes
        data_buf[n] = ir_thermometer_1.read(); 
        n++;
      }

      if (n < Thermometer::expected_data_frame_size) {
        Serial.println("There is an error with the data received!");
        Serial.print(n);
        Serial.print(" Data received instead");
        Serial.println(" ");
      } else {
        // The first 2 bytes are the process temperature reading
        Serial.print("Temperature: ");
        for (size_t i = 0; i < 2; i++) {
          Serial.print(data_buf[i], HEX);
          Serial.print(" ");
        }
        Serial.println(" ");
      }

    }
    else {
      ir_thermometer_1.read();
      // Serial.print("Flushing the useless byte: ");
      // Serial.print(ir_thermometer_1.read(), HEX);
      // Serial.println("");
    }

  }
  delay(100);
}
