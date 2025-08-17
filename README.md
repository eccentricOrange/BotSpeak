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
| 0x0A | config | motor controller mode | 1 uint_8 |
| 0x0B | config | motor controller rate | 1 uint_32 |
| 0x0C | config | motor PID KP values | 1 float_32 |
| 0x0D | config | motor PID KI values | 1 float_32 |
| 0x0E | config | motor PID KD values | 1 float_32 |
| 0x0F | config | motor PID filter coefficients | 1 float_32 |
| 0x10 | config | motor feedforward params | 2 float_32 |
| 0x11 | config | motor reverse directions | 4 bool |
| 0x12 | config | battery voltage meas on/off | 1 bool |
| 0x13 | config | battery voltage meas rate | 1 uint_32 |
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

## With `colcon` (ROS 2)
1. Navigate to your workspace root.
2. Build the package:
   ```bash
   colcon build --packages-select botspeak
   ```
3. Source the workspace:
   ```bash
   source install/setup.bash
   ```
4. Run the executables:
   ```bash
   ros2 run botspeak test_frame_ops
   ros2 run botspeak test_real_world
   ```

## With traditional `cmake`
1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```
2. Configure and build:
   ```bash
   cmake ..
   make
   ```
3. Run the executables from the `build` directory:
   ```bash
   ./test_frame_ops
   ./test_real_world
   ```
4. Optionally, install the library and executables (may require `sudo`):
   ```bash
   sudo make install
   ```