#pragma once

#include <esp_wpa2.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPUpdateServer.h>
#include "HTTP_handlers.h"

extern WebServer server;

bool tryConnect();
bool setup_wifi();
void config_server();