#pragma once
#include "adc.h"
#include "avr.h"
#include "list.h"

class Measurement
{
public:
	static Measurement &instance();
	void process();

	void measure();
	unsigned int motorVoltage();
	unsigned int batteryVoltage();
	unsigned int converterVoltage();
	unsigned int CapacitorPlus();
	unsigned int CapacitorMinus();
	unsigned int CapacitorVoltage();

private:
	Measurement(); // private constructor for singleton
	static Measurement instance_;

	class Value
	{
	public:
		Value(const byte adc_port);
		void process();
		unsigned int value();

	private:
		const byte adc_port_;
		List<unsigned int> average_buffer_;
	};
	Value battery_;
	Value converter_;
	Value motor_;
	Value capacitor_plus_;
	Value capacitor_minus_;
};
