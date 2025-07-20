#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef BOT_SPEAK_H__
#define BOT_SPEAK_H__

#define START_BYTE (uint8_t)0xAA
#define END_BYTE (uint8_t)0xBB

typedef struct {

    uint32_t frameID;
    uint32_t timestamp;
    uint8_t dataLength;
    uint8_t* data;

} DataFrame_TypeDef;

void botSpeak_serialize(void* sourceBuffer, uint8_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint8_t* destinationLength);
void botSpeak_deserialize(void* destinationBuffer, uint8_t *numberElements, uint8_t elementSize, uint8_t* sourceBuffer, uint8_t sourceLength);
int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint8_t* destinationLength);
int botSpeak_unpackFrame(DataFrame_TypeDef* destinationFrame, uint8_t* sourceBuffer, uint8_t sourceLength);

int parseFrameID(DataFrame_TypeDef* frame);
extern int parseConfigFrame(DataFrame_TypeDef* frame);
extern int parseRequestFrame(DataFrame_TypeDef* frame);
extern int parseResponseFrame(DataFrame_TypeDef* frame);
extern int parseCommandFrame(DataFrame_TypeDef* frame);
extern int parseEmergencyFrame(DataFrame_TypeDef* frame);

#endif // BOT_SPEAK_H__