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
	#define PRC_PPD42NS			303
	#define PRC_CJMCU1100		304
	//#define PRC_ZE08CH20		305

	#define PRC_MQ4				314
	#define PRC_MQ135			315
	#define PRC_MQ136			316
	#define PRC_MQ7				317

    #define PRC_BTN				401
    #define PRC_BEEPER          402

    // ---[ COMMON APP CONFIG ]---
    #define REPORT_TIMEOUT      60000
    #define SLIM_BUILD          0       // 1=debug mode: only required processes to get more progmem
    #define NOWIFI_BUILD        0       // 1=debug mode: NO wifi process to get more progmem

    // ---[ BTN ]---
    #define BTN_SUPPORT_HOLD    0
    #define BTN_SUPPORT_DBLCLCK 0

    // ---[ DISPLAY ]---
    #define OLED_ADDR           0x3C
    #define MAIN_FONT           System5x7
    //#define MAIN_FONT             fixednums7x15
    #define MAX_DISPLAY_WARNINGS    12

    // ---[ PWR MNGMNT PRC ]---
    #define PWRMNGMTPIN_PROCESS1    12
    #define PWRMNGMTPIN_PROCESS2    11
    #define PWRMNGMTPIN_PROCESS3    10

    #define PWRMNGMTPINS                {PWRMNGMTPIN_PROCESS1, PWRMNGMTPIN_PROCESS2, PWRMNGMTPIN_PROCESS3}
    #define MAXTASKCOUNT                4
    #define CONSUMERPROCESSTIMEOUT      20000   // 60000 is production

    // ---[ ADC MUX ]---
    #define ADCMUX_SIGNAL_PIN	    A1  //4
    #define ADCMUXPINS			    {6, 7, 8, 9}  //{9, 8, 7, 6}

    #define MUXCHANNEL_MQ4          2
    #define MUXCHANNEL_MQ136        4
    #define MUXCHANNEL_MQ135        3
    #define MUXCHANNEL_CJMCU1100    5
    #define MUXCHANNEL_MQ7          6
    #define MUXCHANNEL_BTN          7
    #define MUXCHANNEL_PM1          8
    #define MUXCHANNEL_PM2          9
	
    // ---[ SENSORS ]---
    #define MQ_READINGS_PER_RESULT		5
    #define MQ_READING_TIMEOUT          12000

    #define PPD42NS_ANALOG_PIN_ADDR1    8   // PM1
    #define PPD42NS_ANALOG_PIN_ADDR2    9   // PM2.5

    // ---[ SENSOR PRE-BURN TIMEOUT ]---
    #define MHZ19_PREBURN_TIMEOUT       90000
    #define MQ135_PREBURN_TIMEOUT       75000
    #define MQ136_PREBURN_TIMEOUT       77500
    #define MQ7_PREBURN_TIMEOUT         70000
    #define MQ4_PREBURN_TIMEOUT         74500
    #define CJMCU1100_PREBURN_TIMEOUT   77000
    #define PPD42NS_PREBURN_TIMEOUT     85000

    // ---[ BME280 ]---
    //#define BME280_ADDRESS              (0x76)

    // ---[ MHZ19 UART ]---
    #define WAIT_READ_TIMES             100
    #define WAIT_READ_DELAY             10

    #define MHZ19_RXPIN 4
    #define MHZ19_TXPIN 5

    // ---[ PPD42NS ]---
    #define PPD42NS_SAMPLE_TIME_MS      15000
    #define PPD42NS_READS_COUNT         3   // number of reads before completion (uses average value!)

    // ---[ WIFI ]---
    #define WIFI_RX_PIN                 2
    #define WIFI_TX_PIN                 3    
    #define WIFI_SSID                   "Jumangee"
    #define WIFI_PWD                    "54d75bc245"

    // ---[ THINGSPEAK ]---
    #define REPORTTOTHINGSPEAK          1
    #if REPORTTOTHINGSPEAK == 1
        #define THINGSPEAK_SERVER       F("api.thingspeak.com")
        #define THINGSPEAK_CHANNEL1_KEY F("43RGUGMOBYBHCJV2")
        #define THINGSPEAK_CHANNEL2_KEY F("6CREDT9BTI3QYL3T")
        #define THINGSPEAKREPORT_URI    F("/update?api_key=")
    #else
        #define THINGSPEAK_SERVER       F("example.com")
        #define THINGSPEAK_CHANNEL1_KEY F("TEST")
        #define THINGSPEAK_CHANNEL2_KEY F("TEST2")
        #define THINGSPEAKREPORT_URI    F("/updateTest.php?api_key=")
    #endif
    #define THINGSPEAKPARAMS            8
    
    #define THINGSPEAK_CHANNELS         2
    #define THINGSPEAKCHANNEL_COMMON        0
    #define THINGSPEAKCHANNEL_AIRQUALITY    1

    // THINGSPEAK/CHANNEL1
    #define THINGSPEAK_PARAM_TEMP       0
    #define THINGSPEAK_PARAM_HUMIDITY   1
    #define THINGSPEAK_PARAM_PRESSURE   2
    #define THINGSPEAK_PARAM_PROCESSES  3
    #define THINGSPEAK_PARAM_FREEMEM    4

    // THINGSPEAK/CHANNEL2
    #define THINGSPEAK_PARAM_COMMON     0
    #define THINGSPEAK_PARAM_CO2        1
    #define THINGSPEAK_PARAM_H2S        2
    #define THINGSPEAK_PARAM_VOCS       3
    #define THINGSPEAK_PARAM_PM25       4
    #define THINGSPEAK_PARAM_CO         5

    // ---[ REAL-TIME CLOCK ]---
    #define RTC_I2C_ADDR                0x68
    #define RESET_CLOCK                 0
    //#define RTC_GET_TEMPERATURE

    // ---[ BEEPER ]---
    #define BEEPER_MELODIES             2
    #define BEEPER_MELODY_LENGTH        8

#endif