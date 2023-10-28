#pragma once

#include <Arduino.h>
#include <array>
#include "dataPacket.h"

class DataTransmitter {
private:
    static constexpr size_t cBufferSize = dataPacket::maxSize() + 2U; // +2 for start byte and checksum

public:
    DataTransmitter();
    ~DataTransmitter() = default;
    DataTransmitter(const DataTransmitter&) = delete;
    DataTransmitter(DataTransmitter&&) = delete;
    DataTransmitter& operator=(const DataTransmitter&) = delete;
    DataTransmitter& operator=(DataTransmitter&&) = delete;

protected:
    void reset();
    bool serialize(const dataPacket dataPacket);
    uint8_t* get_buffer() { return m_buffer.data(); }
    size_t get_buffer_length() const { return m_buffer_length; }

private:
    std::array<uint8_t, cBufferSize> m_buffer;
    size_t m_buffer_length{};
};