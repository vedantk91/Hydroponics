# Self-adaptive Control System for Hydroponics

## Introduction
The Self-adaptive Control System for Hydroponics automates and optimizes the growth conditions for plants in hydroponic setups. Integrating IoT technology with traditional hydroponics, it monitors and adjusts environmental parameters like pH, light intensity, temperature, and nutrient concentration, ensuring optimal plant growth with minimal human intervention.

## Features
- **Continuous Monitoring:** Tracks pH, temperature, humidity, and nutrient levels in real-time.
- **Automated Adjustments:** Automatically regulates light intensity, nutrient concentration, and pH balance.
- **IoT Integration:** Enables remote monitoring and control via a cloud-based dashboard.
- **Scalability:** Suitable for small-scale home gardens and large-scale commercial setups.
- **Low Maintenance:** Significantly reduces the need for constant human intervention.

## System Requirements
- ESP8266 Development Board
- pH, Temperature, and Humidity Sensors
- TDS Sensor
- Peristaltic Pumps for Nutrient Dispensing
- RGB LED Lights
- Adafruit IO Account for the IoT Dashboard

## Setup Instructions
1. **Hardware Assembly:** Connect sensors, peristaltic pumps, and LED lights to the ESP8266 following the assembly guide.
2. **Firmware Installation:** Use the Arduino IDE to flash the firmware onto the ESP8266.
3. **WiFi Configuration:** Connect the device to your WiFi network for remote access.
4. **Dashboard Setup:** On Adafruit IO, create widgets to display sensor data and control devices.

## Usage Guide
- **Monitoring:** View real-time data on the Adafruit IO dashboard.
- **Manual Controls:** Manually adjust light intensity, nutrient dosing, and pH balance via the dashboard.
- **Automated Mode:** Activate to let the system automatically adjust all parameters based on set thresholds.

## Images
Here are some images showcasing the system:

1. ![Hydroponics System Overview](image-url-overview) - An overview of the hydroponics system setup.
2. ![Sensor and Actuator Details](image-url-sensors) - Detailed view of the sensors and actuators.
3. ![IoT Dashboard](Named Figures/Named/dashboard_system.png) - The Adafruit IO dashboard for remote monitoring and control.


## Acknowledgments
- Vedant Kelkar, Manas Pange, and Kirti Sawlani for their groundbreaking work on this project.
- Thanks to all contributors and testers who have helped refine the system.


