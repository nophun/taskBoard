#pragma once

#include <Arduino.h>
#include <array>

struct dataPacket {
    static constexpr size_t cMaxDataSize = 8U;
    static constexpr uint8_t cStartByte = 0x55;
    uint8_t address;
    uint8_t command;
    uint8_t length;     // 0-8
    std::array<uint8_t, cMaxDataSize> data;

    void clear() {
        address = 0U;
        command = 0U;
        length = 0U;
        memset(data.data(), 0U, cMaxDataSize);
    }
    static constexpr uint8_t maxSize() {
        return 3 + cMaxDataSize;
    }
    void print() const {
        Serial.print("a: ");
        Serial.print(address);
        Serial.print(" cmd: ");
        Serial.print(command);
        Serial.print(" len: ");
        Serial.print(length);
        Serial.print(" [");
        for (size_t i = 0; i < length - 1; i++) {
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.print(data[length - 1], HEX);
        Serial.println("]");
    }
}; // size: 11 bytes
