#include <Arduino.h>
#include "helpers.h"

String Helper::convert_IP(IPAddress ip) {
  String ipStr = String(ip[0]) + String(".");
  ipStr += String(ip[1]) + String(".");
  ipStr += String(ip[2]) + String(".");
  ipStr += String(ip[3]);

  return ipStr;
}

String Helper::convert_MAC(uint8_t* mac) {
  String macStr = "";
  if (mac[0] > 0x0f) {
    macStr += String(mac[0], HEX) + String(":");
  } else {
    macStr += String("0") + String(mac[0], HEX) + String(":");
  }
  
  if (mac[1] > 0x0f) {
    macStr += String(mac[1], HEX) + String(":");
  } else {
    macStr += String("0") + String(mac[1], HEX) + String(":");
  }
  
  if (mac[2] > 0x0f) {
    macStr += String(mac[2], HEX) + String(":");
  } else {
    macStr += String("0") + String(mac[2], HEX) + String(":");
  }
  
  if (mac[3] > 0x0f) {
    macStr += String(mac[3], HEX) + String(":");
  } else {
    macStr += String("0") + String(mac[3], HEX) + String(":");
  }
  
  if (mac[4] > 0x0f) {
    macStr += String(mac[4], HEX) + String(":");
  } else {
    macStr += String("0") + String(mac[4], HEX) + String(":");
  }
  
  if (mac[5] > 0x0f) {
    macStr += String(mac[5], HEX);
  } else {
    macStr += String("0") + String(mac[5], HEX);
  }

  return macStr;
}
