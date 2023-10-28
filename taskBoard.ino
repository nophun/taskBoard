#include <Arduino.h>
#include "dataReceiver.h"
#include "dataTransmitter.h"
#include "dataWrapper.h"

DataWrapper data;
static constexpr dataPacket packet1 = {
    .address = 0x01,
    .command = 0x02,
    .length = 0x03,
    .data = {0x04, 0x05, 0x06}
};

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1);
    Serial.println("start");
}

void loop() {
    uint8_t rcv;

    if (Serial1.available()) {
        if (data.receive(static_cast<uint8_t>(Serial1.read()))) {
            dataPacket packet;
            if (data.get_packet(packet)) {
                packet.print();
            } else {
                Serial.println("invalid packet");
            }
        }
    }

    if (Serial.available()) {
        rcv = static_cast<uint8_t>(Serial.read());
        switch (rcv) {
        case 'a':
            if (data.transmit(packet1)) {
                Serial.println("transmitted");
            } else {
                Serial.println("failed to transmit");
            }
            break;

        default:
            break;
        }
        rcv = 0;
    }
    delay(1);
}
