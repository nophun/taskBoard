#include <Arduino.h>
#include <string>
#include "dataReceiver.h"
#include "dataTransmitter.h"
#include "dataWrapper.h"
#include "app.h"
#include "display.h"

EPDTYPE epd(EPDPANEL(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
DataWrapper data;
Display display(&epd);
App app(&display);
static dataPacket packet_request = {};
static dataPacket packet_response = {};
static const std::string new_title = "MVP";
static const std::string new_classification = "HART";
static const std::string new_description = "Minimum implementation";

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1);
    Serial.println("start");

    display.init();

    // set "Hello" as title to display
    display.set_title("Hello");
}

void loop() {
    static uint8_t rcv;

    if (Serial1.available() && data.receive(static_cast<uint8_t>(Serial1.read()))) {
        if (data.get_packet(packet_request)) {
            packet_request.print();
            if (app.get_my_address() == 0x00) {
                // I'm master, dont handle received packet
                return;
            }
            if (app.handle_data_packet(packet_request, packet_response)) {
                data.transmit(packet_response);
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
        case 'p':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::ping);
            packet_request.length = 0x00;
            data.transmit(packet_request);
            break;
        case 'b':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::set_status);
            packet_request.length = 0x03;
            packet_request.status_data.status1 = 0x04;
            packet_request.status_data.status2 = 0x05;
            packet_request.status_data.status3 = 0x06;
            data.transmit(packet_request);
            break;
        case 'v':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_status);
            packet_request.length = 0x03;
            packet_request.status_data.status1 = 0x07;
            packet_request.status_data.status2 = 0x08;
            packet_request.status_data.status3 = 0x09;
            data.transmit(packet_request);
            break;
        case 's':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::set_address);
            packet_request.length = 0x01;
            packet_request.addr_data.address = 0x02;
            data.transmit(packet_request);
            break;
        case 'r':
            packet_request.source = app.get_my_address();
            packet_request.address = App::cBroadcastAddress;
            packet_request.command = static_cast<uint8_t>(App::Command::reset_address);
            packet_request.length = 0x00;
            data.transmit(packet_request);
            break;
        case 't':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_title);
            packet_request.length = static_cast<uint8_t>(new_title.length());
            strcpy(packet_request.title_data.title_str.data(), new_title.data());
            data.transmit(packet_request);
            break;
        case 'c':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_classification);
            packet_request.length = static_cast<uint8_t>(new_classification.length());
            strcpy(packet_request.class_data.class_str.data(), new_classification.data());
            data.transmit(packet_request);
            break;
        case 'd':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_description);
            packet_request.length = static_cast<uint8_t>(new_description.length());
            strcpy(packet_request.desc_data.desc_str.data(), new_description.data());
            data.transmit(packet_request);
            break;
        case 'n':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::set_counter);
            packet_request.length = 2U;
            packet_request.counter_data.counter = 1U;
            packet_request.counter_data.maximum = 10U;
            data.transmit(packet_request);
            break;
        case 'N':
            packet_request.source = app.get_my_address();
            packet_request.address = 0x02;
            packet_request.command = static_cast<uint8_t>(App::Command::get_counter);
            packet_request.length = 0U;
            data.transmit(packet_request);
            break;
        case 'i':
            app.increase_counter();
            break;

        default:
            break;
        }
        rcv = 0;
    }
    delay(1);
}
