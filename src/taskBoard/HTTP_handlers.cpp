#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include "HTTP_handlers.h"
#include "helpers.h"
#include "taskBoard.h"

extern int serialNumber;
extern int typeNumber;
using namespace httpsserver;

#define HTTP_PREFIX "http://"
#define HTTPS_PREFIX "https://"

static const String contentTypes[][2] = {
    {".css",  "text/css"},
    {".gif",  "image/gif"},
    {".htm",  "text/html"},
    {".html", "text/html"},
    {".ico",  "image/x-icon"},
    {".jpg",  "image/jpg"},
    {".js",   "application/javascript"},
    {".pdf",  "application/pdf"},
    {".png",  "image/png"},
    {".txt",  "text/plain"},
    {".xml",  "text/xml"},
    {".zip",  "application/zip"},
    {"", ""}
};

/* 
    Private methods
 */
bool HTTPHandlers::read_file(const String &filename, HTTPResponse *res) {
    // Check if the file exists
    if (!LittleFS.exists(filename.c_str())) {
        return false;
    }
    File file = LittleFS.open(filename.c_str());

    // Set length
    res->setHeader("Content-Length", httpsserver::intToString(file.size()));

    // Content-Type is guessed using the definition of the contentTypes-table defined above
    int cTypeIdx = 0;
    do {
        if (filename.lastIndexOf(contentTypes[cTypeIdx][0]) != -1) {
            std::string value(contentTypes[cTypeIdx][1].c_str());
            res->setHeader("Content-Type", value);
            break;
        }
        cTypeIdx+=1;
    } while (contentTypes[cTypeIdx][0].length() > 0);

    // Read the file and write it to the response
    uint8_t buffer[256];
    size_t length = 0;
    do {
        length = file.read(buffer, 256);
        res->write(buffer, length);
    } while (length > 0);

    file.close();
    return true;
}

void HTTPHandlers::respond_ok(HTTPResponse *res) {
    res->setStatusCode(200);
    res->setStatusText("OK");
    res->setHeader("Content-Type", "text/plain");
}

void HTTPHandlers::respond_redirect(HTTPResponse *res, const String &address) {
    Serial.println("Redirecting to " + address);
    res->setHeader("Location", address.c_str());
    res->setStatusCode(302);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
}

/* 
    Public methods
 */
void HTTPHandlers::handle_secure_root(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_secure_root");
    if (TaskBoard::is_ap_mode()) {
        respond_redirect(res, "/wifi.html");
    } else {
        respond_redirect(res, "/programmer.html");
    }
}

void HTTPHandlers::handle_insecure_root(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_insecure_root");
    String address;
    if (TaskBoard::is_ap_mode()) {
        address = String(HTTPS_PREFIX) + TaskBoard::get_my_ip().toString();
    } else {
        address = String("/programmer.html");
    }
    respond_redirect(res, address);
}

void HTTPHandlers::handle_list_path(HTTPRequest *req, HTTPResponse *res) {
    Helper::list_dir(res, "/", 3);
    Helper::list_dir(&Serial, "/", 3);
    respond_ok(res);
}

void HTTPHandlers::handle_programmer_path(HTTPRequest *req, HTTPResponse *res) {
    String address = (req->isSecure() ? HTTPS_PREFIX : HTTP_PREFIX) +
        TaskBoard::get_my_ip().toString() + String("/programmer.html");
    respond_redirect(res, address);
}

void HTTPHandlers::handle_wifi_path(HTTPRequest *req, HTTPResponse *res) {
    String address = String(HTTPS_PREFIX) + TaskBoard::get_my_ip().toString() + String("/wifi.html");
    Serial.println("Redirecting to " + address);
    respond_redirect(res, address);
}

void HTTPHandlers::handle_restart_path(HTTPRequest *req, HTTPResponse *res) {
    handle_insecure_root(req, res);
    TaskBoard::display_header(DEVICE_ID);
    TaskBoard::display_value("RESTARTING");
    TaskBoard::schedule_restart(millis() + 2000U);
}

void HTTPHandlers::handle_wifi_config(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_wifi_config");

    char query[256] = {};
    size_t len = req->getContentLength();
    req->readChars(query, len);
    query[len] = '\0';

    // Serial.println("Config: (" + String(len) + ") " + String(query));

    std::map<String, String> params;
    Helper::parse_query_string(query, &params);
    // for (auto const& param : params) {
    //     Serial.print(param.first);
    //     Serial.print(": ");
    //     Serial.println(param.second);
    // }

    if (TaskBoard::store_wifi_config(query)) {
        read_file("/success.html", res);
    } else {
        read_file("/failed.html", res);
    }
}

void HTTPHandlers::handle_program(HTTPRequest *req, HTTPResponse *res) {
    String title = "";
    String desc = "";

    String buffer = req->getRequestString().c_str();
    int i = buffer.indexOf('?');
    buffer = buffer.substring(i+1);
    buffer.replace("+", " ");
    buffer.replace("%0A", "\n");
    Serial.println(buffer);

    std::map<String, String> params;
    Helper::parse_query_string(buffer, &params);
    // for (auto const& param : params) {
    //     Serial.print(param.first);
    //     Serial.print(": ");
    //     Serial.println(param.second);
    // }

    title = params["title"];
    desc = params["desc"];

    respond_ok(res);

    TaskBoard::show_task(title, desc);
}

void HTTPHandlers::handle_file(HTTPRequest *req, HTTPResponse *res) {
    String filename = req->getRequestString().c_str();
    // Clean possible '?' in the end of the filename
    int i = filename.indexOf('?');
    if (i != -1) {
        filename = filename.substring(0, i);
    }
    if (!read_file(filename, res)) {
        // Send "404 Not Found" as response, as the file doesn't seem to exist
        res->setStatusCode(404);
        res->setStatusText("Not found");
        res->println("404 Not Found");
        return;
    }
}
