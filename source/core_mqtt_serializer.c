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


/**
 * @brief MQTT protocol version 3.1.1.
 */
#define MQTT_VERSION_3_1_1                          ( ( uint8_t ) 4U )

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
 * @brief Size of the fixed header of a PUBLISH packet.
 */
/**
 * @brief The size of MQTT DISCONNECT packets, per MQTT spec.
 */
#define MQTT_DISCONNECT_PACKET_SIZE                 ( 2UL )

/**
 * @brief A PINGREQ packet is always 2 bytes in size, defined by MQTT 3.1.1 spec.
 */
#define MQTT_PACKET_PINGREQ_SIZE                    ( 2UL )

/**
 * @brief The Remaining Length field of MQTT disconnect packets, per MQTT spec.
 */
#define MQTT_DISCONNECT_REMAINING_LENGTH            ( ( uint8_t ) 0 )

/*
 * Constants relating to CONNACK packets, defined by MQTT 3.1.1 spec.
 */
#define MQTT_PACKET_CONNACK_REMAINING_LENGTH        ( ( uint8_t ) 2U )    /**< @brief A CONNACK packet always has a "Remaining length" of 2. */
#define MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK    ( ( uint8_t ) 0x01U ) /**< @brief The "Session Present" bit is always the lowest bit. */

/*
 * UNSUBACK, PUBACK, PUBREC, PUBREL, and PUBCOMP always have a remaining length
 * of 2.
 */
#define MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH     ( ( uint8_t ) 2 ) /**< @brief PUBACK, PUBREC, PUBREl, PUBCOMP, UNSUBACK Remaining length. */
#define MQTT_PACKET_PINGRESP_REMAINING_LENGTH       ( 0U )            /**< @brief A PINGRESP packet always has a "Remaining length" of 0. */

/**
 * @brief Per the MQTT 3.1.1 spec, the largest "Remaining Length" of an MQTT
 * packet is this value, 256 MB.
 */
#define MQTT_MAX_REMAINING_LENGTH                   ( 268435455UL )

#if ( MQTT_VERSION_5_ENABLED )

/**
 * @brief Per the MQTT spec, the max packet size  can be of  max remaining length + 5 bytes
 */
    #define MQTT_MAX_PACKET_SIZE             ( 268435460U )

/**
 * @brief Version 5 has the value 5.
 */
    #define MQTT_VERSION_5                   ( 5U )

/**
 * @brief Per the MQTT 5 spec, the  session expiry is of  5 bytes.
 */
    #define MQTT_SESSION_EXPIRY_SIZE         ( 5U )

/**
 * @brief Per the MQTT 5 spec, the receive maximum is of 3  bytes.
 */
    #define MQTT_RECEIVE_MAX_SIZE            ( 3U )

/**
 * @brief Per the MQTT 5 spec, the max packet size is of 5 bytes.
 */
    #define MQTT_MAX_PACKET_PROPERTY_SIZE    ( 5U )

/**
 * @brief Per the MQTT 5 spec, the topic alias  is of 3 bytes.
 */
    #define MQTT_TOPIC_ALIAS_SIZE            ( 3U )

/**
 * @brief Per the MQTT 5 spec, the request response  is of 2 bytes.
 */
    #define MQTT_REQUEST_RESPONSE_SIZE       ( 2U )

/**
 * @brief Per the MQTT 5 spec, the request problem  is of 2 bytes.
 */
    #define MQTT_REQUEST_PROBLEM_SIZE        ( 2U )

/**
 * @brief Utf 8 encoded string has 2 byte length field and 1 byte property id.
 */
    #define MQTT_UTF8_LENGTH_SIZE            ( 3U )

/*Publish Properties*/

/**
 * @brief Per the MQTT 5 spec, the will delay  is of 2 bytes.
 */
    #define MQTT_WILL_DELAY_SIZE        ( 5U )

/**
 * @brief Per the MQTT 5 spec, the payload format indicator is of 2 bytes.
 */
    #define MQTT_PAYLOAD_FORMAT_SIZE    ( 2U )

/**
 * @brief Per the MQTT 5 spec, the request problem  is of 2 bytes.
 */
    #define MQTT_MSG_EXPIRY_SIZE        ( 5U )

/*CONNECT PROPERTIES*/

/**
 * @brief Session expiry id.
 */
    #define MQTT_SESSION_EXPIRY_ID      ( 0x11 )

/**
 * @brief Receive maximum id.
 */
    #define MQTT_RECEIVE_MAX_ID         ( 0x21 )

/**
 * @brief Maximum packet size  id.
 */
    #define MQTT_MAX_PACKET_SIZE_ID     ( 0x27 )

/**
 * @brief Topic alias size id.
 */
    #define MQTT_TOPIC_ALIAS_MAX_ID     ( 0x22 )

/**
 * @brief Request response id.
 */
    #define MQTT_REQUEST_RESPONSE_ID    ( 0x19 )

/**
 * @brief Request problem id.
 */
    #define MQTT_REQUEST_PROBLEM_ID     ( 0x17 )

/**
 * @brief User property id.
 */
    #define MQTT_USER_PROPERTY_ID       ( 0x26 )

/**
 * @brief Authentication method id.
 */
    #define MQTT_AUTH_METHOD_ID         ( 0x15 )

/**
 * @brief  Authentication data id.
 */
    #define MQTT_AUTH_DATA_ID           ( 0x16 )

/*Publish PROPERTIES*/

/**
 * @brief Will delay id.
 */
    #define MQTT_WILL_DELAY_ID          ( 0x18 )

/**
 * @brief Payload format id.
 */
    #define MQTT_PAYLOAD_FORMAT_ID      ( 0x01 )

/**
 * @brief Message Expiry id.
 */
    #define MQTT_MSG_EXPIRY_ID          ( 0x02 )

/**
 * @brief Content type id.
 */
    #define MQTT_CONTENT_TYPE_ID        ( 0x03 )

/**
 * @brief Response topic id.
 */
    #define MQTT_RESPONSE_TOPIC_ID      ( 0x08 )

/**
 * @brief Correlation data id.
 */
    #define MQTT_CORRELATION_DATA_ID    ( 0x09 )

/**
 * @brief Topic alias id.
 */
    #define MQTT_TOPIC_ALIAS_ID         ( 0x23 )

/*CONNACK PROPERTIES*/

/**
 * @brief Max qos id.
 */
    #define MQTT_MAX_QOS_ID              ( 0x24 )

/**
 * @brief Retain available id.
 */
    #define MQTT_RETAIN_AVAILABLE_ID     ( 0x25 )

/**
 * @brief Assigned client identifier id.
 */
    #define MQTT_ASSIGNED_CLIENT_ID      ( 0x12 )

/**
 * @brief Reason string id.
 */
    #define MQTT_REASON_STRING_ID        ( 0x1F )

/**
 * @brief Wildcard available id.
 */
    #define MQTT_WILDCARD_ID             ( 0x28 )

/**
 * @brief Subscription available id.
 */
    #define MQTT_SUB_AVAILABLE_ID        ( 0x29 )

/**
 * @brief Shared subscription id.
 */
    #define MQTT_SHARED_SUB_ID           ( 0x2A )

/**
 * @brief Server keep alive id.
 */
    #define MQTT_SERVER_KEEP_ALIVE_ID    ( 0x13 )

/**
 * @brief Response information id.
 */

    #define MQTT_RESPONSE_INFO_ID    ( 0x1A )

/**
 * @brief Server reference  id.
 */
    #define MQTT_SERVER_REF_ID       ( 0x1C )

/**
 * @brief Size of the property id.
 */

    #define CORE_MQTT_ID_SIZE                           ( 1U )

/**
 * @ingroup mqtt_constants
 * @brief The size of MQTT PUBACK, PUBREC, PUBREL, and PUBCOMP packets with reason code, packet id.
 */
    #define MQTT_PUBLISH_ACK_PACKET_SIZE_WITH_REASON    ( 3UL )

/**
 * @brief The Connection is accepted.
 */
    #define MQTT_REASON_SUCCESS                         ( 0x00 )

/**
 * @brief The Client wishes to disconnect but requires
 * that the Server also publishes its Will Message.
 */
    #define MQTT_REASON_SEND_WILL                       ( 0x04 )

/**
 * @brief The message is accepted but there are no
 * subscribers.
 */
    #define MQTT_REASON_NO_MATCHING_SUBSCRIBERS         ( 0x10 )

/**
 * @brief The Server does not wish to reveal the reason for the
 * failure, or none of the other Reason Codes apply.
 */
    #define MQTT_REASON_UNSPECIFIED_ERR                 ( 0x80 )

/**
 * @brief Data within the CONNECT packet could not be
 * correctly parsed.
 */
    #define MQTT_REASON_MALFORMED_PACKET                ( 0x81 )

/**
 * @brief Data in the CONNECT packet does not conform to this
 * specification.
 */
    #define MQTT_REASON_PROTOCOL_ERR                    ( 0x82 )

/**
 * @brief The CONNECT is valid but is not accepted by this
 * Server.
 */
    #define MQTT_REASON_IMPL_SPECIFIC_ERR               ( 0x83 )

/**
 * @brief The Server does not support the version of the MQTT
 * protocol requested by the Client.
 */
    #define MQTT_REASON_UNSUPPORTED_PROTO_VER           ( 0x84 )

/**
 * @brief The Client Identifier is a valid string but is not allowed
 * by the Server.
 */
    #define MQTT_REASON_CLIENT_ID_NOT_VALID             ( 0x85 )

/**
 * @brief The Server does not accept the User Name or
 * Password specified by the Client.
 */
    #define MQTT_REASON_BAD_USER_OR_PASS                ( 0x86 )

/**
 * @brief The Client is not authorized to connect.
 */
    #define MQTT_REASON_NOT_AUTHORIZED                  ( 0x87 )

/**
 * @brief The MQTT Server is not available.
 */
    #define MQTT_REASON_SERVER_UNAVAILABLE              ( 0x88 )

/**
 * @brief The Server is busy, try again later.
 */
    #define MQTT_REASON_SERVER_BUSY                     ( 0x89 )

/**
 * @brief This Client has been banned by administrative action.
 */
    #define MQTT_REASON_BANNED                          ( 0x8A )

/**
 * @brief The Server is shutting down.
 */
    #define MQTT_REASON_SERVER_SHUTTING_DOWN            ( 0x8B )

/**
 * @brief The authentication method is not supported or does not
 * match the authentication method currently in use.
 */
    #define MQTT_REASON_BAD_AUTH_METHOD                 ( 0x8C )

/**
 * @brief The Connection is closed because no packet
 * has been received for 1.5 times the Keepalive
 * time.
 */
    #define MQTT_REASON_KEEP_ALIVE_TIMEOUT              ( 0x8D )

/**
 * @brief Another Connection using the same ClientID
 * has connected causing this Connection to be
 * closed.
 */
    #define MQTT_REASON_SESSION_TAKEN_OVER              ( 0x8E )

/**
 * @brief The Topic Filter is correctly formed, but is not
 * accepted by this Server.
 */
    #define MQTT_REASON_TOPIC_FILTER_INVALID            ( 0x8F )

/**
 * @ingroup mqtt_constants
 * @brief The Topic Name is not malformed, but is not
 * accepted by this Server.
 */
    #define MQTT_REASON_TOPIC_NAME_INVALID              ( 0x90 )

/**
 * @brief The Packet Identifier is already in use.
 */
    #define MQTT_REASON_PACKET_ID_IN_USE                ( 0x91 )

/**
 * @brief The Packet Identifier is not known.
 */
    #define MQTT_REASON_PACKET_ID_NOT_FOUND             ( 0x92 )

/**
 * @brief The Client or Server has received more than
 * Receive Maximum publication for which it has
 * not sent PUBACK or PUBCOMP.
 */
    #define MQTT_REASON_RX_MAX_EXCEEDED                 ( 0x93 )

/**
 * @brief The Connection is accepted.
 */
    #define MQTT_REASON_TOPIC_ALIAS_INVALID             ( 0x94 )

/**
 * @brief The packet exceeded the maximum
 * permissible size.
 */
    #define MQTT_REASON_PACKET_TOO_LARGE                ( 0x95 )

/**
 * @brief The Connection is accepted.
 */
    #define MQTT_REASON_MSG_RATE_TOO_HIGH               ( 0x96 )

/**
 * @brief An implementation or administrative imposed limit has
 * been exceeded.
 */
    #define MQTT_REASON_QUOTA_EXCEEDED                  ( 0x97 )

/**
 * @brief The Connection is closed due to an
 * administrative action.
 */
    #define MQTT_REASON_ADMIN_ACTION                    ( 0x98 )

/**
 * @brief The Payload does not match the specified Payload
 * Format Indicator.
 */
    #define MQTT_REASON_PAYLOAD_FORMAT_INVALID          ( 0x99 )

/**
 * @brief The Server does not support retained messages, and
 * Will Retain was set to 1.
 */
    #define MQTT_REASON_RETAIN_NOT_SUPPORTED            ( 0x9A )

/**
 * @brief The Server does not support the QoS.
 */
    #define MQTT_REASON_QOS_NOT_SUPPORTED               ( 0x9B )

/**
 * @brief The Client should temporarily use another server.
 */
    #define MQTT_REASON_USE_ANOTHER_SERVER              ( 0x9C )

/**
 * @brief The Client should permanently use another server.
 */
    #define MQTT_REASON_SERVER_MOVED                    ( 0x9D )

/**
 * @brief  The Server does not support Shared
 * Subscriptions.
 */
    #define MQTT_REASON_SS_NOT_SUPPORTED                ( 0x9E )

/**
 * @brief The connection rate limit has been exceeded.
 */
    #define MQTT_REASON_CON_RATE_EXCEED                 ( 0x9F )

/**
 * @brief The maximum connection time authorized for
 * this connection has been exceeded.
 */
    #define MQTT_REASON_MAX_CON_TIME                    ( 0xA0 )

/**
 * @brief The Server does not support Subscription
 * Identifiers; the subscription is not accepted.
 */
    #define MQTT_REASON_SUB_ID_NOT_SUP                  ( 0xA1 )

    #define MQTT_SUBSCRIPTION_ID_ID                     ( 0x0B )

/**
 * @brief The Server does not support Wildcard
 * Subscriptions; the subscription is not accepted.
 */
    #define MQTT_REASON_WILDCARD_SUB_NOT_SUP            ( 0xA2 )

#endif /* if ( MQTT_VERSION_5_ENABLED ) */

/**
 * @brief Set a bit in an 8-bit unsigned integer.
 */
#define UINT8_SET_BIT( x, position )      ( ( x ) = ( uint8_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )

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

#if ( MQTT_VERSION_5_ENABLED )

/**
 * @brief Get the 4th byte of a 32-bit unsigned integer.
 */
    #define UINT32_BYTE3( x )    ( ( uint8_t ) ( ( x ) >> 24 ) )

/**
 * @brief Get the 3rd byte of a 32-bit unsigned integer.
 */
    #define UINT32_BYTE2( x )    ( ( uint8_t ) ( ( x ) >> 16 ) )

/**
 * @brief Get the 2nd byte of a 32-bit unsigned integer.
 */
    #define UINT32_BYTE1( x )    ( ( uint8_t ) ( ( x ) >> 8 ) )

/**
 * @brief Get the 1st byte of a 32-bit unsigned integer.
 */
    #define UINT32_BYTE0( x )    ( ( uint8_t ) ( ( x ) & 0x000000FFU ) )

/**
 * @brief Macro for decoding a 4-byte unsigned int from a sequence of bytes.
 *
 * @param[in] ptr A uint8_t* that points to the high byte.
 */
    #define UINT32_DECODE( ptr )                         \
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) | \
                   ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
                   ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) |  \
                   ( ( uint32_t ) ptr[ 3 ] ) )

#endif /* if ( MQTT_VERSION_5_ENABLED ) */

/**
 * @brief A value that represents an invalid remaining length.
 *
 * This value is greater than what is allowed by the MQTT specification.
 */
#define MQTT_REMAINING_LENGTH_INVALID             ( ( size_t ) 268435456 )

/**
 * @brief The minimum remaining length for a QoS 0 PUBLISH.
 *
 * Includes two bytes for topic name length and one byte for topic name.
 */
#define MQTT_MIN_PUBLISH_REMAINING_LENGTH_QOS0    ( 3U )

/*-----------------------------------------------------------*/

/**
 * @brief MQTT Subscription packet types.
 */
typedef enum MQTTSubscriptionType
{
    MQTT_SUBSCRIBE,  /**< @brief The type is a SUBSCRIBE packet. */
    MQTT_UNSUBSCRIBE /**< @brief The type is a UNSUBSCRIBE packet. */
} MQTTSubscriptionType_t;

/*-----------------------------------------------------------*/

/**
 * @brief Serializes MQTT PUBLISH packet into the buffer provided.
 *
 * This function serializes MQTT PUBLISH packet into #MQTTFixedBuffer_t.pBuffer.
 * Copy of the payload into the buffer is done as part of the serialization
 * only if @p serializePayload is true.
 *
 * @brief param[in] pPublishInfo Publish information.
 * @brief param[in] remainingLength Remaining length of the PUBLISH packet.
 * @brief param[in] packetIdentifier Packet identifier of PUBLISH packet.
 * @brief param[in, out] pFixedBuffer Buffer to which PUBLISH packet will be
 * serialized.
 * @brief param[in] serializePayload Copy payload to the serialized buffer
 * only if true. Only PUBLISH header will be serialized if false.
 */
