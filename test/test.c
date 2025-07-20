#include <stdio.h>
#include "BotSpeak.h"

int EXPECT_EQ(uint16_t actual, uint16_t expected, char* test_name) {
    if (actual != expected) {
        printf("Test %s failed: expected %u, got %u\n", test_name, expected, actual);
        return -1;
    }

    printf("Test passed: %s\n", test_name);
    return 0;
}

int EXPECT_EQ_ARRAY(uint8_t* actual, uint8_t* expected, int length, char* test_name) {
    for (int i = 0; i < length; i++) {
        // printf("Comparing index %d: expected %X, got %X\n", i, expected[i], actual[i]);
        if (actual[i] != expected[i]) {
            printf("Test %s failed at index %d: expected %X, got %X\n", test_name, i, expected[i], actual[i]);
            return -1;
        }
    }

    printf("Test passed: %s\n", test_name);
    return 0;
}

void test_bot_speak_encode_request() {
    DataFrame_TypeDef data_frame;
    uint8_t encoded_buffer[40];
    uint16_t encoded_length;

    uint8_t expected_buffer[40] = {
        0xAA,                   // Start byte
        0x25, 0x20, 0x00, 0x00, // Timestamp
        0x24, 0x00, 0x00, 0x00, // Packet ID
        0x00, 0x00,             // Data length
                                // Data: empty
        0xBB                    // End byte
    };

    data_frame.packet_id = 0x24;
    data_frame.timestamp = 0x2025;
    data_frame.data_length = 0;
    data_frame.data = NULL;

    int return_code = bot_speak_encode(&data_frame, encoded_buffer, &encoded_length);

    if (return_code != 0) {
        printf("Encoding failed with error code: %d\n", return_code);
        return;
    }
    EXPECT_EQ(encoded_length, 12, "Encoding length check");
    EXPECT_EQ_ARRAY(encoded_buffer, expected_buffer, encoded_length, "Encoding buffer check");
}

void test_bot_speak_encode_response() {
    DataFrame_TypeDef data_frame;
    uint8_t encoded_buffer[40];
    uint16_t encoded_length;

    uint16_t data_length;
    uint32_t imu_data_source[] = {0x12345678, 0x9ABCDEF0, 0x11223344, 0x55667788};
    uint8_t imu_data_bytes[sizeof(imu_data_source)];
    to_uint8_array_le(imu_data_source, 4, sizeof(uint32_t), imu_data_bytes, &data_length);
    
    uint8_t expected_buffer[40] = {
        0xAA,                   // Start byte
        0x25, 0x20, 0x00, 0x00, // Timestamp
        0x46, 0x00, 0x00, 0x00, // Packet ID
        0x10, 0x00,             // Data length
        0x78, 0x56, 0x34, 0x12, // IMU data
        0xF0, 0xDE, 0xBC, 0x9A, // IMU data
        0x44, 0x33, 0x22, 0x11, // IMU data
        0x88, 0x77, 0x66, 0x55, // IMU data
        0xBB                    // End byte
    };

    data_frame.packet_id = 0x46;
    data_frame.timestamp = 0x2025;
    data_frame.data_length = data_length;
    data_frame.data = imu_data_bytes;

    int return_code = bot_speak_encode(&data_frame, encoded_buffer, &encoded_length);

    if (return_code != 0) {
        printf("Encoding failed with error code: %d\n", return_code);
        return;
    }
    EXPECT_EQ(encoded_length, 28, "Encoding length check");
    EXPECT_EQ_ARRAY(encoded_buffer, expected_buffer, encoded_length, "Encoding buffer check");
}

int main() {
    test_bot_speak_encode_request();
    test_bot_speak_encode_response();
    return 0;
}