#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef BOT_SPEAK_H__
#define BOT_SPEAK_H__

typedef struct {

    uint64_t packet_id;
    uint64_t timestamp;
    uint16_t data_length;
    uint8_t* data;

} Packet_TypeDef;


int assemble_packet(Packet_TypeDef* source_packet, uint8_t* destination_buffer, uint16_t* destination_length);
int parse_packet(Packet_TypeDef* destination_packet, uint8_t* source_buffer, uint16_t source_length);

int parse_packet_ID(Packet_TypeDef* packet);
int parse_config_packet(Packet_TypeDef* packet);
int parse_request_packet(Packet_TypeDef* packet);
int parse_data_packet(Packet_TypeDef* packet);
int parse_command_packet(Packet_TypeDef* packet);
int parse_emergency_packet(Packet_TypeDef* packet);

#endif // BOT_SPEAK_H__