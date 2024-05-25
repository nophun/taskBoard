#define MY_SERVER_PORT 80

#include <map>
#include "taskBoard_wifi.h"
#include "taskBoard.h"
#include "helpers.h"
#include "secrets.h"
#include "cert.h"
#include "private_key.h"

using namespace httpsserver;

SSLCert cert = SSLCert(
  example_crt_DER, example_crt_DER_len,
  example_key_DER, example_key_DER_len
);

// First, we create the HTTPSServer with the certificate created above
HTTPSServer secureServer = HTTPSServer(&cert);
// Additionally, we create an HTTPServer for unencrypted traffic
HTTPServer insecureServer = HTTPServer(MY_SERVER_PORT);

// HTTPUpdateServer httpUpdater;

bool try_connect() {
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

bool connect_open(const String &ssid) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid);
    return try_connect();
}

bool connect_personal(const String &ssid, const String &psk) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, psk);
    return try_connect();
}

bool connect_enterprise(const String &ssid, const String &id, const String &user, const String &passcode) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, WPA2_AUTH_PEAP, id, user, passcode);
    return try_connect();
}

bool connect_enterprise(const char *ssid, const char *id, const char *user, const char *passcode) {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, WPA2_AUTH_PEAP, id, user, passcode);
    return try_connect();
}

bool setup_wifi() {
    String config = TaskBoard::read_wifi_config();
    std::map<String, String> params;
    Helper::parse_query_string(config, &params);

    if (params["ssid"].length() != 0) {
        char mode = params["auth"][0];
        Serial.println("Trying to connect to configured WiFi (" + params["ssid"] + ")[" + params["pass"] + "]");
        switch (mode) {
            case 'n':
                if (connect_open(params["ssid"])) {
                    return true;
                }
                break;
            case 'p':
                if (connect_personal(params["ssid"], params["pass"])) {
                    return true;
                }
                break;
            case 'e':
                if (connect_enterprise(params["ssid"], params["user"], params["user"], params["pass"])) {
                    return true;
                }
                break;
            default:
                break;
        }
        Serial.println("\nWiFi connect failed");
    }

    // Serial.println("Connecting to WiFi (" + String(WIFI_SSID) + ")");
    // if (connect_enterprise(WIFI_SSID, EAP_USERNAME, EAP_PASSWORD)) {
    //     return true;
    // }
    // Serial.println("\nWiFi connect failed");

    Serial.println("Trying default settings (ESP_AP)");
    String def_ssid = "ESP_AP";
    String def_psk = "winteriscoming";
    if (connect_personal(def_ssid, def_psk)) {
        return true;
    }
    Serial.println("\nWiFi connect failed");

    return false;
}

void config_server() {
    ResourceNode *nodeInsecureRoot  = new ResourceNode("/", "GET", &HTTPHandlers::handle_insecure_root);
    ResourceNode *nodeRoot          = new ResourceNode("/", "GET", &HTTPHandlers::handle_root);
    ResourceNode *nodeFile          = new ResourceNode("", "GET", &HTTPHandlers::handle_file);
    ResourceNode *nodeProgram       = new ResourceNode("/program", "GET", &HTTPHandlers::handle_program);
    ResourceNode *nodeWifiConfig    = new ResourceNode("/wificonfig", "POST", &HTTPHandlers::handle_wifi_config);
    ResourceNode *nodeList          = new ResourceNode("/list", "GET", &HTTPHandlers::handle_list);
    ResourceNode *nodeWifi          = new ResourceNode("/wifi", "GET", &HTTPHandlers::handle_wifi);
    ResourceNode *nodeWifiHtml      = new ResourceNode("/wifi.html", "GET", &HTTPHandlers::handle_wifi);

    secureServer.registerNode(nodeRoot);
    insecureServer.registerNode(nodeInsecureRoot);

    secureServer.registerNode(nodeWifiConfig);

    secureServer.registerNode(nodeProgram);
    insecureServer.registerNode(nodeProgram);

    secureServer.registerNode(nodeList);
    insecureServer.registerNode(nodeList);

    secureServer.registerNode(nodeWifi);
    insecureServer.registerNode(nodeWifi);
    insecureServer.registerNode(nodeWifiHtml);
    
    secureServer.setDefaultNode(nodeFile);
    insecureServer.setDefaultNode(nodeFile);

    secureServer.start();
    insecureServer.start();
    if (secureServer.isRunning() && insecureServer.isRunning()) {
        Serial.println("Servers ready.");
    }
}

void loop_server() {
    secureServer.loop();
    insecureServer.loop();
}