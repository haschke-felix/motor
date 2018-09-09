#include "datamanager.h"

DataManager::DataManager(SPI *spi, unsigned int sampling_count) : spi_(spi), sampling_count_(sampling_count)
{
	control_.init();
}

void DataManager::process()
{
	control_.process();
	if(sampling_count_ && ++counter_ >= sampling_count_ &&
	      !spi_->transmitting()){
		// transmit data
		counter_ = 0;
		newTransmission();
	}
}

union intByte{
	int i;
	byte b[2];
};

void DataManager::transmissionFinished()
{
	old_receive_ = receive_;
	const byte * data = spi_->getData();

	receive_.cp1_discharge = data[0];
	receive_.cp1_charge = data[1];
	receive_.cp1_charge_pwm = data[2];
	receive_.cp1_discharge = data[3];
	receive_.cp2_charge = data[4];
	receive_.cp2_charge_pwm = data[5];
	receive_.motor = data[6];
	evaluate();
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

void DataManager::evaluate()
{
	if(receive_.cp1_discharge != old_receive_.cp1_discharge)
		control_.setCP1(receive_.cp1_discharge);
	if(receive_.cp2_discharge != old_receive_.cp2_discharge)
		control_.setCP2(receive_.cp2_discharge);

	if(receive_.cp1_charge != old_receive_.cp1_charge)
		control_.setCP1Charge(receive_.cp1_charge);
	if(receive_.cp2_charge != old_receive_.cp2_charge)
		control_.setCP2Charge(receive_.cp2_charge);

	if(receive_.cp1_charge_pwm != old_receive_.cp1_charge_pwm)
		control_.setCP1ChargePWM(receive_.cp1_charge_pwm);
	if(receive_.cp2_charge_pwm != old_receive_.cp2_charge_pwm)
		control_.setCP2ChargePWM(receive_.cp2_charge_pwm);

	if(receive_.motor != old_receive_.motor)
		control_.setMode(Engine::EngineMode(receive_.motor));
	return;
}

void DataManager::newTransmission()
{
	byte buffer [spi_->len()];
	intIntoArray(transmit_.v,&buffer[0]);
	intIntoArray(transmit_.rpm_is,&buffer[2]);
	intIntoArray(transmit_.rpm_should,&buffer[4]);
	intIntoArray(transmit_.motor_voltage_is,&buffer[6]);
	intIntoArray(transmit_.motor_voltage_should,&buffer[8]);
	intIntoArray(transmit_.motor_pwm,&buffer[10]);
	intIntoArray(transmit_.cp1_voltage,&buffer[12]);
	intIntoArray(transmit_.cp2_voltage,&buffer[14]);

	spi_->transmit(buffer);
}
