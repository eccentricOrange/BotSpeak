# Data structures and IDs

## ID ranges
| ID start | ID end | param type |
| --- | --- | --- |
| 0x00 | 0x1F | config |
| 0x20 | 0x3F | request |
| 0x40 | 0x5F | response |
| 0x60 | 0x7F | commands |
| 0x80 | 0x9F | emergency |

## Exact fields
| ID | param type | param subtype | size |
| --- | --- | --- | --- |
| 0x00 | config | protocol_switch | 1 uint_8 |
| 0x01 | config | imu on/off | 1 bool |
| 0x02 | config | imu processing | 1 bool |
| 0x03 | config | imu rate | 1 uint_16 |
| 0x04 | config | encoder on/off | 1 bool |
| 0x05 | config | encoder processing | 1 bool |
| 0x06 | config | encoder rate | 1 uint_16 |
| 0x07 | config | motor current meas on/off | 1 bool |
| 0x08 | config | motor current meas rate | 1 uint_16 |
| 0x09 | config | battery voltage meas on/off | 1 bool |
| 0x0A | config | battery voltage meas rate | 1 uint_16 |
| 0x21 | request | imu raw | 0 (no data bytes) |
| 0x22 | request | imu processed | 0 (no data bytes) |
| 0x23 | request | imu both | 0 (no data bytes) |
| 0x24 | request | encoder positions and velocities | 0 (no data bytes) |
| 0x26 | request | motor current | 0 (no data bytes) |
| 0x27 | request | battery voltage | 0 (no data bytes) |
| 0x28 | request | user defined button | 0 (no data bytes) |
| 0x41 | response | imu raw | 6 uint_32 |
| 0x42 | response | imu processed | 4 uint_32 |
| 0x43 | response | imu both | 10 uint_32 |
| 0x44 | response | encoder positions and velocities | 8 uint_32 |
| 0x46 | response | motor current | 4 uint_32 |
| 0x47 | response | battery voltage | 1 uint_32 |
| 0x48 | response | user defined button | 1 bool |
| 0x60 | commands | motor desired RPMs | 4 int_32 |
| 0x61 | commands | user defined LED | 1 uint_16 |
| 0x81 | emergency | emergency stop | 1 bool |

____
# NOTE
- All the protocol bytes are little endian
____

# Build and Testing
1. To run tests:
```bash
make run-test
```