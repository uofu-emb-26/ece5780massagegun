#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#include "stm32f0xx.h"

/* Max PWM pulse width (depends on ARR). */
#define MOTOR_PWM_MAX_DUTY 1000

typedef enum {
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_REVERSE = 1
} MotorDirection;

void Motor_PWM_Init(void);
void Motor_SetDutyCycle(uint16_t duty);
void Motor_SetDirection(MotorDirection dir);
/** IN1/PB10 and IN2/PB11 both low (coast / off inputs). */
void Motor_SetInputsBothLow(void);

#endif /* MOTOR_PWM_H */
