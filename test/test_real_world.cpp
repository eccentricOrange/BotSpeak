#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>
#include "bot_speak.h"

#define SERIAL_PORT "/dev/ttyACM0"

int main() {
    using LibSerial::SerialStream;

    SerialStream serialPort;
    serialPort.Open(SERIAL_PORT);
    if (!serialPort.IsOpen()) {
        fprintf(stderr, "Failed to open serial port %s\n", SERIAL_PORT);
        return 1;
    }

    uint8_t dataBuffer[256];
    uint8_t dataLength;

    float readData[4];
    uint8_t numberElements = 4;

    DataFrame_TypeDef frame = {
        .frameID = 0x24,
        .timestamp = 0,
        .dataLength = 0,
        .data = NULL
    };

    botSpeak_packFrame(&frame, dataBuffer, &dataLength);

    while (1)
    {
        /* code */
    
    serialPort.write((const char*)dataBuffer, dataLength);

    // Wait for a response
    uint8_t responseBuffer[256];
    serialPort.read((char*)responseBuffer, BOT_SPEAK_MIN_PACKET_SIZE + 16);


    // Print raw response data
    // printf("Received response: ");
    // for (size_t i = 0; i < BOT_SPEAK_MIN_PACKET_SIZE + 16; ++i) {
    //     printf("%02X ", responseBuffer[i]);
    // }
    // printf("\n");

    DataFrame_TypeDef responseFrame;
    int result = botSpeak_unpackFrame(&responseFrame, responseBuffer, BOT_SPEAK_MIN_PACKET_SIZE + 16);
    if (result != 0) {
        fprintf(stderr, "Failed to unpack frame: %d\n", result);
        return 1;
    }


    botSpeak_deserialize(readData, &numberElements, sizeof(float), responseFrame.data, responseFrame.dataLength);

    printf("Deserialized Data: ");
    for (uint8_t i = 0; i < numberElements; ++i) {
        printf("%f ", readData[i]);
    }
    printf("\n");

    }

    

    return 0;
}