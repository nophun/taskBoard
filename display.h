#pragma once

#include <Arduino.h>
#include <array>
#include "bsp.h"


class Display {
public:
    explicit Display(EPDTYPE *epd) : m_epd(epd) {}
    Display() = delete;
    ~Display() = default;
    Display(const Display&) = delete;
    Display(Display&&) = delete;
    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;

    void init();

    void set_status(uint8_t status1, uint8_t status2, uint8_t status3);
    void set_title(const std::string& title);
    void set_description(const std::string& description);
    void set_classification(const std::string& classification);
    void set_title(const char *title, uint8_t length);
    void set_description(const char *description, uint8_t length);
    void set_classification(const char *classification, uint8_t length);
    void set_counter(uint8_t counter, uint8_t maximum);

private:
    EPDTYPE *m_epd;
    std::string m_title;
    std::string m_description;
    std::string m_classification;
    std::array<uint8_t, 3U> m_status{};
    uint8_t m_counter{};
    uint8_t m_maximum{};
};