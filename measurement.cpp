#include "measurement.h"
#include "usb.h"

#define SAMPLES 5
#if SAMPLES <= 0
#undef SAMPLES
#define SAMPLES 1
#endif

Measurement::Measurement() : battery_(1), converter_(2), motor_(3), capacitor_plus_(4), capacitor_minus_(5)
{

}

Measurement Measurement::instance_;
Measurement &Measurement::instance()
{
	return instance_;
}

void Measurement::process()
{
	battery_.process();
	converter_.process();
	motor_.process();
	capacitor_minus_.process();
	capacitor_plus_.process();
}

unsigned int Measurement::motorVoltage()
{
	return motor_.value();
}

unsigned int Measurement::batteryVoltage()
{
	return battery_.value();
}

unsigned int Measurement::converterVoltage()
{
	return converter_.value();
}

unsigned int Measurement::CapacitorPlus()
{
	return capacitor_plus_.value();
}

unsigned int Measurement::CapacitorMinus()
{
	return capacitor_minus_.value();
}

unsigned int Measurement::CapacitorVoltage()
{
	return CapacitorPlus() - CapacitorMinus();
}


Measurement::Value::Value(const byte adc_port) : adc_port_(adc_port)
{
	for(int i = 0; i < SAMPLES; i++)
		average_buffer_ << 0;
}
void Measurement::Value::process()
{
	average_buffer_.removeFirst();
	average_buffer_ <<  ADC_Read(adc_port_);
}

unsigned int Measurement::Value::value()
{
	unsigned int value;
	for(int i = 0; i < SAMPLES; i++){
		value += average_buffer_.itemAt(i);
	}
	return value / SAMPLES;
}
