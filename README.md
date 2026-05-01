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

## Motor Driver / PCB Specifications

- **Driver type:** Custom H-bridge motor driver PCB
- **Motor supply voltage:** 12V DC
- **Power input protection:** Fuse on the 12V input
- **Voltage spike protection:** TVS diode and capacitors used to reduce motor switching spikes

The PCB layout shown below contains the high-current motor driver section, power input protection, motor output connections, and STM32 control signal connections.
  <img width="765" height="851" alt="PCB layout of STM32 massage gun motor driver" src="https://github.com/user-attachments/assets/30934d5f-3813-4c16-9a0d-5f38cf6f8c94" />


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

## Enclosure

<img width="600" height="458" alt="F2BFF639-5BEB-4AE8-BE30-512F5E2FA703" src="https://github.com/user-attachments/assets/e44141c0-7989-43cd-beb8-d80e303606bb" />
<img width="600" height="509" alt="CA27CDD5-D412-4D03-A61D-AB9DE1F843BF" src="https://github.com/user-attachments/assets/0c43e34f-16ca-4d0e-b860-07df4a36974d" />


## Contributors

- Noah Sikorski
- Rachel Bair
- Dean Smith
- Blandine Sitepieng
