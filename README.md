# Project Argus — Multi-Sensor Ethernet Monitor

A ESP32-based sensor node that reads temperature and humidity data from multiple sensors and serves a real-time graphing dashboard over Ethernet using a W5500 TCP/IP module.

> **Status:** Prototype v0.2 — functional, not yet hardened for production

---

## Hardware

| Component | Role | Interface |
|-----------|------|-----------|
| ESP32-WROOM-32D | Central controller | — |
| W5500 | Ethernet / TCP-IP offload | SPI (VSPI) |
| SHT31-D | Ambient humidity + temperature | I2C |
| DS18B20 ×5 | Multi-point temperature probes | 1-Wire |
| 3.3 V Buck Converter | Power regulation from 12 V rail | — |
| 4.7 kΩ Resistor | 1-Wire pull-up | — |
| 100 µF Capacitor | Decoupling (planned) | — |

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
| RST | tied to 3.3 V (no software reset) |
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

Written using [PlatformIO](https://platformio.org/) with the Arduino framework.

### Serial

| Parameter | Value |
|-----------|-------|
| Baud rate | 460800 |

### Libraries

| Library | Purpose |
|---------|---------|
| `Wire` | I2C driver |
| `Adafruit SHT31 Library` | SHT31 sensor driver |
| `Adafruit BusIO` | Adafruit SHT31 dependency |
| `OneWire` | 1-Wire bus driver |
| `DallasTemperature` | DS18B20 sensor driver |
| `arduino-libraries/Ethernet` | W5500 TCP/IP driver |

> **Note:** The standard `Ethernet` library's `EthernetServer` class does not implement the `begin(uint16_t)` pure virtual required by ESP32 Arduino core 3.x. A local `EthernetServerFixed` wrapper class resolves this without modifying the library.

### Key Identifiers

| Identifier | Type | Author | Purpose |
|------------|------|--------|---------|
| `sht31` | `Adafruit_SHT31` | User | SHT31 sensor instance |
| `oneWire` | `OneWire` | User | 1-Wire bus on GPIO 27 |
| `sensors` | `DallasTemperature` | User | DS18B20 array |
| `ONE_WIRE_BUS` | `#define` | User | GPIO pin for 1-Wire (27) |
| `server` | `EthernetServerFixed` | User | HTTP server on port 80 |
| `shtTemp` | `float` | Claude | Latest SHT31 temperature (°F) |
| `shtHumidity` | `float` | Claude | Latest SHT31 humidity (%) |
| `dsTemps[5]` | `float[]` | Claude | Latest DS18B20 readings (°F) |
| `EthernetServerFixed` | `class` | Claude | ESP32 compatibility wrapper for `EthernetServer` |

### Functions

| Function | Author | Purpose |
|----------|--------|---------|
| `celsiustoFahrenheit(float)` | User | Converts °C to °F |
| `readSensors()` | Claude | Reads all sensors into global variables on a millis() timer |
| `handleClient(EthernetClient&)` | Claude | Parses HTTP request and routes to `/` or `/data` |

---

## Web Interface

The ESP32 serves an HTTP dashboard at its static IP (`192.168.10.150` by default).

| Route | Response |
|-------|----------|
| `GET /` | Full HTML dashboard with Chart.js graphs |
| `GET /data` | JSON snapshot of current sensor readings |

### JSON Format (`/data`)

```json
{
  "sht_temp": 73.5,
  "humidity": 53.1,
  "ds0": 72.3,
  "ds1": 72.7,
  "ds2": 72.2,
  "ds3": 71.9,
  "ds4": 72.2
}
```

### Dashboard

- Real-time line graphs, one per sensor, in a 2×3 grid
- Humidity chart with fixed 0–100% Y-axis
- Temperature charts with 65–120 °F range (server room ambient + internal)
- Color-coded card borders: normal / warm / hot thresholds
- Live value label tracking the end of each line
- Timestamps printed every 10 polls (~20 seconds)
- Built with [Chart.js](https://www.chartjs.org/) and [Orbitron](https://fonts.google.com/specimen/Orbitron) font

> The HTML/JS dashboard lives in `firmware/src/webpage.h` as a raw C++ string literal, served in chunks to work around the W5500's 2 KB socket TX buffer.

---

## Datasheets

- [ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [W5500](https://docs.wiznet.io/img/products/w5500/W5500_ds_v110e.pdf)
- [SHT31](https://sensirion.com/media/documents/213E6A3B/63A5A569/Datasheet_SHT3x_DIS.pdf)
- [DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/DS18B20.pdf)

---

## Author

Christian — May 2026
