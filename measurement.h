#pragma once
#include "adc.h"
#include "avr.h"
#include "list.h"
#include "value.h"

class Measurement
{
public:
	static Measurement &instance();
	void process();

	void measure();
	unsigned int engineVoltage();
	unsigned int batteryVoltage();
	unsigned int converterVoltage();
	unsigned int CapacitorPlus();
	unsigned int CapacitorMinus();
	unsigned int CapacitorVoltage();

private:
	Measurement(); // private constructor for singleton
	static Measurement instance_;

	Value battery_;
	Value converter_;
	Value engine_;
	Value capacitor_plus_;
	Value capacitor_minus_;
};
