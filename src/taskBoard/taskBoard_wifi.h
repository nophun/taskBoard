#pragma once

#include <esp_wpa2.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <HTTPSServer.hpp>
#include <HTTPServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>
#include <HTTPUpdateServer.h>
#include <DNSServer.h>
#include "HTTP_handlers.h"

bool try_connect();
bool connect_wifi();
void config_server();
void start_server();
void stop_server();
void loop_server();
uint32_t setup_wifi_ap();

bool connect_open(const String &ssid);
bool connect_personal(const String &ssid, const String &psk);
bool connect_enterprise(const String &ssid, const String &id, const String &user, const String &passcode);
bool connect_enterprise(const char *ssid, const char *id, const char *user, const char *passcode);