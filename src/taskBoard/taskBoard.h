#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>

class TaskBoard {
public:
    TaskBoard(GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> *display) : m_display(display) {}
    TaskBoard(const TaskBoard&) = delete;
    TaskBoard(TaskBoard&&) = delete;
    TaskBoard& operator=(const TaskBoard&) = delete;
    TaskBoard& operator=(TaskBoard&&) = delete;
    ~TaskBoard() = default;

    bool check_incoming_byte();
    void check_timeout();
    const char *get_title() { return m_buffer.title.data; }
    const char *get_desc() { return m_buffer.desc.data; }
    static void show_task(String &title, String &desc);
    static String limit_title(const String& raw_title);
    static void store_wifi_config(String config);
    static String read_wifi_config();
private:
    inline static const String wificonfig_filename = "/wificonfig";
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

    GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> *m_display;
    SerialRcvState m_serial_rcv_state = {SerialRcvState::TYPE};
    uint8_t m_data_cnt = {0};
    uint8_t m_data_crc = {0x00};
    struct msg m_buffer = {};
    int cnt = {0};
    unsigned long m_last_receive_time = {0};
};
