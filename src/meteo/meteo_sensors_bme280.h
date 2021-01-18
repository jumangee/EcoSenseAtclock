/**
 * BME280 environment sensor process
 * for Processy Firmware Framework
 */

#ifndef _METEOCLOCK_SENSORS_H
#define _METEOCLOCK_SENSORS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"

#define CO2_SENSOR

#ifdef CO2_SENSOR
	#include "MHZ19_uart/MHZ19_uart.h"
	#define MHZ19_RXPIN 3
	#define MHZ19_TXPIN 4
#endif

class Adafruit_BME280;

class EnvironmentSensorsProcess: public IFirmwareProcess {
	private:
		Adafruit_BME280* bme;
		#ifdef CO2_SENSOR
		MHZ19_uart		mhz19;
		bool			mhz19active;
		#endif
		bool ready;
		bool initDone;

	public:
		//@implement
		//@include <Adafruit_BME280.h>
		//@include "meteo_cfg.h"
		//@include "MHZ19_uart/MHZ19_uart.cpp"
		EnvironmentSensorsProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			//this->log("EnvironmentSensorsProcess::start");

			this->ready = false;
			
			TRACELNF("EnvironmentSensorsProcess::init");
			Wire.begin(); // Wire communication begin
			this->bme = new Adafruit_BME280();

			if (bme->begin(CLOCK2BME280_ADDRESS)) {
				bme->setSampling(Adafruit_BME280::MODE_FORCED,
					Adafruit_BME280::SAMPLING_X1, // temperature
					Adafruit_BME280::SAMPLING_X1, // pressure
					Adafruit_BME280::SAMPLING_X1, // humidity
					Adafruit_BME280::FILTER_OFF);
				
				bme->takeForcedMeasurement();

				TRACELNF("BME: OK");
				this->ready = true;
			} else {
				TRACELNF("BME: ERROR");
			}

			#ifdef CO2_SENSOR
			mhz19active = false;
			mhz19.begin(MHZ19_RXPIN, MHZ19_TXPIN);
			mhz19.setAutoCalibration(false);
			mhz19.getStatus();    // первый запрос, в любом случае возвращает -1
			delay(500);
			if (mhz19.getStatus() == 0) {
				mhz19active = true;
			}
			#endif
		}

		//@implement
		//@include <Arduino.h>
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("EnvironmentSensorsProcess::factory");
			return new EnvironmentSensorsProcess(pId, msg);
		}

		//@implement
		//@include <Arduino.h>
		~EnvironmentSensorsProcess() {
			// stop process
			TRACELNF("EnvironmentSensorsProcess::stop");
			delete this->bme;
		}

		//@implement
		//--@include "processy_cfg.h"
		//@include "meteo_cfg.h"
		//@include "stuff.h"
		void update(unsigned long ms) {
			TRACELNF("EnvironmentSensorsProcess::run");
			if (this->ready) {
				IProcessMessage* msg = readBME280();
				if (msg != NULL) {
					this->getHost()->sendMessage(msg);
				}
			} else {
				this->getHost()->sendMessage(new EnvDataMessage());
			}

			this->pause(ENVSENSORS_TIMEOUT);
		}

		//@implement
		//@include <Adafruit_BME280.h>
		IProcessMessage* readBME280() {
			Wire.begin(); // Wire communication begin
			bme->takeForcedMeasurement();

			//float alt = bme->readAltitude(SEALEVELPRESSURE_HPA);  // первый замер до усреднения
			float temp = bme->readTemperature();
			//TRACEF("T=");
			//TRACELN(temp);
			byte hum = bme->readHumidity();
			int pres = (float)bme->readPressure() * 0.00750062;
			//alt = ((float)alt * 1 + bme->readAltitude(SEALEVELPRESSURE_HPA)) / 2;  // усреднение, чтобы не было резких скачков (с)НР

			EnvDataMessage* msg = new EnvDataMessage(temp, hum, pres);

			#ifdef CO2_SENSOR
			if (this->mhz19active) {
				msg->setCO2(mhz19.getPPM());
			}
			#endif

			return msg;
		}
};

#endif
