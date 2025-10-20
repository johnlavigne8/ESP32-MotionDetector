HomeAssistantMotion
===================

An embedded IoT project using an ESP32 microcontroller and HC-SR501 PIR motion sensor to detect motion and publish events to Home Assistant over Wi-Fi via MQTT. 
Built in C using the ESP-IDF framework, this project demonstrates low-power embedded design, GPIO interrupt handling, and reliable real-time communication for smart home automation.

------------------------------------------------------------
FEATURES
------------------------------------------------------------
• Real-time motion detection using PIR sensor and GPIO interrupts
• MQTT-based communication with Home Assistant over Wi-Fi
• Low-power operation with efficient event-driven firmware loop
• Modular firmware structure written in C with ESP-IDF
• Scalable architecture for future sensor integrations

------------------------------------------------------------
HARDWARE REQUIREMENTS
------------------------------------------------------------
• ESP32-WROOM microcontroller (or compatible ESP32 dev board)
• HC-SR501 PIR motion sensor
• USB cable for flashing and power
• Breadboard and jumper wires

------------------------------------------------------------
WIRING GUIDE
------------------------------------------------------------
HC-SR501 Pin    | ESP32 Pin        | Description
------------------------------------------------------------
VCC             | 5V or 3.3V       | Power supply (depends on board)
OUT             | GPIO 23 (example)| Motion detection signal
GND             | GND              | Ground connection

------------------------------------------------------------
SOFTWARE OVERVIEW
------------------------------------------------------------
The firmware initializes GPIO for interrupt-based motion sensing. When motion is detected:
1. The interrupt triggers an MQTT publish event.
2. The ESP32 connects to a Wi-Fi network and sends a motion signal to a Home Assistant MQTT topic (e.g., home/motion/esp32).
3. Home Assistant automations can react (e.g., turn on lights, send alerts).

------------------------------------------------------------
HOME ASSISTANT INTEGRATION
------------------------------------------------------------
In Home Assistant, add an MQTT sensor to listen for motion events:

binary_sensor:
  - platform: mqtt
    name: "ESP32 Motion Detector"
    state_topic: "home/motion/esp32"
    payload_on: "MOTION_DETECTED"
    payload_off: "NO_MOTION"
    device_class: motion

------------------------------------------------------------
CONFIGURATION
------------------------------------------------------------
In motion_main.c, update these defines before building:

#define WIFI_SSID      "YourNetwork"
#define WIFI_PASS      "YourPassword"
#define MQTT_BROKER_IP "192.168.1.10"
#define MOTION_PIN     23

------------------------------------------------------------
BUILDING AND FLASHING
------------------------------------------------------------
1. Ensure ESP-IDF is installed and configured.
2. Run the following commands:
   idf.py set-target esp32
   idf.py build
   idf.py flash
   idf.py monitor

------------------------------------------------------------
TECHNOLOGIES USED
------------------------------------------------------------
C / ESP-IDF
MQTT protocol
Wi-Fi networking
GPIO interrupts
Home Assistant automation

------------------------------------------------------------
AUTHOR
------------------------------------------------------------
John Lavigne
Computer Engineering | Iowa State University
johnlavigne8@gmail.com

------------------------------------------------------------
LICENSE
------------------------------------------------------------
This project is licensed under the MIT License.
Feel free to modify and use it for your own smart home projects.
