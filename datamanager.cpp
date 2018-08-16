#include "datamanager.h"

DataManager::DataManager(SPI *spi, int taste_rate) : spi_(spi), taste_rate_(taste_rate)
{
	control_.init();
}

void DataManager::process()
{
	if(counter_ != 0){
		counter_++;
		if(counter_ == 0xFFFF){
			// start new ransmission
			counter_ = 0;
			newTransmission();
		}

	}
}

union intByte{
	int i;
	byte b[2];
};

void DataManager::transmissionFinished()
{
	byte * data = spi_->getData();

	receive.cp1_discharge = data[0];
	receive.cp1_charge = data[1];
	receive.cp1_charge_pwm = data[2];
	receive.cp1_discharge = data[3];
	receive.cp2_charge = data[4];
	receive.cp2_charge_pwm = data[5];
	receive.motor = data[6];
	counter_ = 1;
}

int DataManager::byteArrayToInt(byte *ptr)
{
	intByte u;
	u.b[0] = ptr[0];
	u.b[1] = ptr[1];
	return u.i;
}

void DataManager::intToByteArray(int val, byte & b1, byte & b2){
	intByte u;
	u.i = val;
	b1 = u.b[0];
	b2 = u.b[1];
}

void DataManager::intIntoArray(int val, byte *ptr)
{
	intByte u;
	u.i = val;
	ptr[0] = u.b[0];
	ptr[1] =	u.b[1];
}

void DataManager::newTransmission()
{
	byte buffer [spi_->len()];
	intIntoArray(transmitt.v,&buffer[0]);
	intIntoArray(transmitt.rpm_is,&buffer[2]);
	intIntoArray(transmitt.rpm_should,&buffer[4]);
	intIntoArray(transmitt.motor_voltage_is,&buffer[6]);
	intIntoArray(transmitt.motor_voltage_should,&buffer[8]);
	intIntoArray(transmitt.motor_pwm,&buffer[10]);
	intIntoArray(transmitt.cp1_voltage,&buffer[12]);
	intIntoArray(transmitt.cp2_voltage,&buffer[14]);

	spi_->transmitt(buffer);
}



//void DataManager::process()
//{
//	if(*evaluate_){
//		evaluate();
//	}
////	control_.process();
//}

//void DataManager::evaluate()
//{

//}

