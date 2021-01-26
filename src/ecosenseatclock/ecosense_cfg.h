#ifndef _ECOSENSEATCLOCK_CFG_H
#define _ECOSENSEATCLOCK_CFG_H

    // ---[ COMMON APP CONFIG ]---
    #define ENVSENSORS_TIMEOUT          5000
    #define READINGS_PER_RESULT         50
    #define REPORT_TIMEOUT              60000

    // ---[ PWR MNGMNT PRC CFG ]---
    #define PWRMNGMTPINS                {12, 11, 10}
    #define MAXTASKCOUNT                10      // 5?
    #define CONSUMERPROCESSTIMEOUT      20000   // 60000 is production

    // ---[ ADC MUX CFG ]---
    #define ADCMUXPINS                  {9, 8, 7, 6}
    #define MQ7_ANALOG_PIN_ADDR         0
    #define MQ136_ANALOG_PIN_ADDR       1
    #define MQ4_ANALOG_PIN_ADDR         2
    #define ZE08CH20_ANALOG_PIN_ADDR    3
    #define MQ135_ANALOG_PIN_ADDR       4
    #define CJMCU1100_ANALOG_PIN_ADDR   5
    #define PPD42NS_ANALOG_PIN_ADDR1    6   // PM1
    #define PPD42NS_ANALOG_PIN_ADDR2    7   // PM2.5

    // ---[ BME280 ]---
    #define BME280_ADDRESS              (0x76)

    // ---[ MHZ19 UART ]---
    #define MHZ19_RXPIN                 3
    #define MHZ19_TXPIN                 4

    // ---[ WIFI ]---
    #define WIFI_RX_PIN                 2
    #define WIFI_TX_PIN                 3    
    #define WIFI_SSID                   "xxx"
    #define WIFI_PWD                    "xxx"

    // ---[ THINGSPEAK ]---
    #define THINGSPEAK_CHANNEL_KEY      "xxx"

    // ---[ REAL-TIME CLOCK ]---
    #define RESET_CLOCK                 0
    #define RTC_GET_TEMPERATURE

#endif