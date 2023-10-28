#pragma once

#include <Arduino.h>
#include <array>

struct cmd1_req {
    uint8_t address;
};

struct cmd1_resp {
    uint8_t address;
};

struct cmd2_resp {
    uint8_t address;
};

struct cmd3_req {
    uint8_t status1;
    uint8_t status2;
    uint8_t status3;
};

struct cmd3_resp {
    uint8_t status1;
    uint8_t status2;
    uint8_t status3;
};

struct dataPacket {
    static constexpr size_t cMaxDataSize = 8U;
    static constexpr size_t cHeaderSize = 4U;
    static constexpr uint8_t cStartByte = 0x55;
    uint8_t source;
    uint8_t address;
    uint8_t command;
    uint8_t length;
    union {
        std::array<uint8_t, cMaxDataSize> data;
        struct cmd1_req cmd1_req;   struct cmd1_resp cmd1_resp;
                                    struct cmd2_resp cmd2_resp;
        struct cmd3_req cmd3_req;   struct cmd3_resp cmd3_resp;
    };

    void clear() {
        address = 0U;
        command = 0U;
        length = 0U;
        memset(data.data(), 0U, cMaxDataSize);
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
}; // size: 11 bytes
