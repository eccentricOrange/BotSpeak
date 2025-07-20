#include <string.h>
#include "bot_speak.h"

void botSpeak_serialize(void* sourceBuffer, uint8_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    uint8_t* source_pointer = (uint8_t*)sourceBuffer;
    int index;

    // Convert to little-endian format
    for (uint8_t i = 0; i < numberElements; ++i) {
        for (uint8_t j = 0; j < elementSize; ++j) {
            index = (i * elementSize) + j;
            destinationBuffer[index] = source_pointer[index];
        }

    }

    // Set the total length of the destination buffer
    *destinationLength = numberElements * elementSize;
}

int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    destinationBuffer[0] = START_BYTE; // Start byte

    // Timestamp: 4 bytes
    destinationBuffer[1] = sourceFrame->timestamp & 0xFF;
    destinationBuffer[2] = (sourceFrame->timestamp >> 8) & 0xFF;
    destinationBuffer[3] = (sourceFrame->timestamp >> 16) & 0xFF;
    destinationBuffer[4] = (sourceFrame->timestamp >> 24) & 0xFF;
    
    // Data length: 1 byte
    destinationBuffer[5] = sourceFrame->dataLength;

    // Packet ID: 4 bytes
    destinationBuffer[6] = sourceFrame->frameID & 0xFF;
    destinationBuffer[7] = (sourceFrame->frameID >> 8) & 0xFF;
    destinationBuffer[8] = (sourceFrame->frameID >> 16) & 0xFF;
    destinationBuffer[9] = (sourceFrame->frameID >> 24) & 0xFF;

    // Data: variable length
    if (sourceFrame->dataLength > 0 && sourceFrame->data != NULL) {
        memcpy(&destinationBuffer[10], sourceFrame->data, sourceFrame->dataLength);
    }

    // Calculate the total length
    *destinationLength = 10 + sourceFrame->dataLength + 1;

    // End byte
    destinationBuffer[(*destinationLength)-1] = END_BYTE;

    return 0;
}