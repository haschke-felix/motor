#include "measurement.h"
#include "usb.h"
#include "Vportpin.h"

#define SAMPLES 5
#if SAMPLES <= 0
#undef SAMPLES
#define SAMPLES 1
#endif

Measurement::Measurement() : battery_(1, SAMPLES), converter_(2, SAMPLES), engine_(3, 50), capacitor_plus_(4,50), capacitor_minus_(5,50)
{
	PortPin(PortPin::C, 1).input();
	PortPin(PortPin::C, 2).input();
	PortPin(PortPin::C, 3).input();
	PortPin(PortPin::C, 4).input();
	PortPin(PortPin::C, 5).input();
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
	engine_.process();
	capacitor_minus_.process();
	capacitor_plus_.process();
}

unsigned int Measurement::engineVoltage()
{
	return engine_.value();
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
