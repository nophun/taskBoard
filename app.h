#pragma once

#include <Arduino.h>
#include "dataPacket.h"

class App {
public:
    App() = default;
    ~App() = default;

    bool handle_data_packet(const dataPacket& request, dataPacket& response);
    void set_my_address(uint8_t address) {
        m_address = address;
    }
    uint8_t get_my_address(void) const {
        return m_address;
    }

    bool cmd0_ping(const dataPacket& request, dataPacket& response) const;
    bool cmd1_set_address(const dataPacket& request, dataPacket& response);
    bool cmd2_set_status(const dataPacket& request, dataPacket& response) const;

    enum class Command {
        ping,
        set_address,
        set_status,
        set_title,
        set_description,
        set_counter
    };

private:
    static constexpr uint8_t cBroadcastAddress = 0x00;
    uint8_t m_address {255U};
};