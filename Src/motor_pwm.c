#include "motor_pwm.h"

#define PWM_OUT_AF_TIM1_CH1 2u

void Motor_PWM_Init(void) {
    /* clocks: GPIOA/GPIOB and TIM1 */
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* PA8 -> TIM1_CH1 (AF2) for shared EN1+EN2 PWM output */
    GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
    GPIOA->MODER |= (2U << GPIO_MODER_MODER8_Pos);
    GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL8_Msk;
    GPIOA->AFR[1] |= (PWM_OUT_AF_TIM1_CH1 << GPIO_AFRH_AFSEL8_Pos);
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8_Msk;

    /* PB10 / PB11 -> GPIO outputs for IN1 / IN2 */
    GPIOB->MODER &= ~(GPIO_MODER_MODER10_Msk | GPIO_MODER_MODER11_Msk);
    GPIOB->MODER |= (1U << GPIO_MODER_MODER10_Pos) | (1U << GPIO_MODER_MODER11_Pos);
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_10 | GPIO_OTYPER_OT_11);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR10_Msk | GPIO_PUPDR_PUPDR11_Msk);

    /* 1 kHz PWM from 8 MHz timer clock: (PSC+1)*(ARR+1)=8000 */
    TIM1->PSC = 7U;
    TIM1->ARR = MOTOR_PWM_MAX_DUTY - 1U;
    TIM1->CCR1 = 0U;

    /* CH1 PWM mode 1 with preload */
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
    TIM1->CCMR1 |= (6U << TIM_CCMR1_OC1M_Pos);
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM1->CCER |= TIM_CCER_CC1E;

    /* advanced timer main output enable + auto-reload preload */
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_ARPE;
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->CR1 |= TIM_CR1_CEN;

    Motor_SetInputsBothLow();
}

void Motor_SetDutyCycle(uint16_t duty) {
    if (duty > MOTOR_PWM_MAX_DUTY) {
        duty = MOTOR_PWM_MAX_DUTY;
    }
    TIM1->CCR1 = duty;
}

void Motor_SetDirection(MotorDirection dir) {
    if (dir == MOTOR_DIR_REVERSE) {
        /* reverse: IN1 low, IN2 high */
        GPIOB->BSRR = GPIO_BSRR_BR_10 | GPIO_BSRR_BS_11;
    } else {
        /* forward: IN1 high, IN2 low */
        GPIOB->BSRR = GPIO_BSRR_BS_10 | GPIO_BSRR_BR_11;
    }
}

void Motor_SetInputsBothLow(void) {
    GPIOB->BSRR = GPIO_BSRR_BR_10 | GPIO_BSRR_BR_11;
}
