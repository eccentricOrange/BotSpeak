#include "BotSpeak.h"
#include <errno.h>

int parse_packet_ID(Packet_TypeDef* packet) {

    switch (packet->packet_id) {
        case 0x00 ... 0x1F:
            parse_config_packet(packet);
            break;

        case 0x20 ... 0x3F:
            parse_request_packet(packet);
            break;

        case 0x40 ... 0x5F:
            parse_data_packet(packet);
            break;

        case 0x60 ... 0x7F:
            parse_command_packet(packet);
            break;

        case 0x80 ... 0x9F:
            parse_emergency_packet(packet);
            break;

        default:
            break;
            return -EINVAL;
    }

    return 0;
}