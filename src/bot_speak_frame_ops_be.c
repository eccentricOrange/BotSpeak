#include <string.h>
#include <errno.h>
#include "bot_speak_be.h"

void botSpeak_serialize(void* sourceBuffer, uint8_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    uint8_t* source_pointer = (uint8_t*)sourceBuffer;
    int index;

    // Convert to little-endian format
    for (uint8_t i = 0; i < numberElements; ++i) {
        for (uint8_t j = 0; j < elementSize; ++j) {
            index = (i * elementSize) + j;
            destinationBuffer[index] = source_pointer[(i * elementSize) + (elementSize - 1 - j)];
        }
    }

    // Set the total length of the destination buffer
    *destinationLength = numberElements * elementSize;
}

void botSpeak_deserialize(void* destinationBuffer, uint8_t *numberElements, uint8_t elementSize, uint8_t* sourceBuffer, uint8_t sourceLength) {

    uint8_t* destination_pointer = (uint8_t*)destinationBuffer;
    int index;

    // Convert from little-endian format
    for (uint8_t i = 0; i < sourceLength / elementSize; ++i) {
        for (uint8_t j = 0; j < elementSize; ++j) {
            index = (i * elementSize) + j;
            destination_pointer[index] = sourceBuffer[(i * elementSize) + (elementSize - 1 - j)];
        }
    }

    // Set the number of elements
    *numberElements = sourceLength / elementSize;
}

int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    destinationBuffer[0] = START_BYTE; // Start byte

    // Timestamp: 4 bytes
    destinationBuffer[1] = (sourceFrame->timestamp >> 24) & 0xFF;
    destinationBuffer[2] = (sourceFrame->timestamp >> 16) & 0xFF;
    destinationBuffer[3] = (sourceFrame->timestamp >> 8) & 0xFF;
    destinationBuffer[4] = sourceFrame->timestamp & 0xFF;
    
    // Data length: 1 byte
    destinationBuffer[5] = sourceFrame->dataLength;

    // Packet ID: 4 bytes
    destinationBuffer[6] = (sourceFrame->frameID >> 24) & 0xFF;
    destinationBuffer[7] = (sourceFrame->frameID >> 16) & 0xFF;
    destinationBuffer[8] = (sourceFrame->frameID >> 8) & 0xFF;
    destinationBuffer[9] = sourceFrame->frameID & 0xFF;

    // Data: variable length
    if (sourceFrame->dataLength > 0 && sourceFrame->data != NULL) {
        memcpy(&destinationBuffer[10], sourceFrame->data, sourceFrame->dataLength);
    }

    // Calculate the total length
    *destinationLength = BOT_SPEAK_MIN_PACKET_SIZE + sourceFrame->dataLength;

    // End byte
    destinationBuffer[(*destinationLength)-1] = END_BYTE;

    return 0;
}


int botSpeak_unpackFrame(DataFrame_TypeDef* destinationFrame, uint8_t* sourceBuffer, uint8_t sourceLength) {

    if (sourceLength < BOT_SPEAK_MIN_PACKET_SIZE || sourceBuffer[0] != START_BYTE || sourceBuffer[sourceLength - 1] != END_BYTE) {
        return -EINVAL; // Invalid frame
    }

    // Extract timestamp
    destinationFrame->timestamp = (sourceBuffer[1] << 24) | (sourceBuffer[2] << 16) | (sourceBuffer[3] << 8) | (sourceBuffer[4]);

    // Extract data length
    destinationFrame->dataLength = sourceBuffer[5];

    // Extract frame ID
    destinationFrame->frameID = (sourceBuffer[6] << 24) | (sourceBuffer[7] << 16) | (sourceBuffer[8] << 8) | (sourceBuffer[9]);

    // Allocate memory for data if data length is greater than zero
    if (destinationFrame->dataLength > 0) {
        destinationFrame->data = (uint8_t*)malloc(destinationFrame->dataLength);
        if (!destinationFrame->data) {
            return -ENOMEM; // Memory allocation failed
        }
        memcpy(destinationFrame->data, &sourceBuffer[10], destinationFrame->dataLength);
    } else {
        destinationFrame->data = NULL;
    }

    return 0;
}