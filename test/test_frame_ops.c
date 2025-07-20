#include <stdio.h>
#include "bot_speak.h"

int testRequestFramePacking(DataFrame_TypeDef* sourceFrame, uint8_t* expectedBytesBuffer) {

    uint8_t actualBytesBuffer[255];
    uint8_t actualBufferLength;

    int status = botSpeak_packFrame(sourceFrame, actualBytesBuffer, &actualBufferLength);

    if (status == 0) {
        printf("[PACKING SUCCEEDED]\n");
    } else {
        printf("[PACKING FAILED]\n");
        return status;
    }
    

    printf("[TESTING BUFFER LENGTH] expected: %d, actual: %d, ", sourceFrame->dataLength + BOT_SPEAK_MIN_PACKET_SIZE, actualBufferLength);
    if (actualBufferLength != BOT_SPEAK_MIN_PACKET_SIZE) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING BUFFER CONTENTS]\n");
    for (int i = 0; i < actualBufferLength; i++) {
        printf("[%02d] expected: 0x%02X, actual: 0x%02X, ", i, expectedBytesBuffer[i], actualBytesBuffer[i]);
        if (expectedBytesBuffer[i] == actualBytesBuffer[i]) {
            printf("Passed!\n");
        } else {
            printf("Failed!\n");
            return -1;
        }
    }

    printf("[FRAME PACKING TEST PASSED]\n");
    return 0;
}

int testResponseFramePacking(DataFrame_TypeDef* sourceFrame, uint8_t* expectedBytesBuffer, uint8_t expectedBufferLength) {

    uint8_t actualBytesBuffer[255];
    uint8_t actualBufferLength;

    int status = botSpeak_packFrame(sourceFrame, actualBytesBuffer, &actualBufferLength);

    if (status == 0) {
        printf("[PACKING SUCCEEDED]\n");
    } else {
        printf("[PACKING FAILED]\n");
        return status;
    }

    printf("[TESTING BUFFER LENGTH] expected: %d, actual: %d, ", expectedBufferLength, actualBufferLength);
    if (expectedBufferLength != actualBufferLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING BUFFER CONTENTS]\n");
    for (int i = 0; i < actualBufferLength; i++) {
        printf("[%02d] expected: 0x%02X, actual: 0x%02X, ", i, expectedBytesBuffer[i], actualBytesBuffer[i]);
        if (expectedBytesBuffer[i] == actualBytesBuffer[i]) {
            printf("Passed!\n");
        } else {
            printf("Failed!\n");
            return -1;
        }
    }

    printf("[FRAME PACKING TEST PASSED]\n");
    return 0;
}


int testMotorCurrentFramePacking() {

    int status;

    DataFrame_TypeDef requestFrame = {
        .frameID = 0x26,
        .timestamp = 0x12345678,
        .dataLength = 0,
        .data = NULL
    };

    uint8_t expectedRequestBuffer[] = {
        START_BYTE,
        0x78, 0x56, 0x34, 0x12, // Timestamp
        0, // Data length
        0x26, 0x00, 0x00, 0x00, // Frame ID
        END_BYTE // End byte
    };

    printf("Testing request frame packing...\n");
    status = testRequestFramePacking(&requestFrame, expectedRequestBuffer);

    if (status != 0) {
        return status;
    }

    printf("\n");

    uint32_t responseData[4] = {0x3931, 0x32BD, 0x31AB, 0x72392313};
    uint8_t serialisedResponseData[16];
    uint8_t serialisedResponseLength;
    botSpeak_serialize(responseData, 4, sizeof(uint32_t), serialisedResponseData, &serialisedResponseLength);

    DataFrame_TypeDef responseFrame = {
        .frameID = 0x46,
        .timestamp = 0x87654321,
        .dataLength = 16,
        .data = serialisedResponseData
    };

    uint8_t expectedResponseBuffer[] = {
        START_BYTE,
        0x21, 0x43, 0x65, 0x87, // Timestamp
        16, // Data length
        0x46, 0x00, 0x00, 0x00, // Frame ID
        0x31, 0x39, 0x00, 0x00, // Data
        0xBD, 0x32, 0x00, 0x00,
        0xAB, 0x31, 0x00, 0x00,
        0x13, 0x23, 0x39, 0x72,
        END_BYTE // End byte
    };

    printf("Testing response frame packing...\n");
    status = testResponseFramePacking(&responseFrame, expectedResponseBuffer, sizeof(expectedResponseBuffer));

    printf("\n");

    if (status != 0) {
        return status;
    }

    return 0;
}

int main() {
    int status;
    printf("\nStarting frame packing tests...\n\n\n");


    status = testMotorCurrentFramePacking();
    if (status != 0) {
        return status;
    }


    printf("All tests completed.\n");
    return 0;
}