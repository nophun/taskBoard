#pragma once

#include <Arduino.h>
#include <WebServer.h>

class HTTPHandlers {
public:
  explicit HTTPHandlers(WebServer *server) : m_server{server} {
    m_handler = this;
  }
  ~HTTPHandlers() = default;

  WebServer *get_server(void) { return m_server; }
  static void handle_root();
  static void handle_file();
  static void handle_info();
  static void handle_program();
  static void handle_generic_args();
  static HTTPHandlers *get() { return m_handler; }

private:
  static void send_response(uint8_t code, String message);
  static bool load_from_fs(WebServer *server);
  
  WebServer *m_server {nullptr};
  inline static HTTPHandlers *m_handler {nullptr};
};

