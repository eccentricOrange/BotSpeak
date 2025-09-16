#include <stdio.h>
#include "bot_speak.h"
#include "int_brain_messages.h"


/// @brief Since we run on a resource-rich environment, we can afford to use a larger buffer for testing.
static const int TEST_BUFFER_SIZE = 256;

/**
 * @brief Test if a request data frame can successfully be converted to a byte-array and corresponding length.
 * @details 1. Attempt to pack the data into a byte-array.
 * @details 2. Test the length (number of bytes). Since there is no actual data, this should always be the same (BOT_SPEAK_MIN_PACKET_SIZE).
 * @details 3. Test the actual contents of the byte-array.
 * @param sourceFrame The data frame we wish to convert.
 * @param expectedBytesBuffer The byte-array we expect to see.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testRequestFramePacking(DataFrame_TypeDef* sourceFrame, uint8_t* expectedBytesBuffer) {
    uint8_t actualBytesBuffer[TEST_BUFFER_SIZE];
    uint8_t actualBufferLength;

    // Attempt to pack
    printf("\n\nTesting request frame packing...\n");
    int status = botSpeak_packFrame(sourceFrame, actualBytesBuffer, &actualBufferLength);
    if (status == 0) {
        printf("[PACKING SUCCEEDED]\n");
    } else {
        printf("[PACKING FAILED]\n");
        return status;
    }
    
    // Test the length (this should always be BOT_SPEAK_MIN_PACKET_SIZE)
    printf("[TESTING BUFFER LENGTH] expected: %d, actual: %d, ", sourceFrame->dataLength + BOT_SPEAK_MIN_PACKET_SIZE, actualBufferLength);
    if (actualBufferLength != BOT_SPEAK_MIN_PACKET_SIZE) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Test each byte, one at a time, while logging to standard output.
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

    // If we got here, all tests passed
    printf("[FRAME PACKING TEST PASSED]\n");
    return 0;
}

/**
 * @brief Test if a response data frame can successfully be converted to a byte-array and corresponding length.
 * @details 1. Attempt to pack the data into a byte-array.
 * @details 2. Test the length (number of bytes). This should the minimum length `BOT_SPEAK_MIN_PACKET_SIZE` plus the data length.
 * @details 3. Test the actual contents of the byte-array.
 * @param sourceFrame The data frame we wish to convert.
 * @param expectedBytesBuffer The byte-array we expect to see.
 * @param expectedBufferLength The expected number of bytes in the byte-array.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testResponseFramePacking(DataFrame_TypeDef* sourceFrame, uint8_t* expectedBytesBuffer, uint8_t expectedBufferLength) {
    uint8_t actualBytesBuffer[TEST_BUFFER_SIZE];
    uint8_t actualBufferLength;

    // Attempt to pack
    printf("\n\nTesting response frame packing...\n");
    int status = botSpeak_packFrame(sourceFrame, actualBytesBuffer, &actualBufferLength);
    if (status == 0) {
        printf("[PACKING SUCCEEDED]\n");
    } else {
        printf("[PACKING FAILED]\n");
        return status;
    }

    // Test the length (this should always be BOT_SPEAK_MIN_PACKET_SIZE + data length)
    printf("[TESTING BUFFER LENGTH] expected: %d, actual: %d, ", expectedBufferLength, actualBufferLength);
    if (expectedBufferLength != actualBufferLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Test each byte, one at a time, while logging to standard output.
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

    // If we got here, all tests passed
    printf("[FRAME PACKING TEST PASSED]\n");
    return 0;
}


/**
 * @brief Unpack a request frame and check its contents, by comparing the unpacked `actualFrame` with `expectedFrame`.
 * @details 1. Attempt to unpack the frame from the source buffer.
 * @details 2. Test the frame ID
 * @details 3. Test the timestamp
 * @details 4. Test the data length. Since this is a request frame, the data length should always be 0.
 * @param expectedFrame The expected frame structure.
 * @param sourceBuffer The byte-array containing the packed frame. The length (number of bytes) of this buffer should be `BOT_SPEAK_MIN_PACKET_SIZE`.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testRequestFrameUnpacking(DataFrame_TypeDef* expectedFrame, uint8_t* sourceBuffer) {
    int status;
    DataFrame_TypeDef actualFrame;

    // Attempt to unpack the frame from the source buffer, and store the result in `actualFrame`.
    printf("\n\nTesting request frame unpacking...\n");
    status = botSpeak_unpackFrame(&actualFrame, sourceBuffer, BOT_SPEAK_MIN_PACKET_SIZE);
    if (status != 0) {
        printf("[UNPACKING FAILED]\n");
        return status;
    } else {
        printf("[UNPACKING SUCCEEDED]\n");
    }

    // Test the frame ID
    printf("[TESTING FRAME ID] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->frameID, actualFrame.frameID);
    if (expectedFrame->frameID != actualFrame.frameID) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Test the timestamp
    printf("[TESTING TIMESTAMP] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->timestamp, actualFrame.timestamp);
    if (expectedFrame->timestamp != actualFrame.timestamp) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }
    
    // Test the data length. Since this is a request frame, the data length should always be 0.
    printf("[TESTING DATA LENGTH] expected: %d, actual: %d, ", expectedFrame->dataLength, 0);
    if (expectedFrame->dataLength != actualFrame.dataLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // If we got here, all tests passed
    printf("[FRAME UNPACKING TEST PASSED]\n");
    return 0;
}


/**
 * @brief Unpack a response frame and check its contents, by comparing the unpacked `actualFrame` with `expectedFrame`.
 * @details 1. Attempt to unpack the frame from the source buffer.
 * @details 2. Test the frame ID
 * @details 3. Test the timestamp
 * @details 4. Test the data length. This should be the number of bytes in the data field.
 * @details 5. Test the data contents one by one.
 * @param expectedFrame The expected frame structure.
 * @param sourceBuffer The byte-array containing the packed frame.
 * @param sourceLength The length (number of bytes) of the source buffer.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testResponseFrameUnpacking(DataFrame_TypeDef* expectedFrame, uint8_t* sourceBuffer, uint8_t sourceLength) {
    int status;
    DataFrame_TypeDef actualFrame;

    // Attempt to unpack the frame from the source buffer, and store the result in `actualFrame`.
    printf("\n\nTesting response frame unpacking...\n");
    status = botSpeak_unpackFrame(&actualFrame, sourceBuffer, sourceLength);
    if (status != 0) {
        printf("[UNPACKING FAILED]\n");
        return status;
    } else {
        printf("[UNPACKING SUCCEEDED]\n");
    }

    // Test the frame ID
    printf("[TESTING FRAME ID] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->frameID, actualFrame.frameID);
    if (expectedFrame->frameID != actualFrame.frameID) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Test the timestamp
    printf("[TESTING TIMESTAMP] expected: 0x%08X, actual: 0x%08X, ", expectedFrame->timestamp, actualFrame.timestamp);
    if (expectedFrame->timestamp != actualFrame.timestamp) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }
    
    // Test the data length. Since this is a response frame, the data length should be the number of bytes in the data field.
    printf("[TESTING DATA LENGTH] expected: %d, actual: %d, ", expectedFrame->dataLength, actualFrame.dataLength);
    if (expectedFrame->dataLength != actualFrame.dataLength) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Test the data contents one by one.
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

    // If we got here, all tests passed
    printf("[FRAME UNPACKING TEST PASSED]\n");
    return 0;
}


/**
 * @brief Test the packing of a motor current request frame.
 * @details 1. Create a request frame with no data and the expected byte-array.
 * @details 2. Pack the frame into a byte-array and check the contents.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testMotorCurrentRequestFramePacking() {
    /// @brief Request frame with no data.
    DataFrame_TypeDef requestFrame = {
        .frameID = REQUEST_MOTOR_CURRENT,
        .timestamp = 0x12345678,
        .dataLength = 0,
        .data = NULL
    };

    /// @brief Expected byte-array for the request frame.
    uint8_t expectedRequestBuffer[] = {
        START_BYTE,             // SOF
        0x78, 0x56, 0x34, 0x12, // Timestamp
        0,                      // Data length
        0x27, 0x00, 0x00, 0x00, // Frame ID
        END_BYTE                // EOF
    };

    // Run the test and return the status
    return testRequestFramePacking(&requestFrame, expectedRequestBuffer);
}

/**
 * @brief Test the serialization and packing of a motor current response frame.
 * @details 1. Serialize the expected current values into a byte-array.
 * @details 2. Create a response frame with the serialized data.
 * @details 3. Pack the response frame into a byte-array and check the contents.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int motorCurrentResponseFramePacking() {
    // Serialize the input current values into a byte-array.
    uint32_t currentValues[UNSERIALIZED_MOTOR_CURRENT_SIZE] = {0x3931, 0x32BD, 0x31AB, 0x72392313};
    uint8_t serialisedResponseData[SERIALIZED_MOTOR_CURRENT_BYTES];
    uint8_t serialisedResponseLength;
    botSpeak_serialize(currentValues, UNSERIALIZED_MOTOR_CURRENT_SIZE, sizeof(uint32_t), serialisedResponseData, &serialisedResponseLength);

    /// @brief Response frame with the serialized data.
    DataFrame_TypeDef responseFrame = {
        .frameID = RESPONSE_MOTOR_CURRENT,
        .timestamp = 0x87654321,
        .dataLength = SERIALIZED_MOTOR_CURRENT_BYTES,
        .data = serialisedResponseData
    };

    /// @brief Expected byte-array for the response frame.
    uint8_t expectedResponseBuffer[] = {
        START_BYTE,             // SOF
        0x21, 0x43, 0x65, 0x87, // Timestamp
        16,                     // Data length
        0x47, 0x00, 0x00, 0x00, // Frame ID
        0x31, 0x39, 0x00, 0x00, // Data[0]
        0xBD, 0x32, 0x00, 0x00, // Data[1]
        0xAB, 0x31, 0x00, 0x00, // Data[2]
        0x13, 0x23, 0x39, 0x72, // Data[3]
        END_BYTE                // EOF
    };

    // Run the test and return the status
    return testResponseFramePacking(&responseFrame, expectedResponseBuffer, sizeof(expectedResponseBuffer));
}


/**
 * @brief Test the unpacking of a motor current request frame.
 * @details 1. Create an expected request frame structure.
 * @details 2. Unpack the request frame from a byte-array and check the contents.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testMotorCurrentRequestFrameUnpacking() {

    /// @brief Expected request frame structure.
    DataFrame_TypeDef expectedRequestFrame = {
        .frameID = REQUEST_MOTOR_CURRENT,
        .timestamp = 0x12345678,
        .dataLength = 0,
        .data = NULL
    };

    /// @brief Byte-array containing the packed request frame.
    uint8_t requestBuffer[] = {
        START_BYTE,             // SOF
        0x78, 0x56, 0x34, 0x12, // Timestamp
        0,                      // Data length
        0x27, 0x00, 0x00, 0x00, // Frame ID
        END_BYTE                // EOF
    };

    // Run the test and return the status
    return testRequestFrameUnpacking(&expectedRequestFrame, requestBuffer);
}


/**
 * @brief Test the unpacking and deserialization of a motor current response frame.
 * @details 1. Create an expected response frame structure.
 * @details 2. Unpack the response frame from a byte-array and check the contents.
 * @details 3. Deserialize the data field and check its contents.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
int testMotorCurrentResponseFrameUnpacking() {
    int status;

    /// @brief Expected deserialized data (this is meaningful data).
    uint32_t expectedDeserializedData[UNSERIALIZED_MOTOR_CURRENT_SIZE] = {0x3931, 0x32BD, 0x31AB, 0x72392313};

    /// @brief Expected serialized data (this is the byte-array representation).
    uint8_t expectedSerializedData[] = {
        0x31, 0x39, 0x00, 0x00, // Data[0]
        0xBD, 0x32, 0x00, 0x00, // Data[1]
        0xAB, 0x31, 0x00, 0x00, // Data[2]
        0x13, 0x23, 0x39, 0x72, // Data[3]
    };

    /// @brief Input response frame with the expected serialized data.
    uint8_t responseBuffer[] = {
        START_BYTE,             // SOF
        0x21, 0x43, 0x65, 0x87, // Timestamp
        16,                     // Data length
        0x47, 0x00, 0x00, 0x00, // Frame ID
        0x31, 0x39, 0x00, 0x00, // Data[0]
        0xBD, 0x32, 0x00, 0x00, // Data[1]
        0xAB, 0x31, 0x00, 0x00, // Data[2]
        0x13, 0x23, 0x39, 0x72, // Data[3]
        END_BYTE                // EOF
    };

    /// @brief Expected response frame structure.
    DataFrame_TypeDef expectedResponseFrame = {
        .frameID = RESPONSE_MOTOR_CURRENT,
        .timestamp = 0x87654321,
        .dataLength = SERIALIZED_MOTOR_CURRENT_BYTES,
        .data = expectedSerializedData
    };

    // Unpack the response frame from the byte-array
    DataFrame_TypeDef actualResponseFrame;
    status = botSpeak_unpackFrame(&actualResponseFrame, responseBuffer, sizeof(responseBuffer));

    // Run the unpacking test and check the status
    status = testResponseFrameUnpacking(&expectedResponseFrame, responseBuffer, sizeof(responseBuffer));
    if (status != 0) {
        return status;
    }

    // Attempt to deserialize the data field
    printf("\n\nTesting response frame deserialization...\n");
    uint8_t deserializedData[SERIALIZED_MOTOR_CURRENT_BYTES];
    uint8_t deserializedLength;
    botSpeak_deserialize(deserializedData, &deserializedLength, sizeof(uint32_t), actualResponseFrame.data, actualResponseFrame.dataLength);

    // Check the deserialized data length
    printf("[TESTING DESERIALIZED DATA LENGTH] expected: %d, actual: %d, ", UNSERIALIZED_MOTOR_CURRENT_SIZE, deserializedLength);
    if (deserializedLength != UNSERIALIZED_MOTOR_CURRENT_SIZE) {
        printf("Failed!\n");
        return -1;
    } else {
        printf("Passed!\n");
    }

    // Check the contents of the deserialized data byte-by-byte
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

    // If we got here, all tests passed
    printf("[FRAME UNPACKING AND DESERIALIZATION TEST PASSED]\n");
    return 0;
}


/**
 * @brief Main function to run all the tests.
 * @return `int` 0 if all tests pass, error code otherwise.
 */
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


    printf("\nAll tests completed.\n");
    return 0;
}