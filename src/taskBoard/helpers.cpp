#include <Arduino.h>
#include <LittleFS.h>
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

/* 
 *  Parse a URI to command and to a set of keys & values.
 */
void Helper::parse(String payload, struct input *p) {
    int i;
    int n = 0;
    String key;
    String val;
    String data;
    bool last = false;

    memset(p, 0x00, sizeof(struct input));
    payload.trim();

    // Parse payload
    i = payload.indexOf('&');
    while (n < MAX_N_DATA) {
        // Last data coming
        if (i == -1) {
            last = true;
            i = payload.length();
        }

        // Cut one data out and shorten payload
        data = payload.substring(0, i);
        payload = payload.substring(i+1);

        // Data empty
        if (data.length() == 0) {
            return;
        }

        // Parse key and value from data
        i = data.indexOf('=');
        if (i != -1) {
            key = data.substring(0, i);
        }
        if (i != data.length()) {
            val = data.substring(i+1);
        }

        p->data[n].key = key;
        p->data[n].val = val;

        // Prepare for next data
        i = payload.indexOf('&');

        n++;
        p->num = n;

        if (last) {
            return;
        }
    }
}

void Helper::parse_query_string(const String& query, std::map<String, String> *params) {
    int start = 0;
    int end = query.indexOf('&');

    while (end != -1) {
        String keyValue = query.substring(start, end);
        process_key_value(keyValue, params);
        start = end + 1;
        end = query.indexOf('&', start);
    }

    // Process the last key-value pair
    if (start < query.length()) {
        String keyValue = query.substring(start);
        process_key_value(keyValue, params);
    }
}

void Helper::process_key_value(const String& keyValue, std::map<String, String> *params) {
    int delimiterPos = keyValue.indexOf('=');
    if (delimiterPos != -1) {
        String key = keyValue.substring(0, delimiterPos);
        Helper::decode_uri(&key);
        String value = keyValue.substring(delimiterPos + 1);
        Helper::decode_uri(&value);
        (*params)[key] = value;
    }
}

void Helper::list_dir(Print *logger, const String& dirname, uint8_t levels) {
    logger->println("Listing directory: %s\r\n");
    logger->println(dirname);

    File root = LittleFS.open(dirname, "r");
    if (!root){
        logger->println("− failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        logger->println(" − not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file){
        if (file.isDirectory()) {
            logger->print("  DIR : ");
            logger->println(file.name());
            if (levels){
                list_dir(logger, file.name(), levels -1);
            }
        } else {
            logger->print("  FILE: ");
            logger->print(file.name());
            logger->print("\tSIZE: ");
            logger->println(file.size());
        }
        file = root.openNextFile();
    }
    root.close();
    file.close();
}

void Helper::decode_uri(String *original) {
    static constexpr size_t URL_SET = 39U;

    static constexpr struct {
        const char *encoded;
        const char *decoded;
    } char_map[URL_SET] = {
        {"%E2%82%AC", "€"},
        {"%C3%A8", "è"},
        {"%C3%A9", "é"},
        {"%C3%A0", "à"},
        {"%C3%BC", "ü"},
        {"%20", " "},
        {"%22", "\""},
        {"%25", "%"},
        {"%2D", "-"},
        {"%2E", "."},
        {"%3C", "<"},
        {"%3E", ">"},
        {"%5C", "\\"},
        {"%5E", "^"},
        {"%5F", "_"},
        {"%60", "`"},
        {"%7B", "{"},
        {"%7C", "|"},
        {"%7D", "}"},
        {"%7E", "~"},
        {"%21", "!"},
        {"%23", "#"},
        {"%24", "$"},
        {"%25", "%"},
        {"%26", "&"},
        {"%27", "'"},
        {"%28", "("},
        {"%29", ")"},
        {"%2A", "*"},
        {"%2B", "+"},
        {"%2C", ","},
        {"%2F", "/"},
        {"%3A", ":"},
        {"%3B", ";"},
        {"%3D", "="},
        {"%3F", "?"},
        {"%40", "@"},
        {"%5B", "["},
        {"%5D", "]"}
    };

    char *pch;
    //iterate through the percent encoding array
    for (int i = 0; i < URL_SET; ++i) {
        original->replace(char_map[i].encoded, char_map[i].decoded);
    }
}
