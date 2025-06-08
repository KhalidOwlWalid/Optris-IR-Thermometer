#include "optris_ir_thermometer.hpp"

Optris_IR_Thermometer::Optris_IR_Thermometer(uint8_t rx_pin, uint8_t tx_pin)
: SoftwareSerial(rx_pin, tx_pin) {
}

void Optris_IR_Thermometer::request_data(CommandType cmd) {
    // Fix the size of the iteration to 3, since we know that in the datasheet
    // the number of bytes for each commands are 3
    for (size_t i=0; i < 3; i++) {
        write(_command_type[cmd][i]);
    }
}