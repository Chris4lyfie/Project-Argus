# Project Argus — Progress Report

> Last updated: May 3, 2026

---

## Hardware

| Item | Status | Notes |
|------|--------|-------|
| ESP32 on breadboard | ✅ Done | Verified correct orientation after initial upside-down install |
| W5500 wired (SPI) | ✅ Done | GPIO 18, 19, 23, 5, 4, 16 |
| SHT31 wired (I2C) | ✅ Done | GPIO 21 (SDA), 22 (SCL) — pins soldered for reliable continuity |
| DS18B20 ×5 wired (1-Wire) | ✅ Done | GPIO 27, 4.7 kΩ pull-up to 3.3 V |
| Power rail (12 V → buck → 3.3 V) | ⏳ Planned | Wiring spec defined; not yet tested under full load |
| Decoupling capacitors | ⏳ Planned | 100 µF electrolytic + 0.1 µF ceramic recommended |

---

## Firmware

| Item | Status | Notes |
|------|--------|-------|
| PlatformIO project setup | ✅ Done | Arduino framework, `esp32dev` board |
| Library dependencies configured | ✅ Done | SHT31, BusIO, OneWire, DallasTemperature, Ethernet in `platformio.ini` |
| Serial communication | ✅ Done | 460800 baud, verified with Hello World |
| SHT31 — temperature + humidity | ✅ Done | Reading over I2C at address `0x44`, output in °F |
| DS18B20 ×5 — temperature array | ✅ Done | Reading all 5 probes by index over 1-Wire, output in °F |
| Celsius → Fahrenheit conversion | ✅ Done | `celsiustoFahrenheit()` helper function |
| W5500 Ethernet | ⏳ Not started | SPI wiring in place; firmware not yet written |
| Network data transmission | ⏳ Not started | Depends on W5500 bring-up |

---

## Known Issues / Notes

- ESP32 was initially installed upside-down on the breadboard, causing all GPIO connections to land on wrong pins. Resolved by reseating correctly.
- SHT31 I2C connection was intermittent until pins were soldered.
- `udev` rules installed on Linux host to ensure stable USB serial permissions during flashing.
- Currently powering ESP32 via USB only during development. External 3.3 V rail (buck converter) must not be connected simultaneously to avoid dual power source conflict.

---

## Next Steps

1. Bring up W5500 Ethernet in firmware
2. Transmit sensor data over the network
3. Test full power rail (12 V → buck → 3.3 V) with all components powered simultaneously
4. Install decoupling capacitors
