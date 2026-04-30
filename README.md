# STM32 Massage Gun Project

## Overview

This project is a basic STM32-controlled massage gun.  
It uses a 12V DC motor to drive a massage head, with speed controlled by a potentiometer.
The STM32 reads the potentiometer voltage and adjusts a PWM signal sent to a motor driver.

## Main Features

- 12V DC motor drive
- STM32 PWM motor control
- Potentiometer speed adjustment
- External motor driver / H-bridge
- Custom PCB for motor power and control
- Basic protection using a fuse and capacitors

## STM32 Pinout

- `PA0` -> USER button input (mode select button)
- `PA1` -> ADC input from potentiometer wiper
- `PA8` -> PWM output to motor driver enable/PWM pin (`TIM1_CH1`)
- `PB10` -> Motor driver `IN1` (direction control)
- `PB11` -> Motor driver `IN2` (direction control)
- `PC6` -> Mode LED (Low)
- `PC7` -> Mode LED (High)
- `PC8` -> Mode LED (Manual)
- `PC9` -> Mode LED (Ramp)
Note: STM32 GND and motor driver GND must be connected together (common ground).

### Potentiometer
- 3.3V -> outer pin
- GND -> outer pin
- ADC input -> center pin

## Setup

1. Connect the potentiometer to 3.3V, GND, and an STM32 ADC pin.
2. Connect STM32 PWM and GPIO pins to the motor driver.
3. Connect the 12V supply to the motor driver through a fuse.
4. Connect the motor to the motor driver outputs.
5. Build and flash the STM32 firmware.
6. Start with the potentiometer turned down, then slowly increase speed.

## Contributors

- Noah Sikorski
- Rachel Bair
- Dean Smith
- Blandine Sitepieng
