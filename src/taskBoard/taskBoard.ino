// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <Arduino.h>
#include <LittleFS.h>
#include "taskBoard.h"
#include "bsp.h"
#include "helpers.h"
#include "taskBoard_wifi.h"
#include "fonts/Pockota_Bold16pt7b.h"
#include "fonts/VaisalaSans_Light7pt7b.h"

OLED oled(OLED_ADDRESS, 5*60);
// 2.13'' EPD Module
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680

// 2.9'' EPD Module
//GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // DEPG0290BS 128x296, SSD1680
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029C90 128x296, SSD1680

TaskBoard taskboard(&display, &oled);

void setup() {
    Serial.begin(115200);
    Serial.println("Start");

    if (LittleFS.begin(true)) {
        Serial.println("Filesystem mounted OK");
    }

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    taskboard.init_display();
    taskboard.display_header("");
    taskboard.display_value("CONNECTING");
    taskboard.display_refresh();

    if (setup_wifi()) {
        Serial.println("\nWiFi connected");
        Serial.print("IP address: ");
        Serial.println(Helper::convert_IP(WiFi.localIP()).c_str());
        
        config_server();
        
        taskboard.display_header(Helper::convert_IP(WiFi.localIP()));
        taskboard.display_value("READY");
    } else {
        taskboard.display_value("NO WIFI");
    }
    taskboard.display_refresh();
}

void loop() {
    taskboard.shall_restart();

    if (taskboard.check_incoming_byte()) {
        String title = taskboard.get_title();
        String desc = taskboard.get_desc();
        taskboard.show_task(title, desc);
    }
    taskboard.check_timeout();

    if (WiFi.status() == WL_CONNECTED) {
        loop_server();
    }

    taskboard.display_refresh();
    delay(1);
}

bool TaskBoard::check_incoming_byte() {
    uint8_t incomingByte;
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        m_last_receive_time = millis();

        switch(m_serial_rcv_state) {
            case SerialRcvState::TYPE:
                if (incomingByte == '1') {
                    strcpy(m_buffer.title.data, "Title");
                    strcpy(m_buffer.desc.data, "Description");
                    return true;
                }
                memset(&m_buffer, 0, sizeof(m_buffer));
                m_buffer.type = static_cast<msg::MsgType>(incomingByte);
                m_data_crc = incomingByte;
                m_serial_rcv_state = SerialRcvState::TASK_LEN;
                break;
            case SerialRcvState::TASK_LEN:
                m_buffer.title.len = incomingByte;
                m_data_crc ^= incomingByte;
                m_data_cnt = 0;
                m_serial_rcv_state = SerialRcvState::TASK_DATA;
                break;
            case SerialRcvState::TASK_DATA:
                m_buffer.title.data[m_data_cnt++] = incomingByte;
                m_data_crc ^= incomingByte;
                if (m_data_cnt == m_buffer.title.len) {
                    m_serial_rcv_state = SerialRcvState::DESC_LEN;
                }
                break;
            case SerialRcvState::DESC_LEN:
                m_buffer.desc.len = incomingByte;
                m_data_crc ^= incomingByte;
                m_data_cnt = 0;
                m_serial_rcv_state = SerialRcvState::DESC_DATA;
                break;
            case SerialRcvState::DESC_DATA:
                m_buffer.desc.data[m_data_cnt++] = incomingByte;
                m_data_crc ^= incomingByte;
                if (m_data_cnt == m_buffer.desc.len) {
                    m_serial_rcv_state = SerialRcvState::CRC;
                }
                break;
            case SerialRcvState::CRC:
                m_serial_rcv_state = SerialRcvState::TYPE;
                if (m_data_crc == incomingByte) {
                    m_buffer.title.data[sizeof(m_buffer.title.data)-1] = '\0';
                    m_buffer.desc.data[sizeof(m_buffer.desc.data)-1] = '\0';
                    return true;
                } else {
                    m_data_cnt = 0;
                    memset(&m_buffer, 0, sizeof(m_buffer));
                }
                break;
            default:
                break;
        }
        Serial.println(" state " + String(static_cast<int>(m_serial_rcv_state))
             + " type " + String(static_cast<int>(m_buffer.type))
             + " title length " + String(m_data_cnt) + " / " + String(m_buffer.title.len) + "+" + String(m_buffer.desc.len)
             + " crc " + String(m_data_crc));
    }

    return false;
}

