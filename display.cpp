#include "display.h"

void Display::set_status(uint8_t status1, uint8_t status2, uint8_t status3) {
	m_status[0] = status1;
	m_status[1] = status2;
	m_status[2] = status3;
}

void Display::set_title(const char *title, uint8_t length) {
	m_title = std::string(title, length);
	Serial.print("set title: ");
    Serial.println(m_title.c_str());
}

void Display::set_description(const char *description, uint8_t length) {
	m_description = std::string(description, length);
	Serial.print("set classification: ");
    Serial.println(m_description.c_str());
}

void Display::set_classification(const char *classification, uint8_t length) {
	m_classification = std::string(classification, length);
	Serial.print("set classification: ");
    Serial.println(m_classification.c_str());
}

void Display::set_counter(uint8_t counter, uint8_t maximum) {
	m_counter = counter;
	m_maximum = maximum;
	Serial.print("set counter: ");
    Serial.print(m_counter);
    Serial.print("/");
    Serial.println(m_maximum);
}
