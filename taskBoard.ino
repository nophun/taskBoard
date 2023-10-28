#include <Arduino.h>
#include "dataReceiver.h"
#include "dataTransmitter.h"
#include "dataWrapper.h"
#include "app.h"

DataWrapper data;
App app;
dataPacket packet_request = {};
dataPacket packet_response = {};

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1);
    Serial.println("start");
}

void loop() {
    uint8_t rcv;

    if (Serial1.available() && data.receive(static_cast<uint8_t>(Serial1.read()))) {
        if (data.get_packet(packet_request)) {
            packet_request.print();
            if (app.get_my_address() == 0x00) {
                // I'm master, dont handle received packet
            } else {
                if (app.handle_data_packet(packet_request, packet_response)) {
                    data.transmit(packet_response);
                }
            }
        } else {
            Serial.println("invalid packet");
        }
    }

    if (Serial.available()) {
        rcv = static_cast<uint8_t>(Serial.read());
        switch (rcv) {
        case 'm':
            app.set_my_address(0x00);
            Serial.print("I'm master now!");
            break;
        case 'a':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::ping);
            packet_request.length = 0x00;
            data.transmit(packet_request);
            break;
        case 'c':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::set_status);
            packet_request.length = 0x03;
            packet_request.cmd3_req.status1 = 0x04;
            packet_request.cmd3_req.status2 = 0x05;
            packet_request.cmd3_req.status3 = 0x06;
            data.transmit(packet_request);
            break;
        case 'd':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_status);
            packet_request.length = 0x03;
            packet_request.cmd3_req.status1 = 0x07;
            packet_request.cmd3_req.status2 = 0x08;
            packet_request.cmd3_req.status3 = 0x09;
            data.transmit(packet_request);
            break;
        case 's':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::set_address);
            packet_request.length = 0x01;
            packet_request.cmd1_req.address = 0x02;
            data.transmit(packet_request);
            break;
        case 'r':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::reset_address);
            packet_request.length = 0x00;
            data.transmit(packet_request);
            break;

        default:
            break;
        }
        rcv = 0;
    }
    delay(1);
}
