#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef BOT_SPEAK_H__
#define BOT_SPEAK_H__

extern const uint8_t MAX_DATA_LENGTH;

typedef struct {

    uint64_t packet_id;
    uint64_t timestamp;
    uint16_t data_length;
    uint8_t* data;

} Packet_TypeDef;


int assemble_packet(Packet_TypeDef* packet, uint8_t* destination_buffer, uint16_t* destination_length);
int parse_packet(Packet_TypeDef* packet, uint8_t* source_buffer, uint16_t source_length);

#endif // BOT_SPEAK_H__