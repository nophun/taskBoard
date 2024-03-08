#define MY_SERVER_PORT 80

#include "taskBoard_wifi.h"
#include "secrets.h"

WebServer server(MY_SERVER_PORT);
HTTPHandlers handler(&server);
HTTPUpdateServer httpUpdater;

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
    Serial.println("Connecting to WiFi (" + String(WIFI_SSID) + ")");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_ID, EAP_USERNAME, EAP_PASSWORD);
    if (tryConnect()) {
        return true;
    }
    Serial.println("\nWiFi connect failed");

    Serial.println("Trying default settings (ESP_AP)");
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin("ESP_AP", "winteriscoming");
    if (tryConnect()) {
        return true;
    }
    Serial.println("\nWiFi connect failed");
    return false;
}

void config_server() {
    server.on("/", HTTPHandlers::handle_root);
    server.on("/program", HTTPHandlers::handle_program);
    server.onNotFound(HTTPHandlers::handle_file);
    server.begin();

    httpUpdater.setup(&server, UPDT_USER, UPDT_PASW);
}