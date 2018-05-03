#include "control.h"

Control::Control()
{

}

void Control::init(){
	engine_.init(&PORTA,&DDRA,0,&PORTB,&DDRB,7,&TCCR1A,&OCR1BH,&OCR1BL);
	cp1_.init(&PORTA,&DDRA,1,&PORTA,&DDRA,3,&DDRB,5,&TCCR1A,&OCR1AH,&OCR1AL,&engine_);
	cp2_.init(&PORTA,&DDRA,2,&PORTA,&DDRA,4,&DDRB,6,&TCCR1A,&OCR1BH,&OCR1BL,&engine_);
}

void Control::process()
{
	engine_.process();
	cp1_.process();
	cp2_.process();
}
