#include <Arduino.h>
#include "HTTP_handlers.h"
#include "helpers.h"

extern int serialNumber;
extern int typeNumber;

void HTTPHandlers::send_response(uint8_t code, String message) {
  WebServer *server = HTTPHandlers::get()->get_server();
  server->sendHeader("Access-Control-Allow-Origin", "*");
  //Response to the HTTP request
  server->send(code, "text/plain", message);
}

void HTTPHandlers::handle_root() {
  uint8_t mac[6]; 
  WiFi.macAddress(mac);
  String macStr = Helper::convert_MAC(mac);

  String ipStr = Helper::convert_IP(WiFi.localIP());
  String gwipStr = Helper::convert_IP(WiFi.gatewayIP());

  String message = "";
  // message += "device type:      " + device_type_strings[typeNumber];
  // message += "\nserial:           ";
  // message += String(serialNumber);
  // message += "\nsw version:       ";
  // message += SW_VERSION;
  message += "\nmy mac:           ";
  message += macStr;
  message += "\nconnected to:     ";
  message += WiFi.SSID();
  message += "\nmy ip:            ";
  message += ipStr;
  message += "\ngateway ip:       ";
  message += gwipStr;
  message += "\nsignal strength:  ";
  message += String(WiFi.RSSI());
  // message += "\nserver ip:        ";
  // message += HLS_SERVER_IP;
  message += "\n";

  HTTPHandlers::get()->send_response(200, message);
}

void HTTPHandlers::handle_generic_args() {
  WebServer *server = HTTPHandlers::get()->get_server();
  String message = "Number of args received:";
  //Get number of parameters
  message += server->args();
  message += "\n";

  for (int i = 0; i < server->args(); i++) {
    //Include the current iteration value
    message += "Arg no" + (String)i + "-> ";
    //Get the name of the parameter
    message += server->argName(i) + ": ";
    //Get the value of the parameter
    message += server->arg(i) + "\n";
  }
  HTTPHandlers::get()->send_response(200, message);
}