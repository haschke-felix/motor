#include"avr.h"
#include"spi.h"
#include"datamanager.h"

volatile byte send_buffer[32];
volatile byte receive_buffer[32];
volatile bool evaluate;


DataManager manager;

int main(void)
{
	initSPI(&receive_buffer[0],&send_buffer[0],&evaluate);  //Initialize slave SPI
	manager.init(&send_buffer[0],&receive_buffer[0],&evaluate);


	while(true)
	{
		manager.process();
	}
}