void TaskBoard::check_timeout() {
    if (m_serial_rcv_state != SerialRcvState::TYPE && millis() - m_last_receive_time > 1000) {
        m_serial_rcv_state = SerialRcvState::TYPE;
    }
}

String TaskBoard::limit_title(const String& raw_title) {
    static constexpr size_t min_title_len = 8;
    static constexpr size_t max_title_len = 28;
    static constexpr size_t line_height_limit = 32;

    auto tag = TaskBoard::get()->m_tag;
    String limited_title;
    size_t len = min_title_len;
    size_t max_len = min(raw_title.length(), max_title_len);

    int16_t tbx, tby;
    uint16_t tbw, tbh;

    if (raw_title.length() <= min_title_len) {
        Serial.println("Title is short enough: " + raw_title);
        return raw_title;
    }

    tag->setFont(&Pockota_Bold16pt7b);
    do {
        limited_title = raw_title.substring(0, len);
        tag->getTextBounds(limited_title, 0, 0, &tbx, &tby, &tbw, &tbh);
        Serial.println("Title '" + limited_title + "' bounds: " + String(tbx) + " " + String(tby) + " " + String(tbw) + " " + String(tbh) + ", len " + String(len));
        
        if (tbh > line_height_limit) {
            len--;
            break;
        } else if (len == max_len) {
            break;
        }
        len++;
    } while (1);
    limited_title = raw_title.substring(0, len);
    Serial.println("Limited string: " + limited_title);
    return limited_title;
}

void TaskBoard::show_task(String &title, String &desc) {
    auto tag = TaskBoard::get()->m_tag;
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    tag->init(115200, true, 50, false);
    tag->setRotation(1);

    String limited_title = TaskBoard::limit_title(title);
    Serial.println("Title: " + limited_title + "\nDesc: "+ desc);

    tag->setFont(&Pockota_Bold16pt7b);
    tag->getTextBounds(desc, 0, 56, &tbx, &tby, &tbw, &tbh);
    Serial.println("desc Text bounds: " + String(tbx) + " " + String(tby) + " " + String(tbw) + " " + String(tbh));
    tag->getTextBounds(limited_title, 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("title Text bounds: " + String(tbx) + " " + String(tby) + " " + String(tbw) + " " + String(tbh));
    tag->setFullWindow();
    tag->firstPage();
    do {
        tag->fillScreen(GxEPD_WHITE); 

        tag->setTextColor(tag->epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
        tag->setCursor(-tbx + (250 - tbw) / 2, 23);
        tag->setFont(&Pockota_Bold16pt7b);
        tag->print(limited_title);

        tag->setTextColor(GxEPD_BLACK);
        tag->setCursor(0, 60);
        tag->setFont(&VaisalaSans_Light7pt7b);
        tag->print(desc);
    }
    while (tag->nextPage());
}

void TaskBoard::init_display() {
    Wire.begin();
    m_taskboard->m_display->start();
}

void TaskBoard::display_header(const String &header) {
    m_taskboard->m_display->set_header(header.c_str(), Alignment::Center);
}

void TaskBoard::display_value(const String &value) {
    m_taskboard->m_display->set_value(value.c_str());
}

void TaskBoard::display_refresh() {
    m_taskboard->m_display->refresh();
}

void TaskBoard::store_wifi_config(String config) {
    File config_file = LittleFS.open(wificonfig_filename, FILE_WRITE);
    if (!config_file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    config_file.print(config);
    config_file.close();
    Serial.println("wifi config stored");
}

String TaskBoard::read_wifi_config() {
    File config_file = LittleFS.open(wificonfig_filename);
    if (!config_file) {
        Serial.println("Failed to open file for reading");
        return "";
    }
    char buffer[256];
    int len = config_file.read(reinterpret_cast<uint8_t *>(buffer), 256);
    buffer[len] = '\0';
    config_file.close();
    return buffer;
}

void TaskBoard::shall_restart() {
    if (m_restart_deadline != 0 && millis() > m_restart_deadline) {
        ESP.restart();
    }
}
