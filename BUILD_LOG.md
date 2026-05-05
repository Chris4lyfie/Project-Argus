# Project Argus — Build Log

A visual record of the build from first wire to working dashboard.

---

## 1. Workstation Setup

Getting the workspace ready before any components were touched.

![Workstation Setup](assets/photos/Workstation%20Setup.jpg)

---

## 2. Soldering Station Setup

Prepared the soldering station for the SHT31 humidity sensor pins.

![Soldering Station Setup](assets/photos/Soldering%20Station%20Setup.jpg)

---

## 3. Humidity Sensor — Soldering

Soldered the SHT31 pins. Not pretty, but functional. Connection was intermittent until the solder joints were cleaned up.

![Soldered (Poorly) Humidity Sensor](assets/photos/Soldered%20%28Poorly%29%20Humidity%20Sensor.jpg)

---

## 4. Humidity Sensor + Temperature Bus — Wiring

SHT31 wired to the ESP32 over I2C (GPIO 21/22). DS18B20 1-Wire bus also started on GPIO 27 with a 4.7 kΩ pull-up resistor.

![Humidity Sensor Wiring and Temperature Sensor Bus](assets/photos/Humidity%20Sensor%20Wiring%20and%20Temperature%20Sensor%20Bus.jpg)

---

## 5. Temperature Sensor Bus — Completed

All five DS18B20 probes wired onto the shared 1-Wire bus.

![Temperature Sensor Bus](assets/photos/Temperature%20Sensor%20Completed%20Bus.jpg)

![Temperature Sensor Bus — Angle 2](assets/photos/Temperature%20Sensor%20Completed%20Bus%20%28Angle%202%29.jpg)

![Temperature Sensor Bus — Angle 3](assets/photos/Temperature%20Sensor%20Completed%20Bus%20%28Angle%203%29.jpg)

![Temperature Sensor Bus — Closeup](assets/photos/Temperature%20Sensor%20Completed%20Bus%20%28Closeup%29.jpg)

---

## 6. Ethernet Wiring

W5500 module wired to the ESP32 over SPI (VSPI — GPIO 18, 19, 23, 5) with INT on GPIO 16.

![Ethernet Wiring](assets/photos/Ethernet%20Wiring.jpg)

![ESP32 to Ethernet Wiring](assets/photos/ESP32%20to%20Ethernet%20Wiring.jpg)

---

## 7. Power Rail Testing

3.3 V output from the buck converter verified on the Ethernet module power rail before full integration.

![3.3V Testing on Ethernet Power Rail](assets/photos/3.3V%20Testing%20on%20Ethernet%20Power%20Rail.jpg)

---

## 8. First Sensor Readings

First successful humidity and temperature readings over serial — confirmed all sensors were communicating correctly before bringing up Ethernet.

![Initial Humidity Sensor Readings](assets/photos/Initial%20Humidity%20Sensor%20Readings.jpg)

![Initial Temperature Readings](assets/photos/Initial%20Temperature%20Readings.jpg)

---

## 9. Live Dashboard

Real-time web dashboard served directly from the ESP32 over Ethernet, displaying all six sensors with Chart.js.

![Project Argus UI](assets/photos/Project%20Argus%20UI.png)

---

*Build by Christian — May 2026*
