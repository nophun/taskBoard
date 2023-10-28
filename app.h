#pragma once

#include <Arduino.h>
#include "dataPacket.h"
#include "display.h"

class App {
public:
    static constexpr uint8_t cBroadcastAddress = 0x00;
    static constexpr uint8_t cInitializeAddress = 0xFF;
    enum class Command {
        ping,
        set_address,
        reset_address,
        set_status,
        set_title,
        set_classification,
        set_description,
        set_counter,
        get_counter
    };

    explicit App(Display* display) : m_display(display) {}
    ~App() = default;
    App(const App&) = delete;
    App(App&&) = delete;
    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    bool handle_data_packet(const dataPacket& request, dataPacket& response);
    bool handle_directed_msg(const dataPacket& request, dataPacket& response);
    bool handle_broadcast_msg(const dataPacket& request, dataPacket& response);

    void set_my_address(uint8_t address) { m_address = address; }
    uint8_t get_my_address(void) const { return m_address; }
    void init_response(dataPacket& response, uint8_t src_address, Command command) const {
        response.init(m_address, src_address, static_cast<uint8_t>(command));
    }

    /* Command handlers */
    bool ping(const dataPacket& request, dataPacket& response) const;
    bool set_address(const dataPacket& request, dataPacket& response);
    bool reset_address(const dataPacket& request, dataPacket& response);
    bool set_status(const dataPacket& request, dataPacket& response);
    bool set_title(const dataPacket& request, dataPacket& response);
    bool set_classification(const dataPacket& request, dataPacket& response);
    bool set_description(const dataPacket& request, dataPacket& response);
    bool set_counter(const dataPacket& request, dataPacket& response);
    bool get_counter(const dataPacket& request, dataPacket& response) const;

    /* Other interactions */
    void increase_counter();

private:
    Display* m_display;
    uint8_t m_address {cInitializeAddress};
    std::string m_title;
    std::string m_description;
    std::string m_classification;
    std::array<uint8_t, 3U> m_status{};
    uint8_t m_counter{};
    uint8_t m_maximum{};
};