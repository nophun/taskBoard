#include <Arduino.h>
#include "dataReceiver.h"

DataReceiver::DataReceiver() {
    reset();
}

void DataReceiver::reset() {
    m_state = State::waiting;
    m_buffer_index = 0;
    m_buffer.fill(0);
}

bool DataReceiver::receive_byte(uint8_t byte) {
    if (m_state == State::ready || m_buffer_index >= sizeof(m_buffer)) {
        reset();
    }
    switch(m_state) {
        case State::waiting:
            if (byte == dataPacket::cStartByte) {
                m_state = State::r_header;
            }
            break;
        case State::r_header:
            m_buffer[m_buffer_index++] = byte;
            if (m_buffer_index == dataPacket::cHeaderSize) {
                if (byte != 0U) {
                    m_state = State::r_data;
                } else {
                    m_state = State::r_checksum;
                }
            }
            break;
        case State::r_data:
            m_buffer[m_buffer_index++] = byte;
            if (m_buffer_index == dataPacket::cHeaderSize + m_buffer[offsetof(dataPacket, length)]) {
                m_state = State::r_checksum;
            }
            break;
        case State::r_checksum:
            m_buffer[m_buffer_index++] = byte;
            m_state = State::ready;
            return true;
        default:
            break;
    }
    return false;
}

bool DataReceiver::validate_buffer(void) {
    if (m_state != State::ready) {
        Serial.println("State not ready");
        return false;
    }
    const uint8_t datalen = m_buffer[offsetof(dataPacket, length)];
    if (datalen > dataPacket::cMaxDataSize) {
        Serial.println("Data size too large");
        return false;
    }
    uint8_t checksum = 0;
    // calculate checksum of all bytes, including the checksum byte
    for (size_t i = 0; i < dataPacket::cHeaderSize + datalen + 1U; ++i) {
        checksum ^= m_buffer[i];
    }
    // check if checksum is correct
    return checksum == 0x00;
}

bool DataReceiver::deserialize(dataPacket& packet) {
    if (!validate_buffer()) {
        reset();
        return false;
    }
    packet.clear();
    memcpy(&packet, m_buffer.data(), sizeof(packet));
    return true;
}