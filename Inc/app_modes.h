#ifndef APP_MODES_H
#define APP_MODES_H

#include <stdint.h>

/**
 * Operating profiles, cycled by USER button (PA0)
 * Starts in Off; each debounced press advances one step; release required before next press.
 *
 * Order: Off -> Low (~MODE_LEVEL_LOW_PCT) -> High (~MODE_LEVEL_HIGH_PCT) -> Manual (PA1 pot)
 *     -> Ramp (MODE_LEVEL_LOW_PCT .. MODE_LEVEL_HIGH_PCT ..) -> Off -> ...
 *
 * LED indicators (PC6..PC9): see mode_leds.c.
 */
typedef enum {
    MOTOR_MODE_OFF = 0,
    MOTOR_MODE_LOW,
    MOTOR_MODE_HIGH,
    MOTOR_MODE_MANUAL,
    MOTOR_MODE_RAMP,
    MOTOR_MODE_COUNT
} MotorRunMode;

void App_ModesInit(void);
void App_ModesUpdateMotor(void);

MotorRunMode App_GetMode(void);

#endif /* APP_MODES_H */
