# Key terms and concepts

## Frame-related part of the program
To do with conversions of data only.

*   **Serialize:** Convert a meaningful data-type to a byte-array.
*   **Deserialize:** Convert a byte-array to a meaningful data-type.
*   **Frame:** A high-level data structure. This contains the data as an ID, a length, a timestamp, and a byte-array. See `DataFrame_TypeDef`.
*   **Pack frame:** Convert a byte-array into a frame.
*   **Unpack frame:** Convert a frame into a byte-array.

## Task-related part of the program
To do something with data already in frame format.

[IN PROGRESS]

___

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
| 0x03 | config | imu rate | 1 uint_32 |
| 0x04 | config | encoder on/off | 1 bool |
| 0x05 | config | encoder processing | 1 bool |
| 0x06 | config | encoder rate | 1 uint_32 |
| 0x07 | config | encoder CPR | 1 uint_32 |
| 0x08 | config | motor current meas on/off | 1 bool |
| 0x09 | config | motor current meas rate | 1 uint_32 |
| 0x0A | config | motor closed loop on/off | 1 bool |
| 0x0B | config | motor closed loop rate | 1 uint_32 |
| 0x0C | config | motor closed loop KP | 1 float_32 |
| 0x0D | config | motor closed loop KI | 1 float_32 |
| 0x0E | config | motor closed loop KD | 1 float_32 |
| 0x0F | config | battery voltage meas on/off | 1 bool |
| 0x10 | config | battery voltage meas rate | 1 uint_32 |
| 0x21 | request | imu raw | 0 (no data bytes) |
| 0x22 | request | imu processed | 0 (no data bytes) |
| 0x23 | request | encoder positions | 0 (no data bytes) |
| 0x24 | request | encoder velocities | 0 (no data bytes) |
| 0x25 | request | motor current | 0 (no data bytes) |
| 0x26 | request | battery voltage | 0 (no data bytes) |
| 0x27 | request | user defined button | 0 (no data bytes) |
| 0x41 | response | imu raw | 9 uint_16 |
| 0x42 | response | imu processed | 4 float_32 |
| 0x43 | response | encoder positions | 4 int_64 |
| 0x44 | response | encoder velocities | 4 float_32 |
| 0x45 | response | motor current | 4 float_32 |
| 0x46 | response | battery voltage | 1 float_32 |
| 0x47 | response | user defined button | 1 bool |
| 0x60 | commands | motor desired RPMs | 4 float_32 |
| 0x61 | commands | user defined LED | 1 uint_16 |
| 0x81 | emergency | emergency stop | 1 bool |

____

# NOTE
- All the protocol bytes are little endian

____

# Build and Testing
1. To run tests:
```bash
make run_test_frame_ops
```