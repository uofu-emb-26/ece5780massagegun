#include "pot_adc.h"

static uint16_t Pot_ADC_ReadChannel(uint32_t chsel) {
    ADC1->CHSELR = chsel;
    ADC1->ISR |= ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR;
    ADC1->CR |= ADC_CR_ADSTART;
    /* tiny blocking read is fine here, this runs in main loop */
    while ((ADC1->ISR & ADC_ISR_EOC) == 0U) {
    }
    return (uint16_t)(ADC1->DR & 0x0FFFU);
}

void Pot_ADC_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    /* PA1 (ADC_IN1) in analog mode */
    GPIOA->MODER |= GPIO_MODER_MODER1_Msk;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR1_Msk;

    /* ADC must be disabled before calibration */
    if ((ADC1->CR & ADC_CR_ADEN) != 0U) {
        ADC1->CR |= ADC_CR_ADDIS;
        while ((ADC1->CR & ADC_CR_ADEN) != 0U) {
        }
    }

    ADC1->CFGR1 = 0U; /* 12-bit, single conversion */
    ADC1->SMPR = 4U;  /* moderate sample time */
    ADC1->CR |= ADC_CR_ADCAL;
    while ((ADC1->CR & ADC_CR_ADCAL) != 0U) {
    }

    ADC1->ISR |= ADC_ISR_ADRDY;
    ADC1->CR |= ADC_CR_ADEN;
    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0U) {
    }
}

uint16_t Pot_ADC_ReadPA1(void) {
    return Pot_ADC_ReadChannel(ADC_CHSELR_CHSEL1);
}
