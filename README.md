# Project Argus — Multi-Sensor Ethernet Monitor

A prototype ESP32-based sensor node that reads temperature and humidity data and exposes it over Ethernet using a W5500 TCP/IP offload module.

> **Status:** Prototype v0.1 — not for production use

---

## Hardware

| Component | Role | Interface |
|-----------|------|-----------|
| ESP32 | Central controller | — |
| W5500 | Ethernet / TCP-IP offload | SPI (VSPI) |
| SHT31 | Humidity + temperature | I2C |
| DS18B20 | Multi-point temperature probes | 1-Wire |

### Block Diagram

![Block Diagram](hardware/docs/Project%20Argus%20Breadboard%20Block%20Diagram.jpg)

### Power

- **Input:** 12 V DC via barrel jack
- **Regulated:** 3.3 V rail via buck converter, shared across all components

---

## Pin Reference

### W5500 — SPI (VSPI)

| Signal | ESP32 GPIO |
|--------|-----------|
| SCLK | 18 |
| MOSI | 23 |
| MISO | 19 |
| CS | 5 |
| RST | 4 |
| INT | 16 |

### SHT31 — I2C

| Signal | ESP32 GPIO |
|--------|-----------|
| SDA | 21 |
| SCL | 22 |

### DS18B20 — 1-Wire

| Signal | ESP32 GPIO |
|--------|-----------|
| DAT | 27 |

> Requires a single 4.7 kΩ pull-up resistor between DAT and 3.3 V.

---

## Firmware

Written using [PlatformIO](https://platformio.org/) with the Arduino framework. See [firmware/](firmware/) for source code and `platformio.ini` for library dependencies.

---

## Datasheets

- [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [W5500](https://docs.wiznet.io/img/products/w5500/W5500_ds_v110e.pdf)
- [SHT31](https://sensirion.com/media/documents/213E6A3B/63A5A569/Datasheet_SHT3x_DIS.pdf)
- [DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf)

---

## Author

Christian — May 2026
