# Yuster Taskboard
![Yuster logo](/Yuster_logo_128px.png)

## Requirements
* Arduino Esp32 board support (tested with 2.0.14)
* GxEPD2 (https://github.com/ZinggJM/GxEPD2, tested with 1.5.5)
* ESP32_HTTPS_Server (https://github.com/fhessel/esp32_https_server, tested with 1.0.0)

## Build
Follow instructions of ESP32_HTTPS_Server for creating local CA certificates. The script is located in ESP32_HTTPS_Server library location extras/create_cert.sh. After creating cert.h and private_key.h, copy them to sketch folder.

Build using Arduino IDE with following board settings:
* Board: ESP32C3 Dev Module
* USB CDC On Boot: Enabled
* CPU Frequency: 160MHz
* Flash Frequency: 80MHz
* Flash Mode: QIO
* Flash Size: 4MB (32Mb)
* Partition Scheme: Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS) 

## Configure WiFi
* Connect programmer to power using a USB cable
* Hold down reset button for 2 seconds and realease
* Connect to Taskboard programmer WiFi AP by following details from programmer screen
* Open web browser and navigate to http://taskboard.local
* Configure WiFi settings
* Restart programmer
