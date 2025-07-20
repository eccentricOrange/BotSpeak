#include "BotSpeak.h"
#include <errno.h>

// int parse_packet_ID(DataFrame_TypeDef* packet) {

//     switch (packet->packet_id) {
//         case 0x00 ... 0x1F:
//             parse_config_packet(packet);
//             break;

//         case 0x20 ... 0x3F:
//             parse_request_packet(packet);
//             break;

//         case 0x40 ... 0x5F:
//             parse_data_packet(packet);
//             break;

//         case 0x60 ... 0x7F:
//             parse_command_packet(packet);
//             break;

//         case 0x80 ... 0x9F:
//             parse_emergency_packet(packet);
//             break;

//         default:
//             break;
//             return -EINVAL;
//     }

//     return 0;
// }

void to_uint8_array_le(const void* source_buffer, size_t num_elements, size_t size_of_element, uint8_t* destination_buffer, uint16_t* destination_length) {

    const uint8_t* source_pointer = (const uint8_t*)source_buffer;
    int index;

    // Convert to little-endian format
    for (size_t i = 0; i < num_elements; ++i) {
        for (size_t j = 0; j < size_of_element; ++j) {
            index = (i * size_of_element) + j;
            destination_buffer[index] = source_pointer[index];
        }

    }

    // Set the total length of the destination buffer
    *destination_length = num_elements * size_of_element;
}

int bot_speak_encode(DataFrame_TypeDef* source_packet, uint8_t* destination_buffer, uint16_t* destination_length)  {

    destination_buffer[0] = 0xAA; // Start byte

    // Timestamp: 4 bytes
    destination_buffer[1] = source_packet->timestamp & 0xFF;
    destination_buffer[2] = (source_packet->timestamp >> 8) & 0xFF;
    destination_buffer[3] = (source_packet->timestamp >> 16) & 0xFF;
    destination_buffer[4] = (source_packet->timestamp >> 24) & 0xFF;

    // Packet ID: 4 bytes
    destination_buffer[5] = source_packet->packet_id & 0xFF;
    destination_buffer[6] = (source_packet->packet_id >> 8) & 0xFF;
    destination_buffer[7] = (source_packet->packet_id >> 16) & 0xFF;
    destination_buffer[8] = (source_packet->packet_id >> 24) & 0xFF;

    // Data length: 2 bytes
    destination_buffer[9] = source_packet->data_length & 0xFF;
    destination_buffer[10] = (source_packet->data_length >> 8) & 0xFF;

    // Data: variable length
    if (source_packet->data != NULL && source_packet->data_length > 0) {
        if (source_packet->data_length > 20) {
            return -EINVAL; // Data length exceeds buffer size
        }
        memcpy(&destination_buffer[11], source_packet->data, source_packet->data_length);
    }

    // Calculate the total length
    *destination_length = 11 + source_packet->data_length + 1;

    // End byte
    destination_buffer[(*destination_length)-1] = 0xBB;

    return 0;
}