#include "ecosense_PPD42sensor.h"
#include "ecosense_cfg.h"

static IFirmwareProcess* PPD42SensorProcess::factory(int pId, IProcessMessage* msg) {
	TRACELNF("PPD42SensorProcess::factory");
	return new PPD42SensorProcess(pId, msg);
}

uint16_t PPD42SensorProcess::getInstantValue() {
	return analogRead( ADCMuxManagement::signalPin() );
}

bool PPD42SensorProcess::readingsDone2(byte countPerResult) {
			if (readingsCount2 >= countPerResult) {
				readingsCount2 = 0;
				value2 = 0;
			}
	
			this->value2 += this->getInstantValue();
			readingsCount2++;
			if (readingsCount2 >= countPerResult) {
				value2 = round(value2 / float(readingsCount2));
				return true;
			}
			return false;
		}

void PPD42SensorProcess::update(unsigned long ms) {
            uint32_t ticket;
            ADCMuxManagement* adc = ADCMuxManagement::get();
            ticket = adc->requestPin(PPD42NS_ANALOG_PIN_ADDR1);	// ADC MUX PIN!
	if (ticket) {
		{
			bool done = this->readingsDone(READINGS_PER_RESULT);
			adc->releasePin(ticket);
			if (!done) {
				return;
			}
		}
		this->getHost()->sendMessage(new AirQualityMsg(PM1, 0, this->getVoltage()));
	}
            ticket = adc->requestPin(PPD42NS_ANALOG_PIN_ADDR2);             // ADC MUX PIN!
	if (ticket) {
		{
			bool done = this->readingsDone2(READINGS_PER_RESULT);
			adc->releasePin(ticket);
			if (!done) {
				return;
			}
		}
		this->getHost()->sendMessage(new AirQualityMsg(PM25, 0, this->getVoltage()));
	}
            
	this->pause(ENVSENSORS_TIMEOUT);
}
