#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#include "stm32f0xx.h"

// max pwm pulse width (depends on ARR)
#define MOTOR_PWM_MAX_DUTY 1000

void Motor_PWM_Init(void);
void Motor_SetDutyCycle(uint16_t duty);

#endif /* MOTOR_PWM_H */
