#pragma once

#include <Arduino.h>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

class HTTPHandlers {
public:
    ~HTTPHandlers() = default;

    static void handle_file(HTTPRequest *req, HTTPResponse *res);
    static void handle_insecure_root(HTTPRequest *req, HTTPResponse *res);
    static void handle_program(HTTPRequest *req, HTTPResponse *res);
    static void handle_root(HTTPRequest *req, HTTPResponse *res);
    static void handle_wifi_config(HTTPRequest *req, HTTPResponse *res);
    static void handle_list(HTTPRequest *req, HTTPResponse *res);

private:
    HTTPHandlers() = default;
};
