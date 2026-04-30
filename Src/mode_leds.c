#include "mode_leds.h"
#include "stm32f0xx.h"

#define MODE_LED_MASK (GPIO_BSRR_BR_6 | GPIO_BSRR_BR_7 | GPIO_BSRR_BR_8 | GPIO_BSRR_BR_9)

/*
 * STM32F072 Discovery (MB1180): LD3–LD6 on PC6–PC9 (blue / orange user LEDs).
 * One LED lit per running mode; all off when Off.
 */
void ModeLeds_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);
    GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR6_Msk | GPIO_OSPEEDR_OSPEEDR7_Msk | GPIO_OSPEEDR_OSPEEDR8_Msk |
                        GPIO_OSPEEDR_OSPEEDR9_Msk);
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR6_Msk | GPIO_PUPDR_PUPDR7_Msk | GPIO_PUPDR_PUPDR8_Msk |
                      GPIO_PUPDR_PUPDR9_Msk);

    ModeLeds_Set(MOTOR_MODE_OFF);
}

void ModeLeds_Set(MotorRunMode mode) {
    /* clear all first so exactly one mode LED is shown */
    GPIOC->BSRR = MODE_LED_MASK;

    switch (mode) {
    case MOTOR_MODE_OFF:
        break;
    case MOTOR_MODE_LOW:
        GPIOC->BSRR = GPIO_BSRR_BS_6;
        break;
    case MOTOR_MODE_HIGH:
        GPIOC->BSRR = GPIO_BSRR_BS_7;
        break;
    case MOTOR_MODE_MANUAL:
        GPIOC->BSRR = GPIO_BSRR_BS_8;
        break;
    case MOTOR_MODE_RAMP:
        GPIOC->BSRR = GPIO_BSRR_BS_9;
        break;
    default:
        break;
    }
}
