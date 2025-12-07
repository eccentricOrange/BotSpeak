#include <string.h>
#include <errno.h>
#include "bot_speak.h"

void botSpeak_serialize(void* sourceBuffer, uint32_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint32_t* destinationLength) {

    uint8_t* source_pointer = (uint8_t*)sourceBuffer;
    int index;

    // Use memcpy for direct copy since endianness does not change
    memcpy(destinationBuffer, source_pointer, numberElements * elementSize);

    // Set the total length of the destination buffer
    *destinationLength = numberElements * elementSize;
}

void botSpeak_deserialize(void* destinationBuffer, uint32_t *numberElements, uint8_t elementSize, uint8_t* sourceBuffer, uint32_t sourceLength) {

    uint8_t* destination_pointer = (uint8_t*)destinationBuffer;
    int index;

    // Use memcpy for direct copy since endianness does not change
    memcpy(destination_pointer, sourceBuffer, sourceLength);

    // Set the number of elements
    *numberElements = sourceLength / elementSize;
}

int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint32_t* destinationLength) {

    destinationBuffer[0] = START_BYTE; // Start byte

    // Timestamp: 4 bytes
    memcpy(&destinationBuffer[1], &sourceFrame->timestamp, sizeof(sourceFrame->timestamp));
    
    // Data length: 4 bytes
    memcpy(&destinationBuffer[5], &sourceFrame->dataLength, sizeof(sourceFrame->dataLength));

    // Packet ID: 4 bytes
    memcpy(&destinationBuffer[9], &sourceFrame->frameID, sizeof(sourceFrame->frameID));

    // Data: variable length
    if (sourceFrame->dataLength > 0 && sourceFrame->data != NULL) {
        memcpy(&destinationBuffer[13], sourceFrame->data, sourceFrame->dataLength);
    }

    // Calculate the total length
    *destinationLength = BOT_SPEAK_MIN_PACKET_SIZE + sourceFrame->dataLength;

    // End byte
    destinationBuffer[(*destinationLength)-1] = END_BYTE;

    return 0;
}


int botSpeak_unpackFrame(DataFrame_TypeDef* destinationFrame, uint8_t* sourceBuffer, uint32_t sourceLength) {

    if (sourceLength < BOT_SPEAK_MIN_PACKET_SIZE || sourceBuffer[0] != START_BYTE || sourceBuffer[sourceLength - 1] != END_BYTE) {
        return -EINVAL; // Invalid frame
    }

    // Extract timestamp
    memcpy(&destinationFrame->timestamp, &sourceBuffer[1], sizeof(destinationFrame->timestamp));

    // Extract data length
    memcpy(&destinationFrame->dataLength, &sourceBuffer[5], sizeof(destinationFrame->dataLength));

    // Extract frame ID
    memcpy(&destinationFrame->frameID, &sourceBuffer[9], sizeof(destinationFrame->frameID));
    
    // Allocate memory for data if data length is greater than zero
    if (destinationFrame->dataLength > 0) {
        destinationFrame->data = (uint8_t*)malloc(destinationFrame->dataLength);
        if (!destinationFrame->data) {
            return -ENOMEM; // Memory allocation failed
        }
        memcpy(destinationFrame->data, &sourceBuffer[13], destinationFrame->dataLength);
    } else {
        destinationFrame->data = NULL;
    }

    return 0;
}