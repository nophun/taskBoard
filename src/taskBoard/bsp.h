#pragma once

#define OLED_ADDRESS          0x3C

// ESP32 C3
#if defined(ARDUINO_ESP32C3_DEV)
static constexpr int EPD_SCK    = 4;
static constexpr int EPD_MOSI   = 6;
static constexpr int EPD_CS     = 7;
static constexpr int EPD_DC     = 1;
static constexpr int EPD_RST    = 2;
static constexpr int EPD_BUSY   = 3;
static constexpr int SDA_PIN 	= 8;
static constexpr int SCL_PIN 	= 9;
static constexpr int LED_PIN    = 10;
// 8266
#elif defined(ARDUINO_ESP8266_NODEMCU_ESP12E)
static constexpr int EPD_SCK    = 14;
static constexpr int EPD_MOSI   = 13;
static constexpr int EPD_CS     = 15;
static constexpr int EPD_DC     = 2;
static constexpr int EPD_RST    = 0;
static constexpr int EPD_BUSY   = 16;
static constexpr int BUTTON_PIN = 4;
static constexpr int LED_PIN    = 12;
// XIAO
#elif defined(ARDUINO_SEEED_XIAO_M0)
static constexpr int EPD_SCK    = 5;
static constexpr int EPD_MOSI   = 4;
static constexpr int EPD_CS     = 1;
static constexpr int EPD_DC     = 3;
static constexpr int EPD_RST    = 0;
static constexpr int EPD_BUSY   = 2;
static constexpr int BUTTON_PIN = 7;
static constexpr int LED_PIN    = 9;
#endif