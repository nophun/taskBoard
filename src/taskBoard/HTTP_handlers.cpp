#include <Arduino.h>
#include <LittleFS.h>
#include "HTTP_handlers.h"
#include "helpers.h"
#include "taskBoard.h"

extern int serialNumber;
extern int typeNumber;

void HTTPHandlers::send_response(uint8_t code, String message) {
    WebServer *server = HTTPHandlers::get()->get_server();
    server->sendHeader("Access-Control-Allow-Origin", "*");
    //Response to the HTTP request
    server->send(code, "text/plain", message);
}

void HTTPHandlers::handle_root() {
    WebServer *server = HTTPHandlers::get()->get_server();
    server->sendHeader("Location", "/programmer.html", true);   //Redirect to our html web page
    server->send(302, "text/plane", "");
    Serial.println("handle_root");
}

void HTTPHandlers::handle_file() {
    WebServer *server = HTTPHandlers::get()->get_server();
    Serial.println("handle_file: " + server->uri());
    if (load_from_fs(server)) return;
    String message = "File Not Detected\n\n";
    message += "URI: ";
    message += server->uri();
    message += "\nMethod: ";
    message += (server->method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server->args();
    message += "\n";
    for (uint8_t i=0; i<server->args(); i++){
        message += " NAME:"+server->argName(i) + "\n VALUE:" + server->arg(i) + "\n";
    }
    server->send(404, "text/plain", message);
}

void HTTPHandlers::handle_info() {
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

    HTTPHandlers::send_response(200, message);
}

void HTTPHandlers::handle_program() {
    WebServer *server = HTTPHandlers::get()->get_server();
    String title = "";
    String desc = "";

    String message = "Number of args received:";
    //Get number of parameters
    message += server->args();
    message += "\n";

    if (server->hasArg("title")) {
        title = server->arg("title");
    }
    if (server->hasArg("desc")) {
        desc = server->arg("desc");
    }

    for (int i = 0; i < server->args(); i++) {
        //Include the current iteration value
        message += "Arg no" + (String)i + "-> ";
        //Get the name of the parameter
        message += server->argName(i) + ": ";
        //Get the value of the parameter
        message += server->arg(i) + "\n";
    }
    HTTPHandlers::send_response(200, message);

    TaskBoard::show_task(title, desc);
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
    HTTPHandlers::send_response(200, message);
}

bool HTTPHandlers::load_from_fs(WebServer *server) {
    String path = server->uri();
    String dataType = "text/plain";
    if (path.endsWith("/")){
        path += "index.html";
    }

    if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
    else if (path.endsWith(".html")) dataType = "text/html";
    else if (path.endsWith(".htm")) dataType = "text/html";
    else if (path.endsWith(".css")) dataType = "text/css";
    else if (path.endsWith(".js")) dataType = "application/javascript";
    else if (path.endsWith(".png")) dataType = "image/png";
    else if (path.endsWith(".gif")) dataType = "image/gif";
    else if (path.endsWith(".jpg")) dataType = "image/jpeg";
    else if (path.endsWith(".ico")) dataType = "image/x-icon";
    else if (path.endsWith(".xml")) dataType = "text/xml";
    else if (path.endsWith(".pdf")) dataType = "application/pdf";
    else if (path.endsWith(".zip")) dataType = "application/zip";
    File dataFile = LittleFS.open(path.c_str(), "r");
    if (server->hasArg("download")) {
        dataType = "application/octet-stream";
    }
    if (server->streamFile(dataFile, dataType) != dataFile.size()) {
    }

    dataFile.close();
    return true;
}