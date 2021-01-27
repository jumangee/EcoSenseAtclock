#ifndef __RTCDS3231_H__
#define __RTCDS3231_H__

    #include <Arduino.h>
    #include <Wire.h>

    //DS3231 Register Addresses
    #define DS3231_REG_TIMEDATE  0x00

    #define DS3231_REG_CONTROL   0x0E
    #define DS3231_REG_STATUS    0x0F
    #define DS3231_REG_AGING     0x10
    #define DS3231_REG_TEMP      0x11

    //DS3231 Register Data Size if not just 1
    #define DS3231_REG_TIMEDATE_SIZE 7
    #define DS3231_REG_ALARMONE_SIZE 4
    #define DS3231_REG_ALARMTWO_SIZE 3

    #define DS3231_REG_TEMP_SIZE 2

    // DS3231 Control Register Bits
    #define DS3231_A1IE  0
    #define DS3231_A2IE  1
    #define DS3231_INTCN 2
    #define DS3231_RS1   3
    #define DS3231_RS2   4
    #define DS3231_CONV  5
    #define DS3231_BBSQW 6
    #define DS3231_EOSC  7
    #define DS3231_AIEMASK (_BV(DS3231_A1IE) | _BV(DS3231_A2IE))
    #define DS3231_RSMASK (_BV(DS3231_RS1) | _BV(DS3231_RS2));

    // DS3231 Status Register Bits
    #define DS3231_A1F      0
    #define DS3231_A2F      1
    #define DS3231_BSY      2
    #define DS3231_EN32KHZ  3
    #define DS3231_OSF      7
    #define DS3231_AIFMASK	(_BV(DS3231_A1F) | _BV(DS3231_A2F))

    struct datetime_t {
        int8_t          sec; /**< seconds after the minute - [ 0 to 59 ] */
        int8_t          min; /**< minutes after the hour - [ 0 to 59 ] */
        int8_t          hour; /**< hours since midnight - [ 0 to 23 ] */
        int8_t          mday; /**< day of the month - [ 1 to 31 ] */
        int8_t          wday; /**< days since Sunday - [ 0 to 6 ] */
        int8_t          mon; /**< months since January - [ 0 to 11 ] */
        int16_t         year; /**< years since 1900 */
        int16_t         yday; /**< days since January 1 - [ 0 to 365 ] */
        int16_t         isdst; /**< Daylight Saving Time flag */
    };

    class RtcDS3231SimpleI2C {
        private:
            uint8_t DS3231_ADDRESS;

        public:
            /*enum DS3231SquareWaveClock
            {
                DS3231SquareWaveClock_1Hz  = 0b00000000,
                DS3231SquareWaveClock_1kHz = 0b00001000,
                DS3231SquareWaveClock_4kHz = 0b00010000,
                DS3231SquareWaveClock_8kHz = 0b00011000,
            };*/

            /*enum DS3231SquareWavePinMode
            {
                DS3231SquareWavePin_ModeNone,
                DS3231SquareWavePin_ModeBatteryBackup,
                DS3231SquareWavePin_ModeClock,
                DS3231SquareWavePin_ModeAlarmOne,
                DS3231SquareWavePin_ModeAlarmTwo,
                DS3231SquareWavePin_ModeAlarmBoth
            };*/

            RtcDS3231SimpleI2C(uint8_t ADDR) {
                DS3231_ADDRESS = ADDR;
            }

            void setIsRunning(bool isRunning)
            {
                uint8_t creg = getReg(DS3231_REG_CONTROL);
                if (isRunning)	{
                    creg &= ~_BV(DS3231_EOSC);
                }
                else	{
                    creg |= _BV(DS3231_EOSC);
                }
                setReg(DS3231_REG_CONTROL, creg);
            } 

            bool getIsRunning()
            {
                uint8_t creg = getReg(DS3231_REG_CONTROL);
                return !(creg & _BV(DS3231_EOSC));
            } 

            void enable32kHzPin(bool enable)
            {
                uint8_t sreg = getReg(DS3231_REG_STATUS);

                if (enable == true)
                {
                    sreg |= _BV(DS3231_EN32KHZ);
                }
                else
                {
                    sreg &= ~_BV(DS3231_EN32KHZ);
                }

                setReg(DS3231_REG_STATUS, sreg);
            }

            void disableSquareWavePin() //DS3231SquareWavePinMode pinMode
            {
                uint8_t creg = getReg(DS3231_REG_CONTROL);

                // clear all relevant bits to a known "off" state
                creg &= ~(DS3231_AIEMASK | _BV(DS3231_BBSQW));
                creg |= _BV(DS3231_INTCN);  // set INTCN to disables SQW

                //DS3231SquareWavePin_ModeNone
                /*switch (pinMode)
                {
                    case DS3231SquareWavePin_ModeNone:
                        break;

                    case DS3231SquareWavePin_ModeBatteryBackup:
                        creg |= _BV(DS3231_BBSQW); // set battery backup flag
                        creg &= ~_BV(DS3231_INTCN); // clear INTCN to enable SQW
                        break;

                    case DS3231SquareWavePin_ModeClock:
                        creg &= ~_BV(DS3231_INTCN); // clear INTCN to enable SQW
                        break;

                    case DS3231SquareWavePin_ModeAlarmOne:
                        creg |= _BV(DS3231_A1IE);
                        break;

                    case DS3231SquareWavePin_ModeAlarmTwo:
                        creg |= _BV(DS3231_A2IE);
                        break;

                    case DS3231SquareWavePin_ModeAlarmBoth:
                        creg |= _BV(DS3231_A1IE) | _BV(DS3231_A2IE);
                        break;
                }*/

                setReg(DS3231_REG_CONTROL, creg);
            }

            uint8_t getReg(uint8_t regAddress)
            {
                Wire.begin();
                Wire.beginTransmission(DS3231_ADDRESS);
                Wire.write(regAddress);
                Wire.endTransmission();

                // control register
                Wire.requestFrom(uint8_t(DS3231_ADDRESS), (uint8_t)1);

                uint8_t regValue = Wire.read();
                return regValue;
            }

            void setReg(uint8_t regAddress, uint8_t regValue)
            {
                Wire.begin();
                Wire.beginTransmission(DS3231_ADDRESS);
                Wire.write(regAddress);
                Wire.write(regValue);
                Wire.endTransmission();
            }

            bool isDateTimeValid()
            {
                uint8_t status = getReg(DS3231_REG_STATUS);
                return !(status & _BV(DS3231_OSF));
            }

            datetime_t getNow()
            {
                datetime_t now;
                Wire.beginTransmission(DS3231_ADDRESS);
                Wire.write(DS3231_REG_TIMEDATE);
                Wire.endTransmission();
                Wire.requestFrom(DS3231_ADDRESS, uint8_t(DS3231_REG_TIMEDATE_SIZE));

                now.sec = BcdToUint8(Wire.read() & 0x7F);
                now.min = BcdToUint8(Wire.read());
                now.hour = BcdHourToUint8(Wire.read());
                now.wday = Wire.read() - 1;
                now.mday = BcdToUint8(Wire.read());

                uint8_t century_and_month = Wire.read();
                now.mon = BcdToUint8(century_and_month & 0x7f) - 1;

                now.year = BcdToUint8(Wire.read()) + 100;
                if (century_and_month & _BV(7))	{	 // century wrap flag
                    now.year += 100;
                }

                now.isdst = 0;

                return now;
            }

            void setTime(datetime_t *now)   {
                // clear the invalid flag
                uint8_t status = getReg(DS3231_REG_STATUS);
                status &= ~_BV(DS3231_OSF); // clear the Oscillator Stop Flag
                setReg(DS3231_REG_STATUS, status);

                // set the date time
                Wire.beginTransmission(DS3231_ADDRESS);
                Wire.write(DS3231_REG_TIMEDATE);

                Wire.write(Uint8ToBcd(now->sec));
                Wire.write(Uint8ToBcd(now->min));
                Wire.write(Uint8ToBcd(now->hour)); // 24 hour mode only
                Wire.write(Uint8ToBcd(now->wday + 1));
                Wire.write(Uint8ToBcd(now->mday));

                uint8_t year = now->year - 100;
                uint8_t centuryFlag = 0;

                if (year >= 100)
                {
                    year -= 100;
                    centuryFlag = _BV(7);
                }

                Wire.write(Uint8ToBcd(now->mon + 1) | centuryFlag);
                Wire.write(Uint8ToBcd(year));

                Wire.endTransmission();
            } 

            uint8_t BcdToUint8(uint8_t val)	{
                return val - 6 * (val >> 4);
            }


            uint8_t Uint8ToBcd(uint8_t val)	{
                return val + 6 * (val / 10);
            }

            uint8_t BcdHourToUint8(uint8_t bcdHour)	{
                uint8_t hour;
                if (bcdHour & 0x40)
                {
                    // 12 hour mode, convert to 24
                    bool isPm = ((bcdHour & 0x20) != 0);

                    hour = BcdToUint8(bcdHour & 0x1f);
                    if (isPm)
                    {
                    hour += 12;
                    }
                }
                else
                {
                    hour = BcdToUint8(bcdHour);
                }
                return hour;
            } 

            int StringToInt(const char* pString)
            {
                int value = 0;

                // skip leading 0 and spaces
                while ('0' == *pString || *pString == ' ')
                {
                    pString++;
                }

                // calculate number until we hit non-numeral char
                while ('0' <= *pString && *pString <= '9')
                {
                    value *= 10;
                    value += *pString - '0';
                    pString++;
                }
                return value;
            } 

            // This function takes a 20 chars long textual (English) representation of a date-time value
            // of the kind "Feb 28 2017 00:38:05" and convert it to a standard time_t value
            // This can be useful to convert the concatenation of the __DATE__ and __TIME__
            // compilation date and time strings to a time_t value.
            //
            // Example:
            //
            //  #define COMPILE_DATE_TIME (__DATE__ " " __TIME__)
            //  time_t compiletime = str20ToTime(COMPILE_DATE_TIME);
            //
            // I'm not happy with this function name/implementation and it might change in the future...
            datetime_t str20ToTime(const char* date)
            {
                datetime_t tm_time;

                // Get the month  (N.B.: Jan=0, Feb=1, etc...)
                switch (date[0])
                {
                case 'J':
                    if ( date[1] == 'a' )
                        tm_time.mon = 0;
                    else if ( date[2] == 'n' )
                        tm_time.mon = 5;
                    else
                        tm_time.mon = 6;
                    break;
                case 'F':
                    tm_time.mon = 1;
                    break;
                case 'A':
                    tm_time.mon = date[1] == 'p' ? 3 : 7;
                    break;
                case 'M':
                    tm_time.mon = date[2] == 'r' ? 2 : 4;
                    break;
                case 'S':
                    tm_time.mon = 8;
                    break;
                case 'O':
                    tm_time.mon = 9;
                    break;
                case 'N':
                    tm_time.mon = 10;
                    break;
                case 'D':
                    tm_time.mon = 11;
                    break;
                }

                // Get the day
                tm_time.mday = (int8_t) StringToInt(date + 4);

                // Get the year
                int16_t year =  (int16_t) StringToInt(date + 7);
                tm_time.year = year - 1900;

                // Get the time
                tm_time.hour = (int8_t) StringToInt(date + 12);
                tm_time.min  = (int8_t) StringToInt(date + 15);
                tm_time.sec  = (int8_t) StringToInt(date + 18);
                tm_time.wday = 0;
                tm_time.isdst = 0;

                return tm_time;
            }  
    };

#endif