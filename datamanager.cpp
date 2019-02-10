#include "datamanager.h"
#include "adc.h"

DataManager::DataManager(unsigned int update_count) : update_count_(update_count)
{
	ADC_Init();
	control_.init();
}

void DataManager::process()
{
	control_.process();
	Measurement::instance().process();

	if (update_count_ && ++counter_ >= update_count_)
	{
		counter_ = 0;
		transmitt();
	}
}

union intByte {
	int i;
	byte b[2];
};

void DataManager::transmitt()
{
	PORTC = 0;
	DDRC = 0;
	USB::print(int(ADC_Read(0)));
	USB::print(",");
	USB::print(control_.getPedalSpeed());
	USB::print(",");
	USB::print(control_.getMode() == Control::CAPACITOR);
	USB::print(", ADC1");
	USB::print(Measurement::instance().batteryVoltage()); // 24V
	USB::print(",");
	USB::print(Measurement::instance().converterVoltage()); // 35V
	USB::print(",");
	USB::print(Measurement::instance().engineVoltage()); // Engine Voltage
	USB::print(",");
	USB::print(Measurement::instance().CapacitorPlus()); // cp +
	USB::print(",");
	USB::println(Measurement::instance().CapacitorMinus()); // cp -
}
