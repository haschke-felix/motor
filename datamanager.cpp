#include "datamanager.h"

DataManager::DataManager(unsigned int update_count) : update_count_(update_count)
{
	control_.init();
	PortPin(PortPin::C, 1).input();
	PortPin(PortPin::C, 2).input();
	PortPin(PortPin::C, 3).input();
	PortPin(PortPin::C, 4).input();
	PortPin(PortPin::C, 5).input();
}

void DataManager::process()
{
	control_.process();
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

// void DataManager::receivedValues(const byte *data)
//{
//	receive_.max_boost_pwm = data[0];
//	control_.setMaxBoostPWM(receive_.max_boost_pwm);
//}

// int DataManager::byteArrayToInt(byte *ptr)
//{
//	intByte u;
//	u.b[0] = ptr[0];
//	u.b[1] = ptr[1];
//	return u.i;
//}

// void DataManager::intToByteArray(int val, byte &b1, byte &b2)
//{
//	intByte u;
//	u.i = val;
//	b1 = u.b[0];
//	b2 = u.b[1];
//}

// void DataManager::intIntoArray(int val, byte *ptr)
//{
//	intByte u;
//	u.i = val;
//	ptr[0] = u.b[0];
//	ptr[1] = u.b[1];
//}

// void DataManager::evaluate()
//{
//	//	if(receive_.cp1_discharge != old_receive_.cp1_discharge)
//	//		control_.setCP1(receive_.cp1_discharge);
//	//	if(receive_.cp2_discharge != old_receive_.cp2_discharge)
//	//		control_.setCP2(receive_.cp2_discharge);

//	//	if(receive_.cp1_charge != old_receive_.cp1_charge)
//	//		control_.setCP1Charge(receive_.cp1_charge);
//	//	if(receive_.cp2_charge != old_receive_.cp2_charge)
//	//		control_.setCP2Charge(receive_.cp2_charge);

//	//	if(receive_.cp1_charge_pwm != old_receive_.cp1_charge_pwm)
//	//		control_.setCP1ChargePWM(receive_.cp1_charge_pwm);
//	//	if(receive_.cp2_charge_pwm != old_receive_.cp2_charge_pwm)
//	//		control_.setCP2ChargePWM(receive_.cp2_charge_pwm);

//	//	if(receive_.motor != old_receive_.motor)
//	//		control_.setMode(Engine::EngineMode(receive_.motor));
//	//	return;
//}

int DataManager::getADC(byte channel)
{
//	if (channel > 7)
//		return 0;
//	ADMUX &= 0xf0 + channel;
//	ADCSRA |= _BV(ADSC);
//	while ((ADCSRA & _BV(ADSC)))
//		;
//	return ADC;
}

void ADC_Init(void)
{
  // die Versorgungsspannung AVcc als Referenz wählen:
  ADMUX = (1 << REFS0);
  // oder interne Referenzspannung als Referenz für den ADC wählen:
  // ADMUX = (1<<REFS1) | (1<<REFS0);

  // Bit ADFR ("free running") in ADCSRA steht beim Einschalten
  // schon auf 0, also single conversion
  ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Frequenzvorteiler
  ADCSRA |= (1 << ADEN);                // ADC aktivieren

  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
     also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */

  ADCSRA |= (1 << ADSC);                // eine ADC-Wandlung
  while (ADCSRA & (1 << ADSC) ) {       // auf Abschluss der Konvertierung warten
  }
  /* ADCW muss einmal gelesen werden, sonst wird Ergebnis der nächsten
     Wandlung nicht übernommen. */
  (void) ADCW;
}

/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel )
{
  // Kanal waehlen, ohne andere Bits zu beeinflußen
  ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
  ADCSRA |= (1 << ADSC);          // eine Wandlung "single conversion"
  while (ADCSRA & (1 << ADSC) ) { // auf Abschluss der Konvertierung warten
  }
  return ADC;                    // ADC auslesen und zurückgeben
}

/* ADC Mehrfachmessung mit Mittelwertbbildung */
/* beachte: Wertebereich der Summenvariablen */
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t nsamples )
{
  uint32_t sum = 0;

  for (uint8_t i = 0; i < nsamples; ++i ) {
    sum += ADC_Read( channel );
  }

  return (uint16_t)( sum / nsamples );
}

void DataManager::transmitt()
{
	PORTC = 0;
	DDRC = 0;
	USB::print(control_.getPedalSpeed());
	USB::print(",");
	USB::print(control_.getMode() == Control::CAPACITOR);
	USB::print(", ADC1");
	USB::print(int(ADC_Read(1)));
	USB::print(",");
	USB::print(int(ADC_Read(2)));
	USB::print(",");
	USB::print(int(ADC_Read(3)));
	USB::print(",");
	USB::print(int(ADC_Read(4)));
	USB::print(",");
	USB::println(int(ADC_Read(5)));
}
