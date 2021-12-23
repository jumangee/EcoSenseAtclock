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
	//#define PRC_ZE08CH20		305

	#define PRC_MQ4				314
	#define PRC_MQ135			315
	#define PRC_MQ136			316
	#define PRC_MQ7				317

    #define PRC_BTN				401    

    // ---[ COMMON APP CONFIG ]---
    #define REPORT_TIMEOUT      60000
    #define SLIM_BUILD          0       // debug mode: ONLY FEW PROCESSES to get more progmem

    // ---[ DISPLAY ]---
    #define OLED_ADDR           0x3C
    #define MAIN_FONT           System5x7
    //#define MAIN_FONT             fixednums7x15

    // ---[ PWR MNGMNT PRC ]---
    #define PWRMNGMTPIN_PROCESS1    12
    #define PWRMNGMTPIN_PROCESS2    11
    #define PWRMNGMTPIN_PROCESS3    10

    #define PWRMNGMTPINS                {PWRMNGMTPIN_PROCESS1, PWRMNGMTPIN_PROCESS2, PWRMNGMTPIN_PROCESS3}
    #define MAXTASKCOUNT                5
    #define CONSUMERPROCESSTIMEOUT      20000   // 60000 is production

    // ---[ ADC MUX ]---
    #define ADCMUX_SIGNAL_PIN	    A1  //4
    #define ADCMUXPINS			    {6, 7, 8, 9}  //{9, 8, 7, 6}

    #define MUXCHANNEL_MQ4          2
    #define MUXCHANNEL_MQ136        4
    #define MUXCHANNEL_MQ135        3
    #define MUXCHANNEL_CJMCU1100    5
    #define MUXCHANNEL_MQ7          6 // broken sensor :(
    #define MUXCHANNEL_BTN          7
	
    // ---[ SENSORS ]---
    #define MQ_READINGS_PER_RESULT		5
    #define MQ_READING_TIMEOUT          12000

    #define PPD42NS_ANALOG_PIN_ADDR1    8   // PM1
    #define PPD42NS_ANALOG_PIN_ADDR2    9   // PM2.5

    // ---[ SENSOR PRE-BURN TIMEOUT ]---
    #define MHZ18_PREBURN_TIMEOUT       180000
    #define MQ135_PREBURN_TIMEOUT       55000
    #define MQ136_PREBURN_TIMEOUT       57500
    #define MQ7_PREBURN_TIMEOUT         50000
    #define MQ4_PREBURN_TIMEOUT         65000
    #define CJMCU1100_PREBURN_TIMEOUT   77500

    // ---[ BME280 ]---
    //#define BME280_ADDRESS              (0x76)

    // ---[ MHZ19 UART ]---
    #define WAIT_READ_TIMES             100
    #define WAIT_READ_DELAY             10

    #define MHZ19_RXPIN 4
    #define MHZ19_TXPIN 5

    // ---[ WIFI ]---
    #define WIFI_RX_PIN                 2
    #define WIFI_TX_PIN                 3    
    #define WIFI_SSID                   "Jumangee"
    #define WIFI_PWD                    "54d75bc245"

    // ---[ THINGSPEAK ]---
    #define REPORTTOTHINGSPEAK          0
    #if REPORTTOTHINGSPEAK == 1
        #define THINGSPEAK_SERVER       F("api.thingspeak.com")
        #define THINGSPEAK_CHANNEL_KEY  "43RGUGMOBYBHCJV2"
        #define THINGSPEAKREPORT_URI    F("/update?api_key=")
    #else
        #define THINGSPEAK_SERVER       F("example.com")
        #define THINGSPEAK_CHANNEL_KEY  "43RGUGMOBYBHCJV2"
        #define THINGSPEAKREPORT_URI    F("/updateTestp.php?api_key=")
    #endif
    #define THINGSPEAKPARAMS            8

    #define THINGSPEAK_PARAM_TEMP       0
    #define THINGSPEAK_PARAM_HUMIDITY   1
    #define THINGSPEAK_PARAM_PRESSURE   2
    #define THINGSPEAK_PARAM_COMMON     3
    #define THINGSPEAK_PARAM_CO2        4
    #define THINGSPEAK_PARAM_H2S        5
    #define THINGSPEAK_PARAM_VOCS       6
    #define THINGSPEAK_PARAM_PM25       7

    // ---[ REAL-TIME CLOCK ]---
    #define RTC_I2C_ADDR                0x68
    #define RESET_CLOCK                 0
    //#define RTC_GET_TEMPERATURE

#endif