#pragma once
#include "avr.h"
#include "list.h"
class Value
{
public:
	Value(const byte adc_port, const int samples);
	virtual void process();
	virtual unsigned int value();

private:
	const byte adc_port_;
	const int samples_;
	List<unsigned int> average_buffer_;
};
