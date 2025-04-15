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

/**
 * @brief Per the MQTT spec, the max packet size  can be of  max remaining length + 5 bytes
 */
    #define MQTT_MAX_PACKET_SIZE             ( 268435460U )

/**
 * @brief Version 5 has the value 5.
 */
    #define MQTT_VERSION_5                   ( ( uint8_t ) 5U )

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
* @ingroup mqtt_constants
* @brief The size of MQTT PUBACK, PUBREC, PUBREL, and PUBCOMP packets with only packet id.
*/
    #define MQTT_PUBLISH_ACK_PACKET_SIZE_WITHOUT_REASON   ( 2UL )

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

/*
*  Position of the properties for the fieldSet
*/
#define MQTT_SUBSCRIPTION_ID_POS                    ( 1 )
#define MQTT_SESSION_EXPIRY_INTERVAL_POS            ( 2 )
#define MQTT_RECEIVE_MAXIMUM_POS                    ( 3 )
#define MQTT_MAX_PACKET_SIZE_POS                    ( 4 )
#define MQTT_TOPIC_ALIAS_MAX_POS                    ( 5 )
#define MQTT_REQUEST_RESPONSE_INFO_POS              ( 6 )
#define MQTT_REQUEST_PROBLEM_INFO_POS               ( 7 )
#define MQTT_USER_PROPERTY_POS                      ( 8 )
#define MQTT_AUTHENTICATION_METHOD_POS              ( 9 )
#define MQTT_AUTHENTICATION_DATA_POS                ( 10 )
#define MQTT_PAYLOAD_FORMAT_INDICATOR_POS          ( 11 )
#define MQTT_MESSAGE_EXPIRY_INTERVAL_POS           ( 12 )
#define MQTT_PUBLISH_TOPIC_ALIAS_POS               ( 13 )
#define MQTT_PUBLISH_RESPONSE_TOPIC_POS            ( 14 )
#define MQTT_PUBLISH_CORRELATION_DATA_POS          ( 15 )
#define MQTT_PUBLISH_SUBSCRIPTION_IDENTIFIER_POS   ( 16 )
#define MQTT_PUBLISH_CONTENT_TYPE_POS              ( 17 )
#define MQTT_REASON_STRING_POS                     ( 18 )


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
 * @brief Set a bit in an 32-bit unsigned integer.
 */
#define UINT32_SET_BIT( x, position )      ( ( x ) = ( uint32_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )
 /**
  * @brief Macro for checking if a bit is set in a 4-byte unsigned int.
  *
  * @param[in] x The unsigned int to check.
  * @param[in] position Which bit to check.
  */
#define UINT32_CHECK_BIT( x, position )    ( ( ( x ) & ( 0x01U << ( position ) ) ) == ( 0x01U << ( position ) ) )
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
 * @brief Calculates the packet size and remaining length of an MQTT
 * SUBSCRIBE or UNSUBSCRIBE packet.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[out] pRemainingLength The Remaining Length of the MQTT SUBSCRIBE or
 * UNSUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT MQTT SUBSCRIBE or
 * UNSUBSCRIBE packet.
 * @param[in] propLen Length of the optional properties in MQTT_SUBSCRIBE or MQTT_UNSUBSCRIBE
 * @param[in] subscriptionType #MQTT_SUBSCRIBE or #MQTT_UNSUBSCRIBE.
 *
 * #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or a subscription is empty; #MQTTSuccess otherwise.
 * 
 */

static MQTTStatus_t calculateSubscriptionPacketSize(MQTTSubscribeInfo_t* pSubscriptionList,
    size_t subscriptionCount,
    size_t* pRemainingLength,
    size_t* pPacketSize,
    size_t propLen,
    MQTTSubscriptionType_t subscriptionType); 


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
uint8_t * encodeString( uint8_t * pDestination,
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
 * @param[out] propBuffer Pointer to the property buffer.
 *
 * @return #MQTTSuccess if PUBLISH is valid; #MQTTBadResponse
 * if the PUBLISH packet doesn't follow MQTT spec.
 */
static MQTTStatus_t deserializePublish(const MQTTPacketInfo_t* pIncomingPacket,
    uint16_t* pPacketId,
    MQTTPublishInfo_t* pPublishInfo,
    MqttPropBuilder_t* propBuffer);

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
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
static MQTTStatus_t decodeAndDiscard( size_t * pPropertyLength,
                                              const uint8_t ** pIndex );

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
 * @brief Validate the length and decode a 4 byte value.
 *
 * @param[out] pProperty To store the decoded property.
 * @param[out] pPropertyLength  Size of the length.
 * @param[out] pUsed Whether the property is decoded befire.
 * @param[out]  pIndex Pointer to the current index of the buffer.
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/

static MQTTStatus_t decodeuint32_t( uint32_t* pProperty,
                                    size_t* pPropertyLength,
                                    bool* pUsed,
                                    const uint8_t** pIndex); 

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

static MQTTStatus_t decodeuint16_t( uint16_t* pProperty,
                                    size_t* pPropertyLength,
                                    bool* pUsed,
                                    const uint8_t** pIndex); 

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
static MQTTStatus_t decodeuint8_t(  uint8_t* pProperty,
                                    size_t* pPropertyLength,
                                    bool* pUsed,
                                    const uint8_t** pIndex); 

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
MQTTStatus_t decodeutf_8(const char** pProperty,
                        uint16_t* pLength,
                        size_t* pPropertyLength,
                        bool* pUsed,
                        const uint8_t** pIndex); 


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
static MQTTStatus_t deserializeConnack(MQTTConnectProperties_t* pConnackProperties,
                                        size_t length,
                                        const uint8_t* const* pIndex,
                                        MqttPropBuilder_t* propBuffer); 

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
static MQTTStatus_t calculatePublishPacketSize( MQTTPublishInfo_t * pPublishInfo,
                                                      size_t * pRemainingLength,
                                                      size_t * pPacketSize,
                                                      uint32_t maxPacketSize, size_t proplen);


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
 * @param[out] propBuffer To store the decoded property.
 * @param[out] pIndex Pointer to the current index of the buffer.
 * @param[out] remainingLength Remaining length of the incoming packet.
 *
 *
 * @return #MQTTSuccess, #MQTTProtocolError and #MQTTMalformedPacket
 **/
static MQTTStatus_t decodeAckProperties(MqttPropBuilder_t* propBuffer,
                                        const uint8_t* pIndex,
                                        size_t remainingLength);

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
static MQTTStatus_t deserializeSimpleAckV5(const MQTTPacketInfo_t* pAck,
                                            uint16_t* pPacketIdentifier,
                                            MQTTReasonCodeInfo_t* pReasonCode,
                                            bool requestProblem,
                                            MqttPropBuilder_t* propBuffer); 

/**
 * @brief Prints and validates the appropriate message for the Disconnect response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard DISCONNECT response code.
 * @param[in] incoming To differentiate between outgoing and incoming disconnect.
 *
 * @return #MQTTSuccess,#MQTTBadParameter and #MQTTProtocolError.
 */
static MQTTStatus_t validateDisconnectResponse( uint8_t reasonCode,
                                                bool incoming );
/*-----------------------------------------------------------*/


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

    return status;
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
            if( pRemainingData[ 1 ] != 0U )
            {
                status = logConnackResponseV5( pRemainingData[ 1 ] );
            }
        }
    }

    return status;
}

static MQTTStatus_t decodeVariableLength(const uint8_t* pBuffer,
    size_t* pLength)
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
        if (multiplier > 2097152U) /* 128 ^ 3 */
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;

            LogError(("Invalid remaining length in the packet.\n"));

            status = MQTTBadResponse;
        }
        else
        {
            /* Get the next byte. It is at the next position after the bytes
                * decoded till now since the header of one byte was read before. */
            encodedByte = pBuffer[bytesDecoded];
            remainingLength += ((size_t)encodedByte & 0x7FU) * multiplier;
            multiplier *= 128U;
            bytesDecoded++;
        }

        /* If the response is incorrect then
            * break out of the loop. */
        if (remainingLength == MQTT_REMAINING_LENGTH_INVALID)
        {
            break;
        }
    } while ((encodedByte & 0x80U) != 0U);

    if (status == MQTTSuccess)
    {
        /* Check that the decoded remaining length conforms to the MQTT specification. */
        expectedSize = remainingLengthEncodedSize(remainingLength);

        if (bytesDecoded != expectedSize)
        {
            LogError(("Expected and actual length of decoded bytes do not match.\n"));
            status = MQTTBadResponse;
        }
        else
        {
            *pLength = remainingLength;
        }
    }

    return status;
}

