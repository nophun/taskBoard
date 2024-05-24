#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include "HTTP_handlers.h"
#include "helpers.h"
#include "taskBoard.h"

extern int serialNumber;
extern int typeNumber;
using namespace httpsserver;

static const std::string contentTypes[][2] = {
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

void HTTPHandlers::handle_root(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_root");
    res->setHeader("Location", "/programmer.html");
    res->setStatusCode(302);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
    res->println("");
}

void HTTPHandlers::handle_insecure_root(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_insecure_root");
    String address = String("https://") + Helper::convert_IP(WiFi.localIP());
    Serial.println(address);
    res->setHeader("Location", address.c_str());
    res->setStatusCode(302);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
    res->println("");
}

void HTTPHandlers::handle_wifi_config(HTTPRequest *req, HTTPResponse *res) {
    Serial.println("handle_wifi_config");
    Serial.println(req->getRequestString().c_str());
    res->setStatusCode(200);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
    res->println("OK");

    char query[256] = {};
    size_t len = req->getContentLength();
    req->readChars(query, len);
    query[len] = '\0';

    // Serial.println("Config: (" + String(len) + ") " + String(query));

    std::map<String, String> params;
    Helper::parse_query_string(query, &params);
    for (auto const& param : params) {
        Serial.print(param.first);
        Serial.print(": ");
        Serial.println(param.second);
    }

    TaskBoard::store_wifi_config(query);
}

void HTTPHandlers::handle_program(HTTPRequest *req, HTTPResponse *res) {
    String title = "";
    String desc = "";

    String buffer = req->getRequestString().c_str();
    int i = buffer.indexOf('?');
    buffer = buffer.substring(i+1);
    Serial.println(buffer);

    std::map<String, String> params;
    Helper::parse_query_string(buffer, &params);
    for (auto const& param : params) {
        Serial.print(param.first);
        Serial.print(": ");
        Serial.println(param.second);
    }

    title = params["title"];
    desc = params["desc"];

    res->setStatusCode(200);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
    res->println("OK");

    TaskBoard::show_task(title, desc);
}

void HTTPHandlers::handle_file(HTTPRequest *req, HTTPResponse *res) {
    std::string filename = req->getRequestString();
    // Check if the file exists
    if (!LittleFS.exists(filename.c_str())) {
        // Send "404 Not Found" as response, as the file doesn't seem to exist
        res->setStatusCode(404);
        res->setStatusText("Not found");
        res->println("404 Not Found");
        return;
    }

    File file = LittleFS.open(filename.c_str());

    // Set length
    res->setHeader("Content-Length", httpsserver::intToString(file.size()));

    // Content-Type is guessed using the definition of the contentTypes-table defined above
    int cTypeIdx = 0;
    do {
        if (filename.rfind(contentTypes[cTypeIdx][0])!=std::string::npos) {
            res->setHeader("Content-Type", contentTypes[cTypeIdx][1]);
            break;
        }
        cTypeIdx+=1;
    } while (strlen(contentTypes[cTypeIdx][0].c_str())>0);

    // Read the file and write it to the response
    uint8_t buffer[256];
    size_t length = 0;
    do {
        length = file.read(buffer, 256);
        res->write(buffer, length);
    } while (length > 0);

    file.close();
}

void HTTPHandlers::handle_list(HTTPRequest *req, HTTPResponse *res) {
    Helper::list_dir(res, "/", 3);
    Helper::list_dir(&Serial, "/", 3);
    res->setStatusCode(200);
    res->setStatusText("Found");
    res->setHeader("Content-Type", "text/plain");
}
