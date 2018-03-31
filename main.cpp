#include"avr.h"
#include"spi.h"

volatile byte send_buffer[32];
volatile byte receive_buffer[32];
volatile bool evaluate;



int main(void)
{
	initSPI(&receive_buffer[0],&send_buffer[0], &evaluate);  //Initialize slave SPI


	while(true)
	{
		if(evaluate){

		}
	}
}
