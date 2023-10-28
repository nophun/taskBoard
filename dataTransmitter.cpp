#include "dataTransmitter.h"

DataTransmitter::DataTransmitter() {
    reset();
}

void DataTransmitter::reset() {
    m_buffer_length = 0;
    m_buffer.fill(0);
}

bool DataTransmitter::serialize(const dataPacket dataPacket) {
    uint8_t checksum = 0;
    reset();
    if (dataPacket.length > dataPacket::cMaxDataSize) {
        return false;
    }
    m_buffer[m_buffer_length++] = dataPacket::cStartByte;
    memcpy(&m_buffer[1], &dataPacket, dataPacket::cHeaderSize + dataPacket.length);
    m_buffer_length += dataPacket::cHeaderSize + dataPacket.length;
    // calculate checksum of all bytes except the start byte and checksum byte
    for (size_t i = 1; i < m_buffer_length; i++) {
        checksum ^= m_buffer[i];
    }
    m_buffer[m_buffer_length++] = checksum;
    return true;
}
