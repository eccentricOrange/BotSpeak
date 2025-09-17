# Integrated Brain Conventions

BotSpeak is used in the Integrated Brain project, a compact hardware-agnostic R&D platform for AMRs. Please see the project's repositories for more details:

*   PCB design and firmware: [int-brain-stm32](https://github.com/eccentricOrange/int-brain-stm32)
*   ROS2 workspace: [int-brain-ros](https://github.com/exMachina316/int-brain-ros/)
*   Docker containers: [int-brain-containers](https://github.com/eccentricOrange/int-brain-containers/)

## Usage

Include the Integrated Brain conventions header along with BotSpeak headers.

```c
#include "bot_speak.h"
#include "int_brain_messages.h"
```

## ID ranges
| ID start | ID end | param type |
| --- | --- | --- |
| 0x00 | 0x1F | config |
| 0x20 | 0x3F | request |
| 0x40 | 0x5F | response |
| 0x60 | 0x7F | commands |
| 0x80 | 0x9F | emergency |

## Exact fields
| ID | param type | param subtype | size | physical unit |
| --- | --- | --- | --- | --- |
| 0x00 | config | protocol_switch | 1 uint_8 | - |
| 0x01 | config | imu on/off | 1 bool | - |
| 0x02 | config | imu processing | 1 bool | - |
| 0x03 | config | imu rate | 1 uint_32 | Hz |
| 0x04 | config | encoder on/off | 1 bool | - |
| 0x05 | config | encoder processing | 1 bool | - |
| 0x06 | config | encoder rate | 1 uint_32 | Hz |
| 0x07 | config | encoder CPR | 1 uint_32 | - |
| 0x08 | config | motor current meas on/off | 1 bool | - |
| 0x09 | config | motor current meas rate | 1 uint_32 | Hz |
| 0x0A | config | motor controller mode | 1 uint_8 | - |
| 0x0B | config | motor controller rate | 1 uint_32 | Hz |
| 0x0C | config | motor PID KP values | 1 float_32 | - |
| 0x0D | config | motor PID KI values | 1 float_32 | - |
| 0x0E | config | motor PID KD values | 1 float_32 | - |
| 0x0F | config | motor PID filter coefficients | 1 float_32 | - |
| 0x10 | config | motor feedforward params | 2 float_32 | - |
| 0x11 | config | motor reverse directions | 4 bool | - |
| 0x12 | config | battery voltage meas on/off | 1 bool | - |
| 0x13 | config | battery voltage meas rate | 1 uint_32 | Hz |
| 0x21 | request | imu gyro raw | 0 (no data bytes) | - |
| 0x22 | request | imu accel raw | 0 (no data bytes) | - |
| 0x23 | request | imu mag raw | 0 (no data bytes) | - |
| 0x24 | request | imu processed | 0 (no data bytes) | - |
| 0x25 | request | encoder positions | 0 (no data bytes) | - |
| 0x26 | request | encoder velocities | 0 (no data bytes) | - |
| 0x27 | request | motor current | 0 (no data bytes) | - |
| 0x28 | request | battery voltage | 0 (no data bytes) | - |
| 0x29 | request | user defined button | 0 (no data bytes) | - |
| 0x41 | response | imu gyro raw | 3 float_32 | rad / s |
| 0x42 | response | imu accel raw | 3 float_32 | m / s^2 |
| 0x43 | response | imu mag raw | 3 float_32 | uT |
| 0x44 | response | imu processed | 4 float_32 | quaternion |
| 0x45 | response | encoder positions | 4 int_64 | ticks |
| 0x46 | response | encoder velocities | 4 float_32 | rad / s |
| 0x47 | response | motor current | 4 float_32 | A |
| 0x48 | response | battery voltage | 1 float_32 | V |
| 0x49 | response | user defined button | 1 bool | - |
| 0x60 | commands | motor desired speed | 4 float_32 | rad / s |
| 0x61 | commands | user defined LED | 1 uint_16 | - |
| 0x81 | emergency | emergency stop | 1 bool | - |