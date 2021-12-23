/**
 * MQ sensors parent process
 * for Processy Firmware Framework
 * MQ sensors parent process
 * 
 * to use, learn MQ sensors article:
 * https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
 * 
 * m-value:
 * [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], 
 * 
 * b-value:
 * [log("RsRo3") - m * log(ppm3)]
 * 
 * 
 * js:
 * var lg = Math.log; var m = (lg(0.41) - lg(0.19))/(lg(4) - lg(60)); console.log( "m=" + m ); console.log( "b=" + ( (lg(0.17) - m * lg(17)) ) )
 * https://www.wolframalpha.com/input/?i=%28+Log10%280.26%29+-++log10%281.62%29+%29+%2F+%28+log10%2810000%29+-+log10%28200%29+%29
 * 
 * curve:
 * [m-value, b-value]
 */

#ifndef _MQ_SENSOR_PROCESS_H
#define _MQ_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "simplesensor_process.h"
#include "adcmux_mngmnt.h"

#include "ecosense_messages.h"

//#define PREHEAT_TIME 45000

class MQSensorProcess: public SimpleSensorProcess {
	protected:
		byte channel;

	public:
		MQSensorProcess(byte channel, IProcessMessage* msg);

		uint16_t getInstantValue();

		/*byte getQuality(float k = .6) {
			return exp((getVoltage()-5)*k)*127;
		}*/

		void update(unsigned long ms);

		AirQualityMsg* getSimpleResultMsg(AirQualityMsg::GasType gas, float value) {
			return new AirQualityMsg(
				gas, 
				value < 2.25 ? AirQualityMsg::GasConcentration::MINIMAL : (
					value < 3.5 ? AirQualityMsg::GasConcentration::NORM : (
						value < 4.25 ? AirQualityMsg::GasConcentration::WARNING : AirQualityMsg::GasConcentration::DANGER
					)
				),
				value
			);
		};

		virtual IProcessMessage* getResultMsg() = 0;
};

#endif
