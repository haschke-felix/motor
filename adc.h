#pragma once
#include "avr.h"


void ADC_Init(void);

/* ADC Einzelmessung */
uint16_t ADC_Read( uint8_t channel );
uint16_t ADC_Read_Avg( uint8_t channel, uint8_t samples);
