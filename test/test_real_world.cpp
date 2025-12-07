#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include "bot_speak.h"
#include "int_brain_messages.h"

/// @brief Hardware serial port for the Integrated Brain, as an example.
/// @details This should be replaced with the actual serial port used by your device.
#define SERIAL_PORT "/dev/int-brain-xxxxxxxxxxxx"

int main()
{
    using LibSerial::SerialStream;

    SerialStream serialPort;
    serialPort.Open(SERIAL_PORT);
    if (!serialPort.IsOpen())
    {
        fprintf(stderr, "Failed to open serial port %s\n", SERIAL_PORT);
        return 1;
    }

    uint8_t dataBuffer[256];
    uint32_t dataLength;
    uint32_t numberElements;

    float readData[UNSERIALIZED_IMU_MAG_RAW_SIZE];

    DataFrame_TypeDef frame = {
        .frameID = REQUEST_IMU_MAG_RAW,
        .timestamp = 0,
        .dataLength = 0,
        .data = NULL
    };

    botSpeak_packFrame(&frame, dataBuffer, &dataLength);

    while (1)
    {
        serialPort.write((const char *)dataBuffer, dataLength);

        // Wait for a response
        uint8_t responseBuffer[256];
        std::streamsize bytesToRead = BOT_SPEAK_MIN_PACKET_SIZE + SERIALIZED_IMU_MAG_RAW_BYTES;
        serialPort.read((char *)responseBuffer, bytesToRead);

        // Print raw response data
        // printf("Received response: ");
        // for (size_t i = 0; i < bytesToRead; ++i) {
        //     printf("%02X ", responseBuffer[i]);
        // }
        // printf("\n");

        DataFrame_TypeDef responseFrame;
        int result = botSpeak_unpackFrame(&responseFrame, responseBuffer, bytesToRead);
        if (result != 0)
        {
            fprintf(stderr, "Failed to unpack frame: %d\n", result);
            return 1;
        }

        printf("[TIMESTAMP: %f s] ", responseFrame.timestamp / 1000.0);

        botSpeak_deserialize(readData, &numberElements, sizeof(float), responseFrame.data, responseFrame.dataLength);
        printf("Deserialized Data: ");
        for (uint8_t i = 0; i < numberElements; ++i)
        {
            printf("%f ", readData[i]);
        }
        printf("\n");

    }

    return 0;
}