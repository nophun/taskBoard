#include "app.h"

bool App::handle_data_packet(const dataPacket& request, dataPacket& response) {
    if (request.address == m_address && request.source != m_address) {
        switch (static_cast<Command>(request.command)) {
            case Command::ping:
                return cmd0_ping(request, response);
            case Command::set_address:
                return cmd1_set_address(request, response);
            case Command::reset_address:
                return cmd2_reset_address(request, response);
            case Command::set_status:
                return cmd3_set_status(request, response);
            case Command::set_title:
                break;
            case Command::set_description:
                break;
            case Command::set_counter:
                break;
            default:
                return false;
        }
    } else if (request.address == cBroadcastAddress) {
        switch (static_cast<Command>(request.command)) {
            case Command::set_address:
                // only set address if input pin is true
                if (true) {
                    cmd1_set_address(request, response);
                }
                break;
            case Command::reset_address:
                cmd2_reset_address(request, response);
                break;
            default:
                break;
        }
        return false; // don't respond to broadcast
    }
    return false;
}

bool App::cmd0_ping(const dataPacket& request, dataPacket& response) const {
    Serial.println("ping");
    response.source = m_address;
    response.address = request.source;
    response.command = request.command;
    response.length = 0U;
    return true;
}

bool App::cmd1_set_address(const dataPacket& request, dataPacket& response) {
    if (request.length != sizeof(request.cmd1_req)) {
        return false;
    }
    Serial.print("set address ");
    Serial.println(request.cmd1_req.address);
    if (request.cmd1_req.address != 0U && request.cmd1_req.address != 255U) {
        // Invalid address
        m_address = request.cmd1_req.address;
    }
    response.source = m_address;
    response.address = request.source;
    response.command = request.command;
    response.length = sizeof(request.cmd1_resp);
    response.cmd1_resp.address = m_address;
    return true;
}

bool App::cmd2_reset_address(const dataPacket& request, dataPacket& response) {
    Serial.println("reset address ");
    set_my_address(255U);
    response.source = m_address;
    response.address = request.source;
    response.command = request.command;
    response.length = sizeof(request.cmd2_resp);
    response.cmd2_resp.address = m_address;
    return true;
}

bool App::cmd3_set_status(const dataPacket& request, dataPacket& response) const {
    if (request.length != sizeof(request.cmd3_req)) {
        return false;
    }
    Serial.print("set status ");
    Serial.print(request.cmd3_req.status1);
    Serial.print(" ");
    Serial.print(request.cmd3_req.status2);
    Serial.print(" ");
    Serial.println(request.cmd3_req.status3);
    response.source = m_address;
    response.address = request.source;
    response.command = request.command;
    response.length = 0U;
    return true;
}