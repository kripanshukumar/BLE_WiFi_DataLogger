# Overview:
This project involves the development of an IoT BLE (Bluetooth Low Energy) data logger using an Arduino platform. The system is designed to scan for BLE devices in its proximity, collect data from them, and upload the data to a cloud server. The device can operate in two modes: Configuration Mode and Normal Mode. In Configuration Mode, the device acts as an Access Point (AP) for easy setup, while in Normal Mode, it connects to a configured WiFi network. Leveraging WiFi connectivity, the system gathers data from surrounding BLE devices, compiles it into JSON format, and uploads it to a cloud server at predefined intervals. The project seamlessly integrates BLE scanning, WiFi configuration, and cloud-based data storage, making it an ideal solution for applications requiring wireless data collection and transmission.

# Key Features:

## BLE Scanning:

* Utilizes the NimBLE library for scanning BLE devices.
* Collects information such as device name, address, manufacturer data, service UUIDs, and RSSI.
* Stores the collected data in a linked list of BLE_DEVICE objects.

## Configuration Mode:

* Activated by asserting a configured pin (CONFIG_PIN).
* Operates as an Access Point (AP) with a dynamically generated SSID and configurable password (AP_PSWD).
*  Allows users to configure WiFi credentials and other parameters.

## WiFi Connection:

*  Connects to a specified WiFi network using the configured SSID and password.
*  Reconnects automatically if the connection is lost.

## File System (SPIFFS):

*  Utilizes SPIFFS (SPI Flash File System) to store configuration parameters persistently.
*  Checks for the existence of a configuration file on startup and creates one if not found.

## Time Synchronization:

*  Configures the device's internal clock using the NTP server (pool.ntp.org) for accurate timestamping of data.

## Data Upload to Cloud:

*  Periodically uploads BLE device data to a specified cloud server using HTTPS POST requests.
*  Constructs a JSON payload containing timestamped BLE device information.

## LED Indicators:

*  Provides visual feedback through LEDs to indicate different states (e.g., configuration found, WiFi connecting, etc.).

## Task Management:

*  Uses FreeRTOS to manage tasks concurrently, ensuring efficient execution of BLE scanning, LED handling, and other functionalities.

# Hardware Requirements:

*  ESP32
*  LEDs for visual feedback.
*  Access to a WiFi network for data uploading.

# Libraries Used:

ArduinoJson
NimBLEDevice
WiFi
FreeRTOS

Note: Ensure to replace placeholders such as cloud server URL and API key with actual values during deployment. The project is modular and can be extended to support additional features and customizations.
