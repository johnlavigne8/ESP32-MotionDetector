HomeAssistantMotion
An embedded IoT project using an ESP32 microcontroller and HC-SR501 PIR motion sensor to detect
motion and publish events to Home Assistant over Wi-Fi via MQTT. Built in C using the ESP-IDF
framework, this project demonstrates low-power embedded design, GPIO interrupt handling, and
reliable real-time communication for smart home automation.
Features
• Real-time motion detection using PIR sensor and GPIO interrupts
• MQTT-based communication with Home Assistant over Wi-Fi
• Low-power operation with efficient event-driven firmware loop
• Modular firmware structure written in C with ESP-IDF
• Scalable architecture for future sensor integrations
Hardware Requirements
• ESP32-WROOM microcontroller (or compatible ESP32 dev board)
• HC-SR501 PIR motion sensor
• USB cable for flashing and power
• Breadboard and jumper wires
Wiring Guide
HC-SR501 Pin ESP32 Pin Description
VCC 5V or 3.3V Power supply (depends on board)
OUT GPIO 23 (example) Motion detection signal
GND GND Ground connection
Home Assistant Integration
In Home Assistant, add an MQTT sensor to listen for motion events:
binary_sensor:
- platform: mqtt
name: 'ESP32 Motion Detector'
state_topic: 'home/motion/esp32'
payload_on: 'MOTION_DETECTED'
payload_off: 'NO_MOTION'
device_class: motion
Configuration
In motion_main.c, update these defines before building:
#define WIFI_SSID 'YourNetwork'
#define WIFI_PASS 'YourPassword'
#define MQTT_BROKER_IP '192.168.1.10'
#define MOTION_PIN 23
Author
John Lavigne
Computer Engineering | Iowa State University
Johnlavigne8@gmail.com
License
This project is licensed under the MIT License. Feel free to modify and use it for your own smart home
projects.
