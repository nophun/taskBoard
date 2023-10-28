#pragma once

#include <Arduino.h>
#include <array>

struct addr_packet {
    uint8_t address;
};

struct status_packet {
    uint8_t status1;
    uint8_t status2;
    uint8_t status3;
};

struct title_packet {
    static constexpr size_t cMaxTitleSize = 32U;
    std::array<char, cMaxTitleSize> title_str;
};

struct class_packet {
    static constexpr size_t cMaxClassificationSize = 16U;
    std::array<char, cMaxClassificationSize> class_str;
};

struct desc_packet {
    static constexpr size_t cMaxDescriptionSize = 128U;
    std::array<char, cMaxDescriptionSize> desc_str;
};

struct counter_packet {
    uint8_t counter;
    uint8_t maximum;
};

struct dataPacket {
    static constexpr size_t cMaxDataSize = 128U;
    static constexpr size_t cHeaderSize = 4U;
    static constexpr uint8_t cStartByte = 0x55;
    uint8_t source;
    uint8_t address;
    uint8_t command;
    uint8_t length;
    union {
        std::array<uint8_t, cMaxDataSize> data;
        struct addr_packet      addr_data;
        struct status_packet    status_data;
        struct title_packet     title_data;
        struct class_packet     class_data;
        struct desc_packet      desc_data;
        struct counter_packet   counter_data;
    };

    void clear() {
        source = 0U;
        address = 0U;
        command = 0U;
        length = 0U;
        memset(data.data(), 0U, cMaxDataSize);
    }
    void init(uint8_t src, uint8_t addr, uint8_t cmd) {
        clear();
        source = src;
        address = addr;
        command = cmd;
    }
    static constexpr uint8_t maxSize() {
        return cHeaderSize + cMaxDataSize;
    }
    void print() const {
        Serial.print(source);
        Serial.print(" -> ");
        Serial.print(address);
        Serial.print(" cmd: ");
        Serial.print(command);
        Serial.print(" len: ");
        Serial.print(length);
        Serial.print(" [");
        if (length != 0U) {
            for (size_t i = 0; i < length - 1; i++) {
                Serial.print(data[i], HEX);
                Serial.print(" ");
            }
            Serial.print(data[length - 1], HEX);
        }
        Serial.println("]");
    }
};
