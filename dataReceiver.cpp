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
                m_state = State::r_address;
            }
            break;
        case State::r_address:
            m_buffer[m_buffer_index++] = byte;
            m_state = State::r_command;
            break;
        case State::r_command:
            m_buffer[m_buffer_index++] = byte;
            m_state = State::r_length;
            break;
        case State::r_length:
            m_buffer[m_buffer_index++] = byte;
            m_state = State::r_data;
            break;
        case State::r_data:
            m_buffer[m_buffer_index++] = byte;
            if (m_buffer_index == m_buffer[2] + 3) {
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
    if (m_buffer[2] > dataPacket::cMaxDataSize) {
        Serial.println("Data size too large");
        return false;
    }
    uint8_t checksum = 0;
    // calculate checksum of all bytes
    for (size_t i = 0; i < m_buffer[2] + 4; ++i) {
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