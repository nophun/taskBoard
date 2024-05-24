#pragma once

#include <Arduino.h>
#include <map>
#include <string>

#define MAX_N_DATA 10

struct datum {
    String key;
    String val;
};

struct input {
    String cmd;
    int num;
    struct datum data[MAX_N_DATA];
};

class Helper {
public:
    Helper() = default;
    ~Helper() = default;

    static String convert_IP(IPAddress ip);
    static String convert_MAC(uint8_t* mac);
    static void parse(String payload, struct input *p);

    static void parse_query_string(const String& query, std::map<String, String> *params);
    static void process_key_value(const String& keyValue, std::map<String, String> *params);
    static void list_dir(Print *logger, const String& dirname, uint8_t levels = 3);

private:
};
