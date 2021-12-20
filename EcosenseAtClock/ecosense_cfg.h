#ifndef _ECOSENSEATCLOCK_CFG_H
#define _ECOSENSEATCLOCK_CFG_H

    // ---[ PROCESS IDs ]---
	#define PRC_DISPLAY			101		// display process
	#define PRC_RTC				102		// system clocks
	#define PRC_DATAREPORT		103		// converts sensor messages to ThingSpeak API messages
	#define PRC_WIFI			104		// wifi report sender
	
	#define PRC_CONSUMER1		221		// pwr consumer group 1
	#define PRC_CONSUMER2		222
	#define PRC_CONSUMER3		223

    #define PRC_BME280          301
    #define PRC_MHZ19			302
	//#define PRC_PPD42NS			303
	#define PRC_CJMCU1100		304
	#define PRC_ZE08CH20		305

	#define PRC_MQ4				314
	#define PRC_MQ135			315
	#define PRC_MQ136			316
	#define PRC_MQ7				317

    // ---[ COMMON APP CONFIG ]---
    #define REPORT_TIMEOUT              60000

    // ---[ PWR MNGMNT PRC CFG ]---
    #define PWRMNGMTPINS                {12, 11, 10}
    #define MAXTASKCOUNT                10      // 5?
    #define CONSUMERPROCESSTIMEOUT      20000   // 60000 is production

    // ---[ ADC MUX CFG ]---
    #define ADCMUX_SIGNAL_PIN	A1  //4
    #define ADCMUXPINS			{6, 7, 8, 9}  //{9, 8, 7, 6}
	
    // ---[ SENSORS ]---
    #define MQ_READINGS_PER_RESULT		5
    #define MQ_READING_TIMEOUT          12000

	// !use ADCMUX_SIGNAL_PIN!
    //#define MQ7_ANALOG_PIN_ADDR         0
    //#define MQ136_ANALOG_PIN_ADDR       1
    //#define MQ4_ANALOG_PIN_ADDR         2
    //#define ZE08CH20_ANALOG_PIN_ADDR    3
    //#define MQ135_ANALOG_PIN_ADDR       4
    //#define CJMCU1100_ANALOG_PIN_ADDR   5
	
    #define PPD42NS_ANALOG_PIN_ADDR1    6   // PM1
    #define PPD42NS_ANALOG_PIN_ADDR2    7   // PM2.5

    // ---[ BME280 ]---
    //#define BME280_ADDRESS              (0x76)

    // ---[ MHZ19 UART ]---
    #define MHZ19_RXPIN                 3
    #define MHZ19_TXPIN                 4

    // ---[ WIFI ]---
    #define WIFI_RX_PIN                 2
    #define WIFI_TX_PIN                 3    
    #define WIFI_SSID                   "Jumangee"
    #define WIFI_PWD                    "xxx"

    // ---[ THINGSPEAK ]---
    #define THINGSPEAK_CHANNEL_KEY      "43RGUGMOBYBHCJV2"

    // ---[ REAL-TIME CLOCK ]---
    #define RTC_I2C_ADDR                0x68
    #define RESET_CLOCK                 0
    #define RTC_GET_TEMPERATURE

#endif