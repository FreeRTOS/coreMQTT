/*
 * coreMQTT <DEVELOPMENT BRANCH>
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file core_mqtt_serializer_private.h
 * @brief Declares the private functions/macros to be used with serialization and
 * deserialization by the core_mqtt library.
 */
#ifndef CORE_MQTT_SERIALIZER_PRIVATE_H
#define CORE_MQTT_SERIALIZER_PRIVATE_H

#include "core_mqtt_serializer.h"

/**
 * @brief Position of the properties for the fieldSet.
 *
 * Each property that can be added to an MQTT packet is assigned a unique bit
 * position (0–31). This macro defines the position of the property
 * in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 *
 * The `fieldSet` is used to track which properties have already been added to prevent
 * duplication, as many MQTT v5 properties must not appear more than once in a packet.
 */

/**
 * @brief Defines the position of the **Subscription Identifier**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SUBSCRIPTION_ID_POS                    ( 1 )

/**
 * @brief Defines the position of the **Session Expiry Interval**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SESSION_EXPIRY_INTERVAL_POS            ( 2 )

/**
 * @brief Defines the position of the **Receive Maximum**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RECEIVE_MAXIMUM_POS                    ( 3 )

/**
 * @brief Defines the position of the **Maximum Packet Size**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_MAX_PACKET_SIZE_POS                    ( 4 )

/**
 * @brief Defines the position of the **Topic Alias Maximum**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_TOPIC_ALIAS_MAX_POS                    ( 5 )

/**
 * @brief Defines the position of the **Request Response Information**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REQUEST_RESPONSE_INFO_POS              ( 6 )

/**
 * @brief Defines the position of the **Request Problem Information**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REQUEST_PROBLEM_INFO_POS               ( 7 )

/**
 * @brief Defines the position of the **Authentication Method**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_AUTHENTICATION_METHOD_POS              ( 9 )

/**
 * @brief Defines the position of the **Authentication Data**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_AUTHENTICATION_DATA_POS                ( 10 )

/**
 * @brief Defines the position of the **Payload Format Indicator**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_PAYLOAD_FORMAT_INDICATOR_POS           ( 11 )

/**
 * @brief Defines the position of the **Message Expiry Interval**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_MESSAGE_EXPIRY_INTERVAL_POS            ( 12 )

/**
 * @brief Defines the position of the **Topic Alias**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_TOPIC_ALIAS_POS                        ( 13 )

/**
 * @brief Defines the position of the **Response Topic**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RESPONSE_TOPIC_POS                     ( 14 )

/**
 * @brief Defines the position of the **Correlation Data**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_CORRELATION_DATA_POS                   ( 15 )

/**
 * @brief Defines the position of the **Content Type**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_CONTENT_TYPE_POS                       ( 16 )

/**
 * @brief Defines the position of the **Reason String**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REASON_STRING_POS                      ( 17 )

/**
 * @brief Defines the position of the **Will Delay Interval**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_WILL_DELAY_POS                         ( 18 )

/**
 * @brief Defines the position of the **Assigned Client Identifier**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_ASSIGNED_CLIENT_ID_POS                 ( 19 )

/**
 * @brief Defines the position of the **Server Keep Alive**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SERVER_KEEP_ALIVE_POS                  ( 20 )

/**
 * @brief Defines the position of the **Response Information**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RESPONSE_INFORMATION_POS               ( 21 )

/**
 * @brief Defines the position of the **Server Reference**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SERVER_REFERENCE_POS                   ( 22 )

/**
 * @brief Defines the position of the **Maximum QoS**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_MAX_QOS_POS                            ( 23 )

/**
 * @brief Defines the position of the **Retain Available**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RETAIN_AVAILABLE_POS                   ( 24 )

/**
 * @brief Defines the position of the **Wildcard Subscription Available**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_WILDCARD_SUBSCRIPTION_AVAILABLE_POS    ( 25 )

/**
 * @brief Defines the position of the **Subscription Identifier Available**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SUBSCRIPTION_ID_AVAILABLE_POS          ( 26 )

/**
 * @brief Defines the position of the **Shared Subscription Available**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SHARED_SUBSCRIPTION_AVAILABLE_POS      ( 27 )

/**
 * @brief Defines the position of the **User property**
 * in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_USER_PROP_POS                          ( 28 )

/* MQTT CONNECT flags. */
#define MQTT_CONNECT_FLAG_CLEAN                     ( 1 ) /**< @brief Clean session. */
#define MQTT_CONNECT_FLAG_WILL                      ( 2 ) /**< @brief Will present. */
#define MQTT_CONNECT_FLAG_WILL_QOS1                 ( 3 ) /**< @brief Will QoS 1. */
#define MQTT_CONNECT_FLAG_WILL_QOS2                 ( 4 ) /**< @brief Will QoS 2. */
#define MQTT_CONNECT_FLAG_WILL_RETAIN               ( 5 ) /**< @brief Will retain. */
#define MQTT_CONNECT_FLAG_PASSWORD                  ( 6 ) /**< @brief Password present. */
#define MQTT_CONNECT_FLAG_USERNAME                  ( 7 ) /**< @brief User name present. */

/**
 * @brief Macro for decoding a 4-byte unsigned int from a sequence of bytes.
 *
 * @param[in] ptr A uint8_t* that points to the high byte.
 */
#define UINT32_DECODE( ptr )                             \
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) | \
                   ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
                   ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) |  \
                   ( ( uint32_t ) ptr[ 3 ] ) )

/**
 * This macro serializes a 32-bit unsigned integer (`val`) into 4 bytes at the
 * specified memory location (`addr`).
 */
#define WRITE_UINT32( addr, val )                                  \
    {                                                              \
        ( addr )[ 3 ] = ( uint8_t ) ( ( ( val ) >> 0 ) & 0xFFU );  \
        ( addr )[ 2 ] = ( uint8_t ) ( ( ( val ) >> 8 ) & 0xFFU );  \
        ( addr )[ 1 ] = ( uint8_t ) ( ( ( val ) >> 16 ) & 0xFFU ); \
        ( addr )[ 0 ] = ( uint8_t ) ( ( ( val ) >> 24 ) & 0xFFU ); \
    }

/**
 * @brief Set a bit in an 8-bit unsigned integer.
 */
#define UINT8_SET_BIT( x, position )      ( ( x ) = ( uint8_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )

/**
 * @brief Clear a bit in an 8-bit unsigned integer.
 */
#define UINT8_CLEAR_BIT( x, position )    ( ( x ) = ( uint8_t ) ( ( x ) & ( ~( 0x01U << ( position ) ) ) ) )

/**
 * @brief Macro for checking if a bit is set in a 1-byte unsigned int.
 *
 * @param[in] x The unsigned int to check.
 * @param[in] position Which bit to check.
 */
#define UINT8_CHECK_BIT( x, position )    ( ( ( x ) & ( 0x01U << ( position ) ) ) == ( 0x01U << ( position ) ) )

/**
 * @brief Get the high byte of a 16-bit unsigned integer.
 */
#define UINT16_HIGH_BYTE( x )             ( ( uint8_t ) ( ( x ) >> 8 ) )

/**
 * @brief Get the low byte of a 16-bit unsigned integer.
 */
#define UINT16_LOW_BYTE( x )              ( ( uint8_t ) ( ( x ) & 0x00ffU ) )

/**
 * @brief Macro for decoding a 2-byte unsigned int from a sequence of bytes.
 *
 * @param[in] ptr A uint8_t* that points to the high byte.
 */
#define UINT16_DECODE( ptr )                            \
    ( uint16_t ) ( ( ( ( uint16_t ) ptr[ 0 ] ) << 8 ) | \
                   ( ( uint16_t ) ptr[ 1 ] ) )

/**
 * @brief Set a bit in an 32-bit unsigned integer.
 */
#define UINT32_SET_BIT( x, position ) \
    ( ( x ) = ( uint32_t ) ( ( x ) | ( ( uint32_t ) 0x01U << ( position ) ) ) )

/**
 * @brief Macro for checking if a bit is set in a 4-byte unsigned int.
 *
 * @param[in] x The unsigned int to check.
 * @param[in] position Which bit to check.
 */
#define UINT32_CHECK_BIT( x, position ) \
    ( ( ( uint32_t ) ( x ) & ( ( uint32_t ) 0x01U << ( position ) ) ) == ( ( uint32_t ) 0x01U << ( position ) ) )

/**
 * @brief A value that represents an invalid remaining length.
 *
 * This value is greater than what is allowed by the MQTT specification.
 */
#define MQTT_REMAINING_LENGTH_INVALID    ( ( uint32_t ) 268435456U )

/**
 * @brief A value that represents the maximum value which can fit in a
 * variable byte integer.
 */
#define MAX_VARIABLE_LENGTH_INT_VALUE    ( ( uint32_t ) 268435455U )

/**
 * @brief Retrieve the size of the remaining length if it were to be encoded.
 *
 * @param[in] length The remaining length to be encoded.
 *
 * @return The size of the remaining length if it were to be encoded.
 */
size_t variableLengthEncodedSize( uint32_t length );

/**
 * @brief Encode a string whose size is at maximum 16 bits in length.
 *
 * @param[out] pDestination Destination buffer for the encoding.
 * @param[in] pSource The source string to encode.
 * @param[in] sourceLength The length of the source string to encode.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * encodeString( uint8_t * pDestination,
                        const char * pSource,
                        uint16_t sourceLength );

/**
 * @brief Validate the length and decode a user property.
 *
 * @param[out] pPropertyKey To store the decoded key.
 * @param[out] pPropertyKeyLen To store the decoded key length.
 * @param[out] pPropertyValue To store the decoded value.
 * @param[out] pPropertyValueLen To store the decoded value length.
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
MQTTStatus_t decodeUserProp( const char ** pPropertyKey,
                             uint16_t * pPropertyKeyLen,
                             const char ** pPropertyValue,
                             uint16_t * pPropertyValueLen,
                             uint32_t * pPropertyLength,
                             uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a 4 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
MQTTStatus_t decodeUint32t( uint32_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a 2 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/

MQTTStatus_t decodeUint16t( uint16_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a 1 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
MQTTStatus_t decodeUint8t( uint8_t * pProperty,
                           uint32_t * pPropertyLength,
                           bool * pUsed,
                           uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a utf 8 string.
 *
 * @param[out] pProperty To store the decoded string.
 * @param[out] pLength  Size of the decoded utf-8 string.
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
MQTTStatus_t decodeUtf8( const char ** pProperty,
                         uint16_t * pLength,
                         uint32_t * pPropertyLength,
                         bool * pUsed,
                         uint8_t ** pIndex );

/**
 * @brief Decodes the variable length by reading a single byte at a time.
 *
 * Uses the algorithm provided in the spec.
 *
 * @param[in] pBuffer Pointer to the buffer.
 * @param[in] bufferLength Length of the remaining buffer.
 * @param[out] pLength Decoded variable length
 *
 * @return #MQTTSuccess if variable length and paramters are valid else #MQTTBadResponse.
 */
MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                   size_t bufferLength,
                                   uint32_t * pLength );

/**
 * @brief Encodes the remaining length of the packet using the variable length
 * encoding scheme provided in the MQTT v3.1.1 specification.
 *
 * @param[out] pDestination The destination buffer to store the encoded remaining
 * length.
 * @param[in] length The remaining length to encode.
 *
 * @return The location of the byte following the encoded value.
 */
uint8_t * encodeRemainingLength( uint8_t * pDestination,
                                 size_t length );

/**
 * @brief Serialize the fixed size part of the ack packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] packetType Type of publish ack
 * @param[in] packetId Packed identifier of the ack packet.
 * @param[in] remainingLength Remaining length of the ack packet.
 * @param[in] reasonCode Reason code for the ack packet.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * serializeAckFixed( uint8_t * pIndex,
                             uint8_t packetType,
                             uint16_t packetId,
                             size_t remainingLength,
                             MQTTSuccessFailReasonCode_t reasonCode );

/**
 * @brief Decodes the property length field in a SUBACK packet.
 *
 * @param[in] pIndex Pointer to the start of the properties in the SUBACK packet.
 * @param[in] remainingLength The remaining length of the MQTT packet being parsed, without Packet ID.
 * @param[out] subackPropertyLength The decoded property length including the size of its encoded representation.
 *
 * @return #MQTTSuccess if the property length is successfully decoded;
 *         #MQTTBadResponse if the decoded property length is greater than the remaining length.
 */
MQTTStatus_t decodeSubackPropertyLength( const uint8_t * pIndex,
                                         uint32_t remainingLength,
                                         uint32_t * subackPropertyLength );

/**
 * @brief Serialize the fixed size part of the disconnect packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the header is to be serialized.
 * @param[in] pReasonCode Reason code for the disconnect packet.
 * @param[in] remainingLength Remaining length of the disconnect packet.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * serializeDisconnectFixed( uint8_t * pIndex,
                                    MQTTSuccessFailReasonCode_t * pReasonCode,
                                    size_t remainingLength );

/**
 * @fn uint8_t * serializeConnectFixedHeader( uint8_t * pIndex, const MQTTConnectInfo_t * pConnectInfo, const MQTTPublishInfo_t * pWillInfo, size_t remainingLength );
 * @brief Serialize the fixed part of the connect packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] pConnectInfo The connect information.
 * @param[in] pWillInfo The last will and testament information.
 * @param[in] remainingLength The remaining length of the packet to be
 * serialized.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * serializeConnectFixedHeader( uint8_t * pIndex,
                                       const MQTTConnectInfo_t * pConnectInfo,
                                       const MQTTPublishInfo_t * pWillInfo,
                                       size_t remainingLength );

/**
 * @fn  uint8_t * serializeSubscribeHeader( size_t remainingLength, uint8_t * pIndex, uint16_t packetId );
 * @brief Serialize the fixed part of the subscribe packet header.
 *
 * @param[in] remainingLength The remaining length of the packet to be
 * serialized.
 * @param[in] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] packetId The packet ID to be serialized.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * serializeSubscribeHeader( size_t remainingLength,
                                    uint8_t * pIndex,
                                    uint16_t packetId );

/**
 * @fn uint8_t * serializeUnsubscribeHeader( size_t remainingLength, uint8_t * pIndex, uint16_t packetId );
 * @brief Serialize the fixed part of the unsubscribe packet header.
 *
 * @param[in] remainingLength The remaining length of the packet to be
 * serialized.
 * @param[in] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] packetId The packet ID to be serialized.
 *
 * @return A pointer to the end of the encoded string.
 */
uint8_t * serializeUnsubscribeHeader( size_t remainingLength,
                                      uint8_t * pIndex,
                                      uint16_t packetId );

#endif /* ifndef CORE_MQTT_SERIALIZER_PRIVATE_H */
