#define MY_SERVER_PORT 80

#include "taskBoard_wifi.h"
#include "secrets.h"

WebServer server(MY_SERVER_PORT);
HTTPHandlers handler(&server);

bool tryConnect() {
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(WiFi.status());

        if (WiFi.status() == WL_CONNECT_FAILED) {
            return false;
        }

        if (counter++ >= 30) {
            return false;
        }
    }
    return true;
}

bool setup_wifi() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin("ESP_AP", "winteriscoming");

    if (!tryConnect()) {
        Serial.print("\nWiFi connect failed\r\n");
        Serial.print("Trying default settings\r\n");
        WiFi.mode(WIFI_STA);
        WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_ID, EAP_USERNAME, EAP_PASSWORD);
        if (!tryConnect()) {
            Serial.print("\nWiFi connect failed\r\n");
            return false;
        }
    }

    return true;
}