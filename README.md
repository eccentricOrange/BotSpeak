# BotSpeak

A minimal protocol-agnostic communication framework and C library for in systems with diverse message types. BotSpeak produces and consumes raw byte arrays, so it can be used over serial, CAN, or any other byte-stream based protocol. The Physical Layer and Data Link Layer of the OSI model should be handled by the developer.

## Key terms and concepts
*   **Serialize:** Convert a meaningful data-type (such as a `float`) to a byte-array.
*   **Deserialize:** Convert a byte-array to a meaningful data-type (such as a `float`).
*   **Frame:** A high-level data structure. This contains the data as an ID, a length, a timestamp, and a byte-array. See `DataFrame_TypeDef`.
*   **Pack frame:** Convert a byte-array into a frame with metadata.
*   **Unpack frame:** Convert a frame with metadata into a byte-array.

## Data frame structure
The data frame is adapted from the Serial Frame Format implemented in the Python CAN library. Source: https://python-can.readthedocs.io/en/stable/interfaces/serial.html#serial-frame-format

However, our implementation allows for a message  length longer than 1 byte (up to 4 bytes). The frame structure is as follows:

| Byte index | Name | Length | Value |
| --- | --- | --- | --- |
| $0$ | Start of Frame (SOF) | 1 byte | `0xAA` |
| $1$ to $4$ | Frame ID | 4 bytes | Unique identifier for the data type |
| $5$ to $8$ | Data Length | 4 bytes | Length of data payload $n$ |
| $9$ to $12$ | Timestamp | 4 bytes | Timestamp of packet inception, expressed as a Unix timestamp |
| $13$ to $(12+n)$ | Data | $n$ bytes | Actual data payload |
| $(13 + n)$ | End of Frame (EOF) | 1 byte | `0xBB` |

This is realised in the software as a structure:

```c
typedef struct {
    uint32_t frameID;
    uint32_t timestamp;
    uint32_t dataLength;
    uint8_t* data;
} DataFrame_TypeDef;
```

## Typical usage flow

![usage-flow](docs/usage-flow.drawio.svg)

BotSpeak handles the end-to-end conversion from useful data types (e.g., an array of floats) to a byte array directly usable for transmission, and vice versa. BotSpeak is designed to transmit receive Little-Endian data, and a separate header is provided for Big-Endian hosts.

It is recommended to define a set of IDs, and data sizes for your application. An example, implemented using Enums, is provided with this library. Please see [Integrated Brain Conventions Documentation](docs/int-brain-conventions.md) for more details.

Typical usage steps:

1. **Serialize data:** Convert useful data types to a byte array using the provided type-agnostic serialization function. If you're providing an array, you're required to provide the size of each element.

1. **Generate a data frame:** Create a `DataFrame_TypeDef` structure with the frame ID, timestamp, and serialized byte array. Simply populate an instance of type `DataFrame_TypeDef`.

1. **Pack the frame into bytes:** Generate a transmission-ready byte array from the `DataFrame_TypeDef` structure using the provided packing function.

1. **Transmit the byte array:** Send the packed byte array over your chosen communication protocol (e.g., UART, CAN, USB).

1. **Receive a byte array:** Receive a byte array from your communication protocol.

1. **Unpack the received byte array into a frame:** Convert the received byte array back into a `DataFrame_TypeDef` structure using the provided unpacking function.

1. **Deserialize the data:** Convert the byte array in the `DataFrame_TypeDef` structure back into useful data types using the provided deserialization function.

Make sure to include the BotSpeak header in your source files:

```c
#include "bot_speak.h"
```

> [!NOTE]
> If you're working on a Big-Endian system, include the Big-Endian specific header instead:
> ```c
> #include "bot_speak_be.h"
> ```

## Build and Testing

### With `colcon` (ROS 2)

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
   ros2 run botspeak test_frame_ops_be
   ros2 run botspeak test_real_world
   ```

### With traditional `cmake`

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