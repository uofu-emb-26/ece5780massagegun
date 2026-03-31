#include "motor_pwm.h"

void Motor_PWM_Init(void) {
    // clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // PC6 to AF0 for TIM3_CH1
    GPIOC->MODER &= ~(GPIO_MODER_MODER6_Msk);
    GPIOC->MODER |= (2 << GPIO_MODER_MODER6_Pos);
    
    GPIOC->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);
    GPIOC->AFR[0] |= (0 << GPIO_AFRL_AFSEL6_Pos);

    // 1kHz PWM at 8MHz
    TIM3->PSC = 7;
    TIM3->ARR = MOTOR_PWM_MAX_DUTY - 1;

    // pwm mode 1
    TIM3->CCMR1 &= ~(TIM_CCMR1_OC1M_Msk);
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);

    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
    TIM3->CCER |= TIM_CCER_CC1E;

    TIM3->CCR1 = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Motor_SetDutyCycle(uint16_t duty) {
    if (duty > MOTOR_PWM_MAX_DUTY) {
        duty = MOTOR_PWM_MAX_DUTY;
    }
    TIM3->CCR1 = duty;
}
