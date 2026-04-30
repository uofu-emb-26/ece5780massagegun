#include "motor_pwm.h"
#include "pot_adc.h"
#include "mode_leds.h"
#include "app_modes.h"

int main(void) {
    /* bring up peripherals first, then the mode state machine */
    Motor_PWM_Init();
    Pot_ADC_Init();
    ModeLeds_Init();
    App_ModesInit();

    while (1) {
        /* keep motor output in sync with current selected mode */
        App_ModesUpdateMotor();
    }
}
