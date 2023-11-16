#include "display.h"
#include <Fonts/FreeMonoBold9pt7b.h>

void Display::init() {
	m_epd->init(115200);
	m_epd->setTextColor(GxEPD_BLACK);
	m_epd->setFont(&FreeMonoBold9pt7b);
	m_epd->setRotation(1);
	m_epd->fillScreen(GxEPD_WHITE);
	// m_epd->update();
}

void Display::set_status(uint8_t status1, uint8_t status2, uint8_t status3) {
	m_status[0] = status1;
	m_status[1] = status2;
	m_status[2] = status3;
}

void Display::set_title(const std::string& title) {
	m_title = title;
	Serial.print("set title: ");
	Serial.println(m_title.c_str());
}

void Display::set_description(const std::string& description) {
	m_description = description;
	Serial.print("set description: ");
	Serial.println(m_description.c_str());
}

void Display::set_classification(const std::string& classification) {
	m_classification = classification;
	Serial.print("set classification: ");
	Serial.println(m_classification.c_str());
}

void Display::set_title(const char *title, uint8_t length) {
	set_title(std::string(title, length));
}

void Display::set_description(const char *description, uint8_t length) {
	set_description(std::string(description, length));
}

void Display::set_classification(const char *classification, uint8_t length) {
	set_classification(std::string(classification, length));
}

void Display::set_counter(uint8_t counter, uint8_t maximum) {
	m_counter = counter;
	m_maximum = maximum;
	Serial.print("set counter: ");
    Serial.print(m_counter);
    Serial.print("/");
    Serial.println(m_maximum);
}
