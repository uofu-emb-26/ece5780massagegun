#ifndef POT_ADC_H
#define POT_ADC_H

#include "stm32f0xx.h"

void Pot_ADC_Init(void);
uint16_t Pot_ADC_ReadPA1(void);

#endif /* POT_ADC_H */
