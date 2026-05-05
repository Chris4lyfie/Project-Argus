# Project Argus — Progress Report

> Last updated: May 4, 2026

---

## Hardware

| Item | Status | Notes |
|------|--------|-------|
| ESP32 on breadboard | ✅ Done | Verified correct orientation after initial upside-down install |
| W5500 wired (SPI) | ✅ Done | GPIO 18, 19, 23, 5; RST tied to 3.3 V; INT on GPIO 16 |
| SHT31 wired (I2C) | ✅ Done | GPIO 21 (SDA), 22 (SCL) — pins soldered for reliable continuity |
| DS18B20 ×5 wired (1-Wire) | ✅ Done | GPIO 27, 4.7 kΩ pull-up to 3.3 V |
| Power rail (12 V → buck → 3.3 V) | ⏳ Planned | Wiring spec defined; not yet tested under full load |
| Decoupling capacitors | ⏳ Planned | 100 µF electrolytic + 0.1 µF ceramic recommended |
| Permanent PCB / enclosure | ⏳ Planned | Currently breadboard only — not suitable for server room deployment |

---

## Firmware

| Item | Status | Author | Notes |
|------|--------|--------|-------|
| PlatformIO project setup | ✅ Done | User | Arduino framework, `esp32dev` board |
| Library dependencies | ✅ Done | User | SHT31, BusIO, OneWire, DallasTemperature, Ethernet in `platformio.ini` |
| Serial communication | ✅ Done | User | 460800 baud, verified |
| SHT31 — init + reading | ✅ Done | User | I2C at `0x44`, output in °F |
| DS18B20 ×5 — init + reading | ✅ Done | User | All 5 probes by index over 1-Wire, output in °F |
| Celsius → Fahrenheit helper | ✅ Done | User | `celsiustoFahrenheit()` |
| W5500 globals (mac, ip, server) | ✅ Done | User | Static IP `192.168.10.150`, `EthernetServerFixed` on port 80 |
| W5500 init in setup() | ✅ Done | User | `Ethernet.init(5)`, `Ethernet.begin(mac, ip)`, `server.begin()` |
| `EthernetServerFixed` wrapper | ✅ Done | Claude | Fixes ESP32 Arduino core 3.x incompatibility with `EthernetServer` |
| Global sensor state variables | ✅ Done | Claude | `shtTemp`, `shtHumidity`, `dsTemps[5]` |
| Non-blocking sensor read timer | ✅ Done | Claude | `readSensors()` called via `millis()` every 2 s, replacing `delay()` |
| HTTP request handler | ✅ Done | Claude | `handleClient()` — routes `GET /` and `GET /data` |
| JSON data endpoint (`/data`) | ✅ Done | Claude | Returns all 6 sensor readings as JSON |
| Chunked HTTP send | ✅ Done | Claude | Works around W5500's 2 KB TX buffer limit |

---

## Web Dashboard (`webpage.h`)

| Item | Status | Author | Notes |
|------|--------|--------|-------|
| HTML/CSS layout | ✅ Done | Claude | Dark theme, 2×3 grid of charts + humidity chart |
| Chart.js integration | ✅ Done | Claude | Loaded from CDN, real-time line graphs |
| Per-sensor individual graphs | ✅ Done | Claude | One chart per sensor, no combined view |
| Live value label on line end | ✅ Done | Claude | Custom Chart.js plugin draws current value at last data point |
| Temperature Y-axis (65–120 °F) | ✅ Done | Claude | Covers room ambient through server internal temps |
| Humidity Y-axis (0–100%) | ✅ Done | Claude | Fixed range, no axis jumping on spikes |
| Color-coded card borders | ✅ Done | Claude | Green / orange / red thresholds at 90 °F and 105 °F |
| Timestamp labels (every 10 polls) | ✅ Done | Claude | Labels stored at data point, not calculated from index |
| Orbitron font for title | ✅ Done | Claude | Loaded from Google Fonts |
| "Last updated" in header | ✅ Done | Claude | Updates every poll |
| Sensor naming | ✅ Done | User decision / Claude impl | SHT31 → "Argus Temp", DS18B20s → "Sensor #1–5" |

---

## Known Issues / Notes

- ESP32 exhibits a crash/restart loop on cold boot before stabilizing. Root cause not fully diagnosed — suspected watchdog timeout during Ethernet init. Monitors cleanly once running.
- Breadboard deployment only — not suitable for permanent server room install until moved to PCB.
- No Ethernet reconnection logic. If the switch reboots or cable is pulled, the ESP32 requires a power cycle.
- MAC address is currently the placeholder `DE:AD:BE:EF:FE:ED` — must be changed if a second device is added to the network.
- Currently powering via USB during development. The external 3.3 V rail (buck converter) must not be connected simultaneously.
- `udev` rules installed on Linux host for stable USB serial permissions during flashing.

---

## Next Steps

1. Diagnose and fix cold boot crash loop
2. Add Ethernet link-loss detection and reconnection logic
3. Test full power rail (12 V → buck → 3.3 V) under load
4. Install decoupling capacitors
5. Move off breadboard — design PCB or at minimum a perfboard build
6. Add enclosure suitable for server room mounting
7. Consider changing MAC address to something unique
