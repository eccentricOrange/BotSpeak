#include <string.h>
#include <errno.h>
#include "bot_speak.h"

void botSpeak_serialize(void* sourceBuffer, uint8_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    uint8_t* source_pointer = (uint8_t*)sourceBuffer;

    // Use memcpy for direct copy since endianness does not change
    memcpy(destinationBuffer, source_pointer, numberElements * elementSize);

    // Set the total length of the destination buffer
    *destinationLength = numberElements * elementSize;
}

void botSpeak_deserialize(void* destinationBuffer, uint8_t *numberElements, uint8_t elementSize, uint8_t* sourceBuffer, uint8_t sourceLength) {

    uint8_t* destination_pointer = (uint8_t*)destinationBuffer;

    // Use memcpy for direct copy since endianness does not change
    memcpy(destination_pointer, sourceBuffer, sourceLength);

    // Set the number of elements
    *numberElements = sourceLength / elementSize;
}

int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint8_t* destinationLength) {

    destinationBuffer[0] = START_BYTE; // Start byte

    // Timestamp: 4 bytes
    memcpy(&destinationBuffer[1], &sourceFrame->timestamp, sizeof(sourceFrame->timestamp));
    
    // Data length: 1 byte
    destinationBuffer[5] = sourceFrame->dataLength;

    // Packet ID: 4 bytes
    memcpy(&destinationBuffer[6], &sourceFrame->frameID, sizeof(sourceFrame->frameID));

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
    memcpy(&destinationFrame->timestamp, &sourceBuffer[1], sizeof(destinationFrame->timestamp));

    // Extract data length
    memcpy(&destinationFrame->dataLength, &sourceBuffer[5], sizeof(destinationFrame->dataLength));

    // Extract frame ID
    memcpy(&destinationFrame->frameID, &sourceBuffer[6], sizeof(destinationFrame->frameID));

    // Extract data
    if (destinationFrame->dataLength > 0) {
        memcpy(destinationFrame->data, &sourceBuffer[10], destinationFrame->dataLength);
    }

    return 0;
}