#pragma once

#include <Arduino.h>
#include <array>
#include "dataPacket.h"

class DataReceiver {
private:
    static constexpr size_t cBufferSize = dataPacket::maxSize() + 1U; // +1 for the checksum
    enum class State {
        waiting,
        r_header,
        r_data,
        r_checksum,
        ready
    };

public:
    DataReceiver();
    ~DataReceiver() = default;
    DataReceiver(const DataReceiver&) = delete;
    DataReceiver(DataReceiver&&) = delete;
    DataReceiver& operator=(const DataReceiver&) = delete;
    DataReceiver& operator=(DataReceiver&&) = delete;

    bool receive_byte(uint8_t byte);
    bool deserialize(dataPacket& packet);

protected:
    void reset();

private:
    bool validate_buffer(void);

    State m_state{State::waiting};
    std::array<uint8_t, cBufferSize> m_buffer;
    size_t m_buffer_index{};
};