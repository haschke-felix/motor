#include "control.h"

Control::Control()
{

}

void Control::init(){
	cp1_.init(&PORTA,&DDRA,1,&PORTA,&DDRA,3,&DDRB,5,&TCCR1A,&OCR1AH,&OCR1AL);
	cp2_.init(&PORTA,&DDRA,2,&PORTA,&DDRA,4,&DDRB,6,&TCCR1A,&OCR1BH,&OCR1BL);
}

void Control::process(){

}
