#include "app_modes.h"
#include "motor_pwm.h"
#include "pot_adc.h"
#include "mode_leds.h"

#include "stm32f0xx.h"

/* USER button on Discovery: PA0, active low (pressed = GND). */
#define USER_BTN_PIN 0u
#define BTN_DOWN() (((GPIOA->IDR) & (1u << USER_BTN_PIN)) == 0u)
#define BTN_UP()   (((GPIOA->IDR) & (1u << USER_BTN_PIN)) != 0u)

/*
 * ========== Tune fixed PWM levels here (% of MOTOR_PWM_MAX_DUTY) ==========
 * Used by: Low mode, High mode, Ramp endpoints, and Manual maximum (pot full CW).
 */
#define MODE_LEVEL_LOW_PCT   50u
#define MODE_LEVEL_HIGH_PCT  80u

#if MODE_LEVEL_HIGH_PCT <= MODE_LEVEL_LOW_PCT
#error MODE_LEVEL_HIGH_PCT must be greater than MODE_LEVEL_LOW_PCT
#endif

#define RAMP_STEP_MS    15u
#define RAMP_STEP_SIZE  8

static volatile uint32_t s_tick_ms;
static volatile MotorRunMode s_mode;

static void user_button_gpio_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~(GPIO_MODER_MODER0_Msk);      /* input mode */
    GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0_Msk);/* speed doesn't matter for input */
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0_Msk);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0;           /* pull-up so released reads high */
}

void SysTick_Handler(void) {
    static uint32_t debouncer;
    /* Wait for a released USER button once at boot so noise / reset state cannot advance Off -> Low. */
    static uint8_t button_wait_release = 1u;

    s_tick_ms++;

    if (button_wait_release != 0u) {
        if (BTN_UP()) {
            button_wait_release = 0u;
            debouncer = 0u;
        }
        return;
    }

    debouncer = (debouncer << 1u);
    if (BTN_DOWN()) {
        debouncer |= 1u;
    }

    if (debouncer == 0x7FFFFFFFu) {
        s_mode = (MotorRunMode)(((uint32_t)s_mode + 1u) % (uint32_t)MOTOR_MODE_COUNT);
        debouncer = 0u;
        button_wait_release = 1u;
    }
}

static uint16_t duty_from_percent(uint32_t pct) {
    if (pct > 100u) {
        pct = 100u;
    }
    return (uint16_t)((pct * (uint32_t)MOTOR_PWM_MAX_DUTY) / 100u);
}

static void motor_force_off(void) {
    Motor_SetInputsBothLow();
    Motor_SetDutyCycle(0u);
}

void App_ModesInit(void) {
    SystemCoreClockUpdate();
    user_button_gpio_init();
    s_mode = MOTOR_MODE_OFF;
    s_tick_ms = 0u;
    motor_force_off();
    if (SysTick_Config(SystemCoreClock / 1000u) != 0u) {
        /* SysTick failed: stay in Off; button still inits for later fix */
    }
}

MotorRunMode App_GetMode(void) {
    return s_mode;
}

void App_ModesUpdateMotor(void) {
    static MotorRunMode prev_mode = MOTOR_MODE_COUNT;
    static uint16_t ramp_duty;
    static int8_t ramp_dir;
    static uint32_t ramp_last_ms;

    const uint16_t duty_low = duty_from_percent(MODE_LEVEL_LOW_PCT);
    const uint16_t duty_high = duty_from_percent(MODE_LEVEL_HIGH_PCT);

    MotorRunMode mode = s_mode;

    if (mode != prev_mode) {
        if (mode == MOTOR_MODE_RAMP) {
            /* start ramp clean every time we enter this mode */
            ramp_duty = duty_low;
            ramp_dir = 1;
            ramp_last_ms = s_tick_ms;
        }
        prev_mode = mode;
    }

    uint16_t duty = 0u;

    switch (mode) {
    case MOTOR_MODE_OFF:
        duty = 0u;
        break;
    case MOTOR_MODE_LOW:
        duty = duty_low;
        break;
    case MOTOR_MODE_HIGH:
        duty = duty_high;
        break;
    case MOTOR_MODE_MANUAL: {
        uint16_t raw = Pot_ADC_ReadPA1();
        /* manual mode maps pot 0..4095 to 0..high-level duty */
        duty = (uint16_t)(((uint32_t)raw * (uint32_t)duty_high) / 4095u);
        break;
    }
    case MOTOR_MODE_RAMP: {
        uint32_t now = s_tick_ms;
        if ((uint32_t)(now - ramp_last_ms) >= RAMP_STEP_MS) {
            ramp_last_ms = now;
            int32_t next = (int32_t)ramp_duty + (int32_t)ramp_dir * (int32_t)RAMP_STEP_SIZE;
            if (next >= (int32_t)duty_high) {
                ramp_duty = duty_high;
                ramp_dir = -1;
            } else if (next <= (int32_t)duty_low) {
                ramp_duty = duty_low;
                ramp_dir = 1;
            } else {
                ramp_duty = (uint16_t)next;
            }
        }
        duty = ramp_duty;
        break;
    }
    default:
        duty = 0u;
        break;
    }

    if (mode == MOTOR_MODE_OFF) {
        motor_force_off();
    } else {
        Motor_SetDirection(MOTOR_DIR_FORWARD);
        Motor_SetDutyCycle(duty);
    }

    ModeLeds_Set(mode);
}
