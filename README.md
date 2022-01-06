# EcoSense@clock
Tiny, simple and cheap indoor air quality controller with meteo station, clocks, OLED and web-reporting using Arduino Nano and ESP-01S

Photos and more description at [Hackaday.io](https://hackaday.io/project/183222-ecosenseclock)


**Features list**
- 5 air quality sensors
- Web reporting (ThingSpeak API, 2 channels)
- On-screen warnings
- Real-time clock
- .96" SSD1306 OLED display
- Dual-type power adapter
- Power management
- Event-driven architecture (using [Processy Firmware Framework](https://github.com/jumangee/arduino-nano-taskman))
- Small memoty consuption and footprint
- Easy support and upgrade

**Intelligent power management scheme**

- Constantly active devices (total: ~150mA):
  - Arduino Nano: ~20mA
  - DS3211: <1mA
  - BME280: 1mA
  - ESP-01: max 120mA (3.3V)
  - OLED: max 15mA
  - MH-Z19B: max 150mA    // CO2 sensor
  - Sensor button // switch through on-screen warnings list
- Periodically powered:
  - Group 1
    - CJMCU-1100: max 190mA    // Formaldehyde Benzene Toluene
    - MQ-136: max 150mA        // Hydrogen Sulfide
  - Group 2
    - MQ-135: max 150mA        // Air Quality (CO, Ammonia, Benzene, Alcohol, smoke)
    - MQ-4: max 150mA            // methane
  - Group 3
    - MQ-7: max 150mA         // CO sensor
    - PPD-42NS: max 90mA      // particle sensor, currently measures only PM2.5
