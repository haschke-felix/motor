#include "value.h"
#include "adc.h"

Value::Value(const byte adc_port, const int samples) : adc_port_(adc_port), samples_(samples)
{
	for (int i = 0; i < samples_; i++) average_buffer_ << 0;
}

void Value::process()
{
	average_buffer_.removeFirst();
	average_buffer_ << ADC_Read(adc_port_);
}

unsigned int Value::value()
{
	unsigned int value;
	for (int i = 0; i < samples_; i++)
	{
		value += average_buffer_.itemAt(i);
	}
	return value / samples_;
}
