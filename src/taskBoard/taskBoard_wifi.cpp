#define MY_SERVER_PORT 80

#include <map>
#include "taskBoard_wifi.h"
#include "taskBoard.h"
#include "helpers.h"
#include "cert.h"
#include "private_key.h"

using namespace httpsserver;
extern DNSServer dnsServer;
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

bool connect_wifi() {
    String config = TaskBoard::read_wifi_config();
    std::map<String, String> params;
    Helper::parse_query_string(config, &params);

    if (params["ssid"].length() != 0) {
        char mode = params["auth"][0];
        Serial.println("Trying to connect to configured WiFi (" + params["ssid"] + ")"/* + "[" + params["pass"] + "]"*/);
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

    return false;
}

void config_server() {
    ResourceNode *nodeInsecureRoot  = new ResourceNode("/", "GET", &HTTPHandlers::handle_insecure_root);
    ResourceNode *nodeSecureRoot    = new ResourceNode("/", "GET", &HTTPHandlers::handle_secure_root);

    ResourceNode *nodeList          = new ResourceNode("/list", "GET", &HTTPHandlers::handle_list_path);
    ResourceNode *nodeProgrammer    = new ResourceNode("/programmer", "GET", &HTTPHandlers::handle_programmer_path);
    ResourceNode *nodeRestart       = new ResourceNode("/restart", "GET", &HTTPHandlers::handle_restart_path);
    ResourceNode *nodeWifi          = new ResourceNode("/wifi", "GET", &HTTPHandlers::handle_wifi_path);
    ResourceNode *nodeWifiHtml      = new ResourceNode("/wifi.html", "GET", &HTTPHandlers::handle_wifi_path);

    ResourceNode *nodeWifiConfig    = new ResourceNode("/wificonfig", "POST", &HTTPHandlers::handle_wifi_config);
    ResourceNode *nodeProgram       = new ResourceNode("/program", "GET", &HTTPHandlers::handle_program);
    ResourceNode *nodeFile          = new ResourceNode("", "GET", &HTTPHandlers::handle_file);

    insecureServer.registerNode(nodeInsecureRoot);
    insecureServer.registerNode(nodeList);
    insecureServer.registerNode(nodeProgram);
    insecureServer.registerNode(nodeProgrammer);
    insecureServer.registerNode(nodeRestart);
    insecureServer.registerNode(nodeWifi);
    insecureServer.registerNode(nodeWifiHtml);
    insecureServer.setDefaultNode(nodeFile);
    secureServer.registerNode(nodeList);
    secureServer.registerNode(nodeProgram);
    secureServer.registerNode(nodeProgrammer);
    secureServer.registerNode(nodeRestart);
    secureServer.registerNode(nodeSecureRoot);
    secureServer.registerNode(nodeWifi);
    secureServer.registerNode(nodeWifiConfig);
    secureServer.setDefaultNode(nodeFile);
}

void start_server() {
    if (!secureServer.isRunning()) {
        secureServer.start();
    }
    if (!insecureServer.isRunning()) {
        insecureServer.start();
    }
    if (secureServer.isRunning() && insecureServer.isRunning()) {
        Serial.println("Servers ready.");
    }
}

void stop_server() {
    if (secureServer.isRunning()) {
        secureServer.stop();
    }
    if (insecureServer.isRunning()) {
        insecureServer.stop();
    }
}

void loop_server() {
    secureServer.loop();
    insecureServer.loop();
}

uint32_t setup_wifi_ap() {
    static constexpr int cDNSPort = 53;
    uint8_t mac[6];
    char ssid[15] = {0};
    uint32_t passcode = 0U;

    WiFi.macAddress(mac);
    sprintf(ssid, "Taskboard_%02X%02X", mac[4], mac[5]);
    passcode = *reinterpret_cast<uint32_t *>(mac) % 100000000;
    String pass_str = String(passcode);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass_str.c_str());
    dnsServer.start(cDNSPort, "*", WiFi.softAPIP());

    return passcode;
}
