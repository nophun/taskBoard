#pragma once

#include <Arduino.h>
#include "dataTransmitter.h"
#include "dataReceiver.h"
#include "dataPacket.h"

class DataWrapper : public DataTransmitter, public DataReceiver {
public:
    DataWrapper() : DataTransmitter(), DataReceiver() {}
    ~DataWrapper() = default;
    DataWrapper(const DataWrapper&) = delete;
    DataWrapper(DataWrapper&&) = delete;
    DataWrapper& operator=(const DataWrapper&) = delete;
    DataWrapper& operator=(DataWrapper&&) = delete;

    bool receive(uint8_t byte) {
        return DataReceiver::receive_byte(byte);
    }
    bool get_packet(dataPacket& packet) {
        return DataReceiver::deserialize(packet);
    } 
    bool transmit(const dataPacket packet) {
        if (DataTransmitter::serialize(packet)) {
            Serial1.write(DataTransmitter::get_buffer(),
                DataTransmitter::get_buffer_length());
            return true;
        }
        return false;
    }
};