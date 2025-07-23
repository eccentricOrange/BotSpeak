/**
 * @file bot_speak.h
 * @brief Function declarations for operations related to BotSpeak frames.
 * @details A frame is the high-level data structure used to encapsulate a message.
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef BOT_SPEAK_H__
#define BOT_SPEAK_H__

/// @brief Start byte for BotSpeak packets (SOF: Start of Frame)
#define START_BYTE (uint8_t)0xAA

/// @brief End byte for BotSpeak packets (EOF: End of Frame)
#define END_BYTE (uint8_t)0xBB

/// @brief Minimum size of a BotSpeak packet
/// @details 11 = 1 (start byte) + 4 (timestamp) + 1 (data length) + 4 (packet ID) + 1 (end byte)
#define BOT_SPEAK_MIN_PACKET_SIZE (1 + 4 + 1 + 4 + 1)

/// @brief Structure to hold BotSpeak frame data. `data` should be a pointer to a byte-array. `dataLength` is number of bytes in the byte-array `data`.
typedef struct {
    uint32_t frameID;
    uint32_t timestamp;
    uint8_t dataLength;
    uint8_t* data;
} DataFrame_TypeDef;

/**
 * @brief Converts a value (or array) of a meaningful data-type into a byte array.
 * 
 * @param sourceBuffer Pointer to value (or array) with the meaningful data-type.
 * @param numberElements Number of elements in the source buffer.
 * @param elementSize Size of each element in bytes. E.g., `sizeof(uint32_t)`.
 * @param destinationBuffer Pointer to the destination buffer (byte array).
 * @param destinationLength Pointer to store the number of bytes in the destination buffer.
 */
void botSpeak_serialize(void* sourceBuffer, uint8_t numberElements, uint8_t elementSize, uint8_t* destinationBuffer, uint8_t* destinationLength);

/**
 * @brief Converts a byte array into a value (or array) of a meaningful data-type.
 * 
 * @param destinationBuffer Pointer to value (or array) where the meaningful data-type will be stored.
 * @param numberElements Pointer to store the number of elements in the destination buffer.
 * @param elementSize Size of each element in bytes. E.g., `sizeof(uint32_t)`.
 * @param sourceBuffer Pointer to the source byte array.
 * @param sourceLength Number of bytes in the source byte array.
 */
void botSpeak_deserialize(void* destinationBuffer, uint8_t *numberElements, uint8_t elementSize, uint8_t* sourceBuffer, uint8_t sourceLength);

/**
 * @brief Packs a DataFrame_TypeDef structure into a byte array.
 * 
 * @param sourceFrame Pointer to the DataFrame_TypeDef structure to be packed.
 * @param destinationBuffer Pointer to the destination byte array.
 * @param destinationLength Pointer to store the number of bytes in the packed data.
 * @return `int` Returns 0 on success, or an error code on failure.
 */
int botSpeak_packFrame(DataFrame_TypeDef* sourceFrame, uint8_t* destinationBuffer, uint8_t* destinationLength);

/**
 * @brief Unpacks a byte array into a DataFrame_TypeDef structure.
 * 
 * @param destinationFrame Pointer to the DataFrame_TypeDef structure to be unpacked.
 * @param sourceBuffer Pointer to the source byte array.
 * @param sourceLength Number of bytes in the source byte array.
 * @return `int` Returns 0 on success, or an error code on failure.
 */
int botSpeak_unpackFrame(DataFrame_TypeDef* destinationFrame, uint8_t* sourceBuffer, uint8_t sourceLength);

#endif // BOT_SPEAK_H__