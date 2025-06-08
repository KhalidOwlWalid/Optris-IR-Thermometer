#pragma once
#include "software_serial_custom.hpp"

enum CommandType {
    CMD_READ_PROCESS_TEMP,
    CMD_READ_HEAD_TEMP,
    CMD_READ_CURR_TARGET_TEMP,
    CMD_READ_CURR_AMBIENT_TEMP,
    CMD_READ_CURR_EMMISIVITY
};

class Optris_IR_Thermometer: public SoftwareSerial {
  public:
    Optris_IR_Thermometer(uint8_t rx_pin, uint8_t tx_pin);
    ~Optris_IR_Thermometer() = default;

    void request_data(CommandType cmd);
    void decode_process_temperature();
 
  private:

    const uint8_t _command_type[5][3] = {
        [CMD_READ_PROCESS_TEMP] = {0x3E, 0x02, 0x00},
        [CMD_READ_HEAD_TEMP] = {0x3E, 0x02, 0x02},
        [CMD_READ_CURR_TARGET_TEMP] = {0x3E, 0x02, 0x04},
        [CMD_READ_CURR_AMBIENT_TEMP] = {0x3E, 0x02, 0x06},
        [CMD_READ_CURR_EMMISIVITY] = {0x3E, 0x02, 0x08},
    };

    bool _initialized;
    bool _read_frame;
    uint8_t _rx_pin;
    uint8_t _tx_pin;
    unsigned long _baudrate;

    float _process_temperature;
    
    // Not implemented
    float _head_temperature;
    float _curr_target_temperature;
    float curr_ambient_temperature;
    float _curr_emmisivity;
};
