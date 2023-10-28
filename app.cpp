#include "app.h"

bool App::handle_data_packet(const dataPacket& request, dataPacket& response) {
    if (request.address == m_address && request.source != m_address) {
        switch (static_cast<Command>(request.command)) {
            case Command::ping:
                return ping(request, response);
            case Command::set_address:
                return set_address(request, response);
            case Command::reset_address:
                return reset_address(request, response);
            case Command::set_status:
                return set_status(request, response);
            case Command::set_title:
                return set_title(request, response);
            case Command::set_classification:
                return set_classification(request, response);
            case Command::set_description:
                return set_description(request, response);
            case Command::set_counter:
                return set_counter(request, response);
            case Command::get_counter:
                return get_counter(request, response);
            default:
                return false;
        }
    } else if (request.address == cBroadcastAddress) {
        switch (static_cast<Command>(request.command)) {
            case Command::set_address:
                // only set address if input pin is true
                if (true) {
                    set_address(request, response);
                }
                break;
            case Command::reset_address:
                reset_address(request, response);
                break;
            default:
                break;
        }
        return false; // don't respond to broadcast
    }
    return false;
}

bool App::ping(const dataPacket& request, dataPacket& response) const {
    init_response(response, request.source, Command::ping);
    Serial.println("ping");
    return true;
}

bool App::set_address(const dataPacket& request, dataPacket& response) {
    if (request.length != sizeof(request.addr_data)) {
        return false;
    }
    Serial.print("set address ");
    Serial.println(request.addr_data.address);

    init_response(response, request.source, Command::set_address);
    if (request.addr_data.address != cBroadcastAddress && request.addr_data.address != cInitializeAddress) {
        // Invalid address
        m_address = request.addr_data.address;
    }
    response.length = sizeof(request.addr_data);
    response.addr_data.address = m_address;
    return true;
}

bool App::reset_address(const dataPacket& request, dataPacket& response) {
    Serial.println("reset address ");

    set_my_address(cInitializeAddress);

    init_response(response, request.source, Command::reset_address);
    response.length = sizeof(request.addr_data);
    response.addr_data.address = m_address;
    return true;
}

bool App::set_status(const dataPacket& request, dataPacket& response) {
    if (request.length != sizeof(request.status_data)) {
        return false;
    }
    Serial.print("set status ");
    Serial.print(request.status_data.status1);
    Serial.print(" ");
    Serial.print(request.status_data.status2);
    Serial.print(" ");
    Serial.println(request.status_data.status3);

    m_display->set_status(request.status_data.status1, request.status_data.status2, request.status_data.status3);
    m_status[0] = request.status_data.status1;
    m_status[1] = request.status_data.status2;
    m_status[2] = request.status_data.status3;

    init_response(response, request.source, Command::set_status);
    return true;
}

bool App::set_title(const dataPacket& request, dataPacket& response) {
    m_display->set_title(request.title_data.title_str.data(), request.length);
    m_title = std::string(request.title_data.title_str.data(), request.length);

    init_response(response, request.source, Command::set_title);
    return true;
}

bool App::set_classification(const dataPacket& request, dataPacket& response) {
    m_display->set_classification(request.class_data.class_str.data(), request.length);
    m_classification = std::string(request.class_data.class_str.data(), request.length);

    init_response(response, request.source, Command::set_classification);
    return true;
}

bool App::set_description(const dataPacket& request, dataPacket& response) {
    m_display->set_description(request.desc_data.desc_str.data(), request.length);
    m_description = std::string(request.desc_data.desc_str.data(), request.length);

    init_response(response, request.source, Command::set_description);
    return true;
}

bool App::set_counter(const dataPacket& request, dataPacket& response) {
    if (request.length != sizeof(request.counter_data)) {
        return false;
    }

    m_display->set_counter(request.counter_data.counter, request.counter_data.maximum);
    m_counter = request.counter_data.counter;
    m_maximum = request.counter_data.maximum;

    init_response(response, request.source, Command::set_counter);
    return true;
}

bool App::get_counter(const dataPacket& request, dataPacket& response) const {
    init_response(response, request.source, Command::get_counter);

    response.length = 2U;
    response.counter_data.counter = m_counter;
    response.counter_data.maximum = m_maximum;
    return true;
}

void App::increase_counter() {
    if (m_counter < m_maximum) {
        m_counter++;
    }
    m_display->set_counter(m_counter, m_maximum);
}
