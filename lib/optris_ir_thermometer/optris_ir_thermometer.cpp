#include "optris_ir_thermometer.hpp"

Optris_IR_Thermometer::Optris_IR_Thermometer(uint8_t rx_pin, uint8_t tx_pin)
: SoftwareSerial(rx_pin, tx_pin) {
}

float Optris_IR_Thermometer::request_data(CommandType cmd) {
    // Fix the size of the iteration to 3, since we know that in the datasheet
    // the number of bytes for each commands are 3
    for (size_t i=0; i < 3; i++) {
        write(_command_type[cmd][i]);
    }
    float data = get_process_temperature();
    return data;
}

uint16_t Optris_IR_Thermometer::get_encoded_process_temperature() {
    uint16_t data_bytes; 
    if (available() > 0) {
        int msb_byte = read();
        int lsb_byte = read();
        data_bytes = ((msb_byte << (8)) | (lsb_byte));
    }
    return data_bytes;
}

float Optris_IR_Thermometer::get_process_temperature() {
    uint16_t encoded_data = get_encoded_process_temperature();

    float data = ((float)(encoded_data) - 1000)/10;
    // float data = ((float)(encoded_data) - 1000)/10;
    return data;
}