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
 * @file core_mqtt_serializer.c
 * @brief Implements the user-facing functions in core_mqtt_serializer.h.
 */
#include <string.h>
#include <assert.h>

#include "core_mqtt_serializer.h"
#include "core_mqtt_utils.h"
#include "core_mqtt_config_defaults.h"

/**
 * @brief Size of the fixed and variable header of a CONNECT packet.
 */
#define MQTT_PACKET_CONNECT_HEADER_SIZE             ( 10UL )

/* MQTT CONNECT flags. */
#define MQTT_CONNECT_FLAG_CLEAN                     ( 1 ) /**< @brief Clean session. */
#define MQTT_CONNECT_FLAG_WILL                      ( 2 ) /**< @brief Will present. */
#define MQTT_CONNECT_FLAG_WILL_QOS1                 ( 3 ) /**< @brief Will QoS 1. */
#define MQTT_CONNECT_FLAG_WILL_QOS2                 ( 4 ) /**< @brief Will QoS 2. */
#define MQTT_CONNECT_FLAG_WILL_RETAIN               ( 5 ) /**< @brief Will retain. */
#define MQTT_CONNECT_FLAG_PASSWORD                  ( 6 ) /**< @brief Password present. */
#define MQTT_CONNECT_FLAG_USERNAME                  ( 7 ) /**< @brief User name present. */

/*
 * Positions of each flag in the first byte of an MQTT PUBLISH packet's
 * fixed header.
 */
#define MQTT_PUBLISH_FLAG_RETAIN                    ( 0 ) /**< @brief MQTT PUBLISH retain flag. */
#define MQTT_PUBLISH_FLAG_QOS1                      ( 1 ) /**< @brief MQTT PUBLISH QoS1 flag. */
#define MQTT_PUBLISH_FLAG_QOS2                      ( 2 ) /**< @brief MQTT PUBLISH QoS2 flag. */
#define MQTT_PUBLISH_FLAG_DUP                       ( 3 ) /**< @brief MQTT PUBLISH duplicate flag. */

/**
 * @brief A PINGREQ packet is always 2 bytes in size, defined by MQTT 5.0 spec.
 */
#define MQTT_PACKET_PINGREQ_SIZE                    ( 2UL )

/*
 * Constants relating to CONNACK packets, defined by MQTT spec.
 */
#define MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK    ( ( uint8_t ) 0x01U ) /**< @brief The "Session Present" bit is always the lowest bit. */

/**
 * @brief Minimum Length of PUBACK, PUBREC, PUBREL, PUBCOMP Packets
 */
#define MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH     ( ( uint8_t ) 2 )

/**
 * @brief A PINGRESP packet always has a "Remaining length" of 0. */
#define MQTT_PACKET_PINGRESP_REMAINING_LENGTH       ( 0U )

/**
 * @brief Per the MQTT spec, the largest "Remaining Length" of an MQTT
 * packet is this value, 256 MB.
 */
#define MQTT_MAX_REMAINING_LENGTH                   ( 268435455UL )

/**
 * @brief Per the MQTT spec, the max packet size  can be of  max remaining length + 5 bytes
 */
#define MQTT_MAX_PACKET_SIZE                        ( 268435460U )

/**
 * @brief Version 5 has the value 5.
 */
#define MQTT_VERSION_5                              ( 5U )

/**
 * @ingroup mqtt_constants
 * @brief The size of MQTT PUBACK, PUBREC, PUBREL, and PUBCOMP packets with reason code, packet id.
 */
#define MQTT_PUBLISH_ACK_PACKET_SIZE_WITH_REASON    ( 3UL )

/**
 * @brief Minimum number of bytes in the CONNACK Packet.
 * CONNECT Acknowledge Flags    0 + 1 = 1
 * CONNECT Reason Code            + 1 = 2
 * Property Length byte (min)     + 1 = 3
 */
#define MQTT_PACKET_CONNACK_MINIMUM_SIZE            ( 3U )

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
#define MQTT_SUBSCRIPTION_ID_POS             ( 1 )

/**
 * @brief Defines the position of the **Session Expiry**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_SESSION_EXPIRY_INTERVAL_POS     ( 2 )

/**
 * @brief Defines the position of the **Receive Maximum**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RECEIVE_MAXIMUM_POS             ( 3 )

/**
 * @brief Defines the position of the **Maximum Packet Size**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_MAX_PACKET_SIZE_POS             ( 4 )

/**
 * @brief Defines the position of the **Topic Alias Maximum**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_TOPIC_ALIAS_MAX_POS             ( 5 )

/**
 * @brief Defines the position of the **Request Response Information**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REQUEST_RESPONSE_INFO_POS       ( 6 )

/**
 * @brief Defines the position of the **Request Problem Information**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REQUEST_PROBLEM_INFO_POS        ( 7 )

/**
 * @brief Defines the position of the **Authentication Method**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_AUTHENTICATION_METHOD_POS       ( 9 )

/**
 * @brief Defines the position of the **Authentication Data**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_AUTHENTICATION_DATA_POS         ( 10 )

/**
 * @brief Defines the position of the **Payload Format Indicator**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_PAYLOAD_FORMAT_INDICATOR_POS    ( 11 )

/**
 * @brief Defines the position of the **Message Expiry Interval**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_MESSAGE_EXPIRY_INTERVAL_POS     ( 12 )

/**
 * @brief Defines the position of the **Topic Alias**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_TOPIC_ALIAS_POS                 ( 13 )

/**
 * @brief Defines the position of the **Response Topic**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_RESPONSE_TOPIC_POS              ( 14 )

/**
 * @brief Defines the position of the **Correlation Data**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_CORRELATION_DATA_POS            ( 15 )

/**
 * @brief Defines the position of the **Content Type**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_CONTENT_TYPE_POS                ( 17 )

/**
 * @brief Defines the position of the **Reason String**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_REASON_STRING_POS               ( 18 )

/**
 * @brief Defines the position of the **Will Delay Interval**
 * property in the `fieldSet` bitfield of the `MQTTPropBuilder_t` struct.
 */
#define MQTT_WILL_DELAY_POS                  ( 19 )

/**
 * @brief A value that represents an invalid remaining length.
 *
 * This value is greater than what is allowed by the MQTT specification.
 */
#define MQTT_REMAINING_LENGTH_INVALID        ( ( size_t ) 268435456 )

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
/*-----------------------------------------------------------*/

/**
 * @brief Serializes MQTT PUBLISH packet into the buffer provided.
 *
 * This function serializes MQTT PUBLISH packet into #MQTTFixedBuffer_t.pBuffer.
 * Copy of the payload into the buffer is done as part of the serialization
 * only if @p serializePayload is true.
 *
 * @param[in] pPublishInfo Publish information containing topic, QoS, payload and other
 * PUBLISH packet fields.
 * @param[in] pPublishProperties MQTT v5.0 properties for the PUBLISH packet. Can be NULL
 * if no properties are needed.
 * @param[in] remainingLength Remaining length of the PUBLISH packet.
 * @param[in] packetIdentifier Packet identifier of PUBLISH packet.
 * @param[in, out] pFixedBuffer Buffer to which PUBLISH packet will be
 * serialized.
 * @param[in] serializePayload Copy payload to the serialized buffer
 * only if true. Only PUBLISH header will be serialized if false.
 */
static void serializePublishCommon( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    size_t remainingLength,
                                    uint16_t packetIdentifier,
                                    const MQTTFixedBuffer_t * pFixedBuffer,
                                    bool serializePayload );

/**
 * @brief Calculates the packet size and remaining length of an MQTT
 * PUBLISH packet.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT PUBLISH packet.
 * @param[out] pPacketSize The total size of the MQTT PUBLISH packet.
 * @param[in] maxPacketSize Max packet size allowed by the server.
 * @param[in] publishPropertyLength Length of the optional properties in MQTT_PUBLISH
 *
 *
 * @return MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; MQTTSuccess otherwise.
 */
static MQTTStatus_t calculatePublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                                size_t * pRemainingLength,
                                                size_t * pPacketSize,
                                                uint32_t maxPacketSize,
                                                size_t publishPropertyLength );

/**
 * @brief Calculates the packet size and remaining length of an MQTT
 * SUBSCRIBE or UNSUBSCRIBE packet.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[out] pRemainingLength The Remaining Length of the MQTT SUBSCRIBE or
 * UNSUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT SUBSCRIBE or
 * MQTT UNSUBSCRIBE packet.
 * @param[in] subscribePropLen Length of the optional properties in MQTT_SUBSCRIBE or MQTT_UNSUBSCRIBE
 * @param[in] maxPacketSize Maximum Packet Size allowed by the broker
 * @param[in] subscriptionType #MQTT_TYPE_SUBSCRIBE or #MQTT_TYPE_UNSUBSCRIBE.
 *
 * @return MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or a subscription is empty; MQTTSuccess otherwise.
 *
 */

static MQTTStatus_t calculateSubscriptionPacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                     size_t subscriptionCount,
                                                     size_t * pRemainingLength,
                                                     size_t * pPacketSize,
                                                     size_t subscribePropLen,
                                                     uint32_t maxPacketSize,
                                                     MQTTSubscriptionType_t subscriptionType );

/**
 * @brief Validates parameters of #MQTT_SerializeSubscribe or
 * #MQTT_SerializeUnsubscribe.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] packetId Packet identifier.
 * @param[in] remainingLength Remaining length of the packet.
 * @param[in] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 */
static MQTTStatus_t validateSubscriptionSerializeParams( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                         size_t subscriptionCount,
                                                         uint16_t packetId,
                                                         size_t remainingLength,
                                                         const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Serialize an MQTT CONNECT packet in the given buffer.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT properties.
 * @param[in] pWillProperties MQTT Will properties.
 * @param[in] remainingLength Remaining Length of MQTT CONNECT packet.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 */
static void serializeConnectPacket( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Retrieve the size of the remaining length if it were to be encoded.
 *
 * @param[in] length The remaining length to be encoded.
 *
 * @return The size of the remaining length if it were to be encoded.
 */
static size_t variableLengthEncodedSize( size_t length );

/**
 * @brief Encode a string whose size is at maximum 16 bits in length.
 *
 * @param[out] pDestination Destination buffer for the encoding.
 * @param[in] pSource The source string to encode.
 * @param[in] sourceLength The length of the source string to encode.
 *
 * @return A pointer to the end of the encoded string.
 */
static uint8_t * encodeString( uint8_t * pDestination,
                               const char * pSource,
                               uint16_t sourceLength );

/**
 * @brief Retrieves and decodes the Remaining Length from the network interface
 * by reading a single byte at a time.
 *
 * @param[in] recvFunc Network interface receive function.
 * @param[in] pNetworkContext Network interface context to the receive function.
 *
 * @return The Remaining Length of the incoming packet.
 */
static size_t getRemainingLength( TransportRecv_t recvFunc,
                                  NetworkContext_t * pNetworkContext );

/**
 * @brief Retrieves, decodes and stores the Remaining Length from the network
 * interface by reading a single byte at a time.
 *
 * @param[in] pBuffer The buffer holding the raw data to be processed
 * @param[in] pIndex Pointer to the index within the buffer to marking the end of raw data
 *            available.
 * @param[in] pIncomingPacket Structure used to hold the fields of the
 *            incoming packet.
 *
 * @return MQTTNeedMoreBytes is returned to show that the incoming
 *         packet is not yet fully received and decoded. Otherwise, MQTTSuccess
 *         shows that processing of the packet was successful.
 */
static MQTTStatus_t processRemainingLength( const uint8_t * pBuffer,
                                            const size_t * pIndex,
                                            MQTTPacketInfo_t * pIncomingPacket );

/**
 * @brief Check if an incoming packet type is valid.
 *
 * @param[in] packetType The packet type to check.
 *
 * @return `true` if the packet type is valid; `false` otherwise.
 */
static bool incomingPacketValid( uint8_t packetType );

/**
 * @brief Check the remaining length of an incoming PUBLISH packet against some
 * value for QoS 0, or for QoS 1 and 2.
 *
 * The remaining length for a QoS 1 and 2 packet will always be two greater than
 * for a QoS 0.
 *
 * @param[in] remainingLength Remaining length of the PUBLISH packet.
 * @param[in] qos The QoS of the PUBLISH.
 * @param[in] qos0Minimum Minimum possible remaining length for a QoS 0 PUBLISH.
 *
 * @return #MQTTSuccess or #MQTTBadResponse.
 */
static MQTTStatus_t checkPublishRemainingLength( size_t remainingLength,
                                                 MQTTQoS_t qos,
                                                 size_t qos0Minimum );

/**
 * @brief Process the flags of an incoming PUBLISH packet.
 *
 * @param[in] publishFlags Flags of an incoming PUBLISH.
 * @param[in, out] pPublishInfo Pointer to #MQTTPublishInfo_t struct where
 * output will be written.
 *
 * @return #MQTTSuccess or #MQTTBadResponse.
 */
static MQTTStatus_t processPublishFlags( uint8_t publishFlags,
                                         MQTTPublishInfo_t * pPublishInfo );


/**
 * @brief Deserialize a PUBLISH packet received from the server.
 *
 * Converts the packet from a stream of bytes to an #MQTTPublishInfo_t and
 * extracts the packet identifier. Also prints out debug log messages about the
 * packet.
 *
 * @param[in] pIncomingPacket Pointer to an MQTT packet struct representing a
 * PUBLISH.
 * @param[out] pPacketId Packet identifier of the PUBLISH.
 * @param[out] pPublishInfo Pointer to #MQTTPublishInfo_t where output is
 * written.
 * @param[out] pPropBuffer Pointer to the property buffer.
 * @param[in] topicAliasMax Maximum allowed Topic Alias.
 *
 * @return #MQTTSuccess if PUBLISH is valid;
 * #MQTTBadResponse if the PUBLISH packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo,
                                        MQTTPropBuilder_t * pPropBuffer,
                                        uint16_t topicAliasMax );

/**
 * @brief Deserialize a PINGRESP packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t.
 *
 * @param[in] pPingresp Pointer to an MQTT packet struct representing a PINGRESP.
 *
 * @return #MQTTSuccess if PINGRESP is valid; #MQTTBadResponse if the PINGRESP
 * packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializePingresp( const MQTTPacketInfo_t * pPingresp );

/**
 * @brief Validate the length and decode a user property.
 *
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pIndex Pointer to the current index in the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t decodeAndDiscard( size_t * pPropertyLength,
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
static MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                          size_t bufferLength,
                                          size_t * pLength );

/**
 * @brief Validate the connack parameters.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts
 * the variable header without connack properties.
 *
 * @param[in] pIncomingPacket Pointer to an MQTT packet struct representing a incoming packet.
 * @param[out] pSessionPresent Whether a session is present or not.
 *
 *
 * @return #MQTTSuccess if connack  without connack properties is valid;
 * #MQTTServerRefused if the server refused the connection;
 * #MQTTBadResponse if the Connack packet doesn't follow MQTT spec.
 */
static MQTTStatus_t validateConnackParams( const MQTTPacketInfo_t * pIncomingPacket,
                                           bool * pSessionPresent );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] responseCode MQTT Verion 5 standard CONNACK response code.
 *
 * @return MQTTServerRefused if response code is valid and MQTTBadResponse if responseCode is invalid, else return MQTTSuccess.
 */
static MQTTStatus_t logConnackResponse( uint8_t responseCode );

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
static MQTTStatus_t decodeuint32_t( uint32_t * pProperty,
                                    size_t * pPropertyLength,
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

static MQTTStatus_t decodeuint16_t( uint16_t * pProperty,
                                    size_t * pPropertyLength,
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
static MQTTStatus_t decodeuint8_t( uint8_t * pProperty,
                                   size_t * pPropertyLength,
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
static MQTTStatus_t decodeutf_8( const char ** pProperty,
                                 uint16_t * pLength,
                                 size_t * pPropertyLength,
                                 bool * pUsed,
                                 uint8_t ** pIndex );

/**
 * @brief Validate the length and decode the connack properties.
 *
 * @param[out] pConnackProperties To store the decoded property.
 * @param[in] length  Length of the properties.
 * @param[in] pIndex Pointer to the start of the properties.
 * @param[out] pPropBuffer Pointer to the property buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t deserializeConnackProperties( MQTTConnectionProperties_t * pConnackProperties,
                                                  size_t length,
                                                  uint8_t * pIndex,
                                                  MQTTPropBuilder_t * pPropBuffer );


/**
 * @brief Prints the appropriate message for the PUBREL, PUBACK response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard PUBREL, PUBACK response code.
 * @param[in] packetIdentifier Packet id of the ack packet.
 *
 * @return #MQTTSuccess, #MQTTServerRefused and #MQTTBadResponse.
 */
static MQTTStatus_t logAckResponse( uint8_t reasonCode,
                                    uint16_t packetIdentifier );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard CONNACK response code.
 * @param[in] packetIdentifier Packet id of the ack packet.
 *
 * @return #MQTTSuccess, #MQTTServerRefused and #MQTTBadResponse.
 */
static MQTTStatus_t logSimpleAckResponse( uint8_t reasonCode,
                                          uint16_t packetIdentifier );

/**
 * @brief Validate the length and decode the publish ack properties.
 *
 * @param[out] pPropBuffer To store the decoded property.
 * @param[in] pIndex Pointer to the current index of the buffer.
 * @param[in] remainingLength Remaining length of the incoming packet.
 *
 *
 * @return #MQTTSuccess, #MQTTBadResponse.
 **/
static MQTTStatus_t decodeAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                         uint8_t * pIndex,
                                         size_t remainingLength );

/**
 * @brief Deserialize an PUBACK, PUBREC, PUBREL, or PUBCOMP packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts
 * the packet identifier, reason code, properties.
 *
 * @param[in] pAck Pointer to the MQTT packet structure representing the packet.
 * @param[out] pPacketIdentifier Packet ID of the ack type packet.
 * @param[out] pReasonCode Structure to store reason code of the ack type packet.
 * @param[in] requestProblem To validate the packet.
 * @param[out] pPropBuffer Pointer to the property buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse, #MQTTBadParameter.
 */
static MQTTStatus_t deserializeSimpleAck( const MQTTPacketInfo_t * pAck,
                                          uint16_t * pPacketIdentifier,
                                          MQTTReasonCodeInfo_t * pReasonCode,
                                          bool requestProblem,
                                          MQTTPropBuilder_t * pPropBuffer );

/**
 * @brief Prints and validates the appropriate message for the Disconnect response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard DISCONNECT response code.
 * @param[in] incoming To differentiate between outgoing and incoming disconnect.
 *
 * @return #MQTTSuccess, #MQTTBadParameter and #MQTTBadResponse.
 */
static MQTTStatus_t validateDisconnectResponse( uint8_t reasonCode,
                                                bool incoming );

/**
 * @brief Deserialize properties in the SUBACK packet received from the server.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts properties.
 *
 * @param[out] pPropBuffer Pointer to the property buffer.
 * @param[in] pIndex Pointer to the start of the properties.
 * @param[out] pSubackPropertyLength Pointer to the length of suback properties
 * @param[in] remainingLength Remaining length of the incoming packet.
 *
 * @return #MQTTSuccess if SUBACK is valid;
 * #MQTTBadResponse if SUBACK is invalid.
 *
 */
static MQTTStatus_t deserializeSubackProperties( MQTTPropBuilder_t * pPropBuffer,
                                                 uint8_t * pIndex,
                                                 size_t * pSubackPropertyLength,
                                                 size_t remainingLength );

/**
 * @brief Deserialize properties in the DISCONNECT packet received from the server.
 *
 * @param[in] pIndex Pointer to the start of the properties.
 * @param[in] disconnectPropertyLength Length of the properties in the DISCONNECT packet.
 *
 * @return #MQTTSuccess if DISCONNECT is valid;
 * #MQTTBadResponse if the DISCONNECT packet is invalid.
 */
static MQTTStatus_t deserializeDisconnectProperties( uint8_t * pIndex,
                                                     size_t disconnectPropertyLength );

/**
 * @brief Deserialize properties in the PUBLISH packet received from the server.
 *
 * Converts the packet from a stream of bytes to an #MQTTPublishInfo_t and
 * extracts properties.
 *
 * @param[out] pPublishInfo Pointer to #MQTTPublishInfo_t where output is
 * written.
 * @param[out] pPropBuffer Pointer to the property buffer.
 * @param[in] pIndex Pointer to the start of the properties.
 * @param[in] topicAliasMax Maximum allowed Topic Alias.
 * @param[in] remainingLength Remaining length of the incoming packet.
 *
 * @return #MQTTSuccess if PUBLISH is valid; #MQTTBadResponse
 * if the PUBLISH packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializePublishProperties( MQTTPublishInfo_t * pPublishInfo,
                                                  MQTTPropBuilder_t * pPropBuffer,
                                                  uint8_t * pIndex,
                                                  uint16_t topicAliasMax,
                                                  size_t remainingLength );

/**
 * @brief Validate the length and decode a utf_8 string
 *
 * @param[in, out] pPropertyLength Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t decodeAndDiscardutf_8( size_t * pPropertyLength,
                                           bool * pUsed,
                                           uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a uint8_t
 *
 * @param[in, out] pPropertyLength  Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t decodeAndDiscard_uint8( size_t * pPropertyLength,
                                            bool * pUsed,
                                            uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a uint32_t
 *
 * @param[in, out] pPropertyLength Value of the remaining property length.
 * @param[in, out] pUsed Whether the property is decoded before.
 * @param[in, out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t decodeAndDiscard_uint32( size_t * pPropertyLength,
                                             bool * pUsed,
                                             uint8_t ** pIndex );

/**
 * @brief Decode the status bytes of a SUBACK packet to a #MQTTStatus_t.
 *
 * @param[in] statusCount Number of status bytes in the SUBACK.
 * @param[in] pStatusStart The first status byte in the SUBACK.
 * @param[out] pReasonCodes The #MQTTReasonCodeInfo_t to store reason codes for each topic filter.
 * @return #MQTTSuccess, #MQTTServerRefused, or #MQTTBadResponse.
 */
static MQTTStatus_t readSubackStatus( size_t statusCount,
                                      const uint8_t * pStatusStart,
                                      MQTTReasonCodeInfo_t * pReasonCodes );

/**
 * @brief Validate the length and decode a utf 8 string.
 *
 * @param[out] pProperty To store the decoded string.
 * @param[out] pLength  Size of the decoded binary string.
 * @param[in, out] pPropertyLength Value of the remaining property length.
 * @param[in, out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTBadResponse
 **/
static MQTTStatus_t decodeBinaryData( const void ** pProperty,
                                      uint16_t * pLength,
                                      size_t * pPropertyLength,
                                      uint8_t ** pIndex );

/**
 * @brief Encode binary data whose size is at maximum 16 bits in length.
 *
 * @param[out] pDestination Destination buffer for the encoding.
 * @param[in] pSource The source binary data to encode.
 * @param[in] sourceLength The length of the source data to encode.
 *
 * @return A pointer to the end of the encoded binary data.
 */
static uint8_t * encodeBinaryData( uint8_t * pDestination,
                                   const void * pSource,
                                   uint16_t sourceLength );

/**
 * @brief Deserialize an MQTT CONNACK packet.
 *
 * @param[out] pConnackProperties To store the deserialized connack properties.
 * @param[in]  pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out]  pSessionPresent Whether a previous session was present.
 * @param[out]  pPropBuffer MQTTPropBuilder_t to store the deserialized properties.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTSuccess, #MQTTServerRefused
 */
MQTTStatus_t deserializeConnack( MQTTConnectionProperties_t * pConnackProperties,
                                 const MQTTPacketInfo_t * pIncomingPacket,
                                 bool * pSessionPresent,
                                 MQTTPropBuilder_t * pPropBuffer );

/**
 * @brief Deserialize an MQTT SUBACK / UNSUBACK packet.
 *
 * @param[in]  incomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out]  pPacketId The packet ID obtained from the buffer.
 * @param[out]  pReasonCodes Struct to store reason code(s) from the acknowledgment packet.
 *                           Contains the success/failure status of the corresponding request.
 * @param[out]  pPropBuffer MQTTPropBuilder_t to store the deserialized properties.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTSuccess, #MQTTServerRefused
 */
static MQTTStatus_t deserializeSuback( const MQTTPacketInfo_t * incomingPacket,
                                       uint16_t * pPacketId,
                                       MQTTReasonCodeInfo_t * pReasonCodes,
                                       MQTTPropBuilder_t * pPropBuffer );

/*-----------------------------------------------------------*/

static size_t variableLengthEncodedSize( size_t length )
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 5.0 spec. */

    /* 1 byte is needed to encode lengths between 0 and 127. */
    if( length < 128U )
    {
        encodedSize = 1U;
    }
    /* 2 bytes are needed to encode lengths between 128 and 16,383. */
    else if( length < 16384U )
    {
        encodedSize = 2U;
    }
    /* 3 bytes are needed to encode lengths between 16,384 and 2,097,151. */
    else if( length < 2097152U )
    {
        encodedSize = 3U;
    }
    /* 4 bytes are needed to encode lengths between 2,097,152 and 268,435,455. */
    else
    {
        encodedSize = 4U;
    }

    LogDebug( ( "Encoded size for length %lu is %lu bytes.",
                ( unsigned long ) length,
                ( unsigned long ) encodedSize ) );

    return encodedSize;
}

/*-----------------------------------------------------------*/

static uint8_t * encodeString( uint8_t * pDestination,
                               const char * pSource,
                               uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    assert( pDestination != NULL );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */
    if( pSource != NULL )
    {
        ( void ) memcpy( ( void * ) pBuffer, ( const void * ) pSource, sourceLength );
    }

    /* Return the pointer to the end of the encoded string. */
    pBuffer = &pBuffer[ sourceLength ];

    return pBuffer;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t calculatePublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                                size_t * pRemainingLength,
                                                size_t * pPacketSize,
                                                uint32_t maxPacketSize,
                                                size_t publishPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t packetSize = 0, propertyAndPayloadLimit = 0;


    assert( pPublishInfo != NULL );
    assert( pRemainingLength != NULL );
    assert( pPacketSize != NULL );

    /* The variable header of a PUBLISH packet always contains the topic name.
     * The first 2 bytes of UTF-8 string contains length of the string.
     */
    packetSize += pPublishInfo->topicNameLength + sizeof( uint16_t );

    /* The variable header of a QoS 1 or 2 PUBLISH packet contains a 2-byte
     * packet identifier. */
    if( pPublishInfo->qos > MQTTQoS0 )
    {
        packetSize += sizeof( uint16_t );
    }

    packetSize += variableLengthEncodedSize( publishPropertyLength );

    /* Calculate the maximum allowed size of the properties and payload combined for
     * the given parameters. */
    propertyAndPayloadLimit = MQTT_MAX_REMAINING_LENGTH - packetSize;

    if( publishPropertyLength > propertyAndPayloadLimit )
    {
        LogError( ( "PUBLISH properties length of %lu cannot exceed "
                    "%lu so as not to exceed the maximum "
                    "remaining length of MQTT 5.0 packet( %lu ).",
                    ( unsigned long ) publishPropertyLength,
                    ( unsigned long ) propertyAndPayloadLimit,
                    MQTT_MAX_REMAINING_LENGTH ) );
        status = MQTTBadParameter;
    }
    else
    {
        packetSize += publishPropertyLength;
        propertyAndPayloadLimit -= publishPropertyLength;
    }

    if( status == MQTTSuccess )
    {
        if( pPublishInfo->payloadLength > propertyAndPayloadLimit )
        {
            LogError( ( "PUBLISH properties and payload combined length of %lu cannot exceed "
                        "%lu so as not to exceed the maximum "
                        "remaining length of MQTT 5.0 packet( %lu ).",
                        ( unsigned long ) ( pPublishInfo->payloadLength + publishPropertyLength ),
                        ( unsigned long ) ( propertyAndPayloadLimit + publishPropertyLength ),
                        MQTT_MAX_REMAINING_LENGTH ) );
            status = MQTTBadParameter;
        }
        else
        {
            packetSize += pPublishInfo->payloadLength;
        }
    }

    if( status == MQTTSuccess )
    {
        /* Set the "Remaining length" output parameter and calculate the full
         * size of the PUBLISH packet. */
        *pRemainingLength = packetSize;

        packetSize += 1U + variableLengthEncodedSize( packetSize );

        if( packetSize > maxPacketSize )
        {
            LogError( ( "Packet size is greater than the allowed maximum packet size." ) );
            status = MQTTBadParameter;
        }

        *pPacketSize = packetSize;
    }

    LogDebug( ( "PUBLISH packet remaining length=%lu and packet size=%lu.",
                ( unsigned long ) *pRemainingLength,
                ( unsigned long ) *pPacketSize ) );

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopic( const MQTTPublishInfo_t * pPublishInfo,
                                                      size_t remainingLength,
                                                      uint8_t * pBuffer,
                                                      size_t * headerSize )
{
    size_t headerLength;
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* The first byte of a PUBLISH packet contains the packet type and flags. */
    uint8_t publishFlags = MQTT_PACKET_TYPE_PUBLISH;

    /* Get the start address of the buffer. */
    pIndex = pBuffer;

    /* Length of serialized packet = First byte
     *                               + Length of encoded remaining length
     *                               + Encoded topic length. */
    headerLength = 1U + variableLengthEncodedSize( remainingLength ) + 2U;

    if( pPublishInfo->qos == MQTTQoS1 )
    {
        LogDebug( ( "Adding QoS as QoS1 in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS1 );
    }
    else if( pPublishInfo->qos == MQTTQoS2 )
    {
        LogDebug( ( "Adding QoS as QoS2 in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS2 );
    }
    else
    {
        /* Empty else MISRA 15.7 */
    }

    if( pPublishInfo->retain == true )
    {
        LogDebug( ( "Adding retain bit in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_RETAIN );
    }

    if( pPublishInfo->dup == true )
    {
        LogDebug( ( "Adding dup bit in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_DUP );
    }

    *pIndex = publishFlags;
    pIndex++;

    /* The "Remaining length" is encoded from the second byte. */
    pIndex = encodeVariableLength( pIndex, remainingLength );

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pIndex = UINT16_HIGH_BYTE( pPublishInfo->topicNameLength );
    pIndex++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pIndex = UINT16_LOW_BYTE( pPublishInfo->topicNameLength );
    pIndex++;

    *headerSize = headerLength;

    return status;
}

/*-----------------------------------------------------------*/

static void serializePublishCommon( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    size_t remainingLength,
                                    uint16_t packetIdentifier,
                                    const MQTTFixedBuffer_t * pFixedBuffer,
                                    bool serializePayload )
{
    uint8_t * pIndex = NULL;
    size_t propertyLength = 0U;
    /* The first byte of a PUBLISH packet contains the packet type and flags. */
    uint8_t publishFlags = MQTT_PACKET_TYPE_PUBLISH;

    assert( pPublishInfo != NULL );
    assert( pFixedBuffer != NULL );
    assert( pFixedBuffer->pBuffer != NULL );
    /* Packet Id should be non zero for Qos 1 and Qos 2. */
    assert( ( pPublishInfo->qos == MQTTQoS0 ) || ( packetIdentifier != 0U ) );
    /* Duplicate flag should be set only for Qos 1 or Qos 2. */
    assert( ( pPublishInfo->dup != true ) || ( pPublishInfo->qos != MQTTQoS0 ) );

    /* Get the start address of the buffer. */
    pIndex = pFixedBuffer->pBuffer;

    if( ( pPublishProperties != NULL ) && ( pPublishProperties->pBuffer != NULL ) )
    {
        propertyLength = pPublishProperties->currentIndex;
    }

    if( pPublishInfo->qos == MQTTQoS1 )
    {
        LogDebug( ( "Adding QoS as QoS1 in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS1 );
    }
    else if( pPublishInfo->qos == MQTTQoS2 )
    {
        LogDebug( ( "Adding QoS as QoS2 in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS2 );
    }
    else
    {
        /* Empty else MISRA 15.7 */
    }

    if( pPublishInfo->retain == true )
    {
        LogDebug( ( "Adding retain bit in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_RETAIN );
    }

    if( pPublishInfo->dup == true )
    {
        LogDebug( ( "Adding dup bit in PUBLISH flags." ) );
        UINT8_SET_BIT( publishFlags, MQTT_PUBLISH_FLAG_DUP );
    }

    *pIndex = publishFlags;
    pIndex++;

    /* The "Remaining length" is encoded from the second byte. */
    pIndex = encodeVariableLength( pIndex, remainingLength );

    /* The topic name is placed after the "Remaining length". */
    pIndex = encodeString( pIndex,
                           pPublishInfo->pTopicName,
                           pPublishInfo->topicNameLength );

    /* A packet identifier is required for QoS 1 and 2 messages. */
    if( pPublishInfo->qos > MQTTQoS0 )
    {
        LogDebug( ( "Adding packet Id in PUBLISH packet." ) );
        /* Place the packet identifier into the PUBLISH packet. */
        *pIndex = UINT16_HIGH_BYTE( packetIdentifier );
        pIndex[ 1U ] = UINT16_LOW_BYTE( packetIdentifier );
        pIndex = &pIndex[ 2U ];
    }

    /* Properties are added after packet identifier. */
    pIndex = encodeVariableLength( pIndex, propertyLength );

    if( propertyLength > 0U )
    {
        ( void ) memcpy( ( void * ) pIndex, ( const void * ) pPublishProperties->pBuffer, propertyLength );
        pIndex = &pIndex[ propertyLength ];
    }

    /* The payload is placed after the packet identifier.
     * Payload is copied over only if required by the flag serializePayload.
     * This will help reduce an unnecessary copy of the payload into the buffer.
     */
    if( ( pPublishInfo->payloadLength > 0U ) &&
        ( serializePayload == true ) )
    {
        LogDebug( ( "Copying PUBLISH payload of length =%lu to buffer",
                    ( unsigned long ) pPublishInfo->payloadLength ) );

        ( void ) memcpy( ( void * ) pIndex, ( const void * ) pPublishInfo->pPayload, pPublishInfo->payloadLength );
        /* Move the index to after the payload. */
        pIndex = &pIndex[ pPublishInfo->payloadLength ];
    }

    /* Ensure that the difference between the end and beginning of the buffer
     * is less than the buffer size. */
    assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
}

/*-----------------------------------------------------------*/

static size_t getRemainingLength( TransportRecv_t recvFunc,
                                  NetworkContext_t * pNetworkContext )
{
    size_t remainingLength = 0, multiplier = 1, bytesDecoded = 0, expectedSize = 0;
    uint8_t encodedByte = 0;
    int32_t bytesReceived = 0;

    /* This algorithm is copied from the MQTT v5.0 spec. */
    do
    {
        if( multiplier > 2097152U ) /* 128 ^ 3 */
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;
        }
        else
        {
            bytesReceived = recvFunc( pNetworkContext, &encodedByte, 1U );

            if( bytesReceived == 1 )
            {
                remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
                multiplier *= 128U;
                bytesDecoded++;
            }
            else
            {
                remainingLength = MQTT_REMAINING_LENGTH_INVALID;
            }
        }

        if( remainingLength == MQTT_REMAINING_LENGTH_INVALID )
        {
            break;
        }
    } while( ( encodedByte & 0x80U ) != 0U );

    /* Check that the decoded remaining length conforms to the MQTT specification. */
    if( remainingLength != MQTT_REMAINING_LENGTH_INVALID )
    {
        expectedSize = variableLengthEncodedSize( remainingLength );

        if( bytesDecoded != expectedSize )
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;
        }
    }

    return remainingLength;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t processRemainingLength( const uint8_t * pBuffer,
                                            const size_t * pIndex,
                                            MQTTPacketInfo_t * pIncomingPacket )
{
    size_t remainingLength = 0;
    size_t multiplier = 1;
    size_t bytesDecoded = 0;
    size_t expectedSize = 0;
    uint8_t encodedByte = 0;
    MQTTStatus_t status = MQTTSuccess;

    /* This algorithm is copied from the MQTT v5.0 spec. */
    do
    {
        if( multiplier > 2097152U ) /* 128 ^ 3 */
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;

            LogError( ( "Invalid remaining length in the packet.\n" ) );

            status = MQTTBadResponse;
        }
        else
        {
            if( *pIndex > ( bytesDecoded + 1U ) )
            {
                /* Get the next byte. It is at the next position after the bytes
                 * decoded till now since the header of one byte was read before. */
                encodedByte = pBuffer[ bytesDecoded + 1U ];

                remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
                multiplier *= 128U;
                bytesDecoded++;
            }
            else
            {
                status = MQTTNeedMoreBytes;
            }
        }

        /* If the response is incorrect, or no more data is available, then
         * break out of the loop. */
        if( ( remainingLength == MQTT_REMAINING_LENGTH_INVALID ) ||
            ( status != MQTTSuccess ) )
        {
            break;
        }
    } while( ( encodedByte & 0x80U ) != 0U );

    if( status == MQTTSuccess )
    {
        /* Check that the decoded remaining length conforms to the MQTT specification. */
        expectedSize = variableLengthEncodedSize( remainingLength );

        if( bytesDecoded != expectedSize )
        {
            LogError( ( "Expected and actual length of decoded bytes do not match.\n" ) );
            status = MQTTBadResponse;
        }
        else
        {
            pIncomingPacket->remainingLength = remainingLength;
            pIncomingPacket->headerLength = bytesDecoded + 1U;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static bool incomingPacketValid( uint8_t packetType )
{
    bool status = false;

    /* Check packet type. Mask out lower bits to ignore flags. */
    switch( packetType & 0xF0U )
    {
        /* Valid incoming packet types. */
        case MQTT_PACKET_TYPE_CONNACK:
        case MQTT_PACKET_TYPE_PUBLISH:
        case MQTT_PACKET_TYPE_PUBACK:
        case MQTT_PACKET_TYPE_PUBREC:
        case MQTT_PACKET_TYPE_PUBCOMP:
        case MQTT_PACKET_TYPE_SUBACK:
        case MQTT_PACKET_TYPE_UNSUBACK:
        case MQTT_PACKET_TYPE_PINGRESP:
        case MQTT_PACKET_TYPE_DISCONNECT:
            status = true;
            break;

        case ( MQTT_PACKET_TYPE_PUBREL & 0xF0U ):

            /* The second bit of a PUBREL must be set. */
            if( ( packetType & 0x02U ) > 0U )
            {
                status = true;
            }

            break;

        /* Any other packet type is invalid. */
        default:
            LogWarn( ( "Incoming packet invalid: Packet type=%u.",
                       ( unsigned int ) packetType ) );
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t checkPublishRemainingLength( size_t remainingLength,
                                                 MQTTQoS_t qos,
                                                 size_t qos0Minimum )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Sanity checks for "Remaining length". */
    if( qos == MQTTQoS0 )
    {
        /* Check that the "Remaining length" is greater than the minimum. */
        if( remainingLength < qos0Minimum )
        {
            LogError( ( "QoS 0 PUBLISH cannot have a remaining length less than %lu.",
                        ( unsigned long ) qos0Minimum ) );

            status = MQTTBadResponse;
        }
    }
    else
    {
        /* Check that the "Remaining length" is greater than the minimum. For
         * QoS 1 or 2, this will be two bytes greater than for QoS 0 due to the
         * packet identifier. */
        if( remainingLength < ( qos0Minimum + 2U ) )
        {
            LogError( ( "QoS 1 or 2 PUBLISH cannot have a remaining length less than %lu.",
                        ( unsigned long ) ( qos0Minimum + 2U ) ) );

            status = MQTTBadResponse;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t processPublishFlags( uint8_t publishFlags,
                                         MQTTPublishInfo_t * pPublishInfo )
{
    MQTTStatus_t status = MQTTSuccess;

    assert( pPublishInfo != NULL );

    /* Check for QoS 2. */
    if( UINT8_CHECK_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS2 ) )
    {
        /* PUBLISH packet is invalid if both QoS 1 and QoS 2 bits are set. */
        if( UINT8_CHECK_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS1 ) )
        {
            LogError( ( "Bad QoS: 3." ) );

            status = MQTTBadResponse;
        }
        else
        {
            pPublishInfo->qos = MQTTQoS2;
        }
    }
    /* Check for QoS 1. */
    else if( UINT8_CHECK_BIT( publishFlags, MQTT_PUBLISH_FLAG_QOS1 ) )
    {
        pPublishInfo->qos = MQTTQoS1;
    }
    /* If the PUBLISH isn't QoS 1 or 2, then it's QoS 0. */
    else
    {
        pPublishInfo->qos = MQTTQoS0;
    }

    if( status == MQTTSuccess )
    {
        LogDebug( ( "QoS is %d.", ( int ) pPublishInfo->qos ) );

        /* Parse the Retain bit. */
        pPublishInfo->retain = UINT8_CHECK_BIT( publishFlags, MQTT_PUBLISH_FLAG_RETAIN );

        LogDebug( ( "Retain bit is %d.", ( int ) pPublishInfo->retain ) );

        /* Parse the DUP bit. */
        pPublishInfo->dup = UINT8_CHECK_BIT( publishFlags, MQTT_PUBLISH_FLAG_DUP );

        LogDebug( ( "DUP bit is %d.", ( int ) pPublishInfo->dup ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t logConnackResponse( uint8_t responseCode )
{
    MQTTStatus_t status = MQTTServerRefused;

    /* Log an error based on the CONNACK response code. */
    switch( responseCode )
    {
        case ( uint8_t ) MQTT_REASON_CONNACK_UNSPECIFIED_ERROR:
            LogError( ( "Connection refused: Unspecified error" ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_MALFORMED_PACKET:
            LogError( ( "Connection refused: Malformed Packet." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_PROTOCOL_ERROR:
            LogError( ( "Connection refused: Protocol Error." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR:
            LogError( ( "Connection refused: Implementation specific error." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION:
            LogError( ( "Connection refused: Unsupported Protocol Version." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID:
            LogError( ( "Connection refused: Client Identifier not valid." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD:
            LogError( ( "Connection refused: Bad User Name or Password." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_NOT_AUTHORIZED:
            LogError( ( "Connection refused: Not authorized." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_UNAVAILABLE:
            LogError( ( "Connection refused: Server unavailable." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_BUSY:
            LogError( ( "Connection refused: Server busy." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_BANNED:
            LogError( ( "Connection refused: Banned." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD:
            LogError( ( "Connection refused: Bad authentication method." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_TOPIC_NAME_INVALID:
            LogError( ( "Connection refused: Topic Name invalid." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_PACKET_TOO_LARGE:
            LogError( ( "Connection refused: Packet too large ." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_QUOTA_EXCEEDED:
            LogError( ( "Connection refused: Quota exceeded." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID:
            LogError( ( "Connection refused: Payload format invalid." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED:
            LogError( ( "Connection refused: Retain not supported." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED:
            LogError( ( "Connection refused: QoS not supported." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_USE_ANOTHER_SERVER:
            LogError( ( "Connection refused: Use another server." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_MOVED:
            LogError( ( "Connection refused: Server moved." ) );
            break;

        case ( uint8_t ) MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED:
            LogError( ( "Connection refused: Connection rate exceeded." ) );
            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t deserializeConnack( MQTTConnectionProperties_t * pConnackProperties,
                                 const MQTTPacketInfo_t * pIncomingPacket,
                                 bool * pSessionPresent,
                                 MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pVariableHeader = NULL;
    MQTTStatus_t statusCopy = MQTTSuccess;

    /*Validate the arguments*/
    status = validateConnackParams( pIncomingPacket, pSessionPresent );

    if( status == MQTTServerRefused )
    {
        statusCopy = status;
    }

    if( ( status == MQTTSuccess ) || ( status == MQTTServerRefused ) )
    {
        pVariableHeader = pIncomingPacket->pRemainingData;
        pVariableHeader = &pVariableHeader[ 2U * ( sizeof( uint8_t ) ) ];
        status = decodeVariableLength( pVariableHeader, pIncomingPacket->remainingLength - 2U, &propertyLength );
    }

    /*Validate the packet size if max packet size is set*/
    if( status == MQTTSuccess )
    {
        /*Validate the remaining length*/
        if( ( pIncomingPacket->remainingLength ) != ( 2U + propertyLength + variableLengthEncodedSize( propertyLength ) ) )
        {
            LogError( ( "Invalid Remaining Length" ) );
            status = MQTTBadResponse;
        }
        /*Deserialize the connack properties.*/
        else
        {
            status = deserializeConnackProperties( pConnackProperties, propertyLength, pVariableHeader, pPropBuffer );
        }
    }

    if( status == MQTTSuccess )
    {
        status = statusCopy;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t calculateSubscriptionPacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                     size_t subscriptionCount,
                                                     size_t * pRemainingLength,
                                                     size_t * pPacketSize,
                                                     size_t subscribePropLen,
                                                     uint32_t maxPacketSize,
                                                     MQTTSubscriptionType_t subscriptionType )
{
    size_t packetSize = 0U, i = 0U;
    MQTTStatus_t status = MQTTSuccess;

    assert( pSubscriptionList != NULL );
    assert( subscriptionCount != 0U );

    /*2 byte packet id*/
    packetSize += sizeof( uint16_t );


    packetSize += subscribePropLen;
    packetSize += variableLengthEncodedSize( subscribePropLen );

    for( i = 0; i < subscriptionCount; i++ )
    {
        packetSize += pSubscriptionList[ i ].topicFilterLength + sizeof( uint16_t );

        if( subscriptionType == MQTT_TYPE_SUBSCRIBE )
        {
            packetSize += 1U;
        }
    }

    /* At this point, the "Remaining length" has been calculated. Return error
     * if the "Remaining length" exceeds what is allowed by MQTT 5. Otherwise,
     * set the output parameter.*/
    if( packetSize > MQTT_MAX_REMAINING_LENGTH )
    {
        LogError( ( "Subscribe packet size %lu exceeds %d. "
                    "Packet size cannot be greater than %d.",
                    ( unsigned long ) packetSize,
                    UINT16_MAX,
                    UINT16_MAX ) );
        status = MQTTBadParameter;
    }

    if( status == MQTTSuccess )
    {
        *pRemainingLength = packetSize;
        packetSize += 1U + variableLengthEncodedSize( packetSize );
        *pPacketSize = packetSize;
    }

    if( packetSize > maxPacketSize )
    {
        LogError( ( "Packet size is greater than the allowed maximum packet size." ) );
        status = MQTTBadParameter;
    }

    LogDebug( ( "Subscription packet remaining length=%lu and packet size=%lu.",
                ( unsigned long ) *pRemainingLength,
                ( unsigned long ) *pPacketSize ) );
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t readSubackStatus( size_t statusCount,
                                      const uint8_t * pStatusStart,
                                      MQTTReasonCodeInfo_t * pReasonCodes )
{
    MQTTStatus_t status = MQTTServerRefused;
    uint8_t subscriptionStatus = 0;
    size_t i = 0;

    assert( pStatusStart != NULL );

    for( i = 0; i < statusCount; i++ )
    {
        subscriptionStatus = pStatusStart[ i ];

        switch( subscriptionStatus )
        {
            case 0x00:
            case 0x01:
            case 0x02:
                LogDebug( ( "Topic Filter %lu accepted, max QoS %u.",
                            ( unsigned long ) i,
                            ( unsigned int ) subscriptionStatus ) );
                status = MQTTSuccess;
                break;

            case 0x80:
                LogWarn( ( "Topic Filter Refused" ) );
                break;

            case 0x83:
                LogWarn( ( "Implementation specific error." ) );
                break;

            case 0x87:
                LogWarn( ( "Not authorized." ) );
                break;

            case 0x8F:
                LogWarn( ( "Topic Name Invalid." ) );
                break;

            case 0x91:
                LogWarn( ( "Packet Identifier In Use." ) );
                break;

            case 0x97:
                LogWarn( ( "Quota Exceeded." ) );
                break;

            case 0x9E:
                LogWarn( ( "Shared Subscriptions Not Supported." ) );
                break;

            case 0xA1:
                LogWarn( ( "Subscription Identifiers Not Supported." ) );
                break;

            case 0xA2:
                LogWarn( ( "Wildcard Subscriptions Not Supported." ) );
                break;

            default:
                LogError( ( "Bad Subscribe status %u.",
                            ( unsigned int ) subscriptionStatus ) );
                status = MQTTBadResponse;
                break;
        }

        if( status == MQTTBadResponse )
        {
            break;
        }
    }

    if( ( status == MQTTSuccess ) || ( status == MQTTServerRefused ) )
    {
        pReasonCodes->reasonCode = pStatusStart;
        pReasonCodes->reasonCodeLength = statusCount;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSuback( const MQTTPacketInfo_t * incomingPacket,
                                       uint16_t * pPacketId,
                                       MQTTReasonCodeInfo_t * pReasonCodes,
                                       MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    size_t remainingLength = 0U;
    size_t statusCount = 0U;
    const uint8_t * pStatusStart;
    size_t propertyLength = 0U;

    /* Validate input parameters using assert */
    assert( incomingPacket != NULL );
    assert( pPacketId != NULL );


    pIndex = incomingPacket->pRemainingData;
    remainingLength = incomingPacket->remainingLength;

    pIndex = incomingPacket->pRemainingData;

    if( pReasonCodes == NULL )
    {
        status = MQTTBadParameter;
    }
    else if( incomingPacket->remainingLength < 4U )
    {
        LogError( ( "Suback Packet Cannot have a remaining Length of less than 4" ) );
        status = MQTTBadResponse;
    }
    else
    {
        *pPacketId = UINT16_DECODE( pIndex );
        pIndex = &pIndex[ 2 ];
        LogDebug( ( "Packet Identifier is %hu.",
                    ( unsigned short ) *pPacketId ) );

        if( *pPacketId == 0U )
        {
            LogError( ( "Packet Id cannot be 0" ) );
            status = MQTTBadResponse;
        }
    }

    if( ( status == MQTTSuccess ) && ( incomingPacket->remainingLength > 4U ) )
    {
        status = deserializeSubackProperties( pPropBuffer,
                                              pIndex,
                                              &propertyLength,
                                              incomingPacket->remainingLength );
    }

    if( status == MQTTSuccess )
    {
        statusCount = remainingLength - sizeof( uint16_t ) - propertyLength - variableLengthEncodedSize( propertyLength );
        pStatusStart = &pIndex[ propertyLength + variableLengthEncodedSize( propertyLength ) ];
        status = readSubackStatus( statusCount, pStatusStart, pReasonCodes );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateSubscriptionSerializeParams( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                         size_t subscriptionCount,
                                                         uint16_t packetId,
                                                         size_t remainingLength,
                                                         const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t packetSize = 0;

    /* Validate all the parameters. */
    if( ( pFixedBuffer == NULL ) || ( pSubscriptionList == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer=%p, "
                    "pSubscriptionList=%p.",
                    ( void * ) pFixedBuffer,
                    ( void * ) pSubscriptionList ) );
        status = MQTTBadParameter;
    }
    /* A buffer must be configured for serialization. */
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionCount == 0U )
    {
        LogError( ( "Subscription count is 0." ) );
        status = MQTTBadParameter;
    }
    else if( packetId == 0U )
    {
        LogError( ( "Packet Id for subscription packet is 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* The serialized packet size = First byte
         * + length of encoded size of remaining length
         * + remaining length. */
        packetSize = 1U + variableLengthEncodedSize( remainingLength )
                     + remainingLength;

        if( packetSize > pFixedBuffer->size )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized packet of size of %lu.",
                        ( unsigned long ) pFixedBuffer->size,
                        ( unsigned long ) packetSize ) );
            status = MQTTNoMemory;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                      uint16_t * pPacketId,
                                      MQTTPublishInfo_t * pPublishInfo,
                                      MQTTPropBuilder_t * propBuffer,
                                      uint32_t maxPacketSize,
                                      uint16_t topicAliasMax )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( pIncomingPacket == NULL ) || ( pPacketId == NULL ) || ( pPublishInfo == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pIncomingPacket=%p, "
                    "pPacketId=%p, pPublishInfo=%p",
                    ( void * ) pIncomingPacket,
                    ( void * ) pPacketId,
                    ( void * ) pPublishInfo ) );
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->type & 0xF0U ) != MQTT_PACKET_TYPE_PUBLISH )
    {
        LogError( ( "Packet is not publish. Packet type: %02x.",
                    ( unsigned int ) pIncomingPacket->type ) );
        status = MQTTBadParameter;
    }
    else if( pIncomingPacket->pRemainingData == NULL )
    {
        LogError( ( "Argument cannot be NULL: "
                    "pIncomingPacket->pRemainingData is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->remainingLength + variableLengthEncodedSize( pIncomingPacket->remainingLength ) + 1U ) > maxPacketSize )
    {
        status = MQTTBadResponse;
    }
    else
    {
        status = deserializePublish( pIncomingPacket, pPacketId, pPublishInfo, propBuffer, topicAliasMax );
    }

    return status;
}
/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSimpleAck( const MQTTPacketInfo_t * pAck,
                                          uint16_t * pPacketIdentifier,
                                          MQTTReasonCodeInfo_t * pReasonCode,
                                          bool requestProblem,
                                          MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = pAck->pRemainingData;

    if( pReasonCode == NULL )
    {
        status = MQTTBadParameter;
    }
    else if( pAck->remainingLength < 2U )
    {
        status = MQTTBadResponse;
    }
    else
    {
        /* Extract the packet identifier (third and fourth bytes) from ACK. */
        *pPacketIdentifier = UINT16_DECODE( pIndex );
        pIndex = &pIndex[ 2 ];

        LogDebug( ( "Packet identifier %hu.",
                    ( unsigned short ) *pPacketIdentifier ) );

        /* Packet identifier cannot be 0. */
        if( *pPacketIdentifier == 0U )
        {
            LogError( ( "Packet identifier cannot be 0." ) );
            status = MQTTBadResponse;
        }
    }

    /* If reason code is success, server can choose to not send the reason code.*/
    if( ( status == MQTTSuccess ) && ( pAck->remainingLength > 2U ) )
    {
        pReasonCode->reasonCode = pIndex;
        pReasonCode->reasonCodeLength = 1U;
        pIndex++;
    }

    if( ( status == MQTTSuccess ) && ( pAck->remainingLength > 3U ) )
    {
        /*Protocol error to send user property and reason string if client has set request problem to false.*/
        if( requestProblem == false )
        {
            LogError( ( "User property and reason string not expected in ACK packet when requestProblem is false." ) );
            status = MQTTBadResponse;
        }
        else
        {
            status = decodeAckProperties( pPropBuffer, pIndex, pAck->remainingLength );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializePingresp( const MQTTPacketInfo_t * pPingresp )
{
    MQTTStatus_t status = MQTTSuccess;

    assert( pPingresp != NULL );

    /* Check the "Remaining length" (second byte) of the received PINGRESP is 0. */
    if( pPingresp->remainingLength != MQTT_PACKET_PINGRESP_REMAINING_LENGTH )
    {
        LogError( ( "PINGRESP does not have remaining length of %u.",
                    MQTT_PACKET_PINGRESP_REMAINING_LENGTH ) );

        status = MQTTBadResponse;
    }

    return status;
}

/*-----------------------------------------------------------*/

uint8_t * MQTT_SerializeConnectFixedHeader( uint8_t * pIndex,
                                            const MQTTConnectInfo_t * pConnectInfo,
                                            const MQTTPublishInfo_t * pWillInfo,
                                            size_t remainingLength )
{
    uint8_t * pIndexLocal = pIndex;
    uint8_t connectFlags = 0U;

    /* The first byte in the CONNECT packet is the control packet type. */
    *pIndexLocal = MQTT_PACKET_TYPE_CONNECT;
    pIndexLocal++;

    /* The remaining length of the CONNECT packet is encoded starting from the
     * second byte. The remaining length does not include the length of the fixed
     * header or the encoding of the remaining length. */
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );

    /* The string "MQTT" is placed at the beginning of the CONNECT packet's variable
     * header. This string is 4 bytes long. */
    pIndexLocal = encodeString( pIndexLocal, "MQTT", 4 );

    /* The MQTT protocol version is the second field of the variable header. */

    *pIndexLocal = MQTT_VERSION_5;

    pIndexLocal++;

    /* Set the clean session flag if needed. */
    if( pConnectInfo->cleanSession == true )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_CLEAN );
    }

    /* Set the flags for username and password if provided. */
    if( pConnectInfo->pUserName != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_USERNAME );
    }

    if( pConnectInfo->pPassword != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_PASSWORD );
    }

    /* Set will flag if a Last Will and Testament is provided. */
    if( pWillInfo != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL );

        /* Flags only need to be changed for Will QoS 1 or 2. */
        if( pWillInfo->qos == MQTTQoS1 )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_QOS1 );
        }
        else if( pWillInfo->qos == MQTTQoS2 )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_QOS2 );
        }
        else
        {
            /* Empty else MISRA 15.7 */
        }

        if( pWillInfo->retain == true )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_RETAIN );
        }
    }

    *pIndexLocal = connectFlags;
    pIndexLocal++;

    /* Write the 2 bytes of the keep alive interval into the CONNECT packet. */
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( pConnectInfo->keepAliveSeconds );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( pConnectInfo->keepAliveSeconds );
    pIndexLocal = &pIndexLocal[ 2 ];

    return pIndexLocal;
}

/*-----------------------------------------------------------*/

static void serializeConnectPacket( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    uint8_t * pIndex = NULL;
    size_t connectPropertyLength = 0U;
    size_t willPropertyLength = 0U;

    assert( pConnectInfo != NULL );
    assert( pFixedBuffer != NULL );
    assert( pFixedBuffer->pBuffer != NULL );

    pIndex = pFixedBuffer->pBuffer;

    /* Serialize the header. */
    pIndex = MQTT_SerializeConnectFixedHeader( pIndex,
                                               pConnectInfo,
                                               pWillInfo,
                                               remainingLength );

    if( ( pConnectProperties != NULL ) && ( pConnectProperties->pBuffer != NULL ) )
    {
        connectPropertyLength = pConnectProperties->currentIndex;
    }

    if( ( pWillProperties != NULL ) && ( pWillProperties->pBuffer != NULL ) )
    {
        willPropertyLength = pWillProperties->currentIndex;
    }

    /* Write the properties length into the CONNECT packet. */
    pIndex = encodeVariableLength( pIndex, connectPropertyLength );

    if( connectPropertyLength > 0U )
    {
        ( void ) memcpy( ( void * ) pIndex, ( const void * ) pConnectProperties->pBuffer, connectPropertyLength );
        pIndex = &pIndex[ connectPropertyLength ];
    }

    /* Write the client identifier into the CONNECT packet. */
    pIndex = encodeString( pIndex,
                           pConnectInfo->pClientIdentifier,
                           pConnectInfo->clientIdentifierLength );

    /* Write the will topic name and message into the CONNECT packet if provided. */
    if( pWillInfo != NULL )
    {
        pIndex = encodeVariableLength( pIndex, willPropertyLength );

        if( willPropertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pWillProperties->pBuffer, willPropertyLength );
            pIndex = &pIndex[ willPropertyLength ];
        }

        pIndex = encodeString( pIndex,
                               pWillInfo->pTopicName,
                               pWillInfo->topicNameLength );

        pIndex = encodeString( pIndex,
                               pWillInfo->pPayload,
                               ( uint16_t ) pWillInfo->payloadLength );
    }

    /* Encode the user name if provided. */
    if( pConnectInfo->pUserName != NULL )
    {
        pIndex = encodeString( pIndex, pConnectInfo->pUserName, pConnectInfo->userNameLength );
    }

    /* Encode the password if provided. */
    if( pConnectInfo->pPassword != NULL )
    {
        pIndex = encodeString( pIndex, pConnectInfo->pPassword, pConnectInfo->passwordLength );
    }

    LogDebug( ( "Length of serialized CONNECT packet is %lu.",
                ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );

    /* Ensure that the difference between the end and beginning of the buffer
     * is less than the buffer size. */
    assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        const MQTTPropBuilder_t * pConnectProperties,
                                        const MQTTPropBuilder_t * pWillProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t remainingLength;
    size_t propertyLength = 0U;
    size_t willPropertyLength = 0U;

    /* The CONNECT packet will always include a 10-byte variable header without connect properties. */
    size_t connectPacketSize = MQTT_PACKET_CONNECT_HEADER_SIZE;

    /* Validate arguments. */
    if( ( pConnectInfo == NULL ) || ( pRemainingLength == NULL ) ||
        ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pConnectInfo=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pConnectInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( ( pConnectInfo->clientIdentifierLength == 0U ) || ( pConnectInfo->pClientIdentifier == NULL ) )
    {
        LogError( ( "Mqtt_GetConnectPacketSize() client identifier must be set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pWillInfo != NULL ) && ( pWillInfo->payloadLength > ( size_t ) UINT16_MAX ) )
    {
        /* The MQTTPublishInfo_t is reused for the will message. The payload
         * length for any other message could be larger than 65,535, but
         * the will message length is required to be represented in 2 bytes.*/
        LogError( ( "The Will Message length must not exceed %d. "
                    "pWillInfo->payloadLength=%lu.",
                    UINT16_MAX,
                    ( unsigned long ) pWillInfo->payloadLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        /*Do Nothing*/
    }

    if( ( pConnectProperties != NULL ) && ( pConnectProperties->pBuffer != NULL ) )
    {
        propertyLength = pConnectProperties->currentIndex;
    }

    if( ( pWillProperties != NULL ) && ( pWillProperties->pBuffer != NULL ) )
    {
        willPropertyLength = pWillProperties->currentIndex;
    }

    if( status == MQTTSuccess )
    {
        /* Add the length of the properties. */
        connectPacketSize += propertyLength;
        connectPacketSize += variableLengthEncodedSize( propertyLength );

        /* Add the length of the client identifier. */
        connectPacketSize += pConnectInfo->clientIdentifierLength + sizeof( uint16_t );

        /* Add the lengths of the will message, topic name and properties if provided. */
        if( pWillInfo != NULL )
        {
            connectPacketSize += willPropertyLength;
            connectPacketSize += variableLengthEncodedSize( willPropertyLength );
            connectPacketSize += pWillInfo->topicNameLength + sizeof( uint16_t ) +
                                 pWillInfo->payloadLength + sizeof( uint16_t );
        }

        /* Add the lengths of the user name and password if provided. */
        if( pConnectInfo->pUserName != NULL )
        {
            connectPacketSize += pConnectInfo->userNameLength + sizeof( uint16_t );
        }

        if( pConnectInfo->pPassword != NULL )
        {
            connectPacketSize += pConnectInfo->passwordLength + sizeof( uint16_t );
        }

        /* At this point, the "Remaining Length" field of the MQTT CONNECT packet has
         * been calculated. */
        remainingLength = connectPacketSize;

        /* Calculate the full size of the MQTT CONNECT packet by adding the size of
         * the "Remaining Length" field plus 1 byte for the "Packet Type" field. */
        connectPacketSize += 1U + variableLengthEncodedSize( connectPacketSize );

        /*
         * It is possible that the remaining length becomes more than the maximum
         * allowed by the MQTTV5-Spec, i.e. 268,435,455. This is because the user may
         * enter a large number of user properties for the connect packet and/or
         * the last will.
         * Hence we need to have a check for this case
         */
        if( remainingLength > MQTT_MAX_REMAINING_LENGTH )
        {
            status = MQTTBadParameter;
        }
    }

    if( status == MQTTSuccess )
    {
        *pRemainingLength = remainingLength;
        *pPacketSize = connectPacketSize;

        LogDebug( ( "CONNECT packet remaining length=%lu and packet size=%lu.",
                    ( unsigned long ) *pRemainingLength,
                    ( unsigned long ) *pPacketSize ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t connectPacketSize = 0;

    /* Validate arguments. */
    if( ( pConnectInfo == NULL ) || ( pFixedBuffer == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pConnectInfo=%p, "
                    "pFixedBuffer=%p.",
                    ( void * ) pConnectInfo,
                    ( void * ) pFixedBuffer ) );
        status = MQTTBadParameter;
    }
    /* A buffer must be configured for serialization. */
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( pWillInfo != NULL ) && ( pWillInfo->pTopicName == NULL ) )
    {
        LogError( ( "pWillInfo->pTopicName cannot be NULL if Will is present." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate CONNECT packet size. Overflow in in this addition is not checked
         * because it is part of the API contract to call Mqtt_GetConnectPacketSize()
         * before this function. */
        connectPacketSize = remainingLength + variableLengthEncodedSize( remainingLength ) + 1U;

        /* Check that the full packet size fits within the given buffer. */
        if( connectPacketSize > pFixedBuffer->size )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized CONNECT packet of size of %lu.",
                        ( unsigned long ) pFixedBuffer->size,
                        ( unsigned long ) connectPacketSize ) );
            status = MQTTNoMemory;
        }
        else
        {
            serializeConnectPacket( pConnectInfo,
                                    pWillInfo,
                                    pConnectProperties,
                                    pWillProperties,
                                    remainingLength,
                                    pFixedBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                          size_t subscriptionCount,
                                          const MQTTPropBuilder_t * pSubscribeProperties,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize,
                                          uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;

    if( ( pSubscribeProperties != NULL ) && ( pSubscribeProperties->pBuffer != NULL ) )
    {
        propertyLength = pSubscribeProperties->currentIndex;
    }

    if( pSubscriptionList == NULL )
    {
        LogError( ( "Argument cannot be null : SubscriptionList" ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionCount == 0U )
    {
        LogError( ( "Subscription count cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        LogError( ( "Max Packet size cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = calculateSubscriptionPacketSize( pSubscriptionList, subscriptionCount,
                                                  pRemainingLength, pPacketSize, propertyLength,
                                                  maxPacketSize, MQTT_TYPE_SUBSCRIBE );
    }

    return status;
}

/*-----------------------------------------------------------*/

uint8_t * MQTT_SerializeSubscribeHeader( size_t remainingLength,
                                         uint8_t * pIndex,
                                         uint16_t packetId )
{
    uint8_t * pIterator = pIndex;

    /* The first byte in SUBSCRIBE is the packet type. */
    *pIterator = MQTT_PACKET_TYPE_SUBSCRIBE;
    pIterator++;

    /* Encode the "Remaining length" starting from the second byte. */
    pIterator = encodeVariableLength( pIterator, remainingLength );

    /* Place the packet identifier into the SUBSCRIBE packet. */
    pIterator[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIterator[ 1 ] = UINT16_LOW_BYTE( packetId );
    /* Advance the pointer. */
    pIterator = &pIterator[ 2 ];

    return pIterator;
}

/*-----------------------------------------------------------*/

uint8_t * MQTT_SerializeUnsubscribeHeader( size_t remainingLength,
                                           uint8_t * pIndex,
                                           uint16_t packetId )
{
    uint8_t * pIterator = pIndex;

    /* The first byte in UNSUBSCRIBE is the packet type. */
    *pIterator = MQTT_PACKET_TYPE_UNSUBSCRIBE;
    pIterator++;

    /* Encode the "Remaining length" starting from the second byte. */
    pIterator = encodeVariableLength( pIterator, remainingLength );

    /* Place the packet identifier into the SUBSCRIBE packet. */
    pIterator[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIterator[ 1 ] = UINT16_LOW_BYTE( packetId );
    /* Increment the pointer. */
    pIterator = &pIterator[ 2 ];

    return pIterator;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                      size_t subscriptionCount,
                                      const MQTTPropBuilder_t * pSubscribeProperties,
                                      uint16_t packetId,
                                      size_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer )
{
    size_t i = 0;
    uint8_t * pIndex = NULL;
    size_t propertyLength = 0U;
    MQTTStatus_t status;

    if( ( pSubscribeProperties != NULL ) && ( pSubscribeProperties->pBuffer != NULL ) )
    {
        propertyLength = pSubscribeProperties->currentIndex;
    }

    /* Validate all the parameters. */
    status =
        validateSubscriptionSerializeParams( pSubscriptionList,
                                             subscriptionCount,
                                             packetId,
                                             remainingLength,
                                             pFixedBuffer );

    if( status == MQTTSuccess )
    {
        pIndex = pFixedBuffer->pBuffer;

        pIndex = MQTT_SerializeSubscribeHeader( remainingLength,
                                                pIndex,
                                                packetId );
        /*Serialize properties*/
        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pSubscribeProperties->pBuffer, propertyLength );
            pIndex = &pIndex[ propertyLength ];
        }

        /* Serialize each subscription topic filter and QoS. */
        for( i = 0; i < subscriptionCount; i++ )
        {
            uint8_t subscriptionOptions = 0U;
            pIndex = encodeString( pIndex,
                                   pSubscriptionList[ i ].pTopicFilter,
                                   pSubscriptionList[ i ].topicFilterLength );

            /* Place the subscription options  */
            if( pSubscriptionList[ i ].qos == MQTTQoS1 )
            {
                LogInfo( ( "Adding QoS as QoS 1 in SUBSCRIBE payload" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_QOS1 );
            }
            else if( pSubscriptionList[ i ].qos == MQTTQoS2 )
            {
                LogInfo( ( "Adding QoS as QoS 2 in SUBSCRIBE payload" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_QOS2 );
            }
            else
            {
                LogInfo( ( "Adding QoS as QoS 0 in SUBSCRIBE payload" ) );
            }

            if( pSubscriptionList[ i ].noLocalOption )
            {
                LogInfo( ( "Adding noLocalOption in SUBSCRIBE payload" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_NO_LOCAL );
            }
            else
            {
                LogDebug( ( "Adding noLocalOption as 0 in SUBSCRIBE payload" ) );
            }

            if( pSubscriptionList[ i ].retainAsPublishedOption )
            {
                LogInfo( ( " retainAsPublishedOption in SUBSCRIBE payload" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_AS_PUBLISHED );
            }
            else
            {
                LogDebug( ( "retainAsPublishedOption as 0 in SUBSCRIBE payload" ) );
            }

            if( pSubscriptionList[ i ].retainHandlingOption == retainSendOnSub )
            {
                LogInfo( ( "Send Retain messages at the time of subscribe" ) );
            }
            else if( pSubscriptionList[ i ].retainHandlingOption == retainSendOnSubIfNotPresent )
            {
                LogInfo( ( "Send retained messages at subscribe only if the subscription does not currently exist" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_HANDLING1 );
            }
            else
            {
                LogInfo( ( "Do not send retained messages at subscribe" ) );
                UINT8_SET_BIT( subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_HANDLING2 );
            }

            *pIndex = subscriptionOptions;
            pIndex = &pIndex[ 1 ];
        }

        LogDebug( ( "Length of serialized SUBSCRIBE packet is %lu.",
                    ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            const MQTTPropBuilder_t * pUnsubscribeProperties,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;

    if( ( pUnsubscribeProperties != NULL ) && ( pUnsubscribeProperties->pBuffer != NULL ) )
    {
        propertyLength = pUnsubscribeProperties->currentIndex;
    }

    /* Validate parameters. */
    if( ( pSubscriptionList == NULL ) || ( pRemainingLength == NULL ) ||
        ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pSubscriptionList=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pSubscriptionList,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionCount == 0U )
    {
        LogError( ( "Subscription count is 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate the MQTT UNSUBSCRIBE packet size. */
        status = calculateSubscriptionPacketSize( pSubscriptionList,
                                                  subscriptionCount,
                                                  pRemainingLength,
                                                  pPacketSize,
                                                  propertyLength,
                                                  maxPacketSize,
                                                  MQTT_TYPE_UNSUBSCRIBE );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        const MQTTPropBuilder_t * pUnsubscribeProperties,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i = 0;
    size_t propertyLength = 0;
    uint8_t * pIndex = NULL;

    if( ( pUnsubscribeProperties != NULL ) && ( pUnsubscribeProperties->pBuffer != NULL ) )
    {
        propertyLength = pUnsubscribeProperties->currentIndex;
    }

    /* Validate all the parameters. */
    status = validateSubscriptionSerializeParams( pSubscriptionList,
                                                  subscriptionCount,
                                                  packetId,
                                                  remainingLength,
                                                  pFixedBuffer );

    if( status == MQTTSuccess )
    {
        /* Get the start of the buffer to the iterator variable. */
        pIndex = pFixedBuffer->pBuffer;

        pIndex = MQTT_SerializeUnsubscribeHeader( remainingLength, pIndex, packetId );

        /*Serialize the properties */

        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pUnsubscribeProperties->pBuffer, propertyLength );
            pIndex = &pIndex[ propertyLength ];
        }

        /* Serialize each subscription topic filter. */
        for( i = 0; i < subscriptionCount; i++ )
        {
            pIndex = encodeString( pIndex,
                                   pSubscriptionList[ i ].pTopicFilter,
                                   pSubscriptionList[ i ].topicFilterLength );
        }

        LogDebug( ( "Length of serialized UNSUBSCRIBE packet is %lu.",
                    ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        const MQTTPropBuilder_t * pPublishProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize,
                                        uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;

    if( ( pPublishProperties != NULL ) && ( pPublishProperties->pBuffer != NULL ) )
    {
        propertyLength = pPublishProperties->currentIndex;
    }

    if( ( pPublishInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pPublishInfo=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pPublishInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = calculatePublishPacketSize( pPublishInfo, pRemainingLength,
                                             pPacketSize, maxPacketSize, propertyLength );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    uint16_t packetId,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t packetSize = 0;

    if( ( pFixedBuffer == NULL ) || ( pPublishInfo == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer=%p, "
                    "pPublishInfo=%p.",
                    ( void * ) pFixedBuffer,
                    ( void * ) pPublishInfo ) );
        status = MQTTBadParameter;
    }
    /* A buffer must be configured for serialization. */
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
        status = MQTTBadParameter;
    }

    /* For serializing a publish, if there exists a payload, then the buffer
     * cannot be NULL. */
    else if( ( pPublishInfo->payloadLength > 0U ) && ( pPublishInfo->pPayload == NULL ) )
    {
        LogError( ( "A nonzero payload length requires a non-NULL payload: "
                    "payloadLength=%lu, pPayload=%p.",
                    ( unsigned long ) pPublishInfo->payloadLength,
                    pPublishInfo->pPayload ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) || ( pPublishInfo->topicNameLength == 0U ) )
    {
        LogError( ( "Invalid topic name for PUBLISH: pTopicName=%p, "
                    "topicNameLength=%hu.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->qos != MQTTQoS0 ) && ( packetId == 0U ) )
    {
        LogError( ( "Packet ID is 0 for PUBLISH with QoS=%u.",
                    ( unsigned int ) pPublishInfo->qos ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->dup == true ) && ( pPublishInfo->qos == MQTTQoS0 ) )
    {
        LogError( ( "Duplicate flag is set for PUBLISH with Qos 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Length of serialized packet = First byte
         *                                + Length of encoded remaining length
         *                                + Remaining length. */
        packetSize = 1U + variableLengthEncodedSize( remainingLength )
                     + remainingLength;
    }

    if( ( status == MQTTSuccess ) && ( packetSize > pFixedBuffer->size ) )
    {
        LogError( ( "Buffer size of %lu is not sufficient to hold "
                    "serialized PUBLISH packet of size of %lu.",
                    ( unsigned long ) pFixedBuffer->size,
                    ( unsigned long ) packetSize ) );
        status = MQTTNoMemory;
    }

    if( status == MQTTSuccess )
    {
        /* Serialize publish with header and payload. */
        serializePublishCommon( pPublishInfo,
                                pPublishProperties,
                                remainingLength,
                                packetId,
                                pFixedBuffer,
                                true );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublishHeader( const MQTTPublishInfo_t * pPublishInfo,
                                          const MQTTPropBuilder_t * pPublishProperties,
                                          uint16_t packetId,
                                          size_t remainingLength,
                                          const MQTTFixedBuffer_t * pFixedBuffer,
                                          size_t * pHeaderSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t packetSize = 0;

    if( ( pFixedBuffer == NULL ) || ( pPublishInfo == NULL ) ||
        ( pHeaderSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer=%p, "
                    "pPublishInfo=%p, pHeaderSize=%p.",
                    ( void * ) pFixedBuffer,
                    ( void * ) pPublishInfo,
                    ( void * ) pHeaderSize ) );
        status = MQTTBadParameter;
    }
    /* A buffer must be configured for serialization. */
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) || ( pPublishInfo->topicNameLength == 0U ) )
    {
        LogError( ( "Invalid topic name for publish: pTopicName=%p, "
                    "topicNameLength=%hu.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->qos != MQTTQoS0 ) && ( packetId == 0U ) )
    {
        LogError( ( "Packet Id is 0 for publish with QoS=%hu.",
                    ( unsigned short ) pPublishInfo->qos ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->dup == true ) && ( pPublishInfo->qos == MQTTQoS0 ) )
    {
        LogError( ( "Duplicate flag is set for PUBLISH with Qos 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Length of serialized packet = First byte
         *                               + Length of encoded remaining length
         *                               + Remaining length
         *                               - Payload Length.
         */
        packetSize = 1U + variableLengthEncodedSize( remainingLength )
                     + remainingLength
                     - pPublishInfo->payloadLength;
    }

    if( ( status == MQTTSuccess ) && ( packetSize > pFixedBuffer->size ) )
    {
        LogError( ( "Buffer size of %lu is not sufficient to hold "
                    "serialized PUBLISH header packet of size of %lu.",
                    ( unsigned long ) pFixedBuffer->size,
                    ( unsigned long ) ( packetSize - pPublishInfo->payloadLength ) ) );
        status = MQTTNoMemory;
    }

    if( status == MQTTSuccess )
    {
        /* Serialize publish without copying the payload. */
        serializePublishCommon( pPublishInfo,
                                pPublishProperties,
                                remainingLength,
                                packetId,
                                pFixedBuffer,
                                false );

        /* Header size is the same as calculated packet size. */
        *pHeaderSize = packetSize;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeAck( const MQTTFixedBuffer_t * pFixedBuffer,
                                uint8_t packetType,
                                uint16_t packetId )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pFixedBuffer == NULL )
    {
        LogError( ( "Provided buffer is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "pFixedBuffer->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    /* The buffer must be able to fit 4 bytes for the packet. */
    else if( pFixedBuffer->size < MQTT_PUBLISH_ACK_PACKET_SIZE )
    {
        LogError( ( "Insufficient memory for packet." ) );
        status = MQTTNoMemory;
    }
    else if( packetId == 0U )
    {
        LogError( ( "Packet ID cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        switch( packetType )
        {
            /* Only publish acks are serialized by the client. */
            case MQTT_PACKET_TYPE_PUBACK:
            case MQTT_PACKET_TYPE_PUBREC:
            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                pFixedBuffer->pBuffer[ 0 ] = packetType;
                pFixedBuffer->pBuffer[ 1 ] = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH;
                pFixedBuffer->pBuffer[ 2 ] = UINT16_HIGH_BYTE( packetId );
                pFixedBuffer->pBuffer[ 3 ] = UINT16_LOW_BYTE( packetId );
                break;

            default:
                LogError( ( "Packet type is not a publish ACK: Packet type=%02x",
                            ( unsigned int ) packetType ) );
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetDisconnectPacketSize( const MQTTPropBuilder_t * pDisconnectProperties,
                                           size_t * pRemainingLength,
                                           size_t * pPacketSize,
                                           uint32_t maxPacketSize,
                                           MQTTSuccessFailReasonCode_t reasonCode )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t length = 0U;
    size_t packetSize = 0U;
    size_t propertyLength = 0U;

    if( ( pDisconnectProperties != NULL ) && ( pDisconnectProperties->pBuffer != NULL ) )
    {
        propertyLength = pDisconnectProperties->currentIndex;
    }

    if( ( pDisconnectProperties != NULL ) && ( pDisconnectProperties->pBuffer != NULL ) )
    {
        propertyLength = pDisconnectProperties->currentIndex;
    }

    /*Validate the arguments.*/
    if( ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL:"
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        LogError( ( "Max packet size cannot be zero." ) );
        status = MQTTBadParameter;
    }
    else if( validateDisconnectResponse( ( uint8_t ) reasonCode, false ) != MQTTSuccess )
    {
        LogError( ( "Invalid reason code." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /*Reason code.*/
        length += 1U;
    }

    if( status == MQTTSuccess )
    {
        /*Validate the length.*/
        if( ( propertyLength + variableLengthEncodedSize( propertyLength ) + 1U ) < MQTT_MAX_REMAINING_LENGTH )
        {
            /*We have successfully calculated the property length.*/
            length += variableLengthEncodedSize( propertyLength ) + propertyLength;
            *pRemainingLength = length;
        }
        else
        {
            status = MQTTBadParameter;
        }
    }

    if( status == MQTTSuccess )
    {
        /*Packet size should be less than max allowed by the server.*/
        packetSize = length + 1U + variableLengthEncodedSize( length );

        if( packetSize > maxPacketSize )
        {
            status = MQTTBadParameter;
            LogError( ( "Packet Size greater than Max Packet Size specified in the CONNACK" ) );
        }
        else
        {
            *pPacketSize = packetSize;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeDisconnect( const MQTTPropBuilder_t * pDisconnectProperties,
                                       MQTTSuccessFailReasonCode_t reasonCode,
                                       size_t remainingLength,
                                       const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    size_t packetSize = 0;
    size_t propertyLength = 0;

    if( ( pDisconnectProperties != NULL ) && ( pDisconnectProperties->pBuffer != NULL ) )
    {
        propertyLength = pDisconnectProperties->currentIndex;
    }

    /* Validate arguments. */
    if( pFixedBuffer == NULL )
    {
        LogError( ( "pFixedBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "pFixedBuffer->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Length of serialized packet = First byte
         *                                + Length of encoded remaining length
         *                                + Remaining length. */
        packetSize = 1U + variableLengthEncodedSize( remainingLength ) + remainingLength;
    }

    if( status == MQTTSuccess )
    {
        if( pFixedBuffer->size < packetSize )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized DISCONNECT packet of size of %lu.",
                        ( unsigned long ) pFixedBuffer->size,
                        ( unsigned long ) packetSize ) );
            status = MQTTNoMemory;
        }
    }

    if( status == MQTTSuccess )
    {
        pIndex = pFixedBuffer->pBuffer;
        pIndex = MQTT_SerializeDisconnectFixed( pIndex,
                                                reasonCode,
                                                remainingLength );

        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pDisconnectProperties->pBuffer, propertyLength );
            pIndex = &pIndex[ propertyLength ];
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetPingreqPacketSize( size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPacketSize == NULL )
    {
        LogError( ( "pPacketSize is NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* MQTT PINGREQ packets always have the same size. */
        *pPacketSize = MQTT_PACKET_PINGREQ_SIZE;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePingreq( const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pFixedBuffer == NULL )
    {
        LogError( ( "pFixedBuffer is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pFixedBuffer->pBuffer == NULL )
    {
        LogError( ( "pFixedBuffer->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Empty else MISRA 15.7 */
    }

    if( status == MQTTSuccess )
    {
        if( pFixedBuffer->size < MQTT_PACKET_PINGREQ_SIZE )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized PINGREQ packet of size of %lu.",
                        ( unsigned long ) pFixedBuffer->size,
                        MQTT_PACKET_PINGREQ_SIZE ) );
            status = MQTTNoMemory;
        }
    }

    if( status == MQTTSuccess )
    {
        /* Ping request packets are always the same. */
        pFixedBuffer->pBuffer[ 0 ] = MQTT_PACKET_TYPE_PINGREQ;
        pFixedBuffer->pBuffer[ 1 ] = 0x00;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetIncomingPacketTypeAndLength( TransportRecv_t readFunc,
                                                  NetworkContext_t * pNetworkContext,
                                                  MQTTPacketInfo_t * pIncomingPacket )
{
    MQTTStatus_t status = MQTTSuccess;
    int32_t bytesReceived = 0;

    if( pIncomingPacket == NULL )
    {
        LogError( ( "Invalid parameter: pIncomingPacket is NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Read a single byte. */
        bytesReceived = readFunc( pNetworkContext,
                                  &( pIncomingPacket->type ),
                                  1U );
    }

    if( bytesReceived == 1 )
    {
        /* Check validity. */
        if( incomingPacketValid( pIncomingPacket->type ) == true )
        {
            pIncomingPacket->remainingLength = getRemainingLength( readFunc,
                                                                   pNetworkContext );

            if( pIncomingPacket->remainingLength == MQTT_REMAINING_LENGTH_INVALID )
            {
                LogError( ( "Incoming packet remaining length invalid." ) );
                status = MQTTBadResponse;
            }
        }
        else
        {
            LogError( ( "Incoming packet invalid: Packet type=%u.",
                        ( unsigned int ) pIncomingPacket->type ) );
            status = MQTTBadResponse;
        }
    }
    else if( ( status != MQTTBadParameter ) && ( bytesReceived == 0 ) )
    {
        status = MQTTNoDataAvailable;
    }

    /* If the input packet was valid, then any other number of bytes received is
     * a failure. */
    else if( status != MQTTBadParameter )
    {
        LogError( ( "A single byte was not read from the transport: "
                    "transportStatus=%ld.",
                    ( long int ) bytesReceived ) );
        status = MQTTRecvFailed;
    }
    else
    {
        /* Empty else MISRA 15.7 */
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ProcessIncomingPacketTypeAndLength( const uint8_t * pBuffer,
                                                      const size_t * pIndex,
                                                      MQTTPacketInfo_t * pIncomingPacket )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pIncomingPacket == NULL )
    {
        LogError( ( "Invalid parameter: pIncomingPacket is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pIndex == NULL )
    {
        LogError( ( "Invalid parameter: pIndex is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pBuffer == NULL )
    {
        LogError( ( "Invalid parameter: pBuffer is NULL." ) );
        status = MQTTBadParameter;
    }
    /* There should be at least one byte in the buffer */
    else if( *pIndex < 1U )
    {
        /* No data is available. There are 0 bytes received from the network
         * receive function. */
        status = MQTTNoDataAvailable;
    }
    else
    {
        /* At least one byte is present which should be deciphered. */
        pIncomingPacket->type = pBuffer[ 0 ];
    }

    if( status == MQTTSuccess )
    {
        /* Check validity. */
        if( incomingPacketValid( pIncomingPacket->type ) == true )
        {
            status = processRemainingLength( pBuffer,
                                             pIndex,
                                             pIncomingPacket );
        }
        else
        {
            LogError( ( "Incoming packet invalid: Packet type=%u.",
                        ( unsigned int ) pIncomingPacket->type ) );
            status = MQTTBadResponse;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/


static MQTTStatus_t decodeAndDiscard( size_t * pPropertyLength,
                                      uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;
    uint16_t keyLength;
    uint16_t valueLength;

    /*Validate the property length and decode the user property received.*/
    if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        keyLength = UINT16_DECODE( pLocalIndex );
        *pPropertyLength -= sizeof( uint16_t );

        if( *pPropertyLength < keyLength )
        {
            status = MQTTBadResponse;
        }
        else
        {
            pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
            *pPropertyLength -= keyLength;
            pLocalIndex = &pLocalIndex[ keyLength ];

            if( *pPropertyLength < sizeof( uint16_t ) )
            {
                status = MQTTBadResponse;
            }
            else
            {
                valueLength = UINT16_DECODE( pLocalIndex );
                *pPropertyLength -= sizeof( uint16_t );
                pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];

                if( *pPropertyLength < ( size_t ) ( valueLength ) )
                {
                    status = MQTTBadResponse;
                }
                else
                {
                    pLocalIndex = &pLocalIndex[ valueLength ];
                    *pPropertyLength -= valueLength;
                }
            }
        }
    }

    *pIndex = pLocalIndex;

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeAndDiscardutf_8( size_t * pPropertyLength,
                                           bool * pUsed,
                                           uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;
    uint16_t propertyLength;

    /*Protocol error to include the same property twice.*/

    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    else if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        propertyLength = UINT16_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pPropertyLength -= sizeof( uint16_t );

        if( *pPropertyLength < propertyLength )
        {
            status = MQTTBadResponse;
        }
        else
        {
            pLocalIndex = &pLocalIndex[ propertyLength ];
            *pPropertyLength -= propertyLength;
            *pUsed = true;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeAndDiscard_uint8( size_t * pPropertyLength,
                                            bool * pUsed,
                                            uint8_t ** pIndex )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = *pIndex;

    if( *pPropertyLength < sizeof( uint8_t ) )
    {
        status = MQTTBadResponse;
    }
    else if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    else
    {
        /* Skip 1 byte */
        pLocalIndex = &pLocalIndex[ sizeof( uint8_t ) ];
        *pPropertyLength -= sizeof( uint8_t );
        *pUsed = true;
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeAndDiscard_uint32( size_t * pPropertyLength,
                                             bool * pUsed,
                                             uint8_t ** pIndex )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = *pIndex;

    if( *pPropertyLength < sizeof( uint32_t ) )
    {
        status = MQTTBadResponse;
    }
    else if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    else
    {
        /* Skip 4 bytes */
        pLocalIndex = &pLocalIndex[ sizeof( uint32_t ) ];
        *pPropertyLength -= sizeof( uint32_t );
        *pUsed = true;
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateConnackParams( const MQTTPacketInfo_t * pIncomingPacket,
                                           bool * pSessionPresent )
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pRemainingData = NULL;

    /*Validate the arguments via asserts. */
    assert( pIncomingPacket != NULL );
    assert( pSessionPresent != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );
    assert( pIncomingPacket->type == MQTT_PACKET_TYPE_CONNACK );

    pRemainingData = pIncomingPacket->pRemainingData;

    /* Remaining Length of the CONNACK cannot be less than 3. */
    if( pIncomingPacket->remainingLength < MQTT_PACKET_CONNACK_MINIMUM_SIZE )
    {
        LogError( ( "Incomplete Connack received" ) );

        status = MQTTBadResponse;
    }

    if( status == MQTTSuccess )
    {
        /* Reserved bits in CONNACK must be 0. */
        if( ( pRemainingData[ 0 ] | 0x01U ) != 0x01U )
        {
            LogError( ( "Reserved bits in CONNACK incorrect." ) );

            status = MQTTBadResponse;
        }
        else
        {
            /* Determine if the "Session Present" bit is set. This is the lowest bit of
             * the third byte in CONNACK. */
            if( ( pRemainingData[ 0 ] & MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK ) == MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK )
            {
                LogDebug( ( "CONNACK session present bit set." ) );
                *pSessionPresent = true;

                /* MQTT 5 specifies that the fourth byte in CONNACK must be 0 if the
                 * "Session Present" bit is set. */
                if( pRemainingData[ 1 ] != 0U )
                {
                    LogError( ( "Session Present bit is set, but connect return code in CONNACK is %u (nonzero).",
                                ( unsigned int ) pRemainingData[ 1 ] ) );
                    status = MQTTBadResponse;
                }
            }
            else
            {
                LogDebug( ( "CONNACK session present bit not set." ) );
                *pSessionPresent = false;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        if( pRemainingData[ 1 ] != 0U )
        {
            status = logConnackResponse( pRemainingData[ 1 ] );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                          size_t bufferLength,
                                          size_t * pLength )
{
    size_t remainingLength = 0;
    size_t multiplier = 1;
    size_t bytesDecoded = 0;
    size_t expectedSize = 0;
    uint8_t encodedByte = 0;
    size_t localBufferLength = bufferLength;
    MQTTStatus_t status = MQTTSuccess;

    /* This algorithm is copied from the MQTT 5.0 spec. */
    do
    {
        if( multiplier > 2097152U ) /* 128 ^ 3 */
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;

            LogError( ( "Invalid remaining length in the packet.\n" ) );

            status = MQTTBadResponse;
        }
        else
        {
            if( localBufferLength > 0U )
            {
                encodedByte = pBuffer[ bytesDecoded ];
                remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
                multiplier *= 128U;
                bytesDecoded++;
                localBufferLength--;
            }
            else
            {
                status = MQTTBadResponse;
            }
        }

        /* If the response is incorrect then
         * break out of the loop. */
        if( ( remainingLength == MQTT_REMAINING_LENGTH_INVALID ) ||
            ( status != MQTTSuccess ) )
        {
            break;
        }
    } while( ( encodedByte & 0x80U ) != 0U );

    if( status == MQTTSuccess )
    {
        /* Check that the decoded remaining length conforms to the MQTT specification. */
        expectedSize = variableLengthEncodedSize( remainingLength );

        if( bytesDecoded != expectedSize )
        {
            LogError( ( "Expected and actual length of decoded bytes do not match.\n" ) );
            status = MQTTBadResponse;
        }
        else
        {
            *pLength = remainingLength;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeuint32_t( uint32_t * pProperty,
                                    size_t * pPropertyLength,
                                    bool * pUsed,
                                    uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /*Validate the length and decode.*/
    else if( *pPropertyLength < sizeof( uint32_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pProperty = UINT32_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint32_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint32_t );
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeuint16_t( uint16_t * pProperty,
                                    size_t * pPropertyLength,
                                    bool * pUsed,
                                    uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /*Validate the length and decode.*/

    else if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pProperty = UINT16_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint16_t );
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeuint8_t( uint8_t * pProperty,
                                   size_t * pPropertyLength,
                                   bool * pUsed,
                                   uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /*Validate the length and decode.*/

    else if( *pPropertyLength < sizeof( uint8_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pProperty = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ sizeof( uint8_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint8_t );

        if( *pProperty > 1U )
        {
            status = MQTTBadResponse;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeutf_8( const char ** pProperty,
                                 uint16_t * pLength,
                                 size_t * pPropertyLength,
                                 bool * pUsed,
                                 uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /*Validate the length and decode.*/
    else if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pLength = UINT16_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pPropertyLength -= sizeof( uint16_t );

        if( *pPropertyLength < *pLength )
        {
            status = MQTTBadResponse;
        }
        else
        {
            *pProperty = ( const char * ) pLocalIndex;
            pLocalIndex = &pLocalIndex[ *pLength ];
            *pPropertyLength -= *pLength;
            *pUsed = true;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeBinaryData( const void ** pProperty,
                                      uint16_t * pLength,
                                      size_t * pPropertyLength,
                                      uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Validate the length and decode.*/

    if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pLength = UINT16_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pPropertyLength -= sizeof( uint16_t );

        if( *pPropertyLength < *pLength )
        {
            status = MQTTBadResponse;
        }
        else
        {
            *pProperty = pLocalIndex;
            pLocalIndex = &pLocalIndex[ *pLength ];
            *pPropertyLength -= *pLength;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeConnackProperties( MQTTConnectionProperties_t * pConnackProperties,
                                                  size_t length,
                                                  uint8_t * pIndex,
                                                  MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pVariableHeader = pIndex;
    size_t propertyLength = length;
    bool sessionExpiry = false;
    bool serverReceiveMax = false;
    bool maxQos = false;
    bool retain = false;
    bool maxPacket = false;
    bool clientId = false;
    bool topicAlias = false;
    bool wildcard = false;
    bool subId = false;
    bool sharedsub = false;
    bool keepAlive = false;
    bool responseInfo = false;
    bool serverReference = false;
    bool authMethod = false;
    bool authData = false;
    bool reasonString = false;

    pVariableHeader = &pVariableHeader[ variableLengthEncodedSize( propertyLength ) ];

    if( pPropBuffer != NULL )
    {
        pPropBuffer->pBuffer = pVariableHeader;
        pPropBuffer->bufferLength = propertyLength;
    }

    /*Decode all the properties received, validate and store them in pConnackProperties.*/

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pVariableHeader;
        pVariableHeader = &pVariableHeader[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_SESSION_EXPIRY_ID:
                status = decodeuint32_t( &pConnackProperties->sessionExpiry, &propertyLength,
                                         &sessionExpiry, &pVariableHeader );
                break;

            case MQTT_RECEIVE_MAX_ID:
                status = decodeuint16_t( &pConnackProperties->serverReceiveMax, &propertyLength,
                                         &serverReceiveMax, &pVariableHeader );
                break;

            case MQTT_MAX_QOS_ID:
                status = decodeuint8_t( &pConnackProperties->serverMaxQos, &propertyLength,
                                        &maxQos, &pVariableHeader );
                break;

            case MQTT_RETAIN_AVAILABLE_ID:
                status = decodeuint8_t( &pConnackProperties->retainAvailable, &propertyLength,
                                        &retain, &pVariableHeader );
                break;

            case MQTT_MAX_PACKET_SIZE_ID:
                status = decodeuint32_t( &pConnackProperties->serverMaxPacketSize, &propertyLength,
                                         &maxPacket, &pVariableHeader );
                break;

            case MQTT_ASSIGNED_CLIENT_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &clientId, &pVariableHeader );
                break;

            case MQTT_TOPIC_ALIAS_MAX_ID:
                status = decodeuint16_t( &pConnackProperties->serverTopicAliasMax, &propertyLength,
                                         &topicAlias, &pVariableHeader );
                break;

            case MQTT_REASON_STRING_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &reasonString, &pVariableHeader );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pVariableHeader );
                break;

            case MQTT_WILDCARD_ID:
                status = decodeuint8_t( &pConnackProperties->isWildcardAvailable, &propertyLength,
                                        &wildcard, &pVariableHeader );
                break;

            case MQTT_SUB_AVAILABLE_ID:
                status = decodeuint8_t( &pConnackProperties->isSubscriptionIdAvailable, &propertyLength,
                                        &subId, &pVariableHeader );
                break;

            case MQTT_SHARED_SUB_ID:
                status = decodeuint8_t( &pConnackProperties->isSharedAvailable, &propertyLength,
                                        &sharedsub, &pVariableHeader );
                break;

            case MQTT_SERVER_KEEP_ALIVE_ID:
                status = decodeuint16_t( &pConnackProperties->serverKeepAlive, &propertyLength,
                                         &keepAlive, &pVariableHeader );
                break;

            case MQTT_RESPONSE_INFO_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &responseInfo, &pVariableHeader );
                break;

            case MQTT_SERVER_REF_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &serverReference, &pVariableHeader );
                break;

            case MQTT_AUTH_METHOD_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &authMethod, &pVariableHeader );
                break;

            case MQTT_AUTH_DATA_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &authData, &pVariableHeader );
                break;

            /*Protocol error to include any other property id.*/
            default:
                status = MQTTBadResponse;
                break;
        }
    }

    if( status == MQTTSuccess )
    {
        /*Receive max cannot be 0.*/
        if( ( serverReceiveMax == true ) && ( pConnackProperties->serverReceiveMax == 0U ) )
        {
            status = MQTTBadResponse;
        }

        /*Maximum packet size cannot be 0.*/
        else if( ( maxPacket == true ) && ( pConnackProperties->serverMaxPacketSize == 0U ) )
        {
            status = MQTTBadResponse;
        }
        /*Protocol error to send response information if the client has not requested it.*/
        else if( ( responseInfo == true ) && ( pConnackProperties->requestResponseInfo == false ) )
        {
            status = MQTTBadResponse;
        }
        else
        {
            /* Empty else MISRA 15.7 */
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t logAckResponse( uint8_t reasonCode,
                                    uint16_t packetIdentifier )
{
    MQTTStatus_t status = MQTTServerRefused;

    /* coverity[misra_c_2012_rule_10_5_violation] */
    switch( ( MQTTSuccessFailReasonCode_t ) reasonCode )
    {
        case MQTT_REASON_PUBACK_SUCCESS:
            ( void ) packetIdentifier;
            status = MQTTSuccess;
            break;

        case MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS:
            LogDebug( ( "Publish accepted with packet id %hu: No matching subscribers.",
                        ( unsigned short ) packetIdentifier ) );
            status = MQTTSuccess;
            break;

        case MQTT_REASON_PUBACK_UNSPECIFIED_ERROR:
            LogError( ( "Publish refused with packet id %hu: Connection rate exceeded.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR:
            LogError( ( "Publish refused with packet id %hu:  The PUBLISH is valid but the receiver is not willing to accept it.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_NOT_AUTHORIZED:
            LogError( ( "Publish refused with packet id %hu: The PUBLISH is not authorized.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_TOPIC_NAME_INVALID:
            LogError( ( "Publish refused with packet id %hu: Topic Name not accepted.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE:
            LogError( ( "Publish refused with packet id %hu: The Packet Identifier is already in use. ",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_QUOTA_EXCEEDED:
            LogError( ( "Publish refused with packet id %hu: Quota exceeded.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID:
            LogError( ( "Publish refused with packet id %hu: Payload format indicator is invalid.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t logSimpleAckResponse( uint8_t reasonCode,
                                          uint16_t packetIdentifier )
{
    MQTTStatus_t status = MQTTServerRefused;

    /* coverity[misra_c_2012_rule_10_5_violation] */
    switch( ( MQTTSuccessFailReasonCode_t ) reasonCode )
    {
        case MQTT_REASON_PUBREL_SUCCESS:
            ( void ) packetIdentifier;
            status = MQTTSuccess;
            break;

        case MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND:
            LogError( ( "Publish refused with packet id %hu: Packet identifier invalid.",
                        ( unsigned short ) packetIdentifier ) );
            status = MQTTServerRefused;
            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t decodeAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                         uint8_t * pIndex,
                                         size_t remainingLength )
{
    size_t propertyLength = 0U;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = pIndex;
    bool reasonString = false;

    /*Decode the property length*/
    status = decodeVariableLength( pLocalIndex, remainingLength - 3U, &propertyLength );

    if( status == MQTTSuccess )
    {
        pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( propertyLength ) ];

        /*Validate the remaining length.*/
        if( remainingLength != ( propertyLength + variableLengthEncodedSize( propertyLength ) + 3U ) )
        {
            status = MQTTBadResponse;
        }
    }

    if( pPropBuffer != NULL )
    {
        pPropBuffer->pBuffer = pLocalIndex;
        pPropBuffer->bufferLength = propertyLength;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        /*Decode the property id.*/
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &reasonString, &pLocalIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateDisconnectResponse( uint8_t reasonCode,
                                                bool incoming )
{
    MQTTStatus_t status;

    /*Validate the reason code.*/
    /* coverity[misra_c_2012_rule_10_5_violation] */
    switch( ( MQTTSuccessFailReasonCode_t ) reasonCode )
    {
        case MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE:

            if( incoming == true )
            {
                status = MQTTBadResponse;
            }
            else
            {
                status = MQTTSuccess;
            }

            break;

        case MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION:
        case MQTT_REASON_DISCONNECT_UNSPECIFIED_ERROR:
        case MQTT_REASON_DISCONNECT_MALFORMED_PACKET:
        case MQTT_REASON_DISCONNECT_PROTOCOL_ERROR:
        case MQTT_REASON_DISCONNECT_IMPLEMENTATION_SPECIFIC_ERROR:
        case MQTT_REASON_DISCONNECT_TOPIC_NAME_INVALID:
        case MQTT_REASON_DISCONNECT_RECEIVE_MAXIMUM_EXCEEDED:
        case MQTT_REASON_DISCONNECT_TOPIC_ALIAS_INVALID:
        case MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE:
        case MQTT_REASON_DISCONNECT_MESSAGE_RATE_TOO_HIGH:
        case MQTT_REASON_DISCONNECT_QUOTA_EXCEEDED:
        case MQTT_REASON_DISCONNECT_ADMINISTRATIVE_ACTION:
        case MQTT_REASON_DISCONNECT_PAYLOAD_FORMAT_INVALID:
            status = MQTTSuccess;
            break;

        case MQTT_REASON_DISCONNECT_NOT_AUTHORIZED:
        case MQTT_REASON_DISCONNECT_SERVER_BUSY:
        case MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN:
        case MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT:
        case MQTT_REASON_DISCONNECT_SESSION_TAKEN_OVER:
        case MQTT_REASON_DISCONNECT_TOPIC_FILTER_INVALID:
        case MQTT_REASON_DISCONNECT_RETAIN_NOT_SUPPORTED:
        case MQTT_REASON_DISCONNECT_QOS_NOT_SUPPORTED:
        case MQTT_REASON_DISCONNECT_USE_ANOTHER_SERVER:
        case MQTT_REASON_DISCONNECT_SERVER_MOVED:
        case MQTT_REASON_DISCONNECT_MAXIMUM_CONNECT_TIME:
        case MQTT_REASON_DISCONNECT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED:
        case MQTT_REASON_DISCONNECT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED:
        case MQTT_REASON_DISCONNECT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED:
        case MQTT_REASON_DISCONNECT_BAD_AUTHENTICATION_METHOD:

            if( incoming == true )
            {
                status = MQTTSuccess;
            }
            else
            {
                status = MQTTBadParameter;
            }

            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static uint8_t * encodeBinaryData( uint8_t * pDestination,
                                   const void * pSource,
                                   uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    /* Typecast const char * typed source buffer to const uint8_t *.
     * This is to use same type buffers in memcpy. */
    const uint8_t * pSourceBuffer = ( const uint8_t * ) pSource;

    assert( pDestination != NULL );
    assert( pSource != NULL );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */

    ( void ) memcpy( pBuffer, pSourceBuffer, sourceLength );

    /* Return the pointer to the end of the encoded string. */
    pBuffer = &pBuffer[ sourceLength ];

    return pBuffer;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializePublishProperties( MQTTPublishInfo_t * pPublishInfo,
                                                  MQTTPropBuilder_t * pPropBuffer,
                                                  uint8_t * pIndex,
                                                  uint16_t topicAliasMax,
                                                  size_t remainingLength )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pLocalIndex = pIndex;
    size_t subscriptionId;
    size_t remainingLengthForProperties;
    bool contentType = false;
    bool messageExpiryInterval = false;
    bool responseTopic = false;
    bool topicAlias = false;
    bool payloadFormatIndicator = false;
    bool correlationData = false;
    uint16_t topicAliasVal;

    /*Decode Property Length */
    remainingLengthForProperties = remainingLength;
    remainingLengthForProperties -= pPublishInfo->topicNameLength + sizeof( uint16_t );
    remainingLengthForProperties -= ( pPublishInfo->qos > MQTTQoS0 ) ? sizeof( uint16_t ) : 0U;

    status = decodeVariableLength( pLocalIndex, remainingLengthForProperties, &propertyLength );
    pPublishInfo->propertyLength = propertyLength;

    if( status == MQTTSuccess )
    {
        status = checkPublishRemainingLength( remainingLength,
                                              pPublishInfo->qos,
                                              pPublishInfo->topicNameLength +
                                              sizeof( uint16_t ) + propertyLength +
                                              variableLengthEncodedSize( propertyLength ) );
    }

    if( status == MQTTSuccess )
    {
        pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( propertyLength ) ];
    }

    if( pPropBuffer != NULL )
    {
        pPropBuffer->pBuffer = pLocalIndex;
        pPropBuffer->bufferLength = propertyLength;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_PAYLOAD_FORMAT_ID:
                status = decodeAndDiscard_uint8( &propertyLength, &payloadFormatIndicator, &pLocalIndex );
                break;

            case MQTT_TOPIC_ALIAS_ID:
                status = decodeuint16_t( &topicAliasVal, &propertyLength, &topicAlias, &pLocalIndex );
                break;

            case MQTT_RESPONSE_TOPIC_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &responseTopic, &pLocalIndex );
                break;

            case MQTT_CORRELATION_DATA_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &correlationData, &pLocalIndex );
                break;

            case MQTT_MSG_EXPIRY_ID:
                status = decodeAndDiscard_uint32( &propertyLength, &messageExpiryInterval, &pLocalIndex );
                break;

            case MQTT_CONTENT_TYPE_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &contentType, &pLocalIndex );
                break;

            case MQTT_SUBSCRIPTION_ID_ID:
                status = decodeVariableLength( pLocalIndex, propertyLength, &subscriptionId );

                if( status == MQTTSuccess )
                {
                    pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( subscriptionId ) ];
                    propertyLength -= variableLengthEncodedSize( subscriptionId );
                }

                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    if( ( status == MQTTSuccess ) && ( topicAlias == true ) )
    {
        if( topicAliasMax < topicAliasVal )
        {
            status = MQTTBadResponse;
            LogError( ( "Topic Alias greater than Topic Alias Max. " ) );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo,
                                        MQTTPropBuilder_t * pPropBuffer,
                                        uint16_t topicAliasMax )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pVariableHeader = NULL;
    const uint8_t * pPacketIdentifierHigh = NULL;
    uint8_t * pIndex = NULL;

    assert( pIncomingPacket != NULL );
    assert( pPacketId != NULL );
    assert( pPublishInfo != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );

    pVariableHeader = pIncomingPacket->pRemainingData;
    pIndex = pVariableHeader;
    /* The flags are the lower 4 bits of the first byte in PUBLISH. */
    status = processPublishFlags( ( pIncomingPacket->type & 0x0FU ), pPublishInfo );

    if( status == MQTTSuccess )
    {
        /* Sanity checks for "Remaining length". A QoS 0 PUBLISH  must have a remaining
         * length of at least 3 to accommodate topic name length (2 bytes) and topic
         * name (at least 1 byte). A QoS 1 or 2 PUBLISH must have a remaining length of
         * at least 5 for the packet identifier in addition to the topic name length and
         * topic name. */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
                                              pPublishInfo->qos,
                                              4U );
    }

    if( status == MQTTSuccess )
    {
        /* Extract the topic name starting from the first byte of the variable header.
         * The topic name string starts at byte 3 in the variable header. */
        pPublishInfo->topicNameLength = UINT16_DECODE( pVariableHeader );
        pIndex = &pIndex[ sizeof( uint16_t ) ];

        /* Sanity checks for topic name length and "Remaining length". The remaining
         * length must be at least as large as the variable length header. */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
                                              pPublishInfo->qos,
                                              pPublishInfo->topicNameLength + sizeof( uint16_t ) + sizeof( uint8_t ) );
    }

    if( status == MQTTSuccess )
    {
        /* Parse the topic. */
        pPublishInfo->pTopicName = ( char * ) ( &pVariableHeader[ sizeof( uint16_t ) ] );
        LogDebug( ( "Topic name length: %hu.", ( unsigned short ) pPublishInfo->topicNameLength ) );

        /* Extract the packet identifier for QoS 1 or 2 PUBLISH packets. Packet
         * identifier starts immediately after the topic name. */
        pPacketIdentifierHigh = ( const uint8_t * ) ( &pPublishInfo->pTopicName[ pPublishInfo->topicNameLength ] );
        pIndex = &pIndex[ pPublishInfo->topicNameLength ];

        if( pPublishInfo->qos > MQTTQoS0 )
        {
            *pPacketId = UINT16_DECODE( pPacketIdentifierHigh );

            LogDebug( ( "Packet identifier %hu.",
                        ( unsigned short ) *pPacketId ) );

            /* Packet identifier cannot be 0. */
            if( *pPacketId == 0U )
            {
                LogError( ( "Packet identifier cannot be 0." ) );
                status = MQTTBadResponse;
            }

            if( status == MQTTSuccess )
            {
                pIndex = &pIndex[ sizeof( uint16_t ) ];
            }
        }
    }

    if( status == MQTTSuccess )
    {
        status = deserializePublishProperties( pPublishInfo, pPropBuffer, pIndex,
                                               topicAliasMax, pIncomingPacket->remainingLength );
    }

    if( status == MQTTSuccess )
    {
        pIndex = &pIndex[ variableLengthEncodedSize( pPublishInfo->propertyLength ) ];
        pIndex = &pIndex[ pPublishInfo->propertyLength ];

        /* Calculate the length of the payload. QoS 1 or 2 PUBLISH packets contain
         * a packet identifier, but QoS 0 PUBLISH packets do not. */
        pPublishInfo->payloadLength = pIncomingPacket->remainingLength - pPublishInfo->topicNameLength -
                                      sizeof( uint16_t ) - pPublishInfo->propertyLength -
                                      variableLengthEncodedSize( pPublishInfo->propertyLength );

        if( pPublishInfo->qos != MQTTQoS0 )
        {
            /* Two more bytes for the packet identifier. */
            pPublishInfo->payloadLength -= sizeof( uint16_t );
        }

        /* Set payload if it exists. */

        pPublishInfo->pPayload = ( pPublishInfo->payloadLength != 0U ) ? pIndex : NULL;

        LogDebug( ( "Payload length %lu.",
                    ( unsigned long ) pPublishInfo->payloadLength ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t updateContextWithConnectProps( const MQTTPropBuilder_t * pPropBuilder,
                                            MQTTConnectionProperties_t * pConnectProperties )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropBuilder == NULL )
    {
        LogError( ( "pPropBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropBuilder->pBuffer == NULL )
    {
        LogError( ( "pPropBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pConnectProperties == NULL )
    {
        LogError( ( "pConnectProperties cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        bool maxPacket = false;
        bool sessionExpiry = false;
        bool receiveMax = false;
        bool topicAlias = false;
        size_t propertyLength = 0U;
        uint8_t * pIndex;

        propertyLength = pPropBuilder->currentIndex;
        pIndex = pPropBuilder->pBuffer; /*Pointer to the buffer*/

        while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
        {
            uint8_t propertyId = *pIndex;
            bool used = false;
            pIndex = &pIndex[ 1 ];
            propertyLength--;

            switch( propertyId )
            {
                case MQTT_SESSION_EXPIRY_ID:

                    /**
                     * This value shall get overwritten if the server sends a sessionExpiry
                     * in the CONNACK.
                     */
                    status = decodeuint32_t( &pConnectProperties->sessionExpiry, &propertyLength,
                                             &sessionExpiry, &pIndex );
                    break;

                case MQTT_RECEIVE_MAX_ID:
                    status = decodeuint16_t( &pConnectProperties->receiveMax, &propertyLength,
                                             &receiveMax, &pIndex );
                    break;

                case MQTT_MAX_PACKET_SIZE_ID:
                    status = decodeuint32_t( &pConnectProperties->maxPacketSize, &propertyLength,
                                             &maxPacket, &pIndex );
                    break;

                case MQTT_TOPIC_ALIAS_MAX_ID:
                    status = decodeuint16_t( &pConnectProperties->topicAliasMax, &propertyLength,
                                             &topicAlias, &pIndex );
                    break;

                case MQTT_REQUEST_PROBLEM_ID:
                case MQTT_REQUEST_RESPONSE_ID:
                    status = decodeAndDiscard_uint8( &propertyLength, &used, &pIndex );
                    break;

                case MQTT_AUTH_DATA_ID:
                case MQTT_AUTH_METHOD_ID:
                    status = decodeAndDiscardutf_8( &propertyLength, &used, &pIndex );
                    break;

                case MQTT_USER_PROPERTY_ID:
                    status = decodeAndDiscard( &propertyLength, &pIndex );
                    break;

                default:
                    status = MQTTBadParameter;
                    break;
            }
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidatePublishParams( const MQTTPublishInfo_t * pPublishInfo,
                                         uint8_t retainAvailable,
                                         uint8_t maxQos,
                                         uint16_t topicAlias,
                                         uint32_t maxPacketSize )
{
    MQTTStatus_t status;

    if( pPublishInfo == NULL )
    {
        LogError( ( "Argument cannot be NULL: pPublishInfo=%p ",
                    ( void * ) pPublishInfo
                    ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->retain == true ) && ( retainAvailable == 0U ) )
    {
        LogError( ( "Retain is not available." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->qos != MQTTQoS0 ) && ( maxQos == 0U ) )
    {
        LogError( ( "Qos value = %hu is not allowed by the server ",
                    ( unsigned short ) pPublishInfo->qos ) );
        status = MQTTBadParameter;
    }
    else if( ( topicAlias == 0U ) && ( pPublishInfo->topicNameLength == 0U ) )
    {
        LogError( ( "Invalid topic name for PUBLISH: pTopicName=%p, "
                    "topicNameLength=%hu.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) && ( pPublishInfo->topicNameLength != 0U ) )
    {
        LogError( ( "Invalid topic name for PUBLISH: pTopicName=%p, "
                    "topicNameLength=%hu.",
                    ( void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        status = MQTTBadParameter;
    }
    else
    {
        status = MQTTSuccess;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidatePublishProperties( uint16_t serverTopicAliasMax,
                                             const MQTTPropBuilder_t * propBuilder,
                                             uint16_t * topicAlias )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pLocalIndex = NULL;
    bool topicAliasBool = false;

    if( ( propBuilder == NULL ) || ( propBuilder->pBuffer == NULL ) )
    {
        LogError( ( "Property Builder is NULL. " ) );
        status = MQTTBadParameter;
    }
    else if( topicAlias == NULL )
    {
        LogError( ( "Topic Alias is NULL" ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = propBuilder->currentIndex;
        pLocalIndex = propBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        bool used = false;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_PAYLOAD_FORMAT_ID:
                status = decodeAndDiscard_uint8( &propertyLength, &used, &pLocalIndex );
                break;

            case MQTT_MSG_EXPIRY_ID:
                status = decodeAndDiscard_uint32( &propertyLength, &used, &pLocalIndex );
                break;

            case MQTT_CONTENT_TYPE_ID:
            case MQTT_CORRELATION_DATA_ID:
            case MQTT_RESPONSE_TOPIC_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &used, &pLocalIndex );
                break;

            case MQTT_TOPIC_ALIAS_ID:
                status = decodeuint16_t( topicAlias, &propertyLength, &topicAliasBool, &pLocalIndex );

                if( ( status == MQTTSuccess ) && ( serverTopicAliasMax < *topicAlias ) )
                {
                    LogError( ( "Protocol Error: Topic Alias greater than Topic Alias Max" ) );
                    status = MQTTBadParameter;
                }

                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateSubscribeProperties( uint8_t isSubscriptionIdAvailable,
                                               const MQTTPropBuilder_t * propBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pLocalIndex = NULL;
    size_t subscriptionId = 0;

    if( ( propBuilder == NULL ) || ( propBuilder->pBuffer == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = propBuilder->currentIndex;
        pLocalIndex = propBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_SUBSCRIPTION_ID_ID:

                status = decodeVariableLength( pLocalIndex, propertyLength, &subscriptionId );

                if( status == MQTTSuccess )
                {
                    propertyLength -= variableLengthEncodedSize( subscriptionId );

                    if( isSubscriptionIdAvailable == 0U )
                    {
                        LogError( ( "Protocol Error : Subscription Id not available" ) );
                        status = MQTTBadParameter;
                    }
                }

                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSubackProperties( MQTTPropBuilder_t * pPropBuffer,
                                                 uint8_t * pIndex,
                                                 size_t * pSubackPropertyLength,
                                                 size_t remainingLength )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pLocalIndex = pIndex;
    const char * pReasonString;
    uint16_t reasonStringLength;
    bool reasonString = false;

    status = decodeVariableLength( pLocalIndex, remainingLength - 2U, &propertyLength );
    *pSubackPropertyLength = propertyLength;

    if( status == MQTTSuccess )
    {
        pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( propertyLength ) ];
    }

    if( pPropBuffer != NULL )
    {
        pPropBuffer->bufferLength = propertyLength;
        pPropBuffer->pBuffer = pLocalIndex;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeutf_8( &pReasonString, &reasonStringLength, &propertyLength,
                                      &reasonString, &pLocalIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId,
                                  bool * pSessionPresent,
                                  MQTTReasonCodeInfo_t * pReasonCode,
                                  MQTTPropBuilder_t * pPropBuffer,
                                  MQTTConnectionProperties_t * pConnectProperties )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t maxPacketSize;

    if( pConnectProperties != NULL )
    {
        maxPacketSize = pConnectProperties->maxPacketSize;
    }

    if( pIncomingPacket == NULL )
    {
        LogError( ( "pIncomingPacket cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pConnectProperties == NULL )
    {
        LogError( ( "pConnectProperties cannot be NULL." ) );
        status = MQTTBadParameter;
    }

    /* Pointer for packet identifier cannot be NULL for packets other than
     * CONNACK and PINGRESP. */
    else if( ( pPacketId == NULL ) &&
             ( ( pIncomingPacket->type != MQTT_PACKET_TYPE_CONNACK ) &&
               ( pIncomingPacket->type != MQTT_PACKET_TYPE_PINGRESP ) ) )
    {
        LogError( ( "pPacketId cannot be NULL for packet type %02x.",
                    ( unsigned int ) pIncomingPacket->type ) );
        status = MQTTBadParameter;
    }
    /* Pointer for session present cannot be NULL for CONNACK. */
    else if( ( pSessionPresent == NULL ) &&
             ( pIncomingPacket->type == MQTT_PACKET_TYPE_CONNACK ) )
    {
        LogError( ( "pSessionPresent cannot be NULL for CONNACK packet." ) );
        status = MQTTBadParameter;
    }

    /* Pointer for remaining data cannot be NULL for packets other
     * than PINGRESP. */
    else if( ( pIncomingPacket->pRemainingData == NULL ) &&
             ( pIncomingPacket->type != MQTT_PACKET_TYPE_PINGRESP ) )
    {
        LogError( ( "Remaining data of incoming packet is NULL." ) );
        status = MQTTBadParameter;
    }
    /*Max packet size cannot be 0.*/
    else if( maxPacketSize == 0U )
    {
        LogError( ( "Max packet size cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->remainingLength +
               variableLengthEncodedSize( pIncomingPacket->remainingLength ) +
               1U ) > maxPacketSize )
    {
        LogError( ( "Packet Size cannot be greater than max packet size. " ) );
        status = MQTTBadResponse;
    }
    else
    {
        /* Make sure response packet is a valid ack. */
        switch( pIncomingPacket->type )
        {
            case MQTT_PACKET_TYPE_CONNACK:
                status = deserializeConnack( pConnectProperties,
                                             pIncomingPacket,
                                             pSessionPresent,
                                             pPropBuffer );
                break;

            case MQTT_PACKET_TYPE_PUBACK:
            case MQTT_PACKET_TYPE_PUBREC:
                status = deserializeSimpleAck( pIncomingPacket,
                                               pPacketId,
                                               pReasonCode,
                                               pConnectProperties->requestProblemInfo,
                                               pPropBuffer );

                if( ( status == MQTTSuccess ) && ( pIncomingPacket->remainingLength > 2U ) )
                {
                    status = logAckResponse( *pReasonCode->reasonCode, *pPacketId );
                }

                break;

            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                status = deserializeSimpleAck( pIncomingPacket,
                                               pPacketId,
                                               pReasonCode,
                                               pConnectProperties->requestProblemInfo,
                                               pPropBuffer );

                if( ( status == MQTTSuccess ) && ( pIncomingPacket->remainingLength > 2U ) )
                {
                    status = logSimpleAckResponse( *pReasonCode->reasonCode, *pPacketId );
                }

                break;

            case MQTT_PACKET_TYPE_SUBACK:
            case MQTT_PACKET_TYPE_UNSUBACK:
                status = deserializeSuback( pIncomingPacket, pPacketId, pReasonCode, pPropBuffer );
                break;

            case MQTT_PACKET_TYPE_PINGRESP:
                status = deserializePingresp( pIncomingPacket );
                break;

            /* Any other packet type is invalid. */
            default:
                LogError( ( "Function called with unknown packet type:(%02x).",
                            ( unsigned int ) pIncomingPacket->type ) );
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

uint8_t * MQTT_SerializeAckFixed( uint8_t * pIndex,
                                  uint8_t packetType,
                                  uint16_t packetId,
                                  size_t remainingLength,
                                  MQTTSuccessFailReasonCode_t reasonCode )
{
    uint8_t * pIndexLocal = pIndex;

    /* The first byte in the publish ack packet is the control packet type. */
    *pIndexLocal = packetType;
    pIndexLocal++;
    /*After the packet type fixed header has remaining length.*/
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );
    /*Encode the packet id.*/
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( packetId );
    pIndexLocal = &pIndexLocal[ 2 ];
    /*We are now sending the ack.*/
    *pIndexLocal = ( uint8_t ) reasonCode;
    pIndexLocal++;
    return pIndexLocal;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetAckPacketSize( size_t * pRemainingLength,
                                    size_t * pPacketSize,
                                    uint32_t maxPacketSize,
                                    size_t ackPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t length = 0U;
    size_t propertyLength = 0U;
    size_t packetSize = 0U;

    propertyLength = ackPropertyLength;

    /*Validate the parameters.*/
    if( ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        status = MQTTBadParameter;
    }
    else
    {
        length += MQTT_PUBLISH_ACK_PACKET_SIZE_WITH_REASON;

        length += variableLengthEncodedSize( propertyLength ) + propertyLength;

        if( length > MQTT_MAX_REMAINING_LENGTH )
        {
            status = MQTTBadParameter;
            LogError( ( "Remaining Length greater than Maximum Remaining Length according to MQTTv5 spec." ) );
        }
        else
        {
            *pRemainingLength = length;
        }
    }

    if( status == MQTTSuccess )
    {
        packetSize = length + 1U + variableLengthEncodedSize( length );

        if( packetSize > maxPacketSize )
        {
            status = MQTTBadParameter;
            LogError( ( "Packet size greater than Max Packet Size specified in the CONNACK" ) );
        }
        else
        {
            *pPacketSize = packetSize;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

uint8_t * MQTT_SerializeDisconnectFixed( uint8_t * pIndex,
                                         MQTTSuccessFailReasonCode_t reasonCode,
                                         size_t remainingLength )
{
    uint8_t * pIndexLocal = pIndex;

    assert( pIndex != NULL );
    /* The first byte in the publish ack packet is the control packet type. */
    *pIndexLocal = MQTT_PACKET_TYPE_DISCONNECT;
    pIndexLocal++;
    /*After the packet type fixed header has remaining length.*/
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );
    /*Encode the reason code.*/
    *pIndexLocal = ( uint8_t ) reasonCode;
    pIndexLocal++;

    return pIndexLocal;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeDisconnect( const MQTTPacketInfo_t * pPacket,
                                         uint32_t maxPacketSize,
                                         MQTTReasonCodeInfo_t * pDisconnectInfo,
                                         MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    size_t propertyLength = 0U;

    /*Validate the arguments*/
    if( ( pPacket == NULL ) || ( pPacket->pRemainingData == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else if( pDisconnectInfo == NULL )
    {
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        status = MQTTBadParameter;
    }
    /*Packet size should not be more than the max allowed by the client.*/
    else if( ( pPacket->remainingLength + variableLengthEncodedSize( pPacket->remainingLength ) + 1U ) > maxPacketSize )
    {
        status = MQTTBadResponse;
    }
    else if( pPacket->remainingLength == 0U )
    {
        /*Do nothing*/
    }
    else
    {
        /* Extract the reason code */
        pIndex = pPacket->pRemainingData;
        pDisconnectInfo->reasonCode = pIndex;
        pDisconnectInfo->reasonCodeLength = 1U;
        pIndex++;
        /*Validate the reason code.*/
        status = validateDisconnectResponse( *pDisconnectInfo->reasonCode, true );
    }

    if( status == MQTTSuccess )
    {
        if( ( pPacket->remainingLength > 1U ) )
        {
            /*Extract the property length.*/
            status = decodeVariableLength( pIndex, pPacket->remainingLength - 1U, &propertyLength );

            if( status == MQTTSuccess )
            {
                pIndex = &pIndex[ variableLengthEncodedSize( propertyLength ) ];
            }

            if( pPropBuffer != NULL )
            {
                pPropBuffer->bufferLength = propertyLength;
                pPropBuffer->pBuffer = pIndex;
            }

            /*Validate the remaining length.*/
            if( pPacket->remainingLength != ( propertyLength + variableLengthEncodedSize( propertyLength ) + 1U ) )
            {
                status = MQTTBadResponse;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        status = deserializeDisconnectProperties( pIndex, propertyLength );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeDisconnectProperties( uint8_t * pIndex,
                                                     size_t disconnectPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = pIndex;
    const char * pReasonString;
    uint16_t reasonStringLength;
    const char * pServerRef;
    uint16_t pServerRefLength;
    size_t propertyLength = disconnectPropertyLength;

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        /*Decode the property id.*/
        uint8_t propertyId = *pLocalIndex;
        bool reasonString = false;
        bool serverRef = false;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        /*Validate the property id and decode accordingly.*/
        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeutf_8( &pReasonString, &reasonStringLength, &propertyLength, &reasonString, &pLocalIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                break;

            case MQTT_SERVER_REF_ID:
                status = decodeutf_8( &pServerRef, &pServerRefLength, &propertyLength, &serverRef, &pLocalIndex );
                break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}


/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_SubscribeId( MQTTPropBuilder_t * pPropertyBuilder,
                                      size_t subscriptionId )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;

    if( subscriptionId == 0U )
    {
        LogError( ( "Subscription Id cannot 0 for subscribe properties : Protocol Error " ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_SUBSCRIPTION_ID_POS ) )
    {
        LogError( ( "Subscription Id already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) +
               variableLengthEncodedSize( subscriptionId ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add subscription id" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_SUBSCRIPTION_ID_ID;
        pIndex++;
        pIndex = encodeVariableLength( pIndex, subscriptionId );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_SUBSCRIPTION_ID_POS );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_UserProp( MQTTPropBuilder_t * pPropertyBuilder,
                                   const MQTTUserProperty_t * userProperty )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( userProperty == NULL )
    {
        LogError( ( "Arguments cannot be NULL : userProperty=%p.", ( void * ) userProperty ) );
        status = MQTTBadParameter;
    }
    else if( ( userProperty->pKey == NULL ) ||
             ( userProperty->pValue == NULL ) ||
             ( userProperty->keyLength == 0U ) ||
             ( userProperty->valueLength == 0U ) )
    {
        LogError( ( "Arguments cannot be NULL : pUserProperties->userProperty->pKey=%p, pUserProperties->userProperty->pValue=%p, Key Length = %u, Value Length = %u",
                    ( void * ) userProperty->pKey,
                    ( void * ) userProperty->pValue,
                    userProperty->keyLength,
                    userProperty->valueLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + userProperty->keyLength +
               userProperty->valueLength + sizeof( uint16_t ) + sizeof( uint16_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property." ) );
        status = MQTTNoMemory;
    }
    else
    {
        const uint8_t * start = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        uint8_t * pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];

        *pIndex = MQTT_USER_PROPERTY_ID;
        pIndex++;

        /*Encoding key*/
        pIndex = encodeString( pIndex, userProperty->pKey, userProperty->keyLength );
        pIndex = encodeString( pIndex, userProperty->pValue, userProperty->valueLength );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - start );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_UpdateDuplicatePublishFlag( uint8_t * pHeader,
                                              bool set )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pHeader == NULL )
    {
        LogError( ( "Header cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( ( ( *pHeader ) & 0xF0U ) != MQTT_PACKET_TYPE_PUBLISH )
    {
        LogError( ( "Header is not publish packet header" ) );
        status = MQTTBadParameter;
    }
    else if( set == true )
    {
        UINT8_SET_BIT( *pHeader, MQTT_PUBLISH_FLAG_DUP );
    }
    else
    {
        UINT8_CLEAR_BIT( *pHeader, MQTT_PUBLISH_FLAG_DUP );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_SessionExpiry( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t sessionExpiry )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_SESSION_EXPIRY_INTERVAL_POS ) )
    {
        LogError( ( "Connect Session Expiry Already Set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint32_t ) + sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_SESSION_EXPIRY_ID;
        pIndex++;
        WRITE_UINT32( &( pIndex[ 0 ] ), sessionExpiry );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_SESSION_EXPIRY_INTERVAL_POS );
        pPropertyBuilder->currentIndex += 5U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnReceiveMax( MQTTPropBuilder_t * pPropertyBuilder,
                                         uint16_t receiveMax )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( receiveMax == 0U ) || ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_RECEIVE_MAXIMUM_POS ) ) )
    {
        LogError( ( "Invalid arguments passed to MQTTPropAdd_ConnReceiveMax." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint16_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        uint8_t * pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_RECEIVE_MAX_ID;
        pIndex++;
        pIndex[ 0 ] = UINT16_HIGH_BYTE( receiveMax );
        pIndex[ 1 ] = UINT16_LOW_BYTE( receiveMax );
        pIndex = &pIndex[ 2 ];
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_RECEIVE_MAXIMUM_POS );
        pPropertyBuilder->currentIndex += 3U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnMaxPacketSize( MQTTPropBuilder_t * pPropertyBuilder,
                                            uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( maxPacketSize == 0U ) || ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_MAX_PACKET_SIZE_POS ) ) )
    {
        LogError( ( "Max packet size already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint32_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_MAX_PACKET_SIZE_ID;
        pIndex++;
        WRITE_UINT32( &( pIndex[ 0 ] ), maxPacketSize );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_MAX_PACKET_SIZE_POS );
        pPropertyBuilder->currentIndex += 5U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnTopicAliasMax( MQTTPropBuilder_t * pPropertyBuilder,
                                            uint16_t topicAliasMax )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_MAX_POS ) )
    {
        LogError( ( "Topic Alias Maximum already set. " ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint16_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_TOPIC_ALIAS_MAX_ID;
        pIndex++;
        pIndex[ 0 ] = UINT16_HIGH_BYTE( topicAliasMax );
        pIndex[ 1 ] = UINT16_LOW_BYTE( topicAliasMax );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_MAX_POS );
        pPropertyBuilder->currentIndex += 3U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnRequestRespInfo( MQTTPropBuilder_t * pPropertyBuilder,
                                              bool requestResponseInfo )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_REQUEST_RESPONSE_INFO_POS ) )
    {
        LogError( ( "Request Response Info already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_REQUEST_RESPONSE_ID;
        pIndex++;
        *pIndex = ( requestResponseInfo ? 1U : 0U );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_REQUEST_RESPONSE_INFO_POS );
        pPropertyBuilder->currentIndex += 2U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnRequestProbInfo( MQTTPropBuilder_t * pPropertyBuilder,
                                              bool requestProblemInfo )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_REQUEST_PROBLEM_INFO_POS ) )
    {
        LogError( ( "Request Problem Info already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_REQUEST_RESPONSE_ID;
        pIndex++;
        *pIndex = ( requestProblemInfo ? 1U : 0U );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_REQUEST_PROBLEM_INFO_POS );
        pPropertyBuilder->currentIndex += 2U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnAuthMethod( MQTTPropBuilder_t * pPropertyBuilder,
                                         const char * authMethod,
                                         uint16_t authMethodLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPropertyBuilder == NULL ) || ( authMethod == NULL ) || ( authMethodLength == 0U ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p, authMethod = %p, authMethodLength = %u", ( void * ) pPropertyBuilder, ( void * ) authMethod, authMethodLength ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS ) )
    {
        LogError( ( "Auth Method already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + authMethodLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_AUTH_METHOD_ID;
        pIndex++;
        pIndex = encodeString( pIndex, authMethod, authMethodLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ConnAuthData( MQTTPropBuilder_t * pPropertyBuilder,
                                       const char * authData,
                                       uint16_t authDataLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPropertyBuilder == NULL ) || ( authData == NULL ) || ( authDataLength == 0U ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p, authMethod = %p, authMethodLength = %u",
                    ( void * ) pPropertyBuilder, ( void * ) authData, authDataLength ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + authDataLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else if( ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_DATA_POS ) ) ||
             ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS ) == false ) )
    {
        LogError( ( "Invalid Auth data" ) );
        status = MQTTBadParameter;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_AUTH_DATA_ID;
        pIndex++;
        pIndex = encodeString( pIndex, authData, authDataLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_DATA_POS );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PubPayloadFormat( MQTTPropBuilder_t * pPropertyBuilder,
                                           bool payloadFormat )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_PAYLOAD_FORMAT_INDICATOR_POS ) ) )
    {
        LogError( ( "Payload Format already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_PAYLOAD_FORMAT_ID;
        pIndex++;
        *pIndex = ( uint8_t ) ( payloadFormat ? 1U : 0U );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_PAYLOAD_FORMAT_INDICATOR_POS );
        pPropertyBuilder->currentIndex += 2U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PubMessageExpiry( MQTTPropBuilder_t * pPropertyBuilder,
                                           uint32_t messageExpiry )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_MESSAGE_EXPIRY_INTERVAL_POS ) ) )
    {
        LogError( ( "Message Expiry Interval already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint32_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_MSG_EXPIRY_ID;
        pIndex++;
        WRITE_UINT32( &( pIndex[ 0 ] ), messageExpiry );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_MESSAGE_EXPIRY_INTERVAL_POS );
        pPropertyBuilder->currentIndex += 5U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_WillDelayInterval( MQTTPropBuilder_t * pPropertyBuilder,
                                            uint32_t willDelayInterval )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPropertyBuilder == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_WILL_DELAY_POS ) ) )
    {
        LogError( ( "Message Expiry Interval already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint32_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_WILL_DELAY_ID;
        pIndex++;
        WRITE_UINT32( &( pIndex[ 0 ] ), willDelayInterval );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_WILL_DELAY_POS );
        pPropertyBuilder->currentIndex += 5U;
    }

    return status;
}

MQTTStatus_t MQTTPropAdd_PubTopicAlias( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint16_t topicAlias )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_POS ) )
    {
        LogError( ( "TopicAlias already present" ) );
        status = MQTTBadParameter;
    }
    else if( topicAlias == 0U )
    {
        LogError( ( "Topic Alias cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint16_t ) +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_TOPIC_ALIAS_ID;
        pIndex++;
        pIndex[ 0 ] = UINT16_HIGH_BYTE( topicAlias );
        pIndex[ 1 ] = UINT16_LOW_BYTE( topicAlias );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_POS );
        pPropertyBuilder->currentIndex += 3U;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PubResponseTopic( MQTTPropBuilder_t * pPropertyBuilder,
                                           const char * responseTopic,
                                           uint16_t responseTopicLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( responseTopic == NULL )
    {
        LogError( ( "Arguments cannot be NULL : responseTopic=%p.", ( void * ) responseTopic ) );
        status = MQTTBadParameter;
    }
    else if( responseTopicLength == 0U )
    {
        LogError( ( "Response Topic Length cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( ( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_RESPONSE_TOPIC_POS ) ) )
    {
        LogError( ( "Response Topic already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( strchr( responseTopic, ( int32_t ) '#' ) != NULL ) ||
             ( strchr( responseTopic, ( int32_t ) '+' ) != NULL ) )
    {
        LogError( ( "Protocol Error : Response Topic contains wildcards" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + responseTopicLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_RESPONSE_TOPIC_ID;
        pIndex++;
        pIndex = encodeString( pIndex, responseTopic, responseTopicLength );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_RESPONSE_TOPIC_POS );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PubCorrelationData( MQTTPropBuilder_t * pPropertyBuilder,
                                             const void * pCorrelationData,
                                             uint16_t correlationLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.",
                    ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( pCorrelationData == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pCorrelationData=%p.", ( void * ) pCorrelationData ) );
        status = MQTTBadParameter;
    }
    else if( correlationLength == 0U )
    {
        LogError( ( "Correlation Data Length cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_CORRELATION_DATA_POS ) )
    {
        LogError( ( "Correlation Data already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + correlationLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_CORRELATION_DATA_ID;
        pIndex++;
        pIndex = encodeBinaryData( pIndex, pCorrelationData, correlationLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_CORRELATION_DATA_POS );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PubContentType( MQTTPropBuilder_t * pPropertyBuilder,
                                         const char * contentType,
                                         uint16_t contentTypeLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( contentType == NULL )
    {
        LogError( ( "Arguments cannot be NULL : contentType=%p.", ( void * ) contentType ) );
        status = MQTTBadParameter;
    }
    else if( contentTypeLength == 0U )
    {
        LogError( ( "Content Type Length cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_CONTENT_TYPE_POS ) )
    {
        LogError( ( "Content type already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + contentTypeLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_CONTENT_TYPE_ID;
        pIndex++;
        pIndex = encodeString( pIndex, contentType, contentTypeLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_CONTENT_TYPE_POS );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ReasonString( MQTTPropBuilder_t * pPropertyBuilder,
                                       const char * pReasonString,
                                       uint16_t reasonStringLength )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) pPropertyBuilder->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( pReasonString == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pReasonString=%p.", ( void * ) pReasonString ) );
        status = MQTTBadParameter;
    }
    else if( reasonStringLength == 0U )
    {
        LogError( ( "Reason String Length cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_REASON_STRING_POS ) )
    {
        LogError( ( "Reason String already set" ) );
        status = MQTTBadParameter;
    }
    else if( ( pPropertyBuilder->currentIndex + sizeof( uint8_t ) + reasonStringLength +
               sizeof( uint16_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_REASON_STRING_ID;
        pIndex++;
        pIndex = encodeString( pIndex, pReasonString, reasonStringLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_REASON_STRING_POS );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubTopicAlias( MQTTPropBuilder_t * propBuffer,
                                        uint16_t * topicAlias )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( topicAlias == NULL )
    {
        LogError( ( "Arguments cannot be NULL : topicAlias=%p.", ( void * ) topicAlias ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint16_t( topicAlias, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubPayloadFormatIndicator( MQTTPropBuilder_t * propBuffer,
                                                    uint8_t * payloadFormat )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( payloadFormat == NULL )
    {
        LogError( ( "Arguments cannot be NULL : payloadFormat=%p.", ( void * ) payloadFormat ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( payloadFormat, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubResponseTopic( MQTTPropBuilder_t * propBuffer,
                                           const char ** responseTopic,
                                           uint16_t * responseTopicLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( responseTopic == NULL ) || ( responseTopicLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : responseTopic=%p, responseTopicLength = %p", ( void * ) responseTopic, ( void * ) responseTopicLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( responseTopic, responseTopicLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubCorrelationData( MQTTPropBuilder_t * propBuffer,
                                             const void ** correlationData,
                                             uint16_t * correlationLength )
{
    MQTTStatus_t status = MQTTSuccess;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( correlationData == NULL ) || ( correlationLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : responseTopic=%p, responseTopicLength = %p", ( void * ) correlationData, ( void * ) correlationLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeBinaryData( correlationData, correlationLength, &propertyLength, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubMessageExpiryInterval( MQTTPropBuilder_t * propBuffer,
                                                   uint32_t * msgExpiryInterval )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( msgExpiryInterval == NULL )
    {
        LogError( ( "Arguments cannot be NULL : msgExpiryInterval=%p.", ( void * ) msgExpiryInterval ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint32_t( msgExpiryInterval, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubContentType( MQTTPropBuilder_t * propBuffer,
                                         const char ** pContentType,
                                         uint16_t * contentTypeLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pContentType == NULL ) || ( contentTypeLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pContentType=%p, contentTypeLength = %p",
                    ( void * ) pContentType, ( void * ) contentTypeLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pContentType, contentTypeLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PubSubscriptionId( MQTTPropBuilder_t * propBuffer,
                                            size_t * subscriptionId )
{
    MQTTStatus_t status = MQTTSuccess;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionId == NULL )
    {
        LogError( ( "Arguments cannot be NULL : subscriptionId=%p.", ( void * ) subscriptionId ) );
        status = MQTTBadParameter;
    }
    else
    {
        const uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        status = decodeVariableLength( startOfProp,
                                       propBuffer->bufferLength - propBuffer->currentIndex,
                                       subscriptionId );

        if( status == MQTTSuccess )
        {
            startOfProp = &startOfProp[ variableLengthEncodedSize( *subscriptionId ) ];

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_SessionExpiry( MQTTPropBuilder_t * propBuffer,
                                        uint32_t * sessionExpiry )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( sessionExpiry == NULL )
    {
        LogError( ( "Arguments cannot be NULL : sessionExpiry=%p.", ( void * ) sessionExpiry ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint32_t( sessionExpiry, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnTopicAliasMax( MQTTPropBuilder_t * propBuffer,
                                            uint16_t * topicAliasMax )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( topicAliasMax == NULL )
    {
        LogError( ( "Arguments cannot be NULL : topicAliasMax=%p.", ( void * ) topicAliasMax ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint16_t( topicAliasMax, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnReceiveMax( MQTTPropBuilder_t * propBuffer,
                                         uint16_t * receiveMax )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( receiveMax == NULL )
    {
        LogError( ( "Arguments cannot be NULL : receiveMax=%p.", ( void * ) receiveMax ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint16_t( receiveMax, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnMaxQos( MQTTPropBuilder_t * propBuffer,
                                     uint8_t * maxQos )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( maxQos == NULL )
    {
        LogError( ( "Arguments cannot be NULL : maxQos=%p.", ( void * ) maxQos ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( maxQos, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnRetainAvailable( MQTTPropBuilder_t * propBuffer,
                                              uint8_t * retainAvailable )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( retainAvailable == NULL )
    {
        LogError( ( "Arguments cannot be NULL : retainAvailable=%p.", ( void * ) retainAvailable ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( retainAvailable, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnMaxPacketSize( MQTTPropBuilder_t * propBuffer,
                                            uint32_t * maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == NULL )
    {
        LogError( ( "Arguments cannot be NULL : maxPacketSize=%p.", ( void * ) maxPacketSize ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint32_t( maxPacketSize, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnClientId( MQTTPropBuilder_t * propBuffer,
                                       const char ** pClientId,
                                       uint16_t * clientIdLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pClientId == NULL ) || ( clientIdLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pClientId=%p, clientIdLength = %p", ( void * ) pClientId, ( void * ) clientIdLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pClientId, clientIdLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnWildcard( MQTTPropBuilder_t * propBuffer,
                                       uint8_t * isWildCardAvailable )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( isWildCardAvailable == NULL )
    {
        LogError( ( "Arguments cannot be NULL : isWildCardAvailable=%p.", ( void * ) isWildCardAvailable ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( isWildCardAvailable, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnSubId( MQTTPropBuilder_t * propBuffer,
                                    uint8_t * isSubIdAvailable )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( isSubIdAvailable == NULL )
    {
        LogError( ( "Arguments cannot be NULL : isSubIdAvailable=%p.", ( void * ) isSubIdAvailable ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( isSubIdAvailable, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_UserProp( MQTTPropBuilder_t * propBuffer,
                                   const char ** pUserPropKey,
                                   uint16_t * pUserPropKeyLen,
                                   const char ** pUserPropVal,
                                   uint16_t * pUserPropValLen )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pUserPropKey == NULL ) ||
             ( pUserPropKeyLen == NULL ) ||
             ( pUserPropVal == NULL ) ||
             ( pUserPropValLen == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pUserPropKey = %p , pUserPropKeyLen = %p , pUserPropVal = %p , pUserPropValLen = %p",
                    ( void * ) pUserPropKey,
                    ( void * ) pUserPropKeyLen,
                    ( void * ) pUserPropVal,
                    ( void * ) pUserPropValLen ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pUserPropKey, pUserPropKeyLen, &propertyLength, &propFlag, &startOfProp );
        propFlag = false;

        if( status == MQTTSuccess )
        {
            status = decodeutf_8( pUserPropVal, pUserPropValLen, &propertyLength, &propFlag, &startOfProp );
        }

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ReasonString( MQTTPropBuilder_t * propBuffer,
                                       const char ** pReasonString,
                                       uint16_t * reasonStringLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pReasonString == NULL ) || ( reasonStringLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pReasonString=%p, reasonStringLength = %p",
                    ( void * ) pReasonString, ( void * ) reasonStringLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pReasonString, reasonStringLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ServerRef( MQTTPropBuilder_t * propBuffer,
                                    const char ** pServerRef,
                                    uint16_t * serverRefLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pServerRef == NULL ) || ( serverRefLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pServerRef=%p, reasonStringLength = %p",
                    ( void * ) pServerRef, ( void * ) serverRefLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pServerRef, serverRefLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnSharedSubAvailable( MQTTPropBuilder_t * propBuffer,
                                                 uint8_t * isSharedSubAvailable )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( ( propBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( isSharedSubAvailable == NULL )
    {
        LogError( ( "Arguments cannot be NULL : isSharedSubAvailable= %p", ( void * ) isSharedSubAvailable ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint8_t( isSharedSubAvailable, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnServerKeepAlive( MQTTPropBuilder_t * propBuffer,
                                              uint16_t * serverKeepAlive )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( ( propBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( serverKeepAlive == NULL )
    {
        LogError( ( "Arguments cannot be NULL : serverKeepAlive=%p.", ( void * ) serverKeepAlive ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeuint16_t( serverKeepAlive, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnResponseInfo( MQTTPropBuilder_t * propBuffer,
                                           const char ** pResponseInfo,
                                           uint16_t * responseInfoLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( ( propBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pResponseInfo == NULL ) || ( responseInfoLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pResponseInfo=%p, responseInfoLength = %p",
                    ( void * ) pResponseInfo, ( void * ) responseInfoLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pResponseInfo, responseInfoLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnAuthMethod( MQTTPropBuilder_t * propBuffer,
                                         const char ** pAuthMethod,
                                         uint16_t * authMethodLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( ( propBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pAuthMethod == NULL ) || ( authMethodLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pAuthMethod=%p, authMethodLength = %p",
                    ( void * ) pAuthMethod, ( void * ) authMethodLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pAuthMethod, authMethodLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ConnAuthData( MQTTPropBuilder_t * propBuffer,
                                       const char ** pAuthData,
                                       uint16_t * authDataLength )
{
    MQTTStatus_t status = MQTTSuccess;
    bool propFlag = false;

    if( ( propBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( ( pAuthData == NULL ) || ( authDataLength == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pAuthData=%p, authDataLength = %p",
                    ( void * ) pAuthData, ( void * ) authDataLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * startOfProp = &propBuffer->pBuffer[ propBuffer->currentIndex ];
        size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
        status = decodeutf_8( pAuthData, authDataLength, &propertyLength, &propFlag, &startOfProp );

        if( status == MQTTSuccess )
        {
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-182 */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_18_2_violation] */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            propBuffer->currentIndex = ( size_t ) ( startOfProp - propBuffer->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_IncomingGetNextProp( MQTTPropBuilder_t * propBuffer,
                                       uint8_t * propertyId )
{
    MQTTStatus_t status = MQTTSuccess;

    if( propBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) propBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propBuffer->pBuffer == NULL )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", ( void * ) propBuffer->pBuffer ) );
        status = MQTTBadParameter;
    }
    else if( propertyId == NULL )
    {
        LogError( ( "Arguments cannot be NULL : propertyId=%p.", ( void * ) propertyId ) );
        status = MQTTBadParameter;
    }
    else
    {
        if( propBuffer->currentIndex < propBuffer->bufferLength )
        {
            *propertyId = propBuffer->pBuffer[ propBuffer->currentIndex ];
            propBuffer->currentIndex += 1U;
        }
        else
        {
            status = MQTTEndOfProperties;
            LogError( ( "End of Property Buffer." ) );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateDisconnectProperties( uint32_t connectSessionExpiry,
                                                const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pIndex = NULL;
    uint32_t sessionExpiry;

    if( ( pPropertyBuilder == NULL ) || ( pPropertyBuilder->pBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        pIndex = &pIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_SESSION_EXPIRY_ID:
                status = decodeuint32_t( &sessionExpiry, &propertyLength, &used, &pIndex );

                if( status == MQTTSuccess )
                {
                    if( ( connectSessionExpiry == 0U ) && ( sessionExpiry != 0U ) )
                    {
                        status = MQTTBadParameter;
                        LogError( ( "Disconnect Session Expiry non-zero while Connect Session Expiry was zero. " ) );
                    }
                }

                break;

            case MQTT_REASON_STRING_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &used, &pIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateUnsubscribeProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pIndex = NULL;


    if( ( pPropertyBuilder != NULL ) && ( pPropertyBuilder->pBuffer != NULL ) )
    {
        propertyLength = pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        pIndex = &pIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateWillProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pIndex = NULL;

    if( ( pPropertyBuilder == NULL ) || ( pPropertyBuilder->pBuffer == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        pIndex = &pIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_WILL_DELAY_ID:
                status = decodeAndDiscard_uint32( &propertyLength, &used, &pIndex );
                break;

            case MQTT_PAYLOAD_FORMAT_ID:
                status = decodeAndDiscard_uint8( &propertyLength, &used, &pIndex );
                break;

            case MQTT_MSG_EXPIRY_ID:
                status = decodeAndDiscard_uint32( &propertyLength, &used, &pIndex );
                break;

            case MQTT_CONTENT_TYPE_ID:
            case MQTT_RESPONSE_TOPIC_ID:
            case MQTT_CORRELATION_DATA_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &used, &pIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidatePublishAckProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    uint8_t * pIndex = NULL;

    if( ( pPropertyBuilder != NULL ) && ( pPropertyBuilder->pBuffer != NULL ) )
    {
        propertyLength = pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        pIndex = &pIndex[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeAndDiscardutf_8( &propertyLength, &used, &pIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard( &propertyLength, &pIndex );
                break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropertyBuilder_Init( MQTTPropBuilder_t * pPropertyBuilder,
                                       uint8_t * buffer,
                                       size_t length )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPropertyBuilder == NULL ) || ( buffer == NULL ) || ( length == 0U ) )
    {
        LogError( ( "Invalid arguments passed to MQTTPropertyBuilder_Init." ) );
        status = MQTTBadParameter;
    }

    if( status == MQTTSuccess )
    {
        pPropertyBuilder->pBuffer = buffer;
        pPropertyBuilder->currentIndex = 0;
        pPropertyBuilder->bufferLength = length;
        pPropertyBuilder->fieldSet = 0; /* 0 means no field is set. */
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeSubackPropertyLength( const uint8_t * pIndex,
                                         size_t remainingLength,
                                         size_t * subackPropertyLength )
{
    MQTTStatus_t status;
    const uint8_t * pLocalIndex = pIndex;
    size_t propertyLength = 0U;

    status = decodeVariableLength( pLocalIndex, remainingLength - sizeof( uint16_t ), &propertyLength );

    if( status == MQTTSuccess )
    {
        *subackPropertyLength = ( propertyLength + variableLengthEncodedSize( propertyLength ) );

        if( *subackPropertyLength > ( remainingLength - sizeof( uint16_t ) ) )
        {
            status = MQTTBadResponse;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_InitConnect( MQTTConnectionProperties_t * pConnectProperties )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pConnectProperties == NULL )
    {
        LogError( ( "pConnectProperties cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        pConnectProperties->receiveMax = UINT16_MAX;
        pConnectProperties->maxPacketSize = MQTT_MAX_PACKET_SIZE;
        pConnectProperties->requestProblemInfo = true;
        pConnectProperties->serverReceiveMax = UINT16_MAX;
        pConnectProperties->serverMaxQos = 2U;
        pConnectProperties->serverMaxPacketSize = MQTT_MAX_PACKET_SIZE;
        pConnectProperties->isWildcardAvailable = 1U;
        pConnectProperties->isSubscriptionIdAvailable = 1U;
        pConnectProperties->isSharedAvailable = 1U;
        pConnectProperties->sessionExpiry = 0U;
        pConnectProperties->topicAliasMax = 0U;
        pConnectProperties->requestResponseInfo = false;
        pConnectProperties->retainAvailable = 1U;
        pConnectProperties->serverTopicAliasMax = 0U;
        pConnectProperties->serverKeepAlive = UINT16_MAX;
    }

    return status;
}

/*-----------------------------------------------------------*/
