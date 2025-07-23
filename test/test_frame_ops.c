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


int testRequestFrameUnpacking(DataFrame_TypeDef* expectedFrame, uint8_t* sourceBuffer) {
    
    DataFrame_TypeDef actualFrame;

    int status = botSpeak_unpackFrame(&actualFrame, sourceBuffer, BOT_SPEAK_MIN_PACKET_SIZE);
    if (status != 0) {
        printf("[UNPACKING FAILED]\n");
        return status;
    } else {
        printf("[UNPACKING SUCCEEDED]\n");
    }

    printf("[TESTING FRAME ID] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->frameID, actualFrame.frameID);
    if (expectedFrame->frameID != actualFrame.frameID) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING TIMESTAMP] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->timestamp, actualFrame.timestamp);
    if (expectedFrame->timestamp != actualFrame.timestamp) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING DATA LENGTH] expected: %d, actual: %d, ", expectedFrame->dataLength, 0);
    if (expectedFrame->dataLength != actualFrame.dataLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[FRAME UNPACKING TEST PASSED]\n");
    return 0;
}


int testResponseFrameUnpacking(DataFrame_TypeDef* expectedFrame, uint8_t* sourceBuffer, uint8_t sourceLength) {
    
    DataFrame_TypeDef actualFrame;
    int status = botSpeak_unpackFrame(&actualFrame, sourceBuffer, sourceLength);
    if (status != 0) {
        printf("[UNPACKING FAILED]\n");
        return status;
    } else {
        printf("[UNPACKING SUCCEEDED]\n");
    }

    printf("[TESTING FRAME ID] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->frameID, actualFrame.frameID);
    if (expectedFrame->frameID != actualFrame.frameID) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING TIMESTAMP] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->timestamp, actualFrame.timestamp);
    if (expectedFrame->timestamp != actualFrame.timestamp) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING DATA LENGTH] expected: %d, actual: %d, ", expectedFrame->dataLength, actualFrame.dataLength);
    if (expectedFrame->dataLength != actualFrame.dataLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING DATA CONTENTS]\n");
    for (int i = 0; i < actualFrame.dataLength; i++) {
        printf("[%02d] expected: 0x%02X, actual: 0x%02X, ", i, expectedFrame->data[i], actualFrame.data[i]);
        if (expectedFrame->data[i] != actualFrame.data[i]) {
            printf("Failed!\n");
            return -1;
        } else {
            printf("Passed!\n");
        }
    }

    printf("[FRAME UNPACKING TEST PASSED]\n");
    return 0;
}


int testMotorCurrentRequestFrameUnpacking() {
    int status;

    printf("\nTesting request frame unpacking...\n");
    DataFrame_TypeDef expectedRequestFrame = {
        .frameID = 0x26,
        .timestamp = 0x12345678,
        .dataLength = 0,
        .data = NULL
    };

    uint8_t requestBuffer[] = {
        START_BYTE,
        0x78, 0x56, 0x34, 0x12, // Timestamp
        0, // Data length
        0x26, 0x00, 0x00, 0x00, // Frame ID
        END_BYTE // End byte
    };

    status = testRequestFrameUnpacking(&expectedRequestFrame, requestBuffer);
    if (status != 0) {
        return status;
    }

    printf("\n[FRAME UNPACKING TEST PASSED]\n");
    return 0;
}


int testMotorCurrentResponseFrameUnpacking() {
    int status;

    printf("\nTesting response frame unpacking...\n");
    uint32_t expectedDeserializedData[4] = {0x3931, 0x32BD, 0x31AB, 0x72392313};
    uint8_t expectedSerializedData[] = {
        0x31, 0x39, 0x00, 0x00, // Data
        0xBD, 0x32, 0x00, 0x00,
        0xAB, 0x31, 0x00, 0x00,
        0x13, 0x23, 0x39, 0x72,
    };
    uint8_t responseBuffer[] = {
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
    DataFrame_TypeDef expectedResponseFrame = {
        .frameID = 0x46,
        .timestamp = 0x87654321,
        .dataLength = 16,
        .data = expectedSerializedData
    };

    DataFrame_TypeDef actualResponseFrame;
    status = botSpeak_unpackFrame(&actualResponseFrame, responseBuffer, sizeof(responseBuffer));

    status = testResponseFrameUnpacking(&expectedResponseFrame, responseBuffer, sizeof(responseBuffer));
    if (status != 0) {
        return status;
    }

    printf("\nTesting response frame deserialization...\n");
    uint8_t deserializedData[16];
    uint8_t deserializedLength;
    botSpeak_deserialize(deserializedData, &deserializedLength, sizeof(uint32_t), actualResponseFrame.data, actualResponseFrame.dataLength);

    printf("[TESTING DESERIALIZED DATA LENGTH] expected: %d, actual: %d, ", 4, deserializedLength);
    if (deserializedLength != 4) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    printf("[TESTING DESERIALIZED DATA CONTENTS]\n");
    for (int i = 0; i < deserializedLength; i++) {
        printf("[%02d] expected: 0x%08X, actual: 0x%08X, ", i, expectedDeserializedData[i], ((uint32_t*)deserializedData)[i]);
        if (expectedDeserializedData[i] != ((uint32_t*)deserializedData)[i]) {
            printf("Failed!\n");
            return -1;
        } else {
            printf("Passed!\n");
        }
    }

    printf("[FRAME UNPACKING AND DESERIALIZATION TEST PASSED]\n");
    return 0;
}


int testMotorCurrentRequestFramePacking() {

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

    return 0;
}

int motorCurrentResponseFramePacking() {

    int status;

    printf("Testing response frame packing...\n");

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


    status = testMotorCurrentRequestFramePacking();
    if (status != 0) {
        return status;
    }

    status = motorCurrentResponseFramePacking();
    if (status != 0) {
        return status;
    }

    status = testMotorCurrentRequestFrameUnpacking();
    if (status != 0) {
        return status;
    }

    status = testMotorCurrentResponseFrameUnpacking();
    if (status != 0) {
        return status;
    }


    printf("All tests completed.\n");
    return 0;
}