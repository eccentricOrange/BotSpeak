#include <errno.h>
#include "bot_speak.h"

int parseFrameID(DataFrame_TypeDef* frame) {
    int status;

    switch (frame->frameID) {

        case 0x00 ... 0x1F:
            status = parseConfigFrame(frame);
            break;

        case 0x20 ... 0x3F:
            status = parseRequestFrame(frame);
            break;

        case 0x40 ... 0x5F:
            status = parseResponseFrame(frame);
            break;

        case 0x60 ... 0x7F:
            status = parseCommandFrame(frame);
            break;

        case 0x80 ... 0x9F:
            status = parseEmergencyFrame(frame);
            break;

        default:
            status = -EINVAL;
            break;

    }

    return status;
}