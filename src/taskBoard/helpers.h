#pragma once

#include <Arduino.h>

class Helper {
public:
  Helper() = default;
  ~Helper() = default;

  static String convert_IP(IPAddress ip);
  static String convert_MAC(uint8_t* mac);
private:
};