static MQTTStatus_t decodeuint32_t(uint32_t* pProperty,
    size_t* pPropertyLength,
    bool* pUsed,
    const uint8_t** pIndex)
{
    const uint8_t* pVariableHeader = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/
    if (*pUsed == true)
    {
        status = MQTTProtocolError;
    }
    /*Validate the length and decode.*/
    else if (*pPropertyLength < sizeof(uint32_t))
    {
        status = MQTTMalformedPacket;
    }
    else
    {
        *pProperty = UINT32_DECODE(pVariableHeader);
        pVariableHeader = &pVariableHeader[sizeof(uint32_t)];
        *pUsed = true;
        *pPropertyLength -= sizeof(uint32_t);
    }

    *pIndex = pVariableHeader;
    return status;
}

static MQTTStatus_t decodeuint16_t(uint16_t* pProperty,
    size_t* pPropertyLength,
    bool* pUsed,
    const uint8_t** pIndex)
{
    const uint8_t* pVariableHeader = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if (*pUsed == true)
    {
        status = MQTTProtocolError;
    }
    /*Validate the length and decode.*/

    else if (*pPropertyLength < sizeof(uint16_t))
    {
        status = MQTTMalformedPacket;
    }
    else
    {
        *pProperty = UINT16_DECODE(pVariableHeader);
        pVariableHeader = &pVariableHeader[sizeof(uint16_t)];
        *pUsed = true;
        *pPropertyLength -= sizeof(uint16_t);
    }

    *pIndex = pVariableHeader;
    return status;
}

static MQTTStatus_t decodeuint8_t(uint8_t* pProperty,
    size_t* pPropertyLength,
    bool* pUsed,
    const uint8_t** pIndex)
{
    const uint8_t* pVariableHeader = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if (*pUsed == true)
    {
        status = MQTTProtocolError;
    }
    /*Validate the length and decode.*/

    else if (*pPropertyLength < sizeof(uint8_t))
    {
        status = MQTTMalformedPacket;
    }
    else
    {
        *pProperty = *pVariableHeader;
        pVariableHeader = &pVariableHeader[sizeof(uint8_t)];
        *pUsed = true;
        *pPropertyLength -= sizeof(uint8_t);

        if (*pProperty > 1U)
        {
            status = MQTTProtocolError;
        }
    }

    *pIndex = pVariableHeader;
    return status;
}

MQTTStatus_t decodeutf_8(const char** pProperty,
    uint16_t* pLength,
    size_t* pPropertyLength,
    bool* pUsed,
    const uint8_t** pIndex)
{
    const uint8_t* pVariableHeader = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if (*pUsed == true)
    {
        status = MQTTProtocolError;
    }
    /*Validate the length and decode.*/

    else if (*pPropertyLength < sizeof(uint16_t))
    {
        status = MQTTMalformedPacket;
    }
    else
    {
        *pLength = UINT16_DECODE(pVariableHeader);
        pVariableHeader = &pVariableHeader[sizeof(uint16_t)];
        *pPropertyLength -= sizeof(uint16_t);

        if (*pPropertyLength < *pLength)
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pProperty = (const char*)pVariableHeader;
            pVariableHeader = &pVariableHeader[*pLength];
            *pPropertyLength -= *pLength;
            *pUsed = true;
            LogDebug(("Reason String is %s", *pProperty));
        }
    }

    *pIndex = pVariableHeader;
    return status;
}


