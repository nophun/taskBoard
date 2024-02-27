// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <Arduino.h>
#include <SPIFFS.h>
#include "taskBoard.h"
#include "bsp.h"
#include "oled.h"
#include "helpers.h"
#include "fonts/tahoma8pt7b.h"
#include "fonts/Pockota_Bold16pt7b.h"

#include "taskBoard_wifi.h"

OLED oled(OLED_ADDRESS, 5*60);
// 2.13'' EPD Module
GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> display(GxEPD2_213_BN(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680
// GxEPD2_3C<GxEPD2_213_Z98c, GxEPD2_213_Z98c::HEIGHT> display(GxEPD2_213_Z98c(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // DEPG0213BN 122x250, SSD1680

// 2.9'' EPD Module
//GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(GxEPD2_290_BS(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // DEPG0290BS 128x296, SSD1680
//GxEPD2_3C<GxEPD2_290_C90c, GxEPD2_290_C90c::HEIGHT> display(GxEPD2_290_C90c(/*CS=5*/ 5, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // GDEM029C90 128x296, SSD1680

TaskBoard taskboard(&display);

void setup_oled() {
    Wire.begin();
    oled.start();
    oled.set_header("", Alignment::Center);
    oled.set_value("CONNECTING");
    oled.refresh();
}

void setup() {
    Serial.begin(115200);
    Serial.println("Start");

    if (SPIFFS.begin(true)) {
        Serial.println("Filesystem mounted OK");
    }

    setup_oled();

    if (setup_wifi()) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(Helper::convert_IP(WiFi.localIP()).c_str());
        
        server.on("/", HTTPHandlers::handle_root);
        server.on("/program", HTTPHandlers::handle_program);
        server.onNotFound(HTTPHandlers::handle_file);
        server.begin();
        
        oled.set_header(Helper::convert_IP(WiFi.localIP()).c_str(), Alignment::Center);
        oled.set_value("READY");
    } else {
        oled.set_value("NO WIFI");
    }
 
    oled.refresh();
}

void loop() {
    if (taskboard.check_incoming_byte()) {
        taskboard.show_task(taskboard.get_title(), taskboard.get_desc());
    }

    taskboard.check_timeout();

    if (WiFi.status() == WL_CONNECTED) {
        server.handleClient();
    }

    delay(1);
}

bool TaskBoard::check_incoming_byte() {
    uint8_t incomingByte;
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        m_last_receive_time = millis();

        switch(m_serial_rcv_state) {
            case SerialRcvState::TYPE:
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

    String limited_title;
    size_t len = min_title_len;
    size_t max_len = min(raw_title.length(), max_title_len);

    int16_t tbx, tby;
    uint16_t tbw, tbh;

    if (raw_title.length() <= min_title_len) {
        Serial.println("Title is short enough: " + raw_title);
        return raw_title;
    }

    display.setFont(&Pockota_Bold16pt7b);
    do {
        limited_title = raw_title.substring(0, len);
        display.getTextBounds(limited_title, 0, 0, &tbx, &tby, &tbw, &tbh);
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

void TaskBoard::show_task(String title, String desc) {
    int16_t tbx, tby;
    uint16_t tbw, tbh;

    display.init(115200, true, 50, false);
    display.setRotation(1);

    String limited_title = TaskBoard::limit_title(title);
    Serial.println("Title: " + limited_title + "\nDesc: "+ desc);

    display.setFont(&Pockota_Bold16pt7b);
    display.getTextBounds(desc, 0, 56, &tbx, &tby, &tbw, &tbh);
    Serial.println("desc Text bounds: " + String(tbx) + " " + String(tby) + " " + String(tbw) + " " + String(tbh));
    display.getTextBounds(limited_title, 0, 0, &tbx, &tby, &tbw, &tbh);
    Serial.println("title Text bounds: " + String(tbx) + " " + String(tby) + " " + String(tbw) + " " + String(tbh));
    display.setFullWindow();
    display.firstPage();
    do {
        display.fillScreen(GxEPD_WHITE); 

        display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
        display.setCursor(-tbx + (250 - tbw) / 2, 23);
        display.setFont(&Pockota_Bold16pt7b);
        display.print(limited_title);

        display.setTextColor(GxEPD_BLACK);
        display.setCursor(0, 60);
        display.setFont(&tahoma8pt7b);
        display.print(desc);
    }
    while (display.nextPage());
}