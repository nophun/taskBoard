#pragma once

#include <Arduino.h>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

class HTTPHandlers {
public:
    ~HTTPHandlers() = default;

    static void handle_file(HTTPRequest *req, HTTPResponse *res);
    static void handle_secure_root(HTTPRequest *req, HTTPResponse *res);
    static void handle_insecure_root(HTTPRequest *req, HTTPResponse *res);
    static void handle_program(HTTPRequest *req, HTTPResponse *res);
    static void handle_programmer_path(HTTPRequest *req, HTTPResponse *res);
    static void handle_wifi_config(HTTPRequest *req, HTTPResponse *res);
    static void handle_list_path(HTTPRequest *req, HTTPResponse *res);
    static void handle_wifi_path(HTTPRequest *req, HTTPResponse *res);
    static void handle_restart_path(HTTPRequest *req, HTTPResponse *res);

private:
    HTTPHandlers() = default;
    static bool read_file(const String &filename, HTTPResponse *res);
    static void respond_ok(HTTPResponse *res);
    static void respond_redirect(HTTPResponse *res, const String &address);
};
