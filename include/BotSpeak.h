#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef BOT_SPEAK_H__
#define BOT_SPEAK_H__

typedef struct {

    uint32_t packet_id;
    uint32_t timestamp;
    uint16_t data_length;
    uint8_t* data;

} DataFrame_TypeDef;

void to_uint8_array_le(const void* src, size_t num_elements, size_t size_of_element, uint8_t* dest, uint16_t* destination_length);
int bot_speak_encode(DataFrame_TypeDef* source_packet, uint8_t* destination_buffer, uint16_t* destination_length);
int bot_speak_decode(DataFrame_TypeDef* destination_packet, uint8_t* source_buffer, uint16_t source_length);

int parse_packet_ID(DataFrame_TypeDef* packet);
int parse_config_packet(DataFrame_TypeDef* packet);
int parse_request_packet(DataFrame_TypeDef* packet);
int parse_data_packet(DataFrame_TypeDef* packet);
int parse_command_packet(DataFrame_TypeDef* packet);
int parse_emergency_packet(DataFrame_TypeDef* packet);

#endif // BOT_SPEAK_H__