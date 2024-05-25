#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include "oled.h"

class TaskBoard {
public:
    TaskBoard(GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> *tag, OLED *display) : m_tag(tag), m_display(display) {
        m_taskboard = this;
    }
    TaskBoard(const TaskBoard&) = delete;
    TaskBoard(TaskBoard&&) = delete;
    TaskBoard& operator=(const TaskBoard&) = delete;
    TaskBoard& operator=(TaskBoard&&) = delete;
    ~TaskBoard() = default;

    static TaskBoard *get() { return m_taskboard; }
    bool check_incoming_byte();
    void check_timeout();
    void shall_restart();
    const char *get_title() { return m_buffer.title.data; }
    const char *get_desc() { return m_buffer.desc.data; }

    static void show_task(String &title, String &desc);
    static void init_display(void);
    static void display_header(const String &header);
    static void display_value(const String &value);
    static void display_refresh();
    static String limit_title(const String& raw_title);
    static bool store_wifi_config(String config);
    static String read_wifi_config();
    static void schedule_restart(unsigned long deadline) {
        m_taskboard->set_restart_deadline(deadline);
    }
private:
    inline static const String wificonfig_filename = "/wificonfig";
    inline static TaskBoard *m_taskboard = nullptr;
    void set_restart_deadline(unsigned long deadline) {
        m_restart_deadline = deadline;
    }
    enum class SerialRcvState {
        TYPE,
        TASK_LEN,
        TASK_DATA,
        DESC_LEN,
        DESC_DATA,
        CRC
    };

    template<uint8_t SIZE = 3>
    struct text {
        uint8_t len;
        char data[SIZE];
    };

    struct msg {
        enum class MsgType {
            Task,
            Desc,
            Class
        };
        MsgType type;
        text<32> title;
        text<200> desc;
    };

    GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> *m_tag;
    OLED *m_display;
    SerialRcvState m_serial_rcv_state = {SerialRcvState::TYPE};
    uint8_t m_data_cnt = {0};
    uint8_t m_data_crc = {0x00};
    struct msg m_buffer = {};
    unsigned long m_last_receive_time = {0};
    unsigned long m_restart_deadline = {0};
};