static MQTTStatus_t decodeBinaryData(const void** pProperty,
    uint16_t* pLength,
    size_t* pPropertyLength,
    bool* pUsed,
    const uint8_t** pIndex)
{
    const uint8_t* pVariableHeader = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /*Protocol error to include the same property twice.*/

    if (*pUsed == true)
    {
        status = MQTTProtocolError;
    }
    /*Validate the length and decode.*/

    else if (*pPropertyLength < sizeof(uint16_t))
    {
        status = MQTTMalformedPacket;
    }
    else
    {
        *pLength = UINT16_DECODE(pVariableHeader);
        pVariableHeader = &pVariableHeader[sizeof(uint16_t)];
        *pPropertyLength -= sizeof(uint16_t);

        if (*pPropertyLength < *pLength)
        {
            status = MQTTMalformedPacket;
        }
        else
        {
            *pProperty = pVariableHeader;
            pVariableHeader = &pVariableHeader[*pLength];
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

static MQTTStatus_t deserializeConnack( MQTTConnectProperties_t * pConnackProperties,
                                        size_t length,
                                        const uint8_t * const * pIndex, 
                                        MqttPropBuilder_t *propBuffer)
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

    propBuffer->pBuffer = pVariableHeader; 
    propBuffer->bufferLength = propertyLength; 

    /*Decode all the properties received, validate and store them in pConnackProperties.*/
    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pVariableHeader;
        pVariableHeader = &pVariableHeader[ 1 ];
        propertyLength -= sizeof( uint8_t );

        switch( propertyId )
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
                status = decodeAndDiscard( &propertyLength, &pVariableHeader );
                break;

            case MQTT_WILDCARD_ID:
                status = decodeuint8_t( &pConnackProperties->isWildcardAvaiable, &propertyLength, &wildcard, &pVariableHeader );
                break;

            case MQTT_SUB_AVAILABLE_ID:
                status = decodeuint8_t( &pConnackProperties->isSubscriptionIdAvailable, &propertyLength, &subId, &pVariableHeader );
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

                status = decodeAuthInfo( pConnackProperties, &authMethod, &authData, &propertyLength, &pVariableHeader, propertyId);
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

static MQTTStatus_t calculatePublishPacketSize( MQTTPublishInfo_t * pPublishInfo,
                                                    size_t * pRemainingLength,
                                                    size_t * pPacketSize,
                                                    uint32_t maxPacketSize, size_t publishPropertyLength)
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
    /*status = MQTT_GetPublishPropertiesSize( pPublishInfo );*/

    if( status == MQTTSuccess )
    {
        packetSize += publishPropertyLength;
        packetSize += remainingLengthEncodedSize(publishPropertyLength);

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


static MQTTStatus_t decodeAckProperties(MqttPropBuilder_t* propBuffer,
                                        const uint8_t * pIndex,
                                        size_t remainingLength)
{
    size_t propertyLength = 0U;
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pLocalIndex = pIndex;

    /*Decode the property length*/
    status = decodeVariableLength( pLocalIndex, &propertyLength );

    uint16_t reasonStringLength = 0U; 
    if( status == MQTTSuccess )
    {
        pLocalIndex = &pLocalIndex[ remainingLengthEncodedSize( propertyLength ) ];

        /*Validate the remaining length.*/
        if (remainingLength != (propertyLength + remainingLengthEncodedSize(propertyLength) + 3U))
        {
            status = MQTTMalformedPacket;
        }
    }
    if (status == MQTTSuccess)
    {
        propBuffer->pBuffer = pLocalIndex; 
        propBuffer->bufferLength = propertyLength;
    }
    if( status == MQTTSuccess )
    {
        while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
        {
            /*Decode the property id.*/
            uint8_t propertyId = *pLocalIndex;
            bool reasonString = false;
            pLocalIndex = &pLocalIndex[ 1 ];
            propertyLength -= sizeof( uint8_t );

            switch(propertyId)
            {
                case MQTT_REASON_STRING_ID:
                    if (reasonString == true)
                    {
                        status = MQTTBadParameter; 
                    }
                    else
                    {
                        reasonStringLength = UINT16_DECODE(pLocalIndex);
                        pLocalIndex = &pLocalIndex[sizeof(uint16_t)];
                        propertyLength -= sizeof(uint16_t);
                        pLocalIndex = &pLocalIndex[reasonStringLength];
                        propertyLength -= reasonStringLength; 
                    }
                    break;

                case MQTT_USER_PROPERTY_ID:
                    status = decodeAndDiscard( &propertyLength, &pLocalIndex );
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
                                            MQTTReasonCodeInfo_t * pReasonCode,
                                            bool requestProblem,
                                            MqttPropBuilder_t *propBuffer)
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

        pReasonCode->reasonCode = pIndex; 
        pReasonCode->reasonCodeLength = 1U;
        pIndex++;
    }

    if( ( pAck->remainingLength > 4U ) )
    {
        /*Protocol error to send user property and reason string if client has set request problem to false.*/
        if( requestProblem == false )
        {
            status = MQTTProtocolError;
        }
        else
        {
            status = decodeAckProperties(propBuffer, pIndex, pAck->remainingLength );
        }
    }

    return status;
}


static MQTTStatus_t calculateSubscriptionPacketSize(MQTTSubscribeInfo_t *pSubscriptionList,
                                                        size_t subscriptionCount,
                                                        size_t *pRemainingLength,
                                                        size_t *pPacketSize, 
                                                        size_t subscribePropLen, 
                                                        MQTTSubscriptionType_t subscriptionType)
{
    size_t packetSize = 0U , i = 0; 
    MQTTStatus_t status = MQTTSuccess ; 
    assert( pSubscriptionList != NULL );
    assert( subscriptionCount != 0U );

    /*2 byte packet id*/
    packetSize += sizeof( uint16_t ) ; 

    if( status == MQTTSuccess )
    {
        packetSize += subscribePropLen;
        packetSize += remainingLengthEncodedSize(subscribePropLen);
    }


    if(status == MQTTSuccess){
        for(i = 0 ; i < subscriptionCount ; i++){
            if((pSubscriptionList[i].topicFilterLength == 0U) || (pSubscriptionList[i].pTopicFilter== NULL)){
                LogError(("Argument cannot be null : pTopicFilter")) ; 
                status = MQTTBadParameter;
                break ;
            }
            packetSize += pSubscriptionList[i].topicFilterLength + sizeof(uint16_t) ; 
            if(subscriptionType == MQTT_SUBSCRIBE)
            {
                packetSize += 1U ; 
            }
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
MQTTStatus_t MQTT_GetSubscribePacketSize(MQTTSubscribeInfo_t *pSubscriptionList,
                                        size_t subscriptionCount,
                                        size_t *pRemainingLength,
                                        size_t *pPacketSize, 
                                        size_t subscribePropLen)
{
    MQTTStatus_t status = MQTTSuccess ; 
    if(pSubscriptionList == NULL){
        LogError(("Argument cannot be null : SubscriptionList")); 
        status = MQTTBadParameter;
    }
    else if(subscriptionCount == 0U)
    {
        LogError(("Subscription count cannot be 0")) ; 
        status = MQTTBadParameter;
    }
    else
    {
        status = calculateSubscriptionPacketSize(pSubscriptionList, subscriptionCount, pRemainingLength, pPacketSize, subscribePropLen, MQTT_SUBSCRIBE);
    }
    return status ; 
}

static MQTTStatus_t validateDisconnectResponse( uint8_t reasonCode,
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

uint8_t * encodeString( uint8_t * pDestination,
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

static uint8_t * encodeBinaryData( uint8_t * pDestination,
                               const void * pSource,
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


MQTTStatus_t deserializePublishProperties(MQTTPublishInfo_t* pPublishInfo, MqttPropBuilder_t *propBuffer,  const uint8_t* pIndex)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    const uint8_t* pLocalIndex = pIndex;

    /*Decode Property Length */

    status = decodeVariableLength(pLocalIndex, &propertyLength);
    pPublishInfo->propertyLength = propertyLength; 
    propBuffer->bufferLength = propertyLength; 

    if (status == MQTTSuccess)
    {
        pLocalIndex = &pLocalIndex[remainingLengthEncodedSize(propertyLength)];
        propBuffer->pBuffer = pLocalIndex; 
    }
    if (status == MQTTSuccess)
    {
        while ((propertyLength > 0U) && (status == MQTTSuccess))
        {
            /** Decode propertyId  -> reason string if or user property id*/
            uint8_t propertyId = *pLocalIndex;
            bool contentType = false;
            bool messageExpiryInterval = false;
            bool responseTopic = false;
            bool topicAlias = false;
            bool payloadFormatIndicator = false;
            bool correlationData = false;
            pLocalIndex = &pLocalIndex[1];
            propertyLength -= sizeof(uint8_t);
            switch (propertyId)
            {
            case MQTT_PAYLOAD_FORMAT_ID:
                decodeuint8_t(&pPublishInfo->payloadFormat, &propertyLength, &payloadFormatIndicator, &pLocalIndex);
                break;
            case MQTT_TOPIC_ALIAS_ID:
                decodeuint16_t(&pPublishInfo->topicAlias, &propertyLength, &topicAlias, &pLocalIndex);
                break;

            case MQTT_RESPONSE_TOPIC_ID:
                decodeutf_8(&pPublishInfo->pResponseTopic, &pPublishInfo->responseTopicLength, &propertyLength, &responseTopic, &pLocalIndex);
                break;

            case MQTT_CORRELATION_DATA_ID:
                decodeBinaryData(&pPublishInfo->pCorrelationData, &pPublishInfo->correlationLength, &propertyLength, &correlationData, &pLocalIndex);
                break;

            case MQTT_MSG_EXPIRY_ID:
                decodeuint32_t(&pPublishInfo->msgExpiryInterval, &propertyLength, &messageExpiryInterval, &pLocalIndex);
                break;

            case MQTT_CONTENT_TYPE_ID:
                decodeutf_8(&pPublishInfo->pContentType, &pPublishInfo->contentTypeLength, &propertyLength, &contentType, &pLocalIndex);
                break;

            case MQTT_SUBSCRIPTION_ID_ID:
                decodeVariableLength(pLocalIndex, &pPublishInfo->subscriptionId);
                break;

            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard(&propertyLength, &pLocalIndex);
                break;
            default:
                status = MQTTProtocolError;
                break;

            }
        }
    }
    return status ; 
}

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


static MQTTStatus_t deserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                            uint16_t * pPacketId,
                                            MQTTPublishInfo_t * pPublishInfo, 
                                            MqttPropBuilder_t *propBuffer)
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pVariableHeader, * pPacketIdentifierHigh = NULL;
    uint8_t * pIndex = NULL;

    assert( pIncomingPacket != NULL );
    assert( pPacketId != NULL );
    assert( pPublishInfo != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );

    pVariableHeader = pIncomingPacket->pRemainingData;
    pIndex = pVariableHeader ; 
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
        4U);
    }

    if( status == MQTTSuccess )
    {
        /* Extract the topic name starting from the first byte of the variable header.
        * The topic name string starts at byte 3 in the variable header. */
        pPublishInfo->topicNameLength = UINT16_DECODE( pVariableHeader );
        pIndex = &pIndex[2] ; 

        /* Sanity checks for topic name length and "Remaining length". The remaining
        * length must be at least as large as the variable length header. */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
        pPublishInfo->qos,
        pPublishInfo->topicNameLength + sizeof( uint16_t ) + sizeof(uint8_t) );
        pIndex = &pIndex[pPublishInfo->topicNameLength] ;
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

            /* Packet identifier cannot be 0. */
            if( *pPacketId == 0U )
            {
                LogError( ( "Packet identifier cannot be 0." ) );
                status = MQTTBadResponse;
            }
            if(status == MQTTSuccess)
            {
                pIndex = &pIndex[2] ; 
            }
        }
    }
    // insert code for properties here, maybe make a new function - 
    if(status == MQTTSuccess)
    {
        status = deserializePublishProperties( pPublishInfo ,propBuffer, pIndex);
        pIndex = &pIndex[remainingLengthEncodedSize(pPublishInfo->propertyLength)]; 
        pIndex = &pIndex[pPublishInfo->propertyLength]; 
    }
    if( status == MQTTSuccess )
    {
        /* Calculate the length of the payload. QoS 1 or 2 PUBLISH packets contain
        * a packet identifier, but QoS 0 PUBLISH packets do not. */
        pPublishInfo->payloadLength = pIncomingPacket->remainingLength - pPublishInfo->topicNameLength - sizeof( uint16_t ) - pPublishInfo->propertyLength - remainingLengthEncodedSize(pPublishInfo->propertyLength);

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

    /* The remaining length of the CONNECT packet is 
    d starting from the
     * second byte. The remaining length does not include the length of the fixed
     * header or the encoding of the remaining length. */
    pIndexLocal = encodeRemainingLength( pIndexLocal, remainingLength );

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

MQTTStatus_t updateContextWithConnectProps(MqttPropBuilder_t* pPropBuilder, MQTTConnectProperties_t* pConnectProperties)
{
    // iterating over the buffer to find relevant properties, 
    MQTTStatus_t status = MQTTSuccess;

    bool maxPacket = false;
    bool sessionExpiry = false;
    bool serverReceiveMax = false;
    bool topicAlias = false;
    bool authMethod = false;
    bool authData = false;
    size_t propertyLength = 0U;
    const uint8_t* pIndex;

    propertyLength = pPropBuilder->currentIndex;
    pIndex = pPropBuilder->pBuffer; /*Pointer to the buffer*/

    while ((propertyLength > 0U) && (status == MQTTSuccess))
    {
        uint8_t propertyId = *pIndex;
        pIndex = &pIndex[1];
        propertyLength--;

        switch (propertyId)
        {
        case MQTT_MAX_PACKET_SIZE_ID:
            status = decodeuint32_t(&pConnectProperties->maxPacketSize, &propertyLength,&maxPacket, &pIndex);
            break;
        case MQTT_SESSION_EXPIRY_ID:
            status = decodeuint32_t(&pConnectProperties->sessionExpiry, &propertyLength,&sessionExpiry, &pIndex);
            break;
        case MQTT_RECEIVE_MAX_ID:
            status = decodeuint16_t(&pConnectProperties->serverReceiveMax, &propertyLength, &serverReceiveMax,  &pIndex);
            break;
        case MQTT_TOPIC_ALIAS_MAX_ID:
            status = decodeuint16_t(&pConnectProperties->serverTopicAliasMax, &propertyLength, &topicAlias, &pIndex);
            break;
        case MQTT_AUTH_METHOD_ID:
        case MQTT_AUTH_DATA_ID:

            status = decodeAuthInfo(pConnectProperties, &authMethod, &authData, &propertyLength, &pIndex, propertyId);
            break;

        }
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


/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            size_t propLen)
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
                                                propLen,
                                                MQTT_UNSUBSCRIBE );
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
                                      MQTTPublishInfo_t * pPublishInfo,
                                      MqttPropBuilder_t * propBuffer)
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
       status = deserializePublish(pIncomingPacket, pPacketId, pPublishInfo, propBuffer) ; 

    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializePing( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId,
                                  bool * pSessionPresent )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pIncomingPacket == NULL )
    {
        LogError( ( "pIncomingPacket cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Make sure response packet is a valid ack. */
        switch( pIncomingPacket->type )
        {
            case MQTT_PACKET_TYPE_PINGRESP:
                status = deserializePingresp( pIncomingPacket );
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


MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                            MQTTPublishInfo_t * pWillInfo,
                                            size_t propLen,
                                            size_t willPropLen , 
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

    if( status == MQTTSuccess )
    {
        connectPacketSize += propLen;
        connectPacketSize += remainingLengthEncodedSize(propLen);
        /* Add the length of the client identifier. */
        connectPacketSize += pConnectInfo->clientIdentifierLength + sizeof( uint16_t );

        /* Add the lengths of the will properties if provided. */
    }

    if( status == MQTTSuccess )
    {
        /* Add the lengths of the will message and topic name if provided. */
        if( pWillInfo != NULL )
        {
            connectPacketSize += willPropLen; 
            connectPacketSize += remainingLengthEncodedSize( willPropLen );
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

        /*
        * It is possible that the remaining length becomes more than the maximum allowed by the MQTTV5-Spec, 
        * i.e. 268,435,455. This is because the user may enter a large number of user properties for the connect packet and/or the last will. 
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

MQTTStatus_t MQTT_DeserializeConnack( MQTTConnectProperties_t * pConnackProperties,
                                        const MQTTPacketInfo_t * pIncomingPacket,
                                        bool * pSessionPresent,
                                        MqttPropBuilder_t *propBuffer)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength;
    size_t remainingLengthSize;
    const uint8_t * pVariableHeader = NULL;

    status = validateConnackParams(pIncomingPacket, pSessionPresent);

    /*Validate the arguments*/
    if( pConnackProperties == NULL )
    {
        status = MQTTBadParameter;
    }
        
    /*status = validateConnackParams( pIncomingPacket, pSessionPresent );*/

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
            status = deserializeConnack( pConnackProperties, propertyLength, &pVariableHeader, propBuffer );
        }
    }
    return status;
}

MQTTStatus_t MQTT_ValidatePublishParams( const MQTTPublishInfo_t * pPublishInfo,
                                            uint8_t retainAvailable,
                                            uint8_t maxQos, 
                                            uint16_t topicAlias,
                                            uint32_t maxPacketSize)
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
    else if ((topicAlias == 0U) && (pPublishInfo->topicNameLength == 0U))
    {
        LogError(("Invalid topic name for PUBLISH: pTopicName=%p, "
            "topicNameLength=%hu.",
            (void*)pPublishInfo->pTopicName,
            (unsigned short)pPublishInfo->topicNameLength));
        status = MQTTBadParameter;
    }
    else if ((pPublishInfo->pTopicName == NULL) && (pPublishInfo->topicNameLength != 0U))
    {
        LogError(("Invalid topic name for PUBLISH: pTopicName=%p, "
            "topicNameLength=%hu.",
            (void*)pPublishInfo->pTopicName,
            (unsigned short)pPublishInfo->topicNameLength));
        status = MQTTBadParameter;
    }
    else if (maxPacketSize == 0U)
    {
        status = MQTTBadParameter;
    }
    else
    {
        status = MQTTSuccess;
    }

    return status;
}

MQTTStatus_t validatePublishProperties(uint16_t serverTopicAliasMax, MqttPropBuilder_t* propBuilder, uint16_t* topicAlias)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = propBuilder->currentIndex;
    const uint8_t* pLocalIndex = propBuilder->pBuffer;
    bool topicAliasBool = false;
    if (status == MQTTSuccess)
    {
        while ((propertyLength > 0U) && (status == MQTTSuccess))
        {
            uint8_t propertyId = *pLocalIndex;
            pLocalIndex = &pLocalIndex[1];
            propertyLength -= sizeof(uint8_t);
            if (propertyId == MQTT_TOPIC_ALIAS_ID)
            {
                decodeuint16_t(topicAlias, &propertyLength, &topicAliasBool, &pLocalIndex);

                if (serverTopicAliasMax < *topicAlias)
                {
                    LogError(("Protocol Error: Topic Alias greater than Topic Alias Max"));
                    status = MQTTBadParameter;
                }
                break;
            }
        }
    }
    return status;
}
MQTTStatus_t validateSubscribeProperties(uint8_t isSubscriptionIdAvailable, MqttPropBuilder_t* propBuilder)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = propBuilder->currentIndex;
    const uint8_t* pLocalIndex = propBuilder->pBuffer;

    if (status == MQTTSuccess)
    {
        while ((propertyLength > 0U) && (status == MQTTSuccess))
        {
            uint8_t propertyId = *pLocalIndex;
            pLocalIndex = &pLocalIndex[1];
            propertyLength -= sizeof(uint8_t);
            if (propertyId == MQTT_SUBSCRIPTION_ID_ID)
            {
                if (isSubscriptionIdAvailable == 0)
                {
                    LogError(("Protocol Error: Topic Alias greater than Topic Alias Max"));
                    status = MQTTBadParameter;
                }
                break;
            }

        }
    }
    return status;
}


MQTTStatus_t MQTT_GetPublishPacketSize( MQTTPublishInfo_t * pPublishInfo,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize,
                                            size_t publishPropertyLength)
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
    else
    {
        status = calculatePublishPacketSize( pPublishInfo, pRemainingLength, pPacketSize, maxPacketSize , publishPropertyLength);
    }

    return status;
}

static MQTTStatus_t readSubackStatusV5(size_t statusCount, const uint8_t* pStatusStart, MQTTReasonCodeInfo_t * ackInfo)
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t subscriptionStatus = 0;
    size_t i = 0;
    assert(pStatusStart != NULL);

    for (i = 0; i < statusCount; i++) {
        subscriptionStatus = pStatusStart[i];
        switch (subscriptionStatus)
        {
        case 0x00:
        case 0x01:
        case 0x02:
            LogDebug(("Topic Filter %lu accepted, max QoS %u.",
                (unsigned long)i,
                (unsigned int)subscriptionStatus));
            break; 
        case 0x80:
            LogWarn(("Topic Filter Refused"));
            /** case 83 , 87, 8F, 91 , 97, 9E, A1 , A2 */
            break;
        case 0x83:
            LogWarn(("Implementation specific error."));
            break; 
        case 0x87:
            LogWarn(("Not authorized."));
        case 0x8F:
            LogWarn(("Topic Name Invalid."));
            break;
        case 0x91:
            LogWarn(("Packet Identifier In Use."));
            break;
        case 0x97:
            LogWarn(("Quota Exceeded."));
            break; 
        case 0x9E:
            LogWarn(("Shared Subscriptions Not Supported."));
            break;
        case 0xA1:
            LogWarn(("Subscription Identifiers Not Supported."));
            break;
        case 0xA2:
            LogWarn(("Wildcard Subscriptions Not Supported."));
            break; 
        default:
            LogError(("Bad Subscribe status %u.",
                (unsigned int)subscriptionStatus));
            status = MQTTBadResponse;
            break;
        }
        if (status == MQTTBadResponse)
        {
            break;
        }
    }
    if (status == MQTTSuccess)
    {
        ackInfo->reasonCode = pStatusStart;
        ackInfo->reasonCodeLength = statusCount; 
    }
    return status;
}

static MQTTStatus_t deserializeSubackProperties(MqttPropBuilder_t *propBuffer, const uint8_t* pIndex)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t propertyLength = 0U;
    const uint8_t* pLocalIndex = pIndex;

    status = decodeVariableLength(pLocalIndex, &propertyLength);


    if (status == MQTTSuccess)
    {
        pLocalIndex = &pLocalIndex[remainingLengthEncodedSize(propertyLength)];
        /**Validate remaining Length */
        propBuffer->bufferLength = propertyLength; 
        propBuffer->pBuffer = pLocalIndex; 
    }
    const char* pReasonString; 
    uint16_t reasonStringLength; 
    if (status == MQTTSuccess)
    {
        while ((propertyLength > 0U) && (status == MQTTSuccess))
        {
            /** Decode propertyId  -> reason string if or user property id*/
            uint8_t propertyId = *pLocalIndex;
            bool reasonString = false;
            pLocalIndex = &pLocalIndex[1];
            propertyLength -= sizeof(uint8_t);
            switch (propertyId)
            {
            case MQTT_REASON_STRING_ID:
                status = decodeutf_8(&pReasonString, &reasonStringLength, &propertyLength,&reasonString,&pLocalIndex);
                break;
            case MQTT_USER_PROPERTY_ID:
                status = decodeAndDiscard(&propertyLength, &pLocalIndex);
                break;
            default:
                status = MQTTProtocolError;
                break;

            }
        } 
    }
    return status;
}
MQTTStatus_t MQTTV5_DeserializeSuback( MQTTReasonCodeInfo_t * subackReasonCodes,
                                       const MQTTPacketInfo_t* pSuback,
                                       uint16_t* pPacketId,
                                       MqttPropBuilder_t *propBuffer)
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t* pIndex = pSuback-> pRemainingData;
    size_t remainingLength = pSuback->remainingLength; 

    assert(pSuback != NULL);
    assert(pPacketId != NULL);

    pIndex = pSuback->pRemainingData;

    if (pSuback->remainingLength < 3U)
    {
        LogError(("Suback Packet Cannot have a remaining Length of less than 3"));
        status = MQTTBadResponse;
    }
    else
    {
        *pPacketId = UINT16_DECODE(pIndex);
        pIndex = &pIndex[2];
        LogDebug(("Packet Identifier is %hu.",
            (unsigned short)*pPacketId));

        if (*pPacketId == 0U)
        {
            LogError(("Packet Id cannot be 0")); 
            status = MQTTBadParameter;
        }
    }
    if (status == MQTTSuccess && pSuback->remainingLength > 4U)
    {
        status = deserializeSubackProperties(propBuffer, pIndex);
    }
    if (status == MQTTSuccess)
    {
        status = readSubackStatusV5(remainingLength - sizeof(uint16_t) - propBuffer->bufferLength - remainingLengthEncodedSize(propBuffer->bufferLength), &pIndex[propBuffer->bufferLength + remainingLengthEncodedSize(propBuffer->bufferLength)] , subackReasonCodes);
    }
    return status;
}
MQTTStatus_t MQTTV5_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                    uint16_t * pPacketId,
                                    MQTTReasonCodeInfo_t * pReasonCode,
                                    bool requestProblem,
                                    uint32_t maxPacketSize,
                                    MqttPropBuilder_t *propBuffer)
{
    MQTTStatus_t status = MQTTSuccess;

    if( pIncomingPacket == NULL || (pReasonCode == NULL))
    {
        LogError( ( "pIncomingPacket , pReasonCode cannot be NULL." ) );
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
                status = deserializeSimpleAckV5( pIncomingPacket, pPacketId, pReasonCode, requestProblem, propBuffer );

                if( status == MQTTSuccess && pIncomingPacket->remainingLength > 2 )
                {
                    status = logAckResponseV5( *pReasonCode->reasonCode, *pPacketId);
                }

                break;

            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                status = deserializeSimpleAckV5( pIncomingPacket, pPacketId, pReasonCode, requestProblem, propBuffer );

                if( status == MQTTSuccess && pIncomingPacket->remainingLength > 2)
                {
                    status = logSimpleAckResponseV5( *pReasonCode->reasonCode, *pPacketId);
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

uint8_t * MQTTV5_SerializeAckFixed( uint8_t * pIndex,
                                    uint8_t packetType,
                                    uint16_t packetId,
                                    size_t remainingLength, 
                                    uint8_t reasonCode)
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
    *pIndexLocal = reasonCode;
    pIndexLocal++;
    return pIndexLocal;
}


MQTTStatus_t MQTTV5_GetAckPacketSize(size_t* pRemainingLength,
    size_t* pPacketSize,
    uint32_t maxPacketSize, 
    size_t ackPropertyLength)
{
    MQTTStatus_t status = MQTTSuccess;
    size_t length = 0U;
    size_t propertyLength = 0U;
    size_t packetSize = 0U;

    propertyLength = ackPropertyLength ; 

    /*Validate the parameters.*/
    if ((pRemainingLength == NULL) || (pPacketSize == NULL))
    {
        status = MQTTBadParameter;
    }
    else if (maxPacketSize == 0U)
    {
        status = MQTTBadParameter;
    }
    else
    {
        length += MQTT_PUBLISH_ACK_PACKET_SIZE_WITH_REASON;

        length += remainingLengthEncodedSize(propertyLength) + propertyLength;

        *pRemainingLength = length;

    }
    if (status == MQTTSuccess)
    {
        packetSize = length + 1U + remainingLengthEncodedSize(length);

        if (packetSize > maxPacketSize)
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

MQTTStatus_t MQTT_GetDisconnectPacketSize(  size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize,
                                            size_t disconnectPropLen, 
                                            MQTTDisconnectReasonCode_t reasonCode )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t length = 0U;
    size_t packetSize = 0U;
    size_t propertyLength = 0U;

    /*Validate the arguments.*/
    if(( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
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
    else if(validateDisconnectResponse( reasonCode, false ) != MQTTSuccess )
    {
        LogError( ( "Invalid reason code." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /*Reason code.*/
        length += 1U;
    }

    propertyLength += disconnectPropLen;

    if( status == MQTTSuccess )
    {
        /*Validate the length.*/
        if( ( propertyLength + remainingLengthEncodedSize(propertyLength) + 1U) < MQTT_MAX_REMAINING_LENGTH )
        {
            /*We have successfully calculated the property length.*/
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

uint8_t * MQTT_SerializeDisconnectFixed( uint8_t * pIndex,
                                        MQTTDisconnectReasonCode_t reasonCode,
                                        size_t remainingLength)
{
    uint8_t * pIndexLocal = pIndex;
    assert( pIndex != NULL );
    /* The first byte in the publish ack packet is the control packet type. */
    *pIndexLocal = MQTT_PACKET_TYPE_DISCONNECT;
    pIndexLocal++;
    /*After the packet type fixed header has remaining length.*/
    pIndexLocal = encodeRemainingLength( pIndexLocal, remainingLength );
    /*Encode the reason code.*/
    *pIndexLocal = reasonCode;
    pIndexLocal++;

    return pIndexLocal;
}


    MQTTStatus_t MQTTV5_DeserializeDisconnect( const MQTTPacketInfo_t * pPacket,
                                               uint32_t maxPacketSize,
                                               MQTTReasonCodeInfo_t* pDisconnectInfo, 
                                               MqttPropBuilder_t *propBuffer)
    {
        MQTTStatus_t status = MQTTSuccess;
        const uint8_t * pIndex = NULL;
        size_t propertyLength = 0U; 

        /*Validate the arguments*/
        if( ( pPacket == NULL ) || ( pPacket->pRemainingData == NULL ) )
        {
            status = MQTTBadParameter;
        }
        else if( ( pDisconnectInfo == NULL ) )
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
        else if (pPacket->remainingLength == 0U)
        {
            /*pIndex = pPacket->pRemainingData; 
            pDisconnectInfo->reasonCode = MQTTReasonSuccess; 
            pDisconnectInfo->reasonCodeLength = 1U; 
            pIndex++; */
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
                status = decodeVariableLength( pIndex, &propertyLength );

                if( status == MQTTSuccess )
                {
                    pIndex = &pIndex[ remainingLengthEncodedSize( propertyLength ) ];

                    propBuffer->bufferLength = propertyLength; 
                    propBuffer->pBuffer = pIndex; 

                    /*Validate the remaining length.*/
                    if( pPacket->remainingLength != ( propertyLength + remainingLengthEncodedSize( propertyLength ) + 1U ) )
                    {
                        status = MQTTMalformedPacket;
                    }
                }
            }
        }
        const char* pReasonString; 
        uint16_t reasonStringLength; 
        const char* pServerRef; 
        uint16_t pServerRefLength; 

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
                        status = decodeutf_8( &pReasonString, &reasonStringLength, &propertyLength,&reasonString, &pIndex );
                        break;

                    case MQTT_USER_PROPERTY_ID:
                        status = decodeAndDiscard( &propertyLength, &pIndex );
                        break;

                    case MQTT_SERVER_REF_ID:
                        status = decodeutf_8( &pServerRef, &pServerRefLength, &propertyLength,&serverRef, &pIndex );
                        break;

                    default:
                        status = MQTTProtocolError;
                        break;
                }
            }
        }

        return status;
    }

/*-----------------------------------------------------------*/
/*
* API calls for Optional Subscribe Properties
*/
    MQTTStatus_t MQTTPropAdd_SubscribeId(MqttPropBuilder_t* pPropertyBuilder, size_t subscriptionId)
    {
        MQTTStatus_t status = MQTTSuccess;
        uint8_t* pIndex; 
        if (subscriptionId == 0)
        {
            LogError(("Subscription Id cannot 0 for subscribe properties : Protocol Error "));
            status = MQTTBadParameter;
        }
        else if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet , MQTT_SUBSCRIPTION_ID_POS))
        {
            LogError(("Subscription Id already set"));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->currentIndex + 1 + remainingLengthEncodedSize(subscriptionId) > pPropertyBuilder->bufferLength)
        {
            LogError(("Buffer too small to add subscription id"));
            status = MQTTBadParameter;
        }
        else {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_SUBSCRIPTION_ID_ID;
            pIndex++;
            pIndex = encodeRemainingLength(pIndex, subscriptionId);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_SUBSCRIPTION_ID_POS);
        }
        return status;
    }

/*
* API call for sending User Properties 
*/

    MQTTStatus_t MQTTPropAdd_UserProp(MqttPropBuilder_t* pPropertyBuilder, MQTTUserProperty_t *userProperty)
    {
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (userProperty == NULL)
        {
            LogError(("Arguments cannot be NULL : userProperty=%p.", (void*)userProperty));
            status = MQTTBadParameter;
        }
        else if (userProperty->pKey == NULL || userProperty->pValue == NULL || userProperty->keyLength == 0U || userProperty->valueLength == 0U)
        {
            LogError(("Arguments cannot be NULL : pUserProperties->userProperty->pKey=%p," , " pUserProperties->userProperty->pValue=%p", "Key Length = %u" , "Value Length = %u" , (void*)userProperty->pKey, (void*)userProperty->pValue, userProperty->keyLength , userProperty->valueLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* start = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            uint8_t* pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;

            *pIndex = MQTT_USER_PROPERTY_ID;
            pIndex++;

            /*Encoding key*/
            pIndex = encodeString(pIndex, userProperty->pKey, userProperty->keyLength);
            pIndex = encodeString(pIndex, userProperty->pValue, userProperty->valueLength);

            pPropertyBuilder->currentIndex += (size_t)(pIndex - start);
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnSessionExpiry(MqttPropBuilder_t* pPropertyBuilder, uint32_t sessionExpiry)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_SESSION_EXPIRY_INTERVAL_POS))
        {
            LogError(("Connect Session Expiry Already Set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_SESSION_EXPIRY_ID;
            pIndex++;
            pIndex[0] = UINT32_BYTE3(sessionExpiry);
            pIndex[1] = UINT32_BYTE2(sessionExpiry);
            pIndex[2] = UINT32_BYTE1(sessionExpiry);
            pIndex[3] = UINT32_BYTE0(sessionExpiry);
            pIndex = &pIndex[4];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_SESSION_EXPIRY_INTERVAL_POS);
            pPropertyBuilder->currentIndex += 5;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnReceiveMax(MqttPropBuilder_t* pPropertyBuilder, uint16_t receiveMax)
    {
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if ((receiveMax == 0) || (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_RECEIVE_MAXIMUM_POS)))
        {
            LogError(("Invalid arguments passed to MQTTPropAdd_ConnReceiveMax."));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_RECEIVE_MAX_ID;
            pIndex++;
            pIndex[0] = UINT16_HIGH_BYTE(receiveMax);
            pIndex[1] = UINT16_LOW_BYTE(receiveMax);
            pIndex = &pIndex[2];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_RECEIVE_MAXIMUM_POS);
            pPropertyBuilder->currentIndex += 3;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnMaxPacketSize(MqttPropBuilder_t* pPropertyBuilder, uint32_t maxPacketSize)
    {
        MQTTStatus_t status = MQTTSuccess;
        uint8_t* pIndex;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if ((maxPacketSize == 0) || (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_MAX_PACKET_SIZE_POS)))
        {
            LogError(("Max packet size already set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_MAX_PACKET_SIZE_ID;
            pIndex++;
            pIndex[0] = UINT32_BYTE3(maxPacketSize);
            pIndex[1] = UINT32_BYTE2(maxPacketSize);
            pIndex[2] = UINT32_BYTE1(maxPacketSize);
            pIndex[3] = UINT32_BYTE0(maxPacketSize);
            pIndex = &pIndex[4];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_MAX_PACKET_SIZE_POS);
            pPropertyBuilder->currentIndex += 5;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnTopicAliasMax(MqttPropBuilder_t* pPropertyBuilder, uint16_t topicAliasMax)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess; 
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_MAX_POS))
        {
            LogError(("Topic Alias Maximum already set. "));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_TOPIC_ALIAS_MAX_ID;
            pIndex++;
            pIndex[0] = UINT16_HIGH_BYTE(topicAliasMax);
            pIndex[1] = UINT16_LOW_BYTE(topicAliasMax);
            pIndex = &pIndex[2];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_TOPIC_ALIAS_MAX_POS); 
            pPropertyBuilder->currentIndex += 3;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnRequestRespInfo(MqttPropBuilder_t* pPropertyBuilder, bool requestResponseInfo)
    {
        uint8_t* pIndex; 
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_REQUEST_RESPONSE_INFO_POS))
        {
            LogError(("Request Response Info already set."));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_REQUEST_RESPONSE_ID;
            pIndex++;
            *pIndex = 1U;
            pIndex++;
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_REQUEST_RESPONSE_INFO_POS);
            pPropertyBuilder->currentIndex += 2;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnRequestProbInfo(MqttPropBuilder_t* pPropertyBuilder, bool requestProblemInfo)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_REQUEST_PROBLEM_INFO_POS))
        {
            LogError(("Request Problem Info already set."));
            status =  MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_REQUEST_RESPONSE_ID;
            pIndex++;
            *pIndex = 1U;
            pIndex++;
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_REQUEST_PROBLEM_INFO_POS);
            pPropertyBuilder->currentIndex += 2;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ConnAuthMethod(MqttPropBuilder_t* pPropertyBuilder,
                                            const char* authMethod,
                                            uint16_t authMethodLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess; 
        if ((pPropertyBuilder == NULL) || (authMethod == NULL) || (authMethodLength == 0U))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p,","authMethod = %p" , "authMethodLength = %u" , (void*)pPropertyBuilder , (void*)authMethod , authMethodLength));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS))
        {
            LogError(("Auth Method already set."));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_AUTH_METHOD_ID;
            pIndex++;
            pIndex = encodeString(pIndex, authMethod, authMethodLength);
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
        }
        return status;
    }


    MQTTStatus_t MQTTPropAdd_ConnAuthData( MqttPropBuilder_t* pPropertyBuilder,
                                           const char* authData,
                                           uint16_t authDataLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess; 
        if ((pPropertyBuilder == NULL) || (authData == NULL) || (authDataLength == 0U))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p,", "authMethod = %p", "authMethodLength = %u", (void*)pPropertyBuilder, (void*)authData, authDataLength));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if ((UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_DATA_POS)) || ( UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS) == 0) )
        {
            LogError(("Invalid Auth data"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_AUTH_DATA_ID;
            pIndex++;
            pIndex = encodeString(pIndex, authData, authDataLength);
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_DATA_POS);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_PubPayloadFormat(MqttPropBuilder_t* pPropertyBuilder, bool payloadFormat)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if ((UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_PAYLOAD_FORMAT_INDICATOR_POS)))
        {
            LogError(("Payload Format already set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_PAYLOAD_FORMAT_ID;
            pIndex++;
            *pIndex = payloadFormat;
            pIndex++;
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_PAYLOAD_FORMAT_INDICATOR_POS);
            pPropertyBuilder->currentIndex += 2;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_PubMessageExpiry(MqttPropBuilder_t* pPropertyBuilder, uint32_t messageExpiry)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if ((UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_MESSAGE_EXPIRY_INTERVAL_POS)))
        {
            LogError(("Message Expiry Interval already set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_MSG_EXPIRY_ID;
            pIndex++;
            pIndex[0] = UINT32_BYTE3(messageExpiry);
            pIndex[1] = UINT32_BYTE2(messageExpiry);
            pIndex[2] = UINT32_BYTE1(messageExpiry);
            pIndex[3] = UINT32_BYTE0(messageExpiry);
            pIndex = &pIndex[4];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_MESSAGE_EXPIRY_INTERVAL_POS);
            pPropertyBuilder->currentIndex += 5;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_PubTopicAlias(MqttPropBuilder_t* pPropertyBuilder,
        uint16_t topicAlias)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_TOPIC_ALIAS_POS))
        {
            LogError(("TopicAlias already present"));
            status = MQTTBadParameter;
        }
        else if (topicAlias == 0U)
        {
            LogError(("Topic Alias cannot be 0"));
            status = MQTTBadParameter; 
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_TOPIC_ALIAS_ID;
            pIndex++;
            pIndex[0] = UINT16_HIGH_BYTE(topicAlias);
            pIndex[1] = UINT16_LOW_BYTE(topicAlias);
            pIndex = &pIndex[2];
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_TOPIC_ALIAS_POS);
            pPropertyBuilder->currentIndex += 3;
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_PubResponseTopic(MqttPropBuilder_t* pPropertyBuilder,
        const char* responseTopic,
        uint16_t responseTopicLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (responseTopic == NULL)
        {
            LogError(("Arguments cannot be NULL : responseTopic=%p.", (void*)responseTopic));
            status = MQTTBadParameter;
        }
        else if (responseTopicLength == 0U)
        {
            LogError(("Response Topic Length cannot be 0"));
            status = MQTTBadParameter;
        }
        else if ((UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_RESPONSE_TOPIC_POS)))
        {
            LogError(("Response Topic already set"));
            status = MQTTBadParameter;
        }
        else if ( (strchr(responseTopic, '#') != NULL) || (strchr(responseTopic, '+') != NULL))
        {
            LogError(("Protocol Error : Response Topic contains wildcards"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_RESPONSE_TOPIC_ID;
            pIndex++;
            pIndex = encodeString(pIndex, responseTopic, responseTopicLength);

            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_RESPONSE_TOPIC_POS);
        }
        return status;
    }
    
    MQTTStatus_t MQTTPropAdd_PubCorrelationData(MqttPropBuilder_t* pPropertyBuilder,
                                                const void* pCorrelationData,
                                                uint16_t correlationLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pCorrelationData == NULL)
        {
            LogError(("Arguments cannot be NULL : pCorrelationData=%p.", (void*)pCorrelationData));
            status = MQTTBadParameter;
        }
        else if (correlationLength == 0U)
        {
            LogError(("Correlation Data Length cannot be 0"));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_CORRELATION_DATA_POS))
        {
            LogError(("Correlation Data already set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_CORRELATION_DATA_ID;
            pIndex++;
            pIndex = encodeBinaryData(pIndex, pCorrelationData, correlationLength);
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_CORRELATION_DATA_POS);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
        }
        return status ;
    }

    MQTTStatus_t MQTTPropAdd_PubContentType(MqttPropBuilder_t* pPropertyBuilder,
        const char* contentType,
        uint16_t contentTypeLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (contentType == NULL)
        {
            LogError(("Arguments cannot be NULL : contentType=%p.", (void*)contentType));
            status = MQTTBadParameter;
        }
        else if (contentTypeLength == 0U)
        {
            LogError(("Content Type Length cannot be 0"));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_CONTENT_TYPE_POS))
        {
            LogError(("Content type already set"));
            status = MQTTBadParameter;
        }
        else
        {

            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_CONTENT_TYPE_ID;
            pIndex++;
            pIndex = encodeString(pIndex, contentType, contentTypeLength);
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_PUBLISH_CONTENT_TYPE_POS);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
        }
        return status;
    }

    MQTTStatus_t MQTTPropAdd_ReasonString(MqttPropBuilder_t* pPropertyBuilder,
        const char* pReasonString,
        uint16_t reasonStringLength)
    {
        uint8_t* pIndex;
        MQTTStatus_t status = MQTTSuccess;
        if ((pPropertyBuilder == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)pPropertyBuilder));
            status = MQTTBadParameter;
        }
        else if (pPropertyBuilder->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)pPropertyBuilder->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pReasonString == NULL)
        {
            LogError(("Arguments cannot be NULL : pReasonString=%p.", (void*)pReasonString));
            status = MQTTBadParameter;
        }
        else if (reasonStringLength == 0U)
        {
            LogError(("Reason String Length cannot be 0"));
            status = MQTTBadParameter;
        }
        else if (UINT32_CHECK_BIT(pPropertyBuilder->fieldSet, MQTT_REASON_STRING_POS))
        {
            LogError(("Reason String already set"));
            status = MQTTBadParameter;
        }
        else
        {
            pIndex = pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex;
            *pIndex = MQTT_REASON_STRING_ID;
            pIndex++;
            pIndex = encodeString(pIndex, pReasonString, reasonStringLength);
            pPropertyBuilder->fieldSet = UINT32_SET_BIT(pPropertyBuilder->fieldSet, MQTT_REASON_STRING_POS);
            pPropertyBuilder->currentIndex += (size_t)(pIndex - (pPropertyBuilder->pBuffer + pPropertyBuilder->currentIndex));
        }
        return status;
    }


    /*
    * Deserializing properties - Publish
    */


    MQTTStatus_t MQTTPropGet_PubTopicAlias(MqttPropBuilder_t *propBuffer, uint16_t* topicAlias)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (topicAlias == NULL)
        {
            LogError(("Arguments cannot be NULL : topicAlias=%p.", (void*)topicAlias));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint16_t(topicAlias, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubPayloadFormatIndicator(MqttPropBuilder_t* propBuffer, uint8_t* payloadFormat)
    {

        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (payloadFormat == NULL)
        {
            LogError(("Arguments cannot be NULL : payloadFormat=%p.", (void*)payloadFormat));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint8_t(payloadFormat, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubResponseTopic(MqttPropBuilder_t* propBuffer, const char** responseTopic, uint16_t* responseTopicLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (responseTopic == NULL || responseTopicLength == NULL)
        {
            LogError(("Arguments cannot be NULL : responseTopic=%p, responseTopicLength = %p", (void*)responseTopic , (void*)responseTopicLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(responseTopic, responseTopicLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubCorrelationData(MqttPropBuilder_t* propBuffer, const void** correlationData, uint16_t* correlationLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (correlationData == NULL || correlationLength == NULL)
        {
            LogError(("Arguments cannot be NULL : responseTopic=%p, responseTopicLength = %p", (void*)correlationData, (void*)correlationLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeBinaryData(correlationData, correlationLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubMessageExpiryInterval(MqttPropBuilder_t* propBuffer, uint32_t* msgExpiryInterval)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if(msgExpiryInterval == NULL)
        {
            LogError(("Arguments cannot be NULL : msgExpiryInterval=%p.", (void*)msgExpiryInterval));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint32_t(msgExpiryInterval, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubContentType(MqttPropBuilder_t* propBuffer, const char** pContentType, uint16_t* contentTypeLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pContentType == NULL || contentTypeLength == NULL)
        {
            LogError(("Arguments cannot be NULL : pContentType=%p, contentTypeLength = %p", (void*)pContentType, (void*)contentTypeLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(pContentType, contentTypeLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_PubSubscriptionId(MqttPropBuilder_t* propBuffer, size_t* subscriptionId)
    {
        MQTTStatus_t status = MQTTSuccess;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (subscriptionId == NULL)
        {
            LogError(("Arguments cannot be NULL : subscriptionId=%p.", (void*)subscriptionId));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeVariableLength(startOfProp, subscriptionId);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnSessionExpiry(MqttPropBuilder_t* propBuffer, uint32_t *sessionExpiry)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (sessionExpiry == NULL)
        {
            LogError(("Arguments cannot be NULL : sessionExpiry=%p.", (void*)sessionExpiry));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint32_t(sessionExpiry, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnTopicAliasMax(MqttPropBuilder_t *propBuffer , uint16_t* topicAliasMax)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (topicAliasMax == NULL)
        {
            LogError(("Arguments cannot be NULL : topicAliasMax=%p.", (void*)topicAliasMax));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint16_t(topicAliasMax, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnReceiveMax(MqttPropBuilder_t* propBuffer, uint16_t* receiveMax)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (receiveMax == NULL)
        {
            LogError(("Arguments cannot be NULL : receiveMax=%p.", (void*)receiveMax));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint16_t(receiveMax, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnMaxQos(MqttPropBuilder_t* propBuffer, uint8_t* maxQos)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (maxQos == NULL)
        {
            LogError(("Arguments cannot be NULL : maxQos=%p.", (void*)maxQos));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint8_t(maxQos, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnRetainAvailable(MqttPropBuilder_t* propBuffer, uint8_t* retainAvailable)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (retainAvailable == NULL)
        {
            LogError(("Arguments cannot be NULL : retainAvailable=%p.", (void*)retainAvailable));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint8_t(retainAvailable, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnMaxPacketSize(MqttPropBuilder_t* propBuffer, uint32_t* maxPacketSize)
    {
        MQTTStatus_t status = MQTTSuccess; 
        bool propFlag = false; 
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (maxPacketSize == NULL)
        {
            LogError(("Arguments cannot be NULL : maxPacketSize=%p.", (void*)maxPacketSize));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint32_t(maxPacketSize, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTTPropGet_ConnClientId(MqttPropBuilder_t* propBuffer, const char** pClientId, uint16_t* clientIdLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pClientId == NULL || clientIdLength == NULL)
        {
            LogError(("Arguments cannot be NULL : pClientId=%p, clientIdLength = %p", (void*)pClientId, (void*)clientIdLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(pClientId, clientIdLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status;
    }

    MQTTStatus_t MQTTPropGet_ConnWildcard(MqttPropBuilder_t* propBuffer, uint8_t* isWildCardAvailable)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (isWildCardAvailable == NULL)
        {
            LogError(("Arguments cannot be NULL : isWildCardAvailable=%p.", (void*)isWildCardAvailable));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint8_t(isWildCardAvailable, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status;
    }

    MQTTStatus_t MQTTPropGet_ConnSubId(MqttPropBuilder_t* propBuffer, uint8_t* isSubIdAvailable)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (isSubIdAvailable == NULL)
        {
            LogError(("Arguments cannot be NULL : isSubIdAvailable=%p.", (void*)isSubIdAvailable));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeuint8_t(isSubIdAvailable, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status;
    }


    MQTTStatus_t MQTTPropGet_UserProp(MqttPropBuilder_t* propBuffer,
        const char** pUserPropKey,
        uint16_t* pUserPropKeyLen,
        const char** pUserPropVal,
        uint16_t* pUserPropValLen)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pUserPropKey == NULL || pUserPropKeyLen == NULL || pUserPropVal==NULL || pUserPropValLen== NULL)
        {
            LogError(("Arguments cannot be NULL : pUserPropKey = %p , pUserPropKeyLen = %p , pUserPropVal = %p , pUserPropValLen = %p", (void *)pUserPropKey, (void*)pUserPropKeyLen , (void*)pUserPropVal , (void*) pUserPropValLen));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(pUserPropKey, pUserPropKeyLen, &propertyLength, &propFlag, &startOfProp);
            propFlag = false;
            if (status == MQTTSuccess)
            {
                status = decodeutf_8(pUserPropVal, pUserPropValLen, &propertyLength, &propFlag, &startOfProp);
            }
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }


    MQTTStatus_t MQTTPropGet_ReasonString(MqttPropBuilder_t* propBuffer, const char** pReasonString, uint16_t* reasonStringLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pReasonString == NULL || reasonStringLength == NULL)
        {
            LogError(("Arguments cannot be NULL : pReasonString=%p, reasonStringLength = %p", (void*)pReasonString, (void*)reasonStringLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(pReasonString, reasonStringLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }
    
    MQTTStatus_t MQTTPropGet_DisconnectServerRef(MqttPropBuilder_t* propBuffer, const char** pServerRef, uint16_t* serverRefLength)
    {
        MQTTStatus_t status = MQTTSuccess;
        bool propFlag = false;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else if (pServerRef == NULL || serverRefLength == NULL)
        {
            LogError(("Arguments cannot be NULL : pServerRef=%p, reasonStringLength = %p", (void*)pServerRef, (void*)serverRefLength));
            status = MQTTBadParameter;
        }
        else
        {
            uint8_t* startOfProp = propBuffer->pBuffer + propBuffer->currentIndex;
            size_t propertyLength = propBuffer->bufferLength - propBuffer->currentIndex;
            status = decodeutf_8(pServerRef, serverRefLength, &propertyLength, &propFlag, &startOfProp);
            if (status == MQTTSuccess)
            {
                propBuffer->currentIndex = (size_t)(startOfProp - propBuffer->pBuffer);
            }
        }
        return status; 
    }

    MQTTStatus_t MQTT_IncomingGetNextProp(MqttPropBuilder_t* propBuffer, uint8_t* propertyId)
    {
        MQTTStatus_t status = MQTTSuccess;
        if ((propBuffer == NULL))
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder=%p.", (void*)propBuffer));
            status = MQTTBadParameter;
        }
        else if (propBuffer->pBuffer == NULL)
        {
            LogError(("Arguments cannot be NULL : pPropertyBuilder->pBuffer=%p.", (void*)propBuffer->pBuffer));
            status = MQTTBadParameter;
        }
        else 
        {
            if (propBuffer->currentIndex < propBuffer->bufferLength)
            {
                *propertyId = *(propBuffer->pBuffer + propBuffer->currentIndex);
                propBuffer->currentIndex += 1;
            }
            else
            {
                status = MQTTBadParameter;
                LogError(("Invalid property buffer"));
            }
        }

        return status;
    }