static void serializePublishCommon( const MQTTPublishInfo_t * pPublishInfo,
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
 *
 * @return false if the packet would exceed the size allowed by the
 * MQTT spec; true otherwise.
 */
static bool calculatePublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize );

/**
 * @brief Calculates the packet size and remaining length of an MQTT
 * SUBSCRIBE or UNSUBSCRIBE packet.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[out] pRemainingLength The Remaining Length of the MQTT SUBSCRIBE or
 * UNSUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT MQTT SUBSCRIBE or
 * UNSUBSCRIBE packet.
 * @param[in] subscriptionType #MQTT_SUBSCRIBE or #MQTT_UNSUBSCRIBE.
 *
 * #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or a subscription is empty; #MQTTSuccess otherwise.
 * 
 */
#if(MQTT_VERSION_5_ENABLED)
static MQTTStatus_t calculateSubscriptionPacketSizeV5(MQTTSubscribeInfo_t* pSubscriptionList,
    MQTTSubscribeProperties_t* pSubscribeProperties,
    size_t subscriptionCount,
    size_t* pRemainingLength,
    size_t* pPacketSize); 
#endif



static MQTTStatus_t calculateSubscriptionPacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                     size_t subscriptionCount,
                                                     size_t * pRemainingLength,
                                                     size_t * pPacketSize,
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
 * @param[in] remainingLength Remaining Length of MQTT CONNECT packet.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 */
static void serializeConnectPacket( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] responseCode MQTT standard CONNACK response code.
 */
static void logConnackResponse( uint8_t responseCode );

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
static uint8_t * encodeRemainingLength( uint8_t * pDestination,
                                        size_t length );

/**
 * @brief Retrieve the size of the remaining length if it were to be encoded.
 *
 * @param[in] length The remaining length to be encoded.
 *
 * @return The size of the remaining length if it were to be encoded.
 */
static size_t remainingLengthEncodedSize( size_t length );

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
 * @brief Deserialize a CONNACK packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t.
 *
 * @param[in] pConnack Pointer to an MQTT packet struct representing a
 * CONNACK.
 * @param[out] pSessionPresent Whether a previous session was present.
 *
 * @return #MQTTSuccess if CONNACK specifies that CONNECT was accepted;
 * #MQTTServerRefused if CONNACK specifies that CONNECT was rejected;
 * #MQTTBadResponse if the CONNACK packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializeConnack( const MQTTPacketInfo_t * pConnack,
                                        bool * pSessionPresent );

/**
 * @brief Decode the status bytes of a SUBACK packet to a #MQTTStatus_t.
 *
 * @param[in] statusCount Number of status bytes in the SUBACK.
 * @param[in] pStatusStart The first status byte in the SUBACK.
 *
 * @return #MQTTSuccess, #MQTTServerRefused, or #MQTTBadResponse.
 */
static MQTTStatus_t readSubackStatus( size_t statusCount,
                                      const uint8_t * pStatusStart );

/**
 * @brief Deserialize a SUBACK packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts
 * the packet identifier.
 *
 * @param[in] pSuback Pointer to an MQTT packet struct representing a SUBACK.
 * @param[out] pPacketIdentifier Packet ID of the SUBACK.
 *
 * @return #MQTTSuccess if SUBACK is valid; #MQTTBadResponse if SUBACK packet
 * doesn't follow the MQTT spec.
 */
static MQTTStatus_t deserializeSuback( const MQTTPacketInfo_t * pSuback,
                                       uint16_t * pPacketIdentifier );

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
 *
 * @return #MQTTSuccess if PUBLISH is valid; #MQTTBadResponse
 * if the PUBLISH packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo );

/**
 * @brief Deserialize an UNSUBACK, PUBACK, PUBREC, PUBREL, or PUBCOMP packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts
 * the packet identifier.
 *
 * @param[in] pAck Pointer to the MQTT packet structure representing the packet.
 * @param[out] pPacketIdentifier Packet ID of the ack type packet.
 *
 * @return #MQTTSuccess if UNSUBACK, PUBACK, PUBREC, PUBREL, or PUBCOMP is valid;
 * #MQTTBadResponse if the packet doesn't follow the MQTT spec.
 */
static MQTTStatus_t deserializeSimpleAck( const MQTTPacketInfo_t * pAck,
                                          uint16_t * pPacketIdentifier );

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

#if ( MQTT_VERSION_5_ENABLED )

    #if ( MQTT_USER_PROPERTY_ENABLED )

/**
 * @brief Get the size of user properties.
 *
 * Validates the user properties and calculates the total size of user property.
 *
 * @param[in] pUserProperty Pointer to an MQTT packet struct representing user properties.
 * @param[in] number Total number of user properties
 * @param[out] pSize Size of the user properties
 *
 * @return #MQTTSuccess if user properties are valid and #MQTTBadParameter  if the user properties are not valid
 */
        static MQTTStatus_t MQTT_GetUserPropertySize( const MQTTUserProperty_t * pUserProperty,
                                                      uint32_t number,
                                                      size_t * pSize );

/**
 * @brief Validate the length and decode a user property.
 *
 * @param[out] pUserProperties To store the decoded property.
 * @param[out] count Number of user properties decoded.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
        static MQTTStatus_t decodeutf_8pair( MQTTUserProperties_t * pUserProperties,
                                             uint32_t * count,
                                             size_t * pPropertyLength,
                                             const uint8_t ** pIndex );

    #else /* if ( MQTT_USER_PROPERTY_ENABLED ) */

/**
 * @brief Validate the length and decode a user property.
 *
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
        static MQTTStatus_t decodeAndDiscard( size_t * pPropertyLength,
                                              const uint8_t ** pIndex );
    #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */

/**
 * @brief Get the size of authentication information.
 *
 * Validates the authentication method, data  and calculates the total size of authentication information.
 *
 * @param[in] pAuthInfo Pointer to an MQTT packet struct representing authentication information.
 * @param[out] pPropertyLength Size of the authentication information.
 *
 * @return #MQTTSuccess if user properties are valid and #MQTTBadParameter  if the user properties are not valid
 */

    static MQTTStatus_t MQTT_GetAuthInfoSize( const MQTTAuthInfo_t * pAuthInfo,
                                              size_t * pPropertyLength );

/**
 * @brief Get the size of connect properties.
 *
 * Validates the connect properties, calculates the total size of connect properties and stores it int MQTTConnectProperties_t.
 *
 * @param[out] pConnectProperties Pointer to an MQTT packet struct representing connect properties.
 *
 * @return #MQTTSuccess if connect properties are valid and  #MQTTBadParameter  if the connect properties are not valid.
 */

    static MQTTStatus_t MQTT_GetConnectPropertiesSize( MQTTConnectProperties_t * pConnectProperties );

/**
 * @brief Get the size of will properties.
 *
 * Validates the publish properties,calculates the total size of publish properties and stores it in MQTTPublishInfo_t.
 *
 * @param[out] pPublishProperties Pointer to an MQTT packet struct representing publish properties.
 *
 * @return #MQTTSuccess if publish properties are valid and  #MQTTBadParameter  if they are not valid.
 */

    static MQTTStatus_t MQTT_GetPublishPropertiesSize( MQTTPublishInfo_t * pPublishProperties );

/**
 * @brief Decodes the variable length by reading a single byte at a time.
 *
 * Uses the algorithm provided in the spec.
 *
 * @param[in] pBuffer Pointer to the buffer.
 * @param[out] pLength Decoded variable length
 *
 * @return #MQTTSuccess if variable length and paramters are valid else #MQTTBadParameter.
 */
    static MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
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
 * @return #MQTTSuccess if connack  without connack properties is valid; #MQTTBadResponse if the Connack
 * packet doesn't follow MQTT spec.
 */
    static MQTTStatus_t validateConnackParams( const MQTTPacketInfo_t * pIncomingPacket,
                                               bool * pSessionPresent );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] responseCode MQTT Verion 5 standard CONNACK response code.
 *
 * @return MQTTServerRefused if response code is valid and MQTTProtocolError if responseCode is invalid.
 */
    static MQTTStatus_t logConnackResponseV5( uint8_t responseCode );

/**
 * @brief Serialize a MQTT CONNECT packet in the given buffer.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT Properties parameters.
 * @param[in] remainingLength Remaining Length of MQTT CONNECT packet.
 * @param[in] pFixedBuffer Buffer for packet serialization.
 *
 */

    static void serializeConnectPacketV5( const MQTTConnectInfo_t * pConnectInfo,
                                          const MQTTPublishInfo_t * pWillInfo,
                                          const MQTTConnectProperties_t * pConnectProperties,
                                          size_t remainingLength,
                                          const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Validate the length and decode a 4 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pUsed Whether the property is decoded befire.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/

    static MQTTStatus_t decodeuint32_t( uint32_t * pProperty,
                                        size_t * pPropertyLength,
                                        bool * pUsed,
                                        const uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a 2 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pUsed Whether the property is decoded befire.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/

    static MQTTStatus_t decodeuint16_t( uint16_t * pProperty,
                                        size_t * pPropertyLength,
                                        bool * pUsed,
                                        const uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a 1 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pUsed Whether the property is decoded befire.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
    static MQTTStatus_t decodeuint8_t( uint8_t * pProperty,
                                       size_t * pPropertyLength,
                                       bool * pUsed,
                                       const uint8_t ** pIndex );

/**
 * @brief Validate the length and decode a utf 8 string.
 *
 * @param[out] pProperty To store the decoded string.
 * @param[out] pLength  Size of the decoded utf-8 string.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pUsed Whether the property is decoded before.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
    static MQTTStatus_t decodeutf_8( const char ** pProperty,
                                     uint16_t * pLength,
                                     size_t * pPropertyLength,
                                     bool * pUsed,
                                     const uint8_t ** pIndex );


/**
 * @brief Validate the length and decode authentication information.
 *
 * @param[out] pConnackProperties To store the decoded property.
 * @param[out] pAuthMethod Whether the authentication method is decoded before.
 * @param[out] pAuthData  Whether the authentication data is decoded before.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 * @param[in]  id To differentiate between authentication method and  authentication data.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
    static MQTTStatus_t decodeAuthInfo( const MQTTConnectProperties_t * pConnackProperties,
                                        bool * pAuthMethod,
                                        bool * pAuthData,
                                        size_t * pPropertyLength,
                                        const uint8_t ** pIndex,
                                        const uint8_t id );

/**
 * @brief Validate the length and decode the connack properties.
 *
 * @param[out] pConnackProperties To store the decoded property.
 * @param[out] length  Length of the property.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
    static MQTTStatus_t deserializeConnackV5( MQTTConnectProperties_t * pConnackProperties,
                                              size_t length,
                                              const uint8_t * const * pIndex );

/**
 * @brief Calculates the packet size and remaining length of an MQTT
 * PUBLISH packet.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT PUBLISH packet.
 * @param[out] pPacketSize The total size of the MQTT PUBLISH packet.
 * @param[in] maxPacketSize Max packet size allowed by the server.
 *
 *
 * @return MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; true otherwise.
 */
    static MQTTStatus_t calculatePublishPacketSizeV5( MQTTPublishInfo_t * pPublishInfo,
                                                      size_t * pRemainingLength,
                                                      size_t * pPacketSize,
                                                      uint32_t maxPacketSize );

/**
 * @brief Serialize a MQTT Publish Ack packet in the given buffer.
 *
 * @param[in] pAckInfo MQTT PUB ACK packet parameters.
 * @param[in] packetType Type of pub ack.
 * @param[in] packetId Packet identifier.
 * @param[in] remainingLength Remaining Length of PUB ACK packet.
 * @param[in] pFixedBuffer Buffer for packet serialization.
 *
 */
    static void serializePubAckPacketV5( const MQTTAckInfo_t * pAckInfo,
                                         uint8_t packetType,
                                         uint16_t packetId,
                                         size_t remainingLength,
                                         const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Prints the appropriate message for the PUBREL, PUBACK response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard PUBREL, PUBACK response code.
 * @param[in] packetIdentifier Packet id of the ack packet.
 *
 * @return #MQTTSuccess, #MQTTServerRefused and #MQTTProtocolError.
 */
    static MQTTStatus_t logAckResponseV5( uint8_t reasonCode,
                                          uint16_t packetIdentifier );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard CONNACK response code.
 * @param[in] packetIdentifier Packet id of the ack packet.
 *
 * @return #MQTTSuccess, #MQTTServerRefused and #MQTTProtocolError.
 */
    static MQTTStatus_t logSimpleAckResponseV5( uint8_t reasonCode,
                                                uint16_t packetIdentifier );

/**
 * @brief Validate the length and decode the publish ack properties.
 *
 * @param[out] pAckInfo To store the decoded property.
 * @param[out] pIndex Pointer to the current index of the buffer.
 * @param[out] remainingLength Remaining length of the incoming packet.
 *
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
    static MQTTStatus_t decodeAckProperties( MQTTAckInfo_t * pAckInfo,
                                             const uint8_t * pIndex,
                                             size_t remainingLength );

/**
 * @brief Deserialize an PUBACK, PUBREC, PUBREL, or PUBCOMP packet.
 *
 * Converts the packet from a stream of bytes to an #MQTTStatus_t and extracts
 * the packet identifier, reason code, properties.
 *
 * @param[in] pAck Pointer to the MQTT packet structure representing the packet.
 * @param[out] pPacketIdentifier Packet ID of the ack type packet.
 * @param[out] pAckInfo Structure to store the ack properties.
 * @param[in] requestProblem To validate the packet.
 *
 * @return #MQTTSuccess, #MQTTBadResponse, #MQTTProtocolError and  #MQTTMalformedPacket.
 */
    static MQTTStatus_t deserializeSimpleAckV5( const MQTTPacketInfo_t * pAck,
                                                uint16_t * pPacketIdentifier,
                                                MQTTAckInfo_t * pAckInfo,
                                                bool requestProblem );

/**
 * @brief Serialize the publish properties in the given buffer.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] pIndex  Pointer to the current index of the buffer.
 *
 * @return  Pointer to the new index of the buffer.
 */
    static uint8_t * serializePublishProperties( const MQTTPublishInfo_t * pPublishInfo,
                                                 uint8_t * pIndex );

/**
 * @brief Serialize an MQTT CONNECT packet in the given buffer.
 *
 * @param[in] pDisconnectInfo MQTT DISCONNECT packet parameters.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 * @param[in] remainingLength Remaining Length of MQTT DISCONNECT packet.
 * @param[in] sessionExpiry Session Expiry Interval.
 *
 */
    static void serializeDisconnectPacketV5( const MQTTAckInfo_t * pDisconnectInfo,
                                             const MQTTFixedBuffer_t * pFixedBuffer,
                                             size_t remainingLength,
                                             uint32_t sessionExpiry );

/**
 * @brief Prints and validates the appropriate message for the Disconnect response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard DISCONNECT response code.
 * @param[in] incoming To differentiate between outgoing and incoming disconnect.
 *
 * @return #MQTTSuccess,#MQTTBadParameter and #MQTTProtocolError.
 */
    static MQTTStatus_t validateDisconnectResponseV5( uint8_t reasonCode,
                                                      bool incoming );
/*-----------------------------------------------------------*/

    #if ( MQTT_USER_PROPERTY_ENABLED )
        static MQTTStatus_t MQTT_GetUserPropertySize( const MQTTUserProperty_t * pUserProperty,
                                                      uint32_t number,
                                                      size_t * pSize )
        {
            MQTTStatus_t status = MQTTSuccess;
            uint32_t i = 0;

            /*Number of user properties can't be more than the max user properties specified*/
            if( number > ( uint32_t ) MAX_USER_PROPERTY )
            {
                status = MQTTBadParameter;
            }
            else
            {
                for( ; ( i < number ); i++ )
                {
                    /*Validate the key and value*/
                    if( ( pUserProperty[ i ].keyLength == 0U ) || ( pUserProperty[ i ].valueLength == 0U ) || ( pUserProperty[ i ].pKey == NULL ) || ( pUserProperty[ i ].pValue == NULL ) )
                    {
                        status = MQTTBadParameter;
                        break;
                    }
                    else
                    {
                        *pSize += ( pUserProperty[ i ].keyLength );
                        *pSize += MQTT_UTF8_LENGTH_SIZE;
                        *pSize += ( pUserProperty[ i ].valueLength );
                        *pSize += 2U;
                    }
                }
            }

            return status;
        }

        static MQTTStatus_t decodeutf_8pair( MQTTUserProperties_t * pUserProperties,
                                             uint32_t * count,
                                             size_t * pPropertyLength,
                                             const uint8_t ** pIndex )
        {
            const uint8_t * pVariableHeader = *pIndex;
            MQTTStatus_t status = MQTTSuccess;
            MQTTUserProperty_t discardUserProperty;
            MQTTUserProperty_t * pUserProperty;

            if( *count == ( uint32_t ) MAX_USER_PROPERTY )
            {
                pUserProperty = &discardUserProperty;
            }
            else
            {
                pUserProperty = pUserProperties->userProperty;
                pUserProperty = &pUserProperty[ *count ];
            }

            /*Validate the property length and decode the user property received.*/
            if( *pPropertyLength < sizeof( uint16_t ) )
            {
                status = MQTTMalformedPacket;
            }
            else
            {
                pUserProperty->keyLength = UINT16_DECODE( pVariableHeader );
                *pPropertyLength -= sizeof( uint16_t );

                if( *pPropertyLength < pUserProperty->keyLength )
                {
                    status = MQTTMalformedPacket;
                }
                else
                {
                    pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];
                    pUserProperty->pKey = ( const char * ) pVariableHeader;
                    *pPropertyLength -= pUserProperty->keyLength;
                    pVariableHeader = &pVariableHeader[ pUserProperty->keyLength ];

                    if( *pPropertyLength < sizeof( uint16_t ) )
                    {
                        status = MQTTMalformedPacket;
                    }
                    else
                    {
                        pUserProperty->valueLength = UINT16_DECODE( pVariableHeader );
                        *pPropertyLength -= sizeof( uint16_t );
                        pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];

                        if( *pPropertyLength < ( size_t ) ( pUserProperty->valueLength ) )
                        {
                            status = MQTTMalformedPacket;
                        }
                        else
                        {
                            pUserProperty->pValue = ( const char * ) pVariableHeader;
                            pVariableHeader = &pVariableHeader[ pUserProperty->valueLength ];
                            *pPropertyLength -= pUserProperty->valueLength;
                        }
                    }
                }
            }

            *pIndex = pVariableHeader;

            if( ( *count == ( uint32_t ) MAX_USER_PROPERTY ) && ( status == MQTTSuccess ) )
            {
                LogDebug( ( "Discarded additional user property with key %s and value %s. ",
                            discardUserProperty.pKey,
                            discardUserProperty.pValue ) );
            }
            else
            {
                *count += 1U;
            }

            return status;
        }
    #else /* if ( MQTT_USER_PROPERTY_ENABLED ) */
        static MQTTStatus_t decodeAndDiscard( size_t * pPropertyLength,
                                              const uint8_t ** pIndex )
        {
            const uint8_t * pVariableHeader = *pIndex;
            MQTTStatus_t status = MQTTSuccess;
            uint16_t keyLength;
            uint16_t valueLength;
            const char * pKey;
            const char * pValue;

            /*Validate the property length and decode the user property received.*/
            if( *pPropertyLength < sizeof( uint16_t ) )
            {
                status = MQTTMalformedPacket;
            }
            else
            {
                keyLength = UINT16_DECODE( pVariableHeader );
                *pPropertyLength -= sizeof( uint16_t );

                if( *pPropertyLength < keyLength )
                {
                    status = MQTTMalformedPacket;
                }
                else
                {
                    pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];
                    pKey = ( const char * ) pVariableHeader;
                    *pPropertyLength -= keyLength;
                    pVariableHeader = &pVariableHeader[ keyLength ];

                    if( *pPropertyLength < sizeof( uint16_t ) )
                    {
                        status = MQTTMalformedPacket;
                    }
                    else
                    {
                        valueLength = UINT16_DECODE( pVariableHeader );
                        *pPropertyLength -= sizeof( uint16_t );
                        pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];

                        if( *pPropertyLength < ( size_t ) ( valueLength ) )
                        {
                            status = MQTTMalformedPacket;
                        }
                        else
                        {
                            pValue = ( const char * ) pVariableHeader;
                            pVariableHeader = &pVariableHeader[ valueLength ];
                            *pPropertyLength -= valueLength;
                        }
                    }
                }
            }

            *pIndex = pVariableHeader;

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Discarded additional user property with key %s and value %s. ",
                            pKey,
                            pValue ) );
            }

            return status;
        }

    #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */

    static MQTTStatus_t MQTT_GetAuthInfoSize( const MQTTAuthInfo_t * pAuthInfo,
                                              size_t * pPropertyLength )
    {
        MQTTStatus_t status = MQTTSuccess;

        /*Validate the parameters*/
        if( ( pAuthInfo->authMethodLength == 0U ) && ( pAuthInfo->authDataLength != 0U ) )
        {
            status = MQTTBadParameter;
        }
        else if( ( pAuthInfo->authMethodLength != 0U ) && ( pAuthInfo->pAuthMethod == NULL ) )
        {
            status = MQTTBadParameter;
        }
        else if( ( pAuthInfo->authDataLength != 0U ) && ( pAuthInfo->pAuthData == NULL ) )
        {
            status = MQTTBadParameter;
        }
        else
        {
            /*Add authentication method and data if provided*/
            if( pAuthInfo->authMethodLength != 0U )
            {
                *pPropertyLength += pAuthInfo->authMethodLength;
                *pPropertyLength += MQTT_UTF8_LENGTH_SIZE;

                if( pAuthInfo->authDataLength != 0U )
                {
                    *pPropertyLength += pAuthInfo->authDataLength;
                    *pPropertyLength += MQTT_UTF8_LENGTH_SIZE;
                }
            }
        }

        return status;
    }
    static MQTTStatus_t MQTT_GetSubscribePropertiesSize(MQTTSubscribeProperties_t *pSubscribeProperties )
    {
        size_t propertyLength = 0;
        MQTTStatus_t status = MQTTSuccess;

        if(pSubscribeProperties->subscriptionId != 0 ){
            propertyLength += 1U ; 
            propertyLength += remainingLengthEncodedSize(pSubscribeProperties->subscriptionId);
        }
        #if ( MQTT_USER_PROPERTY_ENABLED )
            /*Get the length of the user properties*/
            if( ( status == MQTTSuccess ) && ( pSubscribeProperties->pUserProperties != NULL ) )
            {
                status = MQTT_GetUserPropertySize( pSubscribeProperties->pUserProperties->userProperty, pSubscribeProperties->pUserProperties->count, &propertyLength );
            }
        #endif

        /*Variable length encoded values cannot have a value of more than 268435455U*/
        if( ( status == MQTTSuccess ) && ( pSubscribeProperties->propertyLength > MQTT_MAX_REMAINING_LENGTH ) )
        {
            status = MQTTBadParameter;
        }

        if( status == MQTTSuccess )
        {
            pSubscribeProperties->propertyLength = propertyLength;
        }

        return status;
    }
    static MQTTStatus_t MQTT_GetConnectPropertiesSize( MQTTConnectProperties_t * pConnectProperties )
    {
        size_t propertyLength = 0;
        MQTTStatus_t status = MQTTSuccess;

        /*Validate the arguments*/
        if( pConnectProperties->maxPacketSize == 0U )
        {
            status = MQTTBadParameter;
        }
        else if( pConnectProperties->receiveMax == 0U )
        {
            status = MQTTBadParameter;
        }
        else
        {
            /*Add the lengths of the parameters if applicable*/
            if( pConnectProperties->sessionExpiry != 0U )
            {
                propertyLength += MQTT_SESSION_EXPIRY_SIZE;
            }

            if( pConnectProperties->receiveMax != ( uint16_t ) UINT16_MAX )
            {
                propertyLength += MQTT_RECEIVE_MAX_SIZE;
            }

            if( pConnectProperties->maxPacketSize != MQTT_MAX_PACKET_SIZE )
            {
                propertyLength += MQTT_MAX_PACKET_PROPERTY_SIZE;
            }

            if( pConnectProperties->topicAliasMax != 0U )
            {
                propertyLength += MQTT_TOPIC_ALIAS_SIZE;
            }

            if( pConnectProperties->requestResponseInfo != false )
            {
                propertyLength += MQTT_REQUEST_RESPONSE_SIZE;
            }

            if( pConnectProperties->requestProblemInfo != true )
            {
                propertyLength += MQTT_REQUEST_PROBLEM_SIZE;
            }

            if( pConnectProperties->pOutgoingAuth != NULL )
            {
                /*Valid authentication parameters*/
                status = MQTT_GetAuthInfoSize( pConnectProperties->pOutgoingAuth, &propertyLength );
            }
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            /*Get the length of the user properties*/
            if( ( status == MQTTSuccess ) && ( pConnectProperties->pOutgoingUserProperty != NULL ) )
            {
                status = MQTT_GetUserPropertySize( pConnectProperties->pOutgoingUserProperty->userProperty, pConnectProperties->pOutgoingUserProperty->count, &propertyLength );
            }
        #endif

        /*Variable length encoded values cannot have a value of more than 268435455U*/
        if( ( status == MQTTSuccess ) && ( pConnectProperties->propertyLength > MQTT_MAX_REMAINING_LENGTH ) )
        {
            status = MQTTBadParameter;
        }

        if( status == MQTTSuccess )
        {
            pConnectProperties->propertyLength = propertyLength;
        }

        return status;
    }

    static MQTTStatus_t MQTT_GetPublishPropertiesSize( MQTTPublishInfo_t * pPublishProperties )
    {
        size_t propertyLength = 0U;
        MQTTStatus_t status = MQTTSuccess;

        /*Add the length of all the parameters which are provided*/
        /*Only applicable for will*/
        if( pPublishProperties->willDelay != 0U )
        {
            propertyLength += MQTT_WILL_DELAY_SIZE;
        }

        /*Only applicable for publish packet.*/
        if( pPublishProperties->topicAlias != 0U )
        {
            propertyLength += 3U;
        }

        if( pPublishProperties->payloadFormat != 0U )
        {
            propertyLength += MQTT_PAYLOAD_FORMAT_SIZE;
        }

        if( pPublishProperties->msgExpiryPresent == true )
        {
            propertyLength += MQTT_MSG_EXPIRY_SIZE;
        }

        if( pPublishProperties->contentTypeLength != 0U )
        {
            if( pPublishProperties->pContentType == NULL )
            {
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength += ( pPublishProperties->contentTypeLength );
                propertyLength += MQTT_UTF8_LENGTH_SIZE;
            }
        }

        /*Validate if length and pointers are valid*/
        if( ( status == MQTTSuccess ) && ( pPublishProperties->responseTopicLength != 0U ) )
        {
            if( pPublishProperties->pResponseTopic == NULL )
            {
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength += ( pPublishProperties->responseTopicLength );
                propertyLength += MQTT_UTF8_LENGTH_SIZE;
            }
        }

        if( ( status == MQTTSuccess ) && ( pPublishProperties->correlationLength != 0U ) )
        {
            if( pPublishProperties->pCorrelationData == NULL )
            {
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength += ( pPublishProperties->correlationLength );
                propertyLength += MQTT_UTF8_LENGTH_SIZE;
            }
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            /*Get the length of user properties*/
            if( ( status == MQTTSuccess ) && ( pPublishProperties->pUserProperty != NULL ) )
            {
                status = MQTT_GetUserPropertySize( pPublishProperties->pUserProperty->userProperty, pPublishProperties->pUserProperty->count, &propertyLength );
            }
        #endif

        /*Variable encoded can't have a value more than 268435455UL*/
        if( propertyLength > MQTT_MAX_REMAINING_LENGTH )
        {
            status = MQTTBadParameter;
        }

        if( status == MQTTSuccess )
        {
            pPublishProperties->propertyLength = propertyLength;
        }

        return status;
    }


    static void serializeConnectPacketV5( const MQTTConnectInfo_t * pConnectInfo,
                                          const MQTTPublishInfo_t * pWillInfo,
                                          const MQTTConnectProperties_t * pConnectProperties,
                                          size_t remainingLength,
                                          const MQTTFixedBuffer_t * pFixedBuffer )
    {
        uint8_t * pIndex = NULL;

        assert( pConnectInfo != NULL );
        assert( pFixedBuffer != NULL );
        assert( pFixedBuffer->pBuffer != NULL );
        assert( pConnectProperties != NULL );
        pIndex = pFixedBuffer->pBuffer;
        /* Serialize the header. */
        pIndex = MQTT_SerializeConnectFixedHeader( pIndex,
                                                   pConnectInfo,
                                                   pWillInfo,
                                                   remainingLength );

        /* Serialize the connect Properties. */
        pIndex = MQTTV5_SerializeConnectProperties( pIndex, pConnectProperties );
        #if ( MQTT_USER_PROPERTY_ENABLED )
            if( pConnectProperties->pOutgoingUserProperty != NULL )
            {
                uint32_t i = 0;
                uint32_t size = pConnectProperties->pOutgoingUserProperty->count;
                const MQTTUserProperty_t * pUserProperty = pConnectProperties->pOutgoingUserProperty->userProperty;

                for( ; i < size; i++ )
                {
                    *pIndex = MQTT_USER_PROPERTY_ID;
                    pIndex++;
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pKey, pUserProperty[ i ].keyLength );
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pValue, pUserProperty[ i ].valueLength );
                }
            }
        #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */

        if( pConnectProperties->pOutgoingAuth != NULL )
        {
            /* Serialize the authentication method  string. */
            *pIndex = MQTT_AUTH_METHOD_ID;
            pIndex++;
            pIndex = encodeString( pIndex, pConnectProperties->pOutgoingAuth->pAuthMethod, pConnectProperties->pOutgoingAuth->authMethodLength );

            if( pConnectProperties->pOutgoingAuth->authDataLength != 0U )
            {
                *pIndex = MQTT_AUTH_DATA_ID;
                pIndex++;
                pIndex = encodeString( pIndex, pConnectProperties->pOutgoingAuth->pAuthData, pConnectProperties->pOutgoingAuth->authDataLength );
            }
        }

        /* Write the client identifier into the CONNECT packet. */
        pIndex = encodeString( pIndex,
                               pConnectInfo->pClientIdentifier,
                               pConnectInfo->clientIdentifierLength );

        /* Serialize the will properties,topic name and message into the CONNECT packet if provided. */
        if( pWillInfo != NULL )
        {
            pIndex = serializePublishProperties( pWillInfo, pIndex );
            pIndex = encodeString( pIndex, pWillInfo->pTopicName, pWillInfo->topicNameLength );
            pIndex = encodeString( pIndex, pWillInfo->pPayload, ( uint16_t ) pWillInfo->payloadLength );
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


    static MQTTStatus_t logConnackResponseV5( uint8_t responseCode )
    {
        MQTTStatus_t status = MQTTServerRefused;

        /* Log an error based on the CONNACK response code. */
        switch( responseCode )
        {
            case ( uint8_t ) MQTT_REASON_UNSPECIFIED_ERR:
                LogError( ( "Connection refused: Unspecified error" ) );
                break;

            case ( uint8_t ) MQTT_REASON_MALFORMED_PACKET:
                LogError( ( "Connection refused: Malformed Packet." ) );
                break;

            case ( uint8_t ) MQTT_REASON_PROTOCOL_ERR:
                LogError( ( "Connection refused: Protocol Error." ) );
                break;

            case ( uint8_t ) MQTT_REASON_IMPL_SPECIFIC_ERR:
                LogError( ( "Connection refused: Implementation specific error." ) );
                break;

            case ( uint8_t ) MQTT_REASON_UNSUPPORTED_PROTO_VER:
                LogError( ( "Connection refused: Unsupported Protocol Version." ) );
                break;

            case ( uint8_t ) MQTT_REASON_CLIENT_ID_NOT_VALID:
                LogError( ( "Connection refused: Client Identifier not valid." ) );
                break;

            case ( uint8_t ) MQTT_REASON_BAD_USER_OR_PASS:
                LogError( ( "Connection refused: Bad User Name or Password." ) );
                break;

            case ( uint8_t ) MQTT_REASON_NOT_AUTHORIZED:
                LogError( ( "Connection refused: Not authorized." ) );
                break;

            case ( uint8_t ) MQTT_REASON_SERVER_UNAVAILABLE:
                LogError( ( "Connection refused: Server unavailable." ) );
                break;

            case ( uint8_t ) MQTT_REASON_SERVER_BUSY:
                LogError( ( "Connection refused: Server busy." ) );
                break;

            case ( uint8_t ) MQTT_REASON_BANNED:
                LogError( ( "Connection refused: Banned." ) );
                break;

            case ( uint8_t ) MQTT_REASON_BAD_AUTH_METHOD:
                LogError( ( "Connection refused: Bad authentication method." ) );
                break;

            case ( uint8_t ) MQTT_REASON_TOPIC_NAME_INVALID:
                LogError( ( "Connection refused: Topic Name invalid." ) );
                break;

            case ( uint8_t ) MQTT_REASON_PACKET_TOO_LARGE:
                LogError( ( "Connection refused: Packet too large ." ) );
                break;

            case ( uint8_t ) MQTT_REASON_QUOTA_EXCEEDED:
                LogError( ( "Connection refused: Quota exceeded." ) );
                break;

            case ( uint8_t ) MQTT_REASON_PAYLOAD_FORMAT_INVALID:
                LogError( ( "Connection refused: Payload format invalid." ) );
                break;

            case ( uint8_t ) MQTT_REASON_RETAIN_NOT_SUPPORTED:
                LogError( ( "Connection refused: Retain not supported." ) );
                break;

            case ( uint8_t ) MQTT_REASON_QOS_NOT_SUPPORTED:
                LogError( ( "Connection refused: QoS not supported." ) );
                break;

            case ( uint8_t ) MQTT_REASON_USE_ANOTHER_SERVER:
                LogError( ( "Connection refused: Use another server." ) );
                break;

            case ( uint8_t ) MQTT_REASON_SERVER_MOVED:
                LogError( ( "Connection refused: Server moved." ) );
                break;

            case ( uint8_t ) MQTT_REASON_CON_RATE_EXCEED:
                LogError( ( "Connection refused: Connection rate exceeded." ) );
                break;

            default:
                status = MQTTProtocolError;
                break;
        }

        return status;
    }

    static MQTTStatus_t validateConnackParams( const MQTTPacketInfo_t * pIncomingPacket,
                                               bool * pSessionPresent )
    {
        MQTTStatus_t status = MQTTSuccess;

        /*Validate the arguments*/
        if( pIncomingPacket == NULL )
        {
            LogError( ( "pIncomingPacket cannot be NULL." ) );
            status = MQTTBadParameter;
        }
        else if( pSessionPresent == NULL )
        {
            LogError( ( "pSessionPresent cannot be NULL for CONNACK packet." ) );
            status = MQTTBadParameter;
        }
        else if( pIncomingPacket->pRemainingData == NULL )
        {
            LogError( ( "Remaining data of incoming packet is NULL." ) );
            status = MQTTBadParameter;
        }
        else if( pIncomingPacket->type != MQTT_PACKET_TYPE_CONNACK )
        {
            LogError( ( "Packet type is invalid." ) );
            status = MQTTBadParameter;
        }
        else
        {
            const uint8_t * pRemainingData = NULL;
            pRemainingData = pIncomingPacket->pRemainingData;

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

            if( status == MQTTSuccess )
            {
                if( pRemainingData[ 1 ] != 0u )
                {
                    status = logConnackResponseV5( pRemainingData[ 1 ] );
                }
            }
        }

        return status;
    }

    static MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                              size_t * pLength )
    {
        size_t remainingLength = 0;
        size_t multiplier = 1;
        size_t bytesDecoded = 0;
        size_t expectedSize = 0;
        uint8_t encodedByte = 0;
        MQTTStatus_t status = MQTTSuccess;

        /* This algorithm is copied from the MQTT v3.1.1 spec. */
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
                /* Get the next byte. It is at the next position after the bytes
                 * decoded till now since the header of one byte was read before. */
                encodedByte = pBuffer[ bytesDecoded ];
                remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
                multiplier *= 128U;
                bytesDecoded++;
            }

            /* If the response is incorrect then
             * break out of the loop. */
            if( remainingLength == MQTT_REMAINING_LENGTH_INVALID )
            {
                break;
            }
        } while( ( encodedByte & 0x80U ) != 0U );

        if( status == MQTTSuccess )
        {
            /* Check that the decoded remaining length conforms to the MQTT specification. */
            expectedSize = remainingLengthEncodedSize( remainingLength );

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

    static MQTTStatus_t decodeuint32_t( uint32_t * pProperty,
                                        size_t * pPropertyLength,
                                        bool * pUsed,
                                        const uint8_t ** pIndex )
    {
        const uint8_t * pVariableHeader = *pIndex;
        MQTTStatus_t status = MQTTSuccess;

        /*Protocol error to include the same property twice.*/
        if( *pUsed == true )
        {
            status = MQTTProtocolError;
        }
        /*Validate the length and decode.*/
        else if( *pPropertyLength < sizeof( uint32_t ) )
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pProperty = UINT32_DECODE( pVariableHeader );
            pVariableHeader = &pVariableHeader[ sizeof( uint32_t ) ];
            *pUsed = true;
            *pPropertyLength -= sizeof( uint32_t );
        }

        *pIndex = pVariableHeader;
        return status;
    }

    static MQTTStatus_t decodeuint16_t( uint16_t * pProperty,
                                        size_t * pPropertyLength,
                                        bool * pUsed,
                                        const uint8_t ** pIndex )
    {
        const uint8_t * pVariableHeader = *pIndex;
        MQTTStatus_t status = MQTTSuccess;

        /*Protocol error to include the same property twice.*/

        if( *pUsed == true )
        {
            status = MQTTProtocolError;
        }
        /*Validate the length and decode.*/

        else if( *pPropertyLength < sizeof( uint16_t ) )
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pProperty = UINT16_DECODE( pVariableHeader );
            pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];
            *pUsed = true;
            *pPropertyLength -= sizeof( uint16_t );
        }

        *pIndex = pVariableHeader;
        return status;
    }

    static MQTTStatus_t decodeuint8_t( uint8_t * pProperty,
                                       size_t * pPropertyLength,
                                       bool * pUsed,
                                       const uint8_t ** pIndex )
    {
        const uint8_t * pVariableHeader = *pIndex;
        MQTTStatus_t status = MQTTSuccess;

        /*Protocol error to include the same property twice.*/

        if( *pUsed == true )
        {
            status = MQTTProtocolError;
        }
        /*Validate the length and decode.*/

        else if( *pPropertyLength < sizeof( uint8_t ) )
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pProperty = *pVariableHeader;
            pVariableHeader = &pVariableHeader[ sizeof( uint8_t ) ];
            *pUsed = true;
            *pPropertyLength -= sizeof( uint8_t );

            if( *pProperty > 1U )
            {
                status = MQTTProtocolError;
            }
        }

        *pIndex = pVariableHeader;
        return status;
    }

    static MQTTStatus_t decodeutf_8( const char ** pProperty,
                                     uint16_t * pLength,
                                     size_t * pPropertyLength,
                                     bool * pUsed,
                                     const uint8_t ** pIndex )
    {
        const uint8_t * pVariableHeader = *pIndex;
        MQTTStatus_t status = MQTTSuccess;

        /*Protocol error to include the same property twice.*/

        if( *pUsed == true )
        {
            status = MQTTProtocolError;
        }
        /*Validate the length and decode.*/

        else if( *pPropertyLength < sizeof( uint16_t ) )
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pLength = UINT16_DECODE( pVariableHeader );
            pVariableHeader = &pVariableHeader[ sizeof( uint16_t ) ];
            *pPropertyLength -= sizeof( uint16_t );

            if( *pPropertyLength < *pLength )
            {
                status = MQTTMalformedPacket;
            }
            else
            {
                *pProperty = ( const char * ) pVariableHeader;
                pVariableHeader = &pVariableHeader[ *pLength ];
                *pPropertyLength -= *pLength;
                *pUsed = true;
            }
        }

        *pIndex = pVariableHeader;
        return status;
    }

    static MQTTStatus_t decodeAuthInfo( const MQTTConnectProperties_t * pConnackProperties,
                                        bool * pAuthMethod,
                                        bool * pAuthData,
                                        size_t * pPropertyLength,
                                        const uint8_t ** pIndex,
                                        const uint8_t id )
    {
        MQTTStatus_t status = MQTTSuccess;

        if( pConnackProperties->pOutgoingAuth == NULL )
        {
            status = MQTTProtocolError;
        }
        else if( id == ( uint8_t ) MQTT_AUTH_METHOD_ID )
        {
            /*Decode the authenticaton method */
            status = decodeutf_8( &pConnackProperties->pIncomingAuth->pAuthMethod, &pConnackProperties->pIncomingAuth->authMethodLength, pPropertyLength, pAuthMethod, pIndex );
        }
        else
        {
            /*Decode the authentication data */
            status = decodeutf_8( &pConnackProperties->pIncomingAuth->pAuthData, &pConnackProperties->pIncomingAuth->authDataLength, pPropertyLength, pAuthData, pIndex );
        }

        return status;
    }

    static MQTTStatus_t deserializeConnackV5( MQTTConnectProperties_t * pConnackProperties,
                                              size_t length,
                                              const uint8_t * const * pIndex )
    {
        MQTTStatus_t status = MQTTSuccess;
        const uint8_t * pVariableHeader = *pIndex;
        size_t propertyLength = length;
        bool sessionExpiry = false;
        bool serverReceiveMax = false;
        bool maxQos = false;
        bool retain = false;
        bool maxPacket = false;
        bool clientId = false;
        bool topicAlias = false;
        bool reasonString = false;
        bool wildcard = false;
        bool subId = false;
        bool sharedsub = false;
        bool keepAlive = false;
        bool responseInfo = false;
        bool serverRef = false;
        bool authMethod = false;
        bool authData = false;

        pVariableHeader = &pVariableHeader[ remainingLengthEncodedSize( propertyLength ) ];

        /*Decode all the properties received, validate and store them in pConnackProperties.*/
        while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
        {
            uint8_t packetId = *pVariableHeader;
            pVariableHeader = &pVariableHeader[ 1 ];
            propertyLength -= sizeof( uint8_t );

            switch( packetId )
            {
                case MQTT_SESSION_EXPIRY_ID:
                    status = decodeuint32_t( &pConnackProperties->sessionExpiry, &propertyLength, &sessionExpiry, &pVariableHeader );
                    break;

                case MQTT_RECEIVE_MAX_ID:
                    status = decodeuint16_t( &pConnackProperties->serverReceiveMax, &propertyLength, &serverReceiveMax, &pVariableHeader );
                    break;

                case MQTT_MAX_QOS_ID:
                    status = decodeuint8_t( &pConnackProperties->serverMaxQos, &propertyLength, &maxQos, &pVariableHeader );
                    break;

                case MQTT_RETAIN_AVAILABLE_ID:
                    status = decodeuint8_t( &pConnackProperties->retainAvailable, &propertyLength, &retain, &pVariableHeader );
                    break;

                case MQTT_MAX_PACKET_SIZE_ID:
                    status = decodeuint32_t( &pConnackProperties->serverMaxPacketSize, &propertyLength, &maxPacket, &pVariableHeader );
                    break;

                case MQTT_ASSIGNED_CLIENT_ID:
                    status = decodeutf_8( &pConnackProperties->pClientIdentifier, &pConnackProperties->clientIdLength, &propertyLength, &clientId, &pVariableHeader );
                    break;

                case MQTT_TOPIC_ALIAS_MAX_ID:
                    status = decodeuint16_t( &pConnackProperties->serverTopicAliasMax, &propertyLength, &topicAlias, &pVariableHeader );
                    break;

                case MQTT_REASON_STRING_ID:
                    status = decodeutf_8( &pConnackProperties->pReasonString, &pConnackProperties->reasonStringLength, &propertyLength, &reasonString, &pVariableHeader );
                    break;

                case MQTT_USER_PROPERTY_ID:
                    #if ( MQTT_USER_PROPERTY_ENABLED )
                        status = decodeutf_8pair( pConnackProperties->pIncomingUserProperty, &pConnackProperties->pIncomingUserProperty->count, &propertyLength, &pVariableHeader );
                    #else
                        status = decodeAndDiscard( &propertyLength, &pVariableHeader );
                    #endif
                    break;

                case MQTT_WILDCARD_ID:
                    status = decodeuint8_t( &pConnackProperties->isWildcardAvaiable, &propertyLength, &wildcard, &pVariableHeader );
                    break;

                case MQTT_SUB_AVAILABLE_ID:
                    status = decodeuint8_t( &pConnackProperties->subscriptionId, &propertyLength, &subId, &pVariableHeader );
                    break;

                case MQTT_SHARED_SUB_ID:
                    status = decodeuint8_t( &pConnackProperties->isSharedAvailable, &propertyLength, &sharedsub, &pVariableHeader );
                    break;

                case MQTT_SERVER_KEEP_ALIVE_ID:
                    status = decodeuint16_t( &pConnackProperties->serverKeepAlive, &propertyLength, &keepAlive, &pVariableHeader );
                    break;

                case MQTT_RESPONSE_INFO_ID:
                    status = decodeutf_8( &pConnackProperties->pResponseInfo, &pConnackProperties->responseInfoLength, &propertyLength, &responseInfo, &pVariableHeader );
                    break;

                case MQTT_SERVER_REF_ID:
                    status = decodeutf_8( &pConnackProperties->pServerRef, &pConnackProperties->serverRefLength, &propertyLength, &serverRef, &pVariableHeader );
                    break;

                case MQTT_AUTH_METHOD_ID:
                case MQTT_AUTH_DATA_ID:

                    status = decodeAuthInfo( pConnackProperties, &authMethod, &authData, &propertyLength, &pVariableHeader, packetId );
                    break;

                /*Protocol error to include any other property id.*/
                default:
                    status = MQTTProtocolError;
                    break;
            }
        }

        if( status == MQTTSuccess )
        {
            /*Receive max cannot be 0.*/
            if( ( serverReceiveMax == true ) && ( pConnackProperties->serverReceiveMax == 0U ) )
            {
                status = MQTTProtocolError;
            }

            /*Maximum packet size cannot be 0.*/
            else if( ( maxPacket == true ) && ( pConnackProperties->serverMaxPacketSize == 0U ) )
            {
                status = MQTTProtocolError;
            }
            /*Protocol error to send response information if the client has not requested it.*/
            else if( ( responseInfo == true ) && ( pConnackProperties->requestResponseInfo == false ) )
            {
                status = MQTTProtocolError;
            }

            else
            {
                /* MISRA Empty body */
            }
        }

        return status;
    }

    static MQTTStatus_t calculatePublishPacketSizeV5( MQTTPublishInfo_t * pPublishInfo,
                                                      size_t * pRemainingLength,
                                                      size_t * pPacketSize,
                                                      uint32_t maxPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t packetSize = 0, payloadLimit = 0;

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

        /*Calculate the size of the publish properties*/
        status = MQTT_GetPublishPropertiesSize( pPublishInfo );

        if( status == MQTTSuccess )
        {
            packetSize += pPublishInfo->propertyLength;
            packetSize += remainingLengthEncodedSize( pPublishInfo->propertyLength );

            /* Calculate the maximum allowed size of the payload for the given parameters.
             * This calculation excludes the "Remaining length" encoding, whose size is not
             * yet known. */
            payloadLimit = MQTT_MAX_REMAINING_LENGTH - packetSize - 1U;

            /* Ensure that the given payload fits within the calculated limit. */
            if( pPublishInfo->payloadLength > payloadLimit )
            {
                LogError( ( "PUBLISH payload length of %lu cannot exceed "
                            "%lu so as not to exceed the maximum "
                            "remaining length of MQTT 3.1.1 packet( %lu ).",
                            ( unsigned long ) pPublishInfo->payloadLength,
                            ( unsigned long ) payloadLimit,
                            MQTT_MAX_REMAINING_LENGTH ) );
                status = MQTTBadParameter;
            }
        }

        if( status == MQTTSuccess )
        {
            /* Add the length of the PUBLISH payload. At this point, the "Remaining length"
             * has been calculated. */
            packetSize += pPublishInfo->payloadLength;

            /* Now that the "Remaining length" is known, recalculate the payload limit
             * based on the size of its encoding. */
            payloadLimit -= remainingLengthEncodedSize( packetSize );

            /* Check that the given payload fits within the size allowed by MQTT spec. */
            if( pPublishInfo->payloadLength > payloadLimit )
            {
                LogError( ( "PUBLISH payload length of %lu cannot exceed "
                            "%lu so as not to exceed the maximum "
                            "remaining length of MQTT 3.1.1 packet( %lu ).",
                            ( unsigned long ) pPublishInfo->payloadLength,
                            ( unsigned long ) payloadLimit,
                            MQTT_MAX_REMAINING_LENGTH ) );
                status = MQTTBadParameter;
            }
            else
            {
                /* Set the "Remaining length" output parameter and calculate the full
                 * size of the PUBLISH packet. */
                *pRemainingLength = packetSize;

                packetSize += 1U + remainingLengthEncodedSize( packetSize );

                if( packetSize > maxPacketSize )
                {
                    LogError( ( "Packet size is greater than the allowed maximum packet size." ) );
                    status = MQTTBadParameter;
                }

                *pPacketSize = packetSize;
            }
        }

        LogDebug( ( "PUBLISH packet remaining length=%lu and packet size=%lu.",
                    ( unsigned long ) *pRemainingLength,
                    ( unsigned long ) *pPacketSize ) );

        return status;
    }

    static void serializePubAckPacketV5( const MQTTAckInfo_t * pAckInfo,
                                         uint8_t packetType,
                                         uint16_t packetId,
                                         size_t remainingLength,
                                         const MQTTFixedBuffer_t * pFixedBuffer )
    {
        uint8_t * pIndex = NULL;

        assert( pAckInfo != NULL );
        assert( pFixedBuffer != NULL );
        assert( pFixedBuffer->pBuffer != NULL );
        pIndex = pFixedBuffer->pBuffer;
        /* Serialize the header including reason code and property length */
        pIndex = MQTTV5_SerializeAckFixed( pIndex,
                                           packetType,
                                           packetId,
                                           remainingLength,
                                           pAckInfo->propertyLength );

        /*Serialize the reason string if provided.*/
        if( pAckInfo->reasonStringLength != 0U )
        {
            *pIndex = MQTT_REASON_STRING_ID;
            pIndex++;
            pIndex = encodeString( pIndex, pAckInfo->pReasonString, pAckInfo->reasonStringLength );
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            /*Serialize the user properties if provided.*/
            if( pAckInfo->pUserProperty != NULL )
            {
                uint32_t i = 0;
                uint32_t size = pAckInfo->pUserProperty->count;
                const MQTTUserProperty_t * pUserProperty = pAckInfo->pUserProperty->userProperty;

                for( ; i < size; i++ )
                {
                    *pIndex = MQTT_USER_PROPERTY_ID;
                    pIndex++;
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pKey, pUserProperty[ i ].keyLength );
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pValue, pUserProperty[ i ].valueLength );
                }
            }
        #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */

        /* Ensure that the difference between the end and beginning of the buffer
         * is less than the buffer size. */
        assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
    }

/* coverity[misra_c_2012_rule_2_7_violation] */
    static MQTTStatus_t logAckResponseV5( uint8_t reasonCode,
                                          uint16_t packetIdentifier )
    {
        MQTTStatus_t status = MQTTServerRefused;

        switch( reasonCode )
        {
            case MQTT_REASON_SUCCESS:
                status = MQTTSuccess;
                break;

            case MQTT_REASON_NO_MATCHING_SUBSCRIBERS:
                LogDebug( ( "Publish accepted with packet id %hu: No matching subscribers.",
                            ( unsigned short ) packetIdentifier ) );
                status = MQTTSuccess;
                break;

            case MQTT_REASON_UNSPECIFIED_ERR:
                LogError( ( "Publish refused with packet id %hu: Connection rate exceeded.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_IMPL_SPECIFIC_ERR:
                LogError( ( "Publish refused with packet id %hu:  The PUBLISH is valid but the receiver is not willing to accept it.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_NOT_AUTHORIZED:
                LogError( ( "Publish refused with packet id %hu: The PUBLISH is not authorized.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_TOPIC_NAME_INVALID:
                LogError( ( "Publish refused with packet id %hu: Topic Name not accepted.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_PACKET_ID_IN_USE:
                LogError( ( "Publish refused with packet id %hu: The Packet Identifier is already in use. ",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_QUOTA_EXCEEDED:
                LogError( ( "Publish refused with packet id %hu: Quota exceeded.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            case MQTT_REASON_PAYLOAD_FORMAT_INVALID:
                LogError( ( "Publish refused with packet id %hu: Payload format indicator is invalid.",
                            ( unsigned short ) packetIdentifier ) );
                break;

            default:
                status = MQTTProtocolError;
                break;
        }

        return status;
    }

/* coverity[misra_c_2012_rule_2_7_violation] */
    static MQTTStatus_t logSimpleAckResponseV5( uint8_t reasonCode,
                                                uint16_t packetIdentifier )
    {
        MQTTStatus_t status = MQTTServerRefused;

        switch( reasonCode )
        {
            case MQTT_REASON_SUCCESS:
                status = MQTTSuccess;
                break;

            case MQTT_REASON_PACKET_ID_NOT_FOUND:
                LogError( ( "Publish refused with packet id %hu: Packet identifier invalid.",
                            ( unsigned short ) packetIdentifier ) );
                status = MQTTServerRefused;
                break;

            default:
                status = MQTTProtocolError;
                break;
        }

        return status;
    }


    static MQTTStatus_t decodeAckProperties( MQTTAckInfo_t * pAckInfo,
                                             const uint8_t * pIndex,
                                             size_t remainingLength )
    {
        size_t propertyLength = 0U;
        MQTTStatus_t status = MQTTSuccess;
        const uint8_t * pLocalIndex = pIndex;

        /*Decode the property length*/
        status = decodeVariableLength( pLocalIndex, &propertyLength );

        if( status == MQTTSuccess )
        {
            pLocalIndex = &pLocalIndex[ remainingLengthEncodedSize( propertyLength ) ];

            /*Validate the remaining length.*/
            if( remainingLength != ( propertyLength + remainingLengthEncodedSize( propertyLength ) + 3U ) )
            {
                status = MQTTMalformedPacket;
            }
        }

        if( status == MQTTSuccess )
        {
            while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
            {
                /*Decode the property id.*/
                uint8_t packetId = *pLocalIndex;
                bool reasonString = false;
                pLocalIndex = &pLocalIndex[ 1 ];
                propertyLength -= sizeof( uint8_t );

                switch( packetId )
                {
                    case MQTT_REASON_STRING_ID:
                        status = decodeutf_8( &pAckInfo->pReasonString, &pAckInfo->reasonStringLength, &propertyLength, &reasonString, &pLocalIndex );
                        break;

                    case MQTT_USER_PROPERTY_ID:
                        #if ( MQTT_USER_PROPERTY_ENABLED )
                            status = decodeutf_8pair( pAckInfo->pUserProperty, &pAckInfo->pUserProperty->count, &propertyLength, &pLocalIndex );
                        #else
                            status = decodeAndDiscard( &propertyLength, &pLocalIndex );
                        #endif
                        break;

                    default:
                        status = MQTTProtocolError;
                        break;
                }
            }
        }

        return status;
    }

    static MQTTStatus_t deserializeSimpleAckV5( const MQTTPacketInfo_t * pAck,
                                                uint16_t * pPacketIdentifier,
                                                MQTTAckInfo_t * pAckInfo,
                                                bool requestProblem )
    {
        MQTTStatus_t status = MQTTSuccess;
        const uint8_t * pIndex = pAck->pRemainingData;

        if( pAck->remainingLength < 2U )
        {
            status = MQTTMalformedPacket;
        }

        if( status == MQTTSuccess )
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
            pAckInfo->reasonCode = *pIndex;
            pIndex++;
        }

        if( ( pAck->remainingLength > 4U ) )
        {
            /*Protocol error to send user property and reason string if client has set request problem to false.*/
            if( requestProblem == false )
            {
                status = MQTTProtocolError;
            }

            #if ( MQTT_USER_PROPERTY_ENABLED )
                else if( pAckInfo->pUserProperty == NULL )
                {
                    status = MQTTBadParameter;
                }
            #endif
            else
            {
                status = decodeAckProperties( pAckInfo, pIndex, pAck->remainingLength );
            }
        }

        return status;
    }


    static MQTTStatus_t calculateSubscriptionPacketSizeV5(MQTTSubscribeInfo_t *pSubscriptionList,
                                                     MQTTSubscribeProperties_t *pSubscribeProperties,
                                                     size_t subscriptionCount,
                                                     size_t *pRemainingLength,
                                                     size_t *pPacketSize)
    {
        size_t packetSize = 0U , i = 0; 
        MQTTStatus_t status = MQTTSuccess ; 
        assert( pSubscriptionList != NULL );
        assert( subscriptionCount != 0U );

        /*2 byte packet id*/
        packetSize += sizeof( uint16_t ) ; 
        /* Length of properties */

        status = MQTT_GetSubscribePropertiesSize(pSubscribeProperties) ; 
        LogDebug(("Property Length is %lu" , (unsigned long)pSubscribeProperties->propertyLength ));
        if( status == MQTTSuccess )
        {
            packetSize += pSubscribeProperties->propertyLength;
            packetSize += remainingLengthEncodedSize( pSubscribeProperties->propertyLength );
        }


        if(status == MQTTSuccess){
            for(i = 0 ; i < subscriptionCount ; i++){
                if((pSubscriptionList[i].topicFilterLength == 0U) || (pSubscriptionList[i].pTopicFilter== NULL)){
                    LogError(("Argument cannot be null : pTopicFilter")) ; 
                    status = MQTTBadParameter;
                    break ;
                }
                packetSize += pSubscriptionList[i].topicFilterLength + sizeof(uint16_t) + 1U;
            }
        }
        if( packetSize > MQTT_MAX_REMAINING_LENGTH )
        {
            LogError( ( "Subscribe packet size %lu exceeds %d. "
                        "Packet size cannot be greater than %d.",
                        ( unsigned long ) packetSize,
                        UINT16_MAX,
                        UINT16_MAX ) );
            status = MQTTBadParameter;
        }
        if(status == MQTTSuccess){
            *pRemainingLength = packetSize ; 
            packetSize += 1U + remainingLengthEncodedSize(packetSize) ;
            *pPacketSize = packetSize ; 

        }
        LogDebug( ( "Subscription packet remaining length=%lu and packet size=%lu.",
                    ( unsigned long ) *pRemainingLength,
                    ( unsigned long ) *pPacketSize ) );
        return status ; 
    }
    MQTTStatus_t MQTTV5_GetSubscribePacketSize(MQTTSubscribeInfo_t *pSubscriptionList,
                                            MQTTSubscribeProperties_t * pSubscribeProperties,
                                            size_t subscriptionCount,
                                            size_t *pRemainingLength,
                                            size_t *pPacketSize)
    {
        MQTTStatus_t status = MQTTSuccess ; 
        if(pSubscriptionList == NULL || pSubscribeProperties == NULL){
            LogError(("Argument cannot be null : pSubscriptionList, pSubscriptionProperties")); 
            status = MQTTBadParameter;
        }else if(subscriptionCount == 0U){
            LogError(("Subscription count cannot be 0")) ; 
            status = MQTTBadParameter;
        }else{
            status = calculateSubscriptionPacketSizeV5(pSubscriptionList, pSubscribeProperties, subscriptionCount, pRemainingLength, pPacketSize);
        }
        return status ; 
    }

    static uint8_t * serializePublishProperties( const MQTTPublishInfo_t * pPublishInfo,
                                                 uint8_t * pIndex )
    {
        uint8_t * pIndexLocal = pIndex;

        pIndexLocal = MQTT_SerializePublishProperties( pPublishInfo, pIndexLocal );

        /*Serialize the provided will properties which has variable length.*/
        if( pPublishInfo->contentTypeLength != 0U )
        {
            *pIndexLocal = MQTT_CONTENT_TYPE_ID;
            pIndexLocal++;
            pIndexLocal = encodeString( pIndexLocal, pPublishInfo->pContentType, pPublishInfo->contentTypeLength );
        }

        if( pPublishInfo->responseTopicLength != 0U )
        {
            *pIndexLocal = MQTT_RESPONSE_TOPIC_ID;
            pIndexLocal++;
            pIndexLocal = encodeString( pIndexLocal, pPublishInfo->pResponseTopic, pPublishInfo->responseTopicLength );
        }

        if( pPublishInfo->correlationLength != 0U )
        {
            *pIndexLocal = MQTT_CORRELATION_DATA_ID;
            pIndexLocal++;
            pIndexLocal = encodeString( pIndexLocal, pPublishInfo->pCorrelationData, pPublishInfo->correlationLength );
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            if( pPublishInfo->pUserProperty != NULL )
            {
                uint32_t i = 0;
                uint32_t size = pPublishInfo->pUserProperty->count;
                const MQTTUserProperty_t * pUserProperty = pPublishInfo->pUserProperty->userProperty;

                for( ; i < size; i++ )
                {
                    *pIndexLocal = MQTT_USER_PROPERTY_ID;
                    pIndexLocal++;
                    pIndexLocal = encodeString( pIndexLocal, pUserProperty[ i ].pKey, pUserProperty[ i ].keyLength );
                    pIndexLocal = encodeString( pIndexLocal, pUserProperty[ i ].pValue, pUserProperty[ i ].valueLength );
                }
            }
        #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */
        return pIndexLocal;
    }

    static MQTTStatus_t validateDisconnectResponseV5( uint8_t reasonCode,
                                                      bool incoming )
    {
        MQTTStatus_t status;

        /*Validate the reason code.*/
        switch( reasonCode )
        {
            case MQTT_REASON_SEND_WILL:

                if( incoming == true )
                {
                    status = MQTTProtocolError;
                }
                else
                {
                    status = MQTTSuccess;
                }

                break;

            case MQTT_REASON_SUCCESS:
            case MQTT_REASON_UNSPECIFIED_ERR:
            case MQTT_REASON_MALFORMED_PACKET:
            case MQTT_REASON_PROTOCOL_ERR:
            case MQTT_REASON_IMPL_SPECIFIC_ERR:
            case MQTT_REASON_TOPIC_NAME_INVALID:
            case MQTT_REASON_RX_MAX_EXCEEDED:
            case MQTT_REASON_TOPIC_ALIAS_INVALID:
            case MQTT_REASON_PACKET_TOO_LARGE:
            case MQTT_REASON_MSG_RATE_TOO_HIGH:
            case MQTT_REASON_QUOTA_EXCEEDED:
            case MQTT_REASON_ADMIN_ACTION:
            case MQTT_REASON_PAYLOAD_FORMAT_INVALID:
                status = MQTTSuccess;
                break;

            case MQTT_REASON_NOT_AUTHORIZED:
            case MQTT_REASON_SERVER_BUSY:
            case MQTT_REASON_SERVER_SHUTTING_DOWN:
            case MQTT_REASON_KEEP_ALIVE_TIMEOUT:
            case MQTT_REASON_SESSION_TAKEN_OVER:
            case MQTT_REASON_TOPIC_FILTER_INVALID:
            case MQTT_REASON_RETAIN_NOT_SUPPORTED:
            case MQTT_REASON_QOS_NOT_SUPPORTED:
            case MQTT_REASON_USE_ANOTHER_SERVER:
            case MQTT_REASON_SERVER_MOVED:
            case MQTT_REASON_MAX_CON_TIME:
            case MQTT_REASON_SS_NOT_SUPPORTED:
            case MQTT_REASON_WILDCARD_SUB_NOT_SUP:

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
                status = MQTTProtocolError;
                break;
        }

        return status;
    }

#endif /* if ( MQTT_VERSION_5_ENABLED ) */
/*-----------------------------------------------------------*/

static size_t remainingLengthEncodedSize( size_t length )
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 3.1.1 spec. */

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

static uint8_t * encodeRemainingLength( uint8_t * pDestination,
                                        size_t length )
{
    uint8_t lengthByte;
    uint8_t * pLengthEnd = NULL;
    size_t remainingLength = length;

    assert( pDestination != NULL );

    pLengthEnd = pDestination;

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
    do
    {
        lengthByte = ( uint8_t ) ( remainingLength % 128U );
        remainingLength = remainingLength / 128U;

        /* Set the high bit of this byte, indicating that there's more data. */
        if( remainingLength > 0U )
        {
            UINT8_SET_BIT( lengthByte, 7 );
        }

        /* Output a single encoded byte. */
        *pLengthEnd = lengthByte;
        pLengthEnd++;
    } while( remainingLength > 0U );

    return pLengthEnd;
}

/*-----------------------------------------------------------*/

static uint8_t * encodeString( uint8_t * pDestination,
                               const char * pSource,
                               uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    /* Typecast const char * typed source buffer to const uint8_t *.
     * This is to use same type buffers in memcpy. */
    const uint8_t * pSourceBuffer = ( const uint8_t * ) pSource;

    assert( pDestination != NULL );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */
    if( pSourceBuffer != NULL )
    {
        ( void ) memcpy( pBuffer, pSourceBuffer, sourceLength );
    }

    /* Return the pointer to the end of the encoded string. */
    pBuffer = &pBuffer[ sourceLength ];

    return pBuffer;
}

/*-----------------------------------------------------------*/

static bool calculatePublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
{
    bool status = true;
    size_t packetSize = 0, payloadLimit = 0;

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

    /* Calculate the maximum allowed size of the payload for the given parameters.
     * This calculation excludes the "Remaining length" encoding, whose size is not
     * yet known. */
    payloadLimit = MQTT_MAX_REMAINING_LENGTH - packetSize - 1U;

    /* Ensure that the given payload fits within the calculated limit. */
    if( pPublishInfo->payloadLength > payloadLimit )
    {
        LogError( ( "PUBLISH payload length of %lu cannot exceed "
                    "%lu so as not to exceed the maximum "
                    "remaining length of MQTT 3.1.1 packet( %lu ).",
                    ( unsigned long ) pPublishInfo->payloadLength,
                    ( unsigned long ) payloadLimit,
                    MQTT_MAX_REMAINING_LENGTH ) );
        status = false;
    }
    else
    {
        /* Add the length of the PUBLISH payload. At this point, the "Remaining length"
         * has been calculated. */
        packetSize += pPublishInfo->payloadLength;

        /* Now that the "Remaining length" is known, recalculate the payload limit
         * based on the size of its encoding. */
        payloadLimit -= remainingLengthEncodedSize( packetSize );

        /* Check that the given payload fits within the size allowed by MQTT spec. */
        if( pPublishInfo->payloadLength > payloadLimit )
        {
            LogError( ( "PUBLISH payload length of %lu cannot exceed "
                        "%lu so as not to exceed the maximum "
                        "remaining length of MQTT 3.1.1 packet( %lu ).",
                        ( unsigned long ) pPublishInfo->payloadLength,
                        ( unsigned long ) payloadLimit,
                        MQTT_MAX_REMAINING_LENGTH ) );
            status = false;
        }
        else
        {
            /* Set the "Remaining length" output parameter and calculate the full
             * size of the PUBLISH packet. */
            *pRemainingLength = packetSize;

            packetSize += 1U + remainingLengthEncodedSize( packetSize );
            *pPacketSize = packetSize;
        }
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
    headerLength = 1U + remainingLengthEncodedSize( remainingLength ) + 2U;

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
    pIndex = encodeRemainingLength( pIndex, remainingLength );

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
                                    size_t remainingLength,
                                    uint16_t packetIdentifier,
                                    const MQTTFixedBuffer_t * pFixedBuffer,
                                    bool serializePayload )
{
    uint8_t * pIndex = NULL;
    const uint8_t * pPayloadBuffer = NULL;

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
    pIndex = encodeRemainingLength( pIndex, remainingLength );

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

    #if ( MQTT_VERSION_5_ENABLED )
        pIndex = serializePublishProperties( pPublishInfo, pIndex );
    #endif

    /* The payload is placed after the packet identifier.
     * Payload is copied over only if required by the flag serializePayload.
     * This will help reduce an unnecessary copy of the payload into the buffer.
     */
    if( ( pPublishInfo->payloadLength > 0U ) &&
        ( serializePayload == true ) )
    {
        LogDebug( ( "Copying PUBLISH payload of length =%lu to buffer",
                    ( unsigned long ) pPublishInfo->payloadLength ) );

        /* Typecast const void * typed payload buffer to const uint8_t *.
         * This is to use same type buffers in memcpy. */
        pPayloadBuffer = ( const uint8_t * ) pPublishInfo->pPayload;

        ( void ) memcpy( pIndex, pPayloadBuffer, pPublishInfo->payloadLength );
        /* Move the index to after the payload. */
        pIndex = &pIndex[ pPublishInfo->payloadLength ];
    }

    /* Ensure that the difference between the end and beginning of the buffer
     * is less than the buffer size. */
    assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
}

static size_t getRemainingLength( TransportRecv_t recvFunc,
                                  NetworkContext_t * pNetworkContext )
{
    size_t remainingLength = 0, multiplier = 1, bytesDecoded = 0, expectedSize = 0;
    uint8_t encodedByte = 0;
    int32_t bytesReceived = 0;

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
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
        expectedSize = remainingLengthEncodedSize( remainingLength );

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

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
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
        expectedSize = remainingLengthEncodedSize( remainingLength );

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
            status = true;
            break;

        case ( MQTT_PACKET_TYPE_PUBREL & 0xF0U ):

            /* The second bit of a PUBREL must be set. */
            if( ( packetType & 0x02U ) > 0U )
            {
                status = true;
            }

            break;

        case MQTT_PACKET_TYPE_DISCONNECT:
            #if ( MQTT_VERSION_5_ENABLED )
                status = true;
            #else
                status = false;
            #endif
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

static void logConnackResponse( uint8_t responseCode )
{
    const char * const pConnackResponses[ 6 ] =
    {
        "Connection accepted.",                               /* 0 */
        "Connection refused: unacceptable protocol version.", /* 1 */
        "Connection refused: identifier rejected.",           /* 2 */
        "Connection refused: server unavailable",             /* 3 */
        "Connection refused: bad user name or password.",     /* 4 */
        "Connection refused: not authorized."                 /* 5 */
    };

    /* Avoid unused parameter warning when assert and logs are disabled. */
    ( void ) responseCode;
    ( void ) pConnackResponses;

    assert( responseCode <= 5U );

    if( responseCode == 0u )
    {
        /* Log at Debug level for a success CONNACK response. */
        LogDebug( ( "%s", pConnackResponses[ 0 ] ) );
    }
    else
    {
        /* Log an error based on the CONNACK response code. */
        LogError( ( "%s", pConnackResponses[ responseCode ] ) );
    }
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeConnack( const MQTTPacketInfo_t * pConnack,
                                        bool * pSessionPresent )
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pRemainingData = NULL;

    assert( pConnack != NULL );
    assert( pSessionPresent != NULL );
    pRemainingData = pConnack->pRemainingData;

    /* According to MQTT 3.1.1, the second byte of CONNACK must specify a
     * "Remaining length" of 2. */
    if( pConnack->remainingLength != MQTT_PACKET_CONNACK_REMAINING_LENGTH )
    {
        LogError( ( "CONNACK does not have remaining length of %u.",
                    ( unsigned int ) MQTT_PACKET_CONNACK_REMAINING_LENGTH ) );

        status = MQTTBadResponse;
    }

    /* Check the reserved bits in CONNACK. The high 7 bits of the third byte
     * in CONNACK must be 0. */
    else if( ( pRemainingData[ 0 ] | 0x01U ) != 0x01U )
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

            /* MQTT 3.1.1 specifies that the fourth byte in CONNACK must be 0 if the
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

    if( status == MQTTSuccess )
    {
        /* In MQTT 3.1.1, only values 0 through 5 are valid CONNACK response codes. */
        if( pRemainingData[ 1 ] > 5U )
        {
            LogError( ( "CONNACK response %u is invalid.",
                        ( unsigned int ) pRemainingData[ 1 ] ) );

            status = MQTTBadResponse;
        }
        else
        {
            /* Print the appropriate message for the CONNACK response code if logs are
             * enabled. */
            logConnackResponse( pRemainingData[ 1 ] );

            /* A nonzero CONNACK response code means the connection was refused. */
            if( pRemainingData[ 1 ] > 0U )
            {
                status = MQTTServerRefused;
            }
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t calculateSubscriptionPacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                     size_t subscriptionCount,
                                                     size_t * pRemainingLength,
                                                     size_t * pPacketSize,
                                                     MQTTSubscriptionType_t subscriptionType )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i = 0, packetSize = 0;

    assert( pSubscriptionList != NULL );
    assert( subscriptionCount != 0U );
    assert( pRemainingLength != NULL );
    assert( pPacketSize != NULL );

    /* The variable header of a subscription packet consists of a 2-byte packet
     * identifier. */
    packetSize += sizeof( uint16_t );

    /* Sum the lengths of all subscription topic filters; add 1 byte for each
     * subscription's QoS if type is MQTT_SUBSCRIBE. */
    for( i = 0; i < subscriptionCount; i++ )
    {
        /* Add the length of the topic filter. MQTT strings are prepended
         * with 2 byte string length field. Hence 2 bytes are added to size. */
        packetSize += pSubscriptionList[ i ].topicFilterLength + sizeof( uint16_t );

        /* Only SUBSCRIBE packets include the QoS. */
        if( subscriptionType == MQTT_SUBSCRIBE )
        {
            packetSize += 1U;
        }

        /* Validate each topic filter. */
        if( ( pSubscriptionList[ i ].topicFilterLength == 0U ) ||
            ( pSubscriptionList[ i ].pTopicFilter == NULL ) )
        {
            status = MQTTBadParameter;
            LogError( ( "Subscription #%lu in %sSUBSCRIBE packet cannot be empty.",
                        ( unsigned long ) i,
                        ( subscriptionType == MQTT_SUBSCRIBE ) ? "" : "UN" ) );
            /* It is not necessary to break as an error status has already been set. */
        }
    }

    /* At this point, the "Remaining length" has been calculated. Return error
     * if the "Remaining length" exceeds what is allowed by MQTT 3.1.1. Otherwise,
     * set the output parameter.*/
    if( packetSize > MQTT_MAX_REMAINING_LENGTH )
    {
        LogError( ( "Subscription packet length of %lu exceeds"
                    "the MQTT 3.1.1 maximum packet length of %lu.",
                    ( unsigned long ) packetSize,
                    MQTT_MAX_REMAINING_LENGTH ) );
        status = MQTTBadParameter;
    }

    if( status == MQTTSuccess )
    {
        *pRemainingLength = packetSize;

        /* Calculate the full size of the subscription packet by adding
         * number of bytes required to encode the "Remaining length" field
         * plus 1 byte for the "Packet type" field. */
        packetSize += 1U + remainingLengthEncodedSize( packetSize );

        /*Set the pPacketSize output parameter. */
        *pPacketSize = packetSize;
    }

    LogDebug( ( "Subscription packet remaining length=%lu and packet size=%lu.",
                ( unsigned long ) *pRemainingLength,
                ( unsigned long ) *pPacketSize ) );

    return status;
}


/*-----------------------------------------------------------*/

static MQTTStatus_t readSubackStatus( size_t statusCount,
                                      const uint8_t * pStatusStart )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t subscriptionStatus = 0;
    size_t i = 0;

    assert( pStatusStart != NULL );

    /* Iterate through each status byte in the SUBACK packet. */
    for( i = 0; i < statusCount; i++ )
    {
        /* Read a single status byte in SUBACK. */
        subscriptionStatus = pStatusStart[ i ];

        /* MQTT 3.1.1 defines the following values as status codes. */
        switch( subscriptionStatus )
        {
            case 0x00:
            case 0x01:
            case 0x02:

                LogDebug( ( "Topic filter %lu accepted, max QoS %u.",
                            ( unsigned long ) i,
                            ( unsigned int ) subscriptionStatus ) );
                break;

            case 0x80:

                LogWarn( ( "Topic filter %lu refused.", ( unsigned long ) i ) );

                /* Application should remove subscription from the list */
                status = MQTTServerRefused;

                break;

            default:
                LogError( ( "Bad SUBSCRIBE status %u.",
                            ( unsigned int ) subscriptionStatus ) );

                status = MQTTBadResponse;

                break;
        }

        /* Stop parsing the subscription statuses if a bad response was received. */
        if( status == MQTTBadResponse )
        {
            break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSuback( const MQTTPacketInfo_t * pSuback,
                                       uint16_t * pPacketIdentifier )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t remainingLength;
    const uint8_t * pVariableHeader = NULL;

    assert( pSuback != NULL );
    assert( pPacketIdentifier != NULL );

    remainingLength = pSuback->remainingLength;
    pVariableHeader = pSuback->pRemainingData;

    /* A SUBACK must have a remaining length of at least 3 to accommodate the
     * packet identifier and at least 1 return code. */
    if( remainingLength < 3U )
    {
        LogError( ( "SUBACK cannot have a remaining length less than 3." ) );
        status = MQTTBadResponse;
    }
    else
    {
        /* Extract the packet identifier (first 2 bytes of variable header) from SUBACK. */
        *pPacketIdentifier = UINT16_DECODE( pVariableHeader );

        LogDebug( ( "Packet identifier %hu.",
                    ( unsigned short ) *pPacketIdentifier ) );

        if( *pPacketIdentifier == 0U )
        {
            status = MQTTBadResponse;
        }
        else
        {
            status = readSubackStatus( remainingLength - sizeof( uint16_t ),
                                       &pVariableHeader[ sizeof( uint16_t ) ] );
        }
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
        packetSize = 1U + remainingLengthEncodedSize( remainingLength ) + remainingLength;

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

static MQTTStatus_t deserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo )
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pVariableHeader, * pPacketIdentifierHigh = NULL;

    assert( pIncomingPacket != NULL );
    assert( pPacketId != NULL );
    assert( pPublishInfo != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );

    pVariableHeader = pIncomingPacket->pRemainingData;
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
                                              MQTT_MIN_PUBLISH_REMAINING_LENGTH_QOS0 );
    }

    if( status == MQTTSuccess )
    {
        /* Extract the topic name starting from the first byte of the variable header.
         * The topic name string starts at byte 3 in the variable header. */
        pPublishInfo->topicNameLength = UINT16_DECODE( pVariableHeader );

        /* Sanity checks for topic name length and "Remaining length". The remaining
         * length must be at least as large as the variable length header. */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
                                              pPublishInfo->qos,
                                              pPublishInfo->topicNameLength + sizeof( uint16_t ) );
    }

    if( status == MQTTSuccess )
    {
        /* Parse the topic. */
        pPublishInfo->pTopicName = ( const char * ) ( &pVariableHeader[ sizeof( uint16_t ) ] );
        LogDebug( ( "Topic name length: %hu.", ( unsigned short ) pPublishInfo->topicNameLength ) );

        /* Extract the packet identifier for QoS 1 or 2 PUBLISH packets. Packet
         * identifier starts immediately after the topic name. */
        pPacketIdentifierHigh = ( const uint8_t * ) ( &pPublishInfo->pTopicName[ pPublishInfo->topicNameLength ] );

        if( pPublishInfo->qos > MQTTQoS0 )
        {
            *pPacketId = UINT16_DECODE( pPacketIdentifierHigh );

            LogDebug( ( "Packet identifier %hu.",
                        ( unsigned short ) *pPacketId ) );

            /* Advance pointer two bytes to start of payload as in the QoS 0 case. */
            pPacketIdentifierHigh = &pPacketIdentifierHigh[ sizeof( uint16_t ) ];

            /* Packet identifier cannot be 0. */
            if( *pPacketId == 0U )
            {
                LogError( ( "Packet identifier cannot be 0." ) );
                status = MQTTBadResponse;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        /* Calculate the length of the payload. QoS 1 or 2 PUBLISH packets contain
         * a packet identifier, but QoS 0 PUBLISH packets do not. */
        pPublishInfo->payloadLength = pIncomingPacket->remainingLength - pPublishInfo->topicNameLength - sizeof( uint16_t );

        if( pPublishInfo->qos != MQTTQoS0 )
        {
            /* Two more bytes for the packet identifier. */
            pPublishInfo->payloadLength -= sizeof( uint16_t );
        }

        /* Set payload if it exists. */
        pPublishInfo->pPayload = ( pPublishInfo->payloadLength != 0U ) ? pPacketIdentifierHigh : NULL;

        LogDebug( ( "Payload length %lu.",
                    ( unsigned long ) pPublishInfo->payloadLength ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSimpleAck( const MQTTPacketInfo_t * pAck,
                                          uint16_t * pPacketIdentifier )
{
    MQTTStatus_t status = MQTTSuccess;

    assert( pAck != NULL );
    assert( pPacketIdentifier != NULL );

    /* Check that the "Remaining length" of the received ACK is 2. */
    if( pAck->remainingLength != MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH )
    {
        LogError( ( "ACK does not have remaining length of %u.",
                    ( unsigned int ) MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH ) );

        status = MQTTBadResponse;
    }
    else
    {
        /* Extract the packet identifier (third and fourth bytes) from ACK. */
        *pPacketIdentifier = UINT16_DECODE( pAck->pRemainingData );

        LogDebug( ( "Packet identifier %hu.",
                    ( unsigned short ) *pPacketIdentifier ) );

        /* Packet identifier cannot be 0. */
        if( *pPacketIdentifier == 0U )
        {
            LogError( ( "Packet identifier cannot be 0." ) );
            status = MQTTBadResponse;
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
    pIndexLocal = encodeRemainingLength( pIndexLocal, remainingLength );

    /* The string "MQTT" is placed at the beginning of the CONNECT packet's variable
     * header. This string is 4 bytes long. */
    pIndexLocal = encodeString( pIndexLocal, "MQTT", 4 );

    /* The MQTT protocol version is the second field of the variable header. */
    #if ( MQTT_VERSION_5_ENABLED )
        *pIndexLocal = MQTT_VERSION_5;
    #else
        *pIndexLocal = MQTT_VERSION_3_1_1;
    #endif
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
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    uint8_t * pIndex = NULL;

    assert( pConnectInfo != NULL );
    assert( pFixedBuffer != NULL );
    assert( pFixedBuffer->pBuffer != NULL );

    pIndex = pFixedBuffer->pBuffer;

    /* Serialize the header. */
    pIndex = MQTT_SerializeConnectFixedHeader( pIndex,
                                               pConnectInfo,
                                               pWillInfo,
                                               remainingLength );

    /* Write the client identifier into the CONNECT packet. */
    pIndex = encodeString( pIndex,
                           pConnectInfo->pClientIdentifier,
                           pConnectInfo->clientIdentifierLength );

    /* Write the will topic name and message into the CONNECT packet if provided. */
    if( pWillInfo != NULL )
    {
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
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t remainingLength;

    /* The CONNECT packet will always include a 10-byte variable header. */
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
         * the will message length is required to be represented in 2 bytes.
         * By bounding the payloadLength of the will message, the CONNECT
         * packet will never be larger than 327699 bytes. */
        LogError( ( "The Will Message length must not exceed %d. "
                    "pWillInfo->payloadLength=%lu.",
                    UINT16_MAX,
                    ( unsigned long ) pWillInfo->payloadLength ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Add the length of the client identifier. */
        connectPacketSize += pConnectInfo->clientIdentifierLength + sizeof( uint16_t );

        /* Add the lengths of the will message and topic name if provided. */
        if( pWillInfo != NULL )
        {
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
        connectPacketSize += 1U + remainingLengthEncodedSize( connectPacketSize );

        /* The connectPacketSize calculated from this function's parameters is
         * guaranteed to be less than the maximum MQTT CONNECT packet size, which
         * is 327700. If the maximum client identifier length, the maximum will
         * message topic length, the maximum will topic payload length, the
         * maximum username length, and the maximum password length are all present
         * in the MQTT CONNECT packet, the total size will be calculated to be
         * 327699:
         * (variable length header)10 +
         * (maximum client identifier length) 65535 + (encoded length) 2 +
         * (maximum will message topic name length) 65535 + (encoded length)2 +
         * (maximum will message payload length) 65535 + 2 +
         * (maximum username length) 65535 + (encoded length) 2 +
         * (maximum password length) 65535 + (encoded length) 2 +
         * (packet type field length) 1 +
         * (CONNECT packet encoded length) 3 = 327699 */

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
        connectPacketSize = remainingLength + remainingLengthEncodedSize( remainingLength ) + 1U;

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
                                    remainingLength,
                                    pFixedBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                          size_t subscriptionCount,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

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
        LogError( ( "subscriptionCount is 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Calculate the MQTT SUBSCRIBE packet size. */
        status = calculateSubscriptionPacketSize( pSubscriptionList,
                                                  subscriptionCount,
                                                  pRemainingLength,
                                                  pPacketSize,
                                                  MQTT_SUBSCRIBE );
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
    pIterator = encodeRemainingLength( pIterator, remainingLength );

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
    pIterator = encodeRemainingLength( pIterator, remainingLength );

    /* Place the packet identifier into the SUBSCRIBE packet. */
    pIterator[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIterator[ 1 ] = UINT16_LOW_BYTE( packetId );
    /* Increment the pointer. */
    pIterator = &pIterator[ 2 ];

    return pIterator;
}

MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                      size_t subscriptionCount,
                                      uint16_t packetId,
                                      size_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer )
{
    size_t i = 0;
    uint8_t * pIndex = NULL;

    /* Validate all the parameters. */
    MQTTStatus_t status =
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

        /* Serialize each subscription topic filter and QoS. */
        for( i = 0; i < subscriptionCount; i++ )
        {
            pIndex = encodeString( pIndex,
                                   pSubscriptionList[ i ].pTopicFilter,
                                   pSubscriptionList[ i ].topicFilterLength );

            /* Place the QoS in the SUBSCRIBE packet. */
            *pIndex = ( uint8_t ) ( pSubscriptionList[ i ].qos );
            pIndex++;
        }

        LogDebug( ( "Length of serialized SUBSCRIBE packet is %lu.",
                    ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

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
                                                  MQTT_UNSUBSCRIBE );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i = 0;
    uint8_t * pIndex = NULL;

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
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPublishInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pPublishInfo=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( void * ) pPublishInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
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
    else
    {
        /* Calculate the "Remaining length" field and total packet size. If it exceeds
         * what is allowed in the MQTT standard, return an error. */
        if( calculatePublishPacketSize( pPublishInfo, pRemainingLength, pPacketSize ) == false )
        {
            LogError( ( "PUBLISH packet remaining length exceeds %lu, which is the "
                        "maximum size allowed by MQTT 3.1.1.",
                        MQTT_MAX_REMAINING_LENGTH ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
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

    #if ( !MQTT_VERSION_5_ENABLED )
        else if( ( pPublishInfo->pTopicName == NULL ) || ( pPublishInfo->topicNameLength == 0U ) )
        {
            LogError( ( "Invalid topic name for PUBLISH: pTopicName=%p, "
                        "topicNameLength=%hu.",
                        ( void * ) pPublishInfo->pTopicName,
                        ( unsigned short ) pPublishInfo->topicNameLength ) );
            status = MQTTBadParameter;
        }
    #endif
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
        packetSize = 1U + remainingLengthEncodedSize( remainingLength ) + remainingLength;
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
                                remainingLength,
                                packetId,
                                pFixedBuffer,
                                true );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublishHeader( const MQTTPublishInfo_t * pPublishInfo,
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
        packetSize = 1U + remainingLengthEncodedSize( remainingLength ) + remainingLength - pPublishInfo->payloadLength;
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

MQTTStatus_t MQTT_GetDisconnectPacketSize( size_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPacketSize == NULL )
    {
        LogError( ( "pPacketSize is NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* MQTT DISCONNECT packets always have the same size. */
        *pPacketSize = MQTT_DISCONNECT_PACKET_SIZE;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeDisconnect( const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;

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
        /* Empty else MISRA 15.7 */
    }

    if( status == MQTTSuccess )
    {
        if( pFixedBuffer->size < MQTT_DISCONNECT_PACKET_SIZE )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized DISCONNECT packet of size of %lu.",
                        ( unsigned long ) pFixedBuffer->size,
                        MQTT_DISCONNECT_PACKET_SIZE ) );
            status = MQTTNoMemory;
        }
    }

    if( status == MQTTSuccess )
    {
        pFixedBuffer->pBuffer[ 0 ] = MQTT_PACKET_TYPE_DISCONNECT;
        pFixedBuffer->pBuffer[ 1 ] = MQTT_DISCONNECT_REMAINING_LENGTH;
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

MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                      uint16_t * pPacketId,
                                      MQTTPublishInfo_t * pPublishInfo )
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
    else
    {
        status = deserializePublish( pIncomingPacket, pPacketId, pPublishInfo );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId,
                                  bool * pSessionPresent )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pIncomingPacket == NULL )
    {
        LogError( ( "pIncomingPacket cannot be NULL." ) );
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
    else
    {
        /* Make sure response packet is a valid ack. */
        switch( pIncomingPacket->type )
        {
            case MQTT_PACKET_TYPE_CONNACK:
                status = deserializeConnack( pIncomingPacket, pSessionPresent );
                break;

            case MQTT_PACKET_TYPE_SUBACK:
                status = deserializeSuback( pIncomingPacket, pPacketId );
                break;

            case MQTT_PACKET_TYPE_PINGRESP:
                status = deserializePingresp( pIncomingPacket );
                break;

            case MQTT_PACKET_TYPE_UNSUBACK:
            case MQTT_PACKET_TYPE_PUBACK:
            case MQTT_PACKET_TYPE_PUBREC:
            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                status = deserializeSimpleAck( pIncomingPacket, pPacketId );
                break;

            /* Any other packet type is invalid. */
            default:
                LogError( ( "IotMqtt_DeserializeResponse() called with unknown packet type:(%02x).",
                            ( unsigned int ) pIncomingPacket->type ) );
                status = MQTTBadResponse;
                break;
        }
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

#if ( MQTT_VERSION_5_ENABLED )

    MQTTStatus_t MQTTV5_InitConnect( MQTTConnectProperties_t * pConnectProperties )
    {
        MQTTStatus_t status = MQTTSuccess;

        if( pConnectProperties == NULL )
        {
            status = MQTTBadParameter;
        }
        else
        {
            pConnectProperties->receiveMax = UINT16_MAX;
            pConnectProperties->maxPacketSize = MQTT_MAX_PACKET_SIZE;
            pConnectProperties->requestProblemInfo = true;
            pConnectProperties->serverReceiveMax = UINT16_MAX;
            pConnectProperties->serverMaxQos = 1U;
            pConnectProperties->serverMaxPacketSize = MQTT_MAX_PACKET_SIZE;
            pConnectProperties->isWildcardAvaiable = 1U;
            pConnectProperties->subscriptionId = 1U;
            pConnectProperties->isSharedAvailable = 1U;
        }

        return status;
    }

    MQTTStatus_t MQTTV5_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                              MQTTPublishInfo_t * pWillInfo,
                                              MQTTConnectProperties_t * pConnectProperties,
                                              size_t * pRemainingLength,
                                              size_t * pPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t remainingLength;

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
        else if( pConnectProperties == NULL )
        {
            LogError( ( "Argument cannot be NULL: connectProperties" ) );
            status = MQTTBadParameter;
        }
        else if( ( pConnectProperties->pOutgoingAuth != NULL ) && ( pConnectProperties->pIncomingAuth == NULL ) )
        {
            LogError( ( "Incoming Auth cannot be NULL" ) );
            status = MQTTBadParameter;
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            else if( pConnectProperties->pIncomingUserProperty == NULL )
            {
                LogError( ( "Incoming user property cannot be NULL" ) );
                status = MQTTBadParameter;
            }
        #endif
        else
        {
            /* Add the connect properties size. */
            status = MQTT_GetConnectPropertiesSize( pConnectProperties );
        }

        if( status == MQTTSuccess )
        {
            connectPacketSize += pConnectProperties->propertyLength;
            connectPacketSize += remainingLengthEncodedSize( pConnectProperties->propertyLength );
            /* Add the length of the client identifier. */
            connectPacketSize += pConnectInfo->clientIdentifierLength + sizeof( uint16_t );

            /* Add the lengths of the will properties if provided. */
            if( pWillInfo != NULL )
            {
                status = MQTT_GetPublishPropertiesSize( pWillInfo );
            }
        }

        if( status == MQTTSuccess )
        {
            /* Add the lengths of the will message and topic name if provided. */
            if( pWillInfo != NULL )
            {
                connectPacketSize += pWillInfo->propertyLength;
                connectPacketSize += remainingLengthEncodedSize( pWillInfo->propertyLength );
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
            connectPacketSize += 1U + remainingLengthEncodedSize( connectPacketSize );

            /**
             * 268,435,455 - max remaining length  according to spec MQTT-v5;
             *
             * */
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

    uint8_t * MQTT_SerializePublishProperties( const MQTTPublishInfo_t * pPublishInfo,
                                               uint8_t * pIndex )
    {
        uint8_t * pIndexLocal = pIndex;

        pIndexLocal = encodeRemainingLength( pIndexLocal, pPublishInfo->propertyLength );

        /*Serialize the will delay if provided.*/

        if( pPublishInfo->willDelay != 0U )
        {
            *pIndexLocal = MQTT_WILL_DELAY_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT32_BYTE3( pPublishInfo->willDelay );
            pIndexLocal[ 1 ] = UINT32_BYTE2( pPublishInfo->willDelay );
            pIndexLocal[ 2 ] = UINT32_BYTE1( pPublishInfo->willDelay );
            pIndexLocal[ 3 ] = UINT32_BYTE0( pPublishInfo->willDelay );
            pIndexLocal = &pIndexLocal[ 4 ];
        }

        /*Serialize the topic alias if provided*/

        if( pPublishInfo->topicAlias != 0U )
        {
            *pIndexLocal = MQTT_TOPIC_ALIAS_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( pPublishInfo->topicAlias );
            pIndexLocal[ 1 ] = UINT16_LOW_BYTE( pPublishInfo->topicAlias );
            pIndexLocal = &pIndexLocal[ 2 ];
        }

        /*Serialize the payload format if provided.*/

        if( pPublishInfo->payloadFormat != 0U )
        {
            *pIndexLocal = MQTT_PAYLOAD_FORMAT_ID;
            pIndexLocal++;
            *pIndexLocal = pPublishInfo->payloadFormat;
            pIndexLocal++;
        }

        /*Serialize the message expiry if provided.*/

        if( pPublishInfo->msgExpiryPresent != false )
        {
            *pIndexLocal = MQTT_MSG_EXPIRY_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT32_BYTE3( pPublishInfo->msgExpiryInterval );
            pIndexLocal[ 1 ] = UINT32_BYTE2( pPublishInfo->msgExpiryInterval );
            pIndexLocal[ 2 ] = UINT32_BYTE1( pPublishInfo->msgExpiryInterval );
            pIndexLocal[ 3 ] = UINT32_BYTE0( pPublishInfo->msgExpiryInterval );
            pIndexLocal = &pIndexLocal[ 4 ];
        }

        return pIndexLocal;
    }

    uint8_t * MQTTV5_SerializeConnectProperties( uint8_t * pIndex,
                                                 const MQTTConnectProperties_t * pConnectProperties )
    {
        uint8_t * pIndexLocal = pIndex;

        pIndexLocal = encodeRemainingLength( pIndexLocal, pConnectProperties->propertyLength );

        /*Serialize session expiry if provided.*/
        if( pConnectProperties->sessionExpiry != 0U )
        {
            *pIndexLocal = MQTT_SESSION_EXPIRY_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT32_BYTE3( pConnectProperties->sessionExpiry );
            pIndexLocal[ 1 ] = UINT32_BYTE2( pConnectProperties->sessionExpiry );
            pIndexLocal[ 2 ] = UINT32_BYTE1( pConnectProperties->sessionExpiry );
            pIndexLocal[ 3 ] = UINT32_BYTE0( pConnectProperties->sessionExpiry );
            pIndexLocal = &pIndexLocal[ 4 ];
        }

        /*Serialize receive max  if provided.*/

        if( pConnectProperties->receiveMax != ( uint16_t ) UINT16_MAX )
        {
            *pIndexLocal = MQTT_RECEIVE_MAX_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( pConnectProperties->receiveMax );
            pIndexLocal[ 1 ] = UINT16_LOW_BYTE( pConnectProperties->receiveMax );
            pIndexLocal = &pIndexLocal[ 2 ];
        }

        /*Serialize the max packet size  if provided.*/

        if( pConnectProperties->maxPacketSize != MQTT_MAX_PACKET_SIZE )
        {
            *pIndexLocal = MQTT_MAX_PACKET_SIZE_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT32_BYTE3( pConnectProperties->maxPacketSize );
            pIndexLocal[ 1 ] = UINT32_BYTE2( pConnectProperties->maxPacketSize );
            pIndexLocal[ 2 ] = UINT32_BYTE1( pConnectProperties->maxPacketSize );
            pIndexLocal[ 3 ] = UINT32_BYTE0( pConnectProperties->maxPacketSize );
            pIndexLocal = &pIndexLocal[ 4 ];
        }

        /*Serialize the topic alias if provided.*/

        if( pConnectProperties->topicAliasMax != 0U )
        {
            *pIndexLocal = MQTT_TOPIC_ALIAS_MAX_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( pConnectProperties->topicAliasMax );
            pIndexLocal[ 1 ] = UINT16_LOW_BYTE( pConnectProperties->topicAliasMax );
            pIndexLocal = &pIndexLocal[ 2 ];
        }

        /*Serialize the request response information if provided.*/

        if( pConnectProperties->requestResponseInfo != false )
        {
            *pIndexLocal = MQTT_REQUEST_RESPONSE_ID;
            pIndexLocal++;
            *pIndexLocal = 1U;
            pIndexLocal++;
        }

        /*Serialize request problem information if provided.*/

        if( pConnectProperties->requestProblemInfo != true )
        {
            *pIndexLocal = MQTT_REQUEST_PROBLEM_ID;
            pIndexLocal++;
            *pIndexLocal = 0U;
            pIndexLocal++;
        }

        return pIndexLocal;
    }

    MQTTStatus_t MQTTV5_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                          const MQTTPublishInfo_t * pWillInfo,
                                          const MQTTConnectProperties_t * pConnectProperties,
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

        else if( pConnectProperties == NULL )
        {
            LogError( ( "Argument cannot be NULL: connectProperties" ) );
            status = MQTTBadParameter;
        }
        else
        {
            /* Calculate CONNECT packet size. Overflow in in this addition is not checked
             * because it is part of the API contract to call Mqtt_GetConnectPacketSize()
             * before this function. */
            connectPacketSize = remainingLength + remainingLengthEncodedSize( remainingLength ) + 1U;

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
                serializeConnectPacketV5( pConnectInfo,
                                          pWillInfo,
                                          pConnectProperties,
                                          remainingLength,
                                          pFixedBuffer );
            }
        }

        return status;
    }

    MQTTStatus_t MQTTV5_DeserializeConnack( MQTTConnectProperties_t * pConnackProperties,
                                            const MQTTPacketInfo_t * pIncomingPacket,
                                            bool * pSessionPresent )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t propertyLength;
        size_t remainingLengthSize;
        const uint8_t * pVariableHeader = NULL;

        /*Validate the arguments*/
        if( pConnackProperties == NULL )
        {
            status = MQTTBadParameter;
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            else if( pConnackProperties->pIncomingUserProperty == NULL )
            {
                status = MQTTBadParameter;
            }
        #endif
        else
        {
            status = validateConnackParams( pIncomingPacket, pSessionPresent );
        }

        if( status == MQTTSuccess )
        {
            pVariableHeader = pIncomingPacket->pRemainingData;
            pVariableHeader = &pVariableHeader[ 2 ];
            remainingLengthSize = remainingLengthEncodedSize( pIncomingPacket->remainingLength );
            status = decodeVariableLength( pVariableHeader, &propertyLength );
        }

        /*Validate the packet size if max packet size is set*/
        if( status == MQTTSuccess )
        {
            if( ( pIncomingPacket->remainingLength + remainingLengthSize + 1U ) > ( pConnackProperties->maxPacketSize ) )
            {
                status = MQTTProtocolError;
            }
            /*Validate the remaining length*/
            else if( ( pIncomingPacket->remainingLength ) != ( 2U + propertyLength + remainingLengthEncodedSize( propertyLength ) ) )
            {
                status = MQTTMalformedPacket;
            }
            /*Deserialize the connack properties.*/
            else
            {
                status = deserializeConnackV5( pConnackProperties, propertyLength, &pVariableHeader );
            }
        }

        return status;
    }

    MQTTStatus_t MQTTV5_ValidatePublishParams( const MQTTPublishInfo_t * pPublishInfo,
                                               uint16_t topicAliasMax,
                                               uint8_t retainAvailable,
                                               uint8_t maxQos )
    {
        MQTTStatus_t status;

        if( pPublishInfo == NULL )
        {
            LogError( ( "Argument cannot be NULL: pPublishInfo=%p ",
                        ( void * ) pPublishInfo
                        ) );
            status = MQTTBadParameter;
        }
        else if( pPublishInfo->topicAlias > topicAliasMax )
        {
            LogError( ( "Invalid topic alias." ) );
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
                        ( unsigned short ) pPublishInfo->topicNameLength ) );
            status = MQTTBadParameter;
        }
        else
        {
            status = MQTTSuccess;
        }

        return status;
    }

    MQTTStatus_t MQTTV5_GetPublishPacketSize( MQTTPublishInfo_t * pPublishInfo,
                                              size_t * pRemainingLength,
                                              size_t * pPacketSize,
                                              uint32_t maxPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;

        if( ( pPublishInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
        {
            LogError( ( "Argument cannot be NULL: pPublishInfo=%p, "
                        "pRemainingLength=%p, pPacketSize=%p.",
                        ( void * ) pPublishInfo,
                        ( void * ) pRemainingLength,
                        ( void * ) pPacketSize ) );
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
        else if( ( pPublishInfo->topicAlias == 0U ) && ( pPublishInfo->topicNameLength == 0U ) )
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
            status = calculatePublishPacketSizeV5( pPublishInfo, pRemainingLength, pPacketSize, maxPacketSize );
        }

        return status;
    }

    MQTTStatus_t MQTTV5_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTAckInfo_t * pAckInfo,
                                        bool requestProblem,
                                        uint32_t maxPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;

        if( pIncomingPacket == NULL )
        {
            LogError( ( "pIncomingPacket cannot be NULL." ) );
            status = MQTTBadParameter;
        }

        /* Pointer for packet identifier cannot be NULL for packets other than
         * CONNACK and PINGRESP. */
        else if( pPacketId == NULL )
        {
            LogError( ( "pPacketId cannot be NULL for packet type %02x.",
                        ( unsigned int ) pIncomingPacket->type ) );
            status = MQTTBadParameter;
        }

        /* Pointer for remaining data cannot be NULL for packets other
         * than PINGRESP. */
        else if( pIncomingPacket->pRemainingData == NULL )
        {
            LogError( ( "Remaining data of incoming packet is NULL." ) );
            status = MQTTBadParameter;
        }
        /*Max packet size cannot be 0.*/
        else if( maxPacketSize == 0U )
        {
            status = MQTTBadParameter;
        }
        else if( ( pIncomingPacket->remainingLength + remainingLengthEncodedSize( pIncomingPacket->remainingLength ) + 1U ) > maxPacketSize )
        {
            status = MQTTProtocolError;
        }
        else
        {
            /* Make sure response packet is a valid ack. */
            switch( pIncomingPacket->type )
            {
                case MQTT_PACKET_TYPE_PUBACK:
                case MQTT_PACKET_TYPE_PUBREC:
                    status = deserializeSimpleAckV5( pIncomingPacket, pPacketId, pAckInfo, requestProblem );

                    if( status == MQTTSuccess )
                    {
                        status = logAckResponseV5( pAckInfo->reasonCode, *pPacketId );
                    }

                    break;

                case MQTT_PACKET_TYPE_PUBREL:
                case MQTT_PACKET_TYPE_PUBCOMP:
                    status = deserializeSimpleAckV5( pIncomingPacket, pPacketId, pAckInfo, requestProblem );

                    if( status == MQTTSuccess )
                    {
                        status = logSimpleAckResponseV5( pAckInfo->reasonCode, *pPacketId );
                    }

                    break;

                /* Any other packet type is invalid. */
                default:
                    LogError( ( "IotMqttv5_DeserializeResponse() called with unknown packet type:(%02x).",
                                ( unsigned int ) pIncomingPacket->type ) );
                    status = MQTTBadResponse;
                    break;
            }
        }

        return status;
    }

    MQTTStatus_t MQTTV5_GetAckPacketSize( MQTTAckInfo_t * pAckInfo,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize,
                                          uint32_t maxPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t length = 0U;
        size_t propertyLength = 0U;
        size_t packetSize = 0U;

        /*Validate the parameters.*/
        if( ( pAckInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
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

            if( pAckInfo->reasonStringLength != 0U )
            {
                if( pAckInfo->pReasonString == NULL )
                {
                    status = MQTTBadParameter;
                }
                else
                {
                    propertyLength += pAckInfo->reasonStringLength;
                    propertyLength += MQTT_UTF8_LENGTH_SIZE;
                }
            }
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            if( ( status == MQTTSuccess ) && ( pAckInfo->pUserProperty != NULL ) )
            {
                status = MQTT_GetUserPropertySize( pAckInfo->pUserProperty->userProperty, pAckInfo->pUserProperty->count, &propertyLength );
            }
        #endif

        if( status == MQTTSuccess )
        {
            /*Validate the length.*/
            if( ( propertyLength + 4U ) < MQTT_MAX_REMAINING_LENGTH )
            {
                /*We have successfully calculated the property length.*/
                pAckInfo->propertyLength = propertyLength;
                length += remainingLengthEncodedSize( propertyLength ) + propertyLength;
                *pRemainingLength = length;
            }
            else
            {
                status = MQTTBadParameter;
            }
        }

        if( status == MQTTSuccess )
        {
            packetSize = length + 1U + remainingLengthEncodedSize( length );

            if( packetSize > maxPacketSize )
            {
                status = MQTTBadParameter;
            }
            else
            {
                *pPacketSize = packetSize;
            }
        }

        return status;
    }

    uint8_t * MQTTV5_SerializeAckFixed( uint8_t * pIndex,
                                        uint8_t packetType,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        size_t propertyLength )
    {
        uint8_t * pIndexLocal = pIndex;

        /* The first byte in the publish ack packet is the control packet type. */
        *pIndexLocal = packetType;
        pIndexLocal++;
        /*After the packet type fixed header has remaining length.*/
        pIndexLocal = encodeRemainingLength( pIndexLocal, remainingLength );
        /*Encode the packet id.*/
        pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( packetId );
        pIndexLocal[ 1 ] = UINT16_LOW_BYTE( packetId );
        pIndexLocal = &pIndexLocal[ 2 ];
        /*We are only sending the ack back if the reason code is success.*/
        *pIndexLocal = MQTT_REASON_SUCCESS;
        pIndexLocal++;
        /*Encode the property length.*/
        pIndexLocal = encodeRemainingLength( pIndexLocal, propertyLength );
        return pIndexLocal;
    }

    MQTTStatus_t MQTTV5_SerializePubAckWithProperty( const MQTTAckInfo_t * pAckInfo,
                                                     size_t remainingLength,
                                                     const MQTTFixedBuffer_t * pFixedBuffer,
                                                     uint8_t packetType,
                                                     uint16_t packetId )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t ackPacketSize = 0;

        /* Validate arguments. */
        if( ( pAckInfo == NULL ) || ( pFixedBuffer == NULL ) )
        {
            LogError( ( "Argument cannot be NULL: pAckInfo=%p, "
                        "pFixedBuffer=%p.",
                        ( void * ) pAckInfo,
                        ( void * ) pFixedBuffer ) );
            status = MQTTBadParameter;
        }
        /* A buffer must be configured for serialization. */
        else if( pFixedBuffer->pBuffer == NULL )
        {
            LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
            status = MQTTBadParameter;
        }
        else
        {
            ackPacketSize = remainingLength + remainingLengthEncodedSize( remainingLength ) + 1U;

            /* Check that the full packet size fits within the given buffer. */
            if( ackPacketSize > pFixedBuffer->size )
            {
                LogError( ( "Buffer size of %lu is not sufficient to hold "
                            "serialized ACK packet of size of %lu.",
                            ( unsigned long ) pFixedBuffer->size,
                            ( unsigned long ) ackPacketSize ) );
                status = MQTTNoMemory;
            }
            else
            {
                serializePubAckPacketV5( pAckInfo, packetType, packetId, remainingLength, pFixedBuffer );
            }
        }

        return status;
    }

    MQTTStatus_t MQTTV5_GetDisconnectPacketSize( MQTTAckInfo_t * pDisconnectInfo,
                                                 size_t * pRemainingLength,
                                                 size_t * pPacketSize,
                                                 uint32_t maxPacketSize,
                                                 uint32_t sessionExpiry,
                                                 uint32_t prevSessionExpiry )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t length = 0U;
        size_t packetSize = 0U;
        size_t propertyLength = 0U;

        /*Validate the arguments.*/
        if( ( pDisconnectInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
        {
            LogError( ( "Argument cannot be NULL: pDisconnectInfo=%p, "
                        "pRemainingLength=%p, pPacketSize=%p.",
                        ( void * ) pDisconnectInfo,
                        ( void * ) pRemainingLength,
                        ( void * ) pPacketSize ) );
            status = MQTTBadParameter;
        }
        else if( maxPacketSize == 0U )
        {
            LogError( ( "Max packet size cannot be zero." ) );
            status = MQTTBadParameter;
        }
        /*Cannot overWrite a session expiry of 0.*/
        else if( ( prevSessionExpiry == 0U ) && ( sessionExpiry != 0U ) )
        {
            LogError( ( "If the Session Expiry in the CONNECT packet was zero, then it is a Protocol Error to set a non-zero Session Expiry Interval in the DISCONNECT packet." ) );
            status = MQTTBadParameter;
        }
        else if( validateDisconnectResponseV5( pDisconnectInfo->reasonCode, false ) != MQTTSuccess )
        {
            LogError( ( "Invalid reason code." ) );
            status = MQTTBadParameter;
        }
        else
        {
            /*Reason code.*/
            length += 1U;

            /*Add session expiry if provided.*/
            if( sessionExpiry != 0U )
            {
                propertyLength += MQTT_SESSION_EXPIRY_SIZE;
            }

            /*Validate the reason string if provided.*/
            if( pDisconnectInfo->reasonStringLength != 0U )
            {
                if( pDisconnectInfo->pReasonString == NULL )
                {
                    status = MQTTBadParameter;
                }
                else
                {
                    propertyLength += pDisconnectInfo->reasonStringLength;
                    propertyLength += MQTT_UTF8_LENGTH_SIZE;
                }
            }
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            if( ( status == MQTTSuccess ) && ( pDisconnectInfo->pUserProperty != NULL ) )
            {
                status = MQTT_GetUserPropertySize( pDisconnectInfo->pUserProperty->userProperty, pDisconnectInfo->pUserProperty->count, &propertyLength );
            }
        #endif

        if( status == MQTTSuccess )
        {
            /*Validate the length.*/
            if( ( propertyLength + 4U ) < MQTT_MAX_REMAINING_LENGTH )
            {
                /*We have successfully calculated the property length.*/
                pDisconnectInfo->propertyLength = propertyLength;
                length += remainingLengthEncodedSize( propertyLength ) + propertyLength;
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
            packetSize = length + 1U + remainingLengthEncodedSize( length );

            if( packetSize > maxPacketSize )
            {
                status = MQTTBadParameter;
            }
            else
            {
                *pPacketSize = packetSize;
            }
        }

        return status;
    }

    uint8_t * MQTTV5_SerializeDisconnectFixed( uint8_t * pIndex,
                                               const MQTTAckInfo_t * pDisconnectInfo,
                                               size_t remainingLength,
                                               uint32_t sessionExpiry )
    {
        uint8_t * pIndexLocal = pIndex;

        assert( pDisconnectInfo != NULL );
        assert( pIndex != NULL );
        /* The first byte in the publish ack packet is the control packet type. */
        *pIndexLocal = MQTT_PACKET_TYPE_DISCONNECT;
        pIndexLocal++;
        /*After the packet type fixed header has remaining length.*/
        pIndexLocal = encodeRemainingLength( pIndexLocal, remainingLength );
        /*Encode the reason code.*/
        *pIndexLocal = pDisconnectInfo->reasonCode;
        pIndexLocal++;
        /*Encode the property length.*/
        pIndexLocal = encodeRemainingLength( pIndexLocal, pDisconnectInfo->propertyLength );

        /*Encode the session expiry if provided. */
        if( sessionExpiry != 0U )
        {
            *pIndexLocal = MQTT_SESSION_EXPIRY_ID;
            pIndexLocal++;
            pIndexLocal[ 0 ] = UINT32_BYTE3( sessionExpiry );
            pIndexLocal[ 1 ] = UINT32_BYTE2( sessionExpiry );
            pIndexLocal[ 2 ] = UINT32_BYTE1( sessionExpiry );
            pIndexLocal[ 3 ] = UINT32_BYTE0( sessionExpiry );
            pIndexLocal = &pIndexLocal[ 4 ];
        }

        return pIndexLocal;
    }

    static void serializeDisconnectPacketV5( const MQTTAckInfo_t * pDisconnectInfo,
                                             const MQTTFixedBuffer_t * pFixedBuffer,
                                             size_t remainingLength,
                                             uint32_t sessionExpiry )
    {
        uint8_t * pIndex = NULL;

        assert( pDisconnectInfo != NULL );
        assert( pFixedBuffer != NULL );
        assert( pFixedBuffer->pBuffer != NULL );
        pIndex = pFixedBuffer->pBuffer;
        /* Serialize the header including reason code and property length */
        pIndex = MQTTV5_SerializeDisconnectFixed( pIndex, pDisconnectInfo,
                                                  remainingLength,
                                                  sessionExpiry );

        /*Serialize the reason string if provided.*/
        if( pDisconnectInfo->reasonStringLength != 0U )
        {
            *pIndex = MQTT_REASON_STRING_ID;
            pIndex++;
            pIndex = encodeString( pIndex, pDisconnectInfo->pReasonString, pDisconnectInfo->reasonStringLength );
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            /*Serialize the user properties if provided.*/
            if( pDisconnectInfo->pUserProperty != NULL )
            {
                uint32_t i = 0;
                uint32_t size = pDisconnectInfo->pUserProperty->count;
                const MQTTUserProperty_t * pUserProperty = pDisconnectInfo->pUserProperty->userProperty;

                for( ; i < size; i++ )
                {
                    *pIndex = MQTT_USER_PROPERTY_ID;
                    pIndex++;
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pKey, pUserProperty[ i ].keyLength );
                    pIndex = encodeString( pIndex, pUserProperty[ i ].pValue, pUserProperty[ i ].valueLength );
                }
            }
        #endif /* if ( MQTT_USER_PROPERTY_ENABLED ) */

        /* Ensure that the difference between the end and beginning of the buffer
         * is less than the buffer size. */
        assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
    }


    MQTTStatus_t MQTTV5_SerializeDisconnectWithProperty( const MQTTAckInfo_t * pDisconnectInfo,
                                                         size_t remainingLength,
                                                         const MQTTFixedBuffer_t * pFixedBuffer,
                                                         uint32_t sessionExpiry )
    {
        MQTTStatus_t status = MQTTSuccess;
        size_t packetSize = 0;

        /* Validate arguments. */
        if( ( pDisconnectInfo == NULL ) || ( pFixedBuffer == NULL ) )
        {
            LogError( ( "Argument cannot be NULL: pDisconnectInfo=%p, "
                        "pFixedBuffer=%p.",
                        ( void * ) pDisconnectInfo,
                        ( void * ) pFixedBuffer ) );
            status = MQTTBadParameter;
        }
        /* A buffer must be configured for serialization. */
        else if( pFixedBuffer->pBuffer == NULL )
        {
            LogError( ( "Argument cannot be NULL: pFixedBuffer->pBuffer is NULL." ) );
            status = MQTTBadParameter;
        }
        else
        {
            packetSize = remainingLength + remainingLengthEncodedSize( remainingLength ) + 1U;

            /* Check that the full packet size fits within the given buffer. */
            if( packetSize > pFixedBuffer->size )
            {
                LogError( ( "Buffer size of %lu is not sufficient to hold "
                            "serialized ACK packet of size of %lu.",
                            ( unsigned long ) pFixedBuffer->size,
                            ( unsigned long ) packetSize ) );
                status = MQTTNoMemory;
            }
            else
            {
                serializeDisconnectPacketV5( pDisconnectInfo, pFixedBuffer, remainingLength, sessionExpiry );
            }
        }

        return status;
    }

    MQTTStatus_t MQTTV5_DeserializeDisconnect( const MQTTPacketInfo_t * pPacket,
                                               MQTTAckInfo_t * pDisconnectInfo,
                                               const char ** pServerRef,
                                               uint16_t * pServerRefLength,
                                               uint32_t maxPacketSize )
    {
        MQTTStatus_t status = MQTTSuccess;
        const uint8_t * pIndex = NULL;
        size_t propertyLength = 0U;

        /*Validate the arguments*/
        if( ( pPacket == NULL ) || ( pPacket->pRemainingData == NULL ) )
        {
            status = MQTTBadParameter;
        }
        else if( ( pDisconnectInfo == NULL ) || ( pServerRef == NULL ) || ( pServerRefLength == NULL ) )
        {
            status = MQTTBadParameter;
        }
        else if( maxPacketSize == 0U )
        {
            status = MQTTBadParameter;
        }
        else if( pPacket->remainingLength == 0U )
        {
            status = MQTTMalformedPacket;
        }
        /*Packet size should not be more than the max allowed by the client.*/
        else if( ( pPacket->remainingLength + remainingLengthEncodedSize( pPacket->remainingLength ) + 1U ) > maxPacketSize )
        {
            status = MQTTProtocolError;
        }

        #if ( MQTT_USER_PROPERTY_ENABLED )
            else if( pDisconnectInfo->pUserProperty == NULL )
            {
                status = MQTTBadParameter;
            }
        #endif
        else
        {
            /* Extract the reason code */
            pIndex = pPacket->pRemainingData;
            pDisconnectInfo->reasonCode = *pIndex;
            pIndex++;
            /*Validate the reason code.*/
            status = validateDisconnectResponseV5( pDisconnectInfo->reasonCode, true );
        }

        if( status == MQTTSuccess )
        {
            if( ( pPacket->remainingLength > 1U ) )
            {
                /*Extract the property length.*/
                status = decodeVariableLength( pIndex, &propertyLength );

                if( status == MQTTSuccess )
                {
                    pIndex = &pIndex[ remainingLengthEncodedSize( propertyLength ) ];

                    /*Validate the remaining length.*/
                    if( pPacket->remainingLength != ( propertyLength + remainingLengthEncodedSize( propertyLength ) + 1U ) )
                    {
                        status = MQTTMalformedPacket;
                    }
                }
            }
        }

        if( status == MQTTSuccess )
        {
            while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
            {
                /*Decode the property id.*/
                uint8_t propertyId = *pIndex;
                bool reasonString = false;
                bool serverRef = false;
                pIndex = &pIndex[ 1 ];
                propertyLength -= sizeof( uint8_t );

                /*Validate the property id and decode accordingly.*/
                switch( propertyId )
                {
                    case MQTT_REASON_STRING_ID:
                        status = decodeutf_8( &pDisconnectInfo->pReasonString, &pDisconnectInfo->reasonStringLength, &propertyLength, &reasonString, &pIndex );
                        break;

                    case MQTT_USER_PROPERTY_ID:
                        #if ( MQTT_USER_PROPERTY_ENABLED )
                            status = decodeutf_8pair( pDisconnectInfo->pUserProperty, &pDisconnectInfo->pUserProperty->count, &propertyLength, &pIndex );
                        #else
                            status = decodeAndDiscard( &propertyLength, &pIndex );
                        #endif
                        break;

                    case MQTT_SERVER_REF_ID:
                        status = decodeutf_8( pServerRef, pServerRefLength, &propertyLength, &serverRef, &pIndex );
                        break;

                    default:
                        status = MQTTProtocolError;
                        break;
                }
            }
        }

        return status;
    }
#endif /* if ( MQTT_VERSION_5_ENABLED ) */
/*-----------------------------------------------------------*/
