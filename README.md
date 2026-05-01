# STM32 Massage Gun Project

## Overview

This is our custom-built STM32-controlled massage gun. We took a standard 12V DC high-torque motor and hooked it up to an eccentric crank mechanism to convert the rotary motion into linear percussive action. 

Instead of just buying an off-the-shelf controller, we designed a custom PCB and wrote the firmware from scratch using an STM32 microcontroller. The system reads analog inputs from a rotary potentiometer, handles user button presses to switch between different massage modes, and outputs a hardware PWM signal to a custom H-bridge motor driver. 

## System Architecture

To get a high-level view of how everything connects, check out the block diagram below. It shows the power delivery (12V battery to regulators and motor), the MCU inputs (button, pot), and the outputs (LEDs, PWM to driver).

<Block diagram here>

## Main Features & Operating Modes

- **12V DC Motor Drive:** High-torque percussive mechanism.
- **STM32 PWM Motor Control:** Hardware timer (`TIM1`) generates clean, high-frequency PWM to avoid annoying motor whine.
- **Custom PCB:** Integrated H-bridge for motor power and logic-level control.
- **Four Operating Modes:** (Switched via the `PA0` USER button)
  - **Manual:** Speed is directly controlled by the potentiometer.
  - **Low:** Fixed low speed for a light massage.
  - **High:** Fixed high speed for deep tissue.
  - **Ramp:** Automatically sweeps the PWM duty cycle up and down.
- **Status Indicators:** Four LEDs on the PCB to let you know exactly which mode you're in.
- **Hardware Protection:** In-line 12V fuse and TVS diodes/bypass caps to handle nasty voltage spikes from the motor coils.

## Hardware & PCB Design

We designed a custom H-bridge motor driver PCB because standard hobby drivers couldn't handle the stall currents without melting. 

- **Motor supply voltage:** 12V DC (usually from a LiPo or beefy wall adapter)
- **Power input protection:** Fast-blow fuse on the 12V input. Seriously, don't bypass the fuse unless you like magic smoke.
- **Voltage spike protection:** TVS diode and beefy decoupling caps used to reduce motor switching noise and flyback voltage.

The PCB layout shown below contains the high-current motor driver section, power input protection, motor output connections, and STM32 control signal connections.

<img width="765" height="851" alt="PCB layout of STM32 massage gun motor driver" src="https://github.com/user-attachments/assets/30934d5f-3813-4c16-9a0d-5f38cf6f8c94" />

## STM32 Pinout & Wiring

Here is the exact pinout we used for the STM32 firmware. **note:** The STM32 ground and the motor driver ground *must* be tied together, otherwise your PWM signals will float and the motor might go full speed at random.

| Pin | Function | Description |
| :--- | :--- | :--- |
| `PA0` | Digital In | USER button (mode select button) |
| `PA1` | Analog In | ADC input from potentiometer wiper |
| `PA8` | Alt Function | PWM output to motor driver enable (`TIM1_CH1`) |
| `PB10` | Digital Out | Motor driver `IN1` (direction control - fixed for one way) |
| `PB11` | Digital Out | Motor driver `IN2` (direction control) |
| `PC6` | Digital Out | Mode LED (Low) |
| `PC7` | Digital Out | Mode LED (High) |
| `PC8` | Digital Out | Mode LED (Manual) |
| `PC9` | Digital Out | Mode LED (Ramp) |

### Potentiometer Wiring
- **Outer pin 1:** 3.3V (from STM32)
- **Outer pin 2:** GND
- **Center pin (wiper):** `PA1` (ADC input)

## Firmware Flow

The code is heavily interrupt and DMA driven. The ADC continuously polls the potentiometer in the background. The main loop is basically a state machine that checks the current mode, reads the button debouncer, and updates the timer's compare register (CCR) to change the duty cycle. 

> **[Insert Firmware State Machine Flowchart Here]**
> *(Placeholder for the flowchart showing the button interrupt debouncing and the transition between Manual, Low, High, and Ramp states)*

## Setup & Getting Started

1. **Wire it up:** Connect the potentiometer to 3.3V, GND, and the STM32 ADC pin (`PA1`).
2. **Hook up logic:** Connect the STM32 PWM (`PA8`) and GPIO direction pins (`PB10`, `PB11`) to the motor driver inputs.
3. **Power:** Connect the 12V supply to the motor driver through a fuse. *Double check polarity!*
4. **Motor connection:** Connect the motor leads to the motor driver output screw terminals.
5. **Flash:** Build the project in STM32CubeIDE and flash the STM32 via ST-Link.
6. **Test:** Start with the potentiometer turned all the way down. Press the user button until you're in Manual mode (check the LED), then slowly increase the speed. 

## Enclosure & Mechanicals

We designed a 3D-printed enclosure to house the motor, battery, custom PCB, and the mechanical linkage. It’s split into halves for easy assembly and troubleshooting. The handle is hollow to fit the battery pack.

<img width="600" height="458" alt="Enclosure CAD view 1" src="https://github.com/user-attachments/assets/e44141c0-7989-43cd-beb8-d80e303606bb" />
<img width="600" height="509" alt="Enclosure CAD view 2" src="https://github.com/user-attachments/assets/0c43e34f-16ca-4d0e-b860-07df4a36974d" />

## Contributors

- **Noah Sikorski**
- **Rachel Bair**
- **Dean Smith**
- **Blandine Sitepieng**
