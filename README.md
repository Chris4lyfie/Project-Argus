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

### Serial

| Parameter | Value |
|-----------|-------|
| Baud rate | 460800 |

### Libraries

| Library | Purpose |
|---------|---------|
| `Wire` | Arduino I2C driver (handles SDA/SCL automatically) |
| `Adafruit SHT31 Library` | SHT31 sensor driver |
| `Adafruit BusIO` | Dependency of Adafruit SHT31 |
| `OneWire` | 1-Wire bus driver |
| `DallasTemperature` | DS18B20 sensor driver |
| `Ethernet` | W5500 TCP/IP driver (not yet implemented) |

### Objects & Naming

| Identifier | Type | Purpose |
|------------|------|---------|
| `sht31` | `Adafruit_SHT31` | SHT31 sensor instance |
| `oneWire` | `OneWire` | 1-Wire bus on GPIO 27 |
| `sensors` | `DallasTemperature` | DS18B20 array, referenced via `oneWire` |
| `ONE_WIRE_BUS` | `#define` | GPIO pin for 1-Wire bus (27) |

### Protocols in Use

| Sensor | Protocol | GPIO Pins | Address / Index |
|--------|----------|-----------|-----------------|
| SHT31 | I2C | SDA: 21, SCL: 22 | `0x44` (default) |
| DS18B20 (×5) | 1-Wire | DAT: 27 | Read by index 0–4 |
| W5500 | SPI (VSPI) | 18, 19, 23, 5, 4, 16 | — (not yet implemented) |

### Helper Functions

| Function | Signature | Purpose |
|----------|-----------|---------|
| `celsiustoFahrenheit` | `float celsiustoFahrenheit(float input)` | Converts °C to °F using `(input × 9.0/5.0) + 32` |

---

## Datasheets

- [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [W5500](https://docs.wiznet.io/img/products/w5500/W5500_ds_v110e.pdf)
- [SHT31](https://sensirion.com/media/documents/213E6A3B/63A5A569/Datasheet_SHT3x_DIS.pdf)
- [DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf)

---

## Author

Christian — May 2026
