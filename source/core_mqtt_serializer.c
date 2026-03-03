/*
 * coreMQTT
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
#include <inttypes.h>
#include <stdbool.h>

#include "core_mqtt.h"
#include "core_mqtt_serializer.h"
#include "transport_interface.h"
#include "private/core_mqtt_serializer_private.h"

/* Include config defaults header to get default values of configs. */
#include "core_mqtt_config_defaults.h"

/**
 * @brief Size of the fixed and variable header of a CONNECT packet.
 */
#define MQTT_PACKET_CONNECT_HEADER_SIZE             ( 10U )

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
#define MQTT_PACKET_PINGREQ_SIZE                    ( 2U )

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
 * @brief Minimum number of bytes in the CONNACK Packet.
 * CONNECT Acknowledge Flags    0 + 1 = 1
 * CONNECT Reason Code            + 1 = 2
 * Property Length byte (min)     + 1 = 3
 */
#define MQTT_PACKET_CONNACK_MINIMUM_SIZE            ( 3U )

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
                                    uint32_t remainingLength,
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
                                                uint32_t * pRemainingLength,
                                                uint32_t * pPacketSize,
                                                uint32_t maxPacketSize,
                                                uint32_t publishPropertyLength );

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
                                                     uint32_t * pRemainingLength,
                                                     uint32_t * pPacketSize,
                                                     uint32_t subscribePropLen,
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
                                                         uint32_t remainingLength,
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
                                    uint32_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );

/**
 * @brief Prints the appropriate message for the CONNACK response code if logs
 * are enabled.
 *
 * @param[in] responseCode MQTT standard CONNACK response code.
 */
static void logConnackResponse( uint8_t responseCode );

/**
 * @brief Retrieve the size of the remaining length if it were to be encoded.
 *
 * @param[in] length The remaining length to be encoded.
 *
 * @return The size of the remaining length if it were to be encoded.
 */
static uint32_t remainingLengthEncodedSize( uint32_t length );

/**
 * @brief Retrieves and decodes the Remaining Length from the network interface
 * by reading a single byte at a time.
 *
 * @param[in] recvFunc Network interface receive function.
 * @param[in] pNetworkContext Network interface context to the receive function.
 *
 * @return The Remaining Length of the incoming packet.
 */
static uint32_t getRemainingLength( TransportRecv_t recvFunc,
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
static MQTTStatus_t checkPublishRemainingLength( uint32_t remainingLength,
                                                 MQTTQoS_t qos,
                                                 uint32_t qos0Minimum );

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
 * @brief Deserialize an MQTT CONNACK packet.
 *
 * @param[out] pConnackProperties To store the deserialized connack properties.
 * @param[in]  pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out]  pSessionPresent Whether a previous session was present.
 * @param[out]  pPropBuffer MQTTPropBuilder_t to store the deserialized properties.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTSuccess, #MQTTServerRefused
 */
static MQTTStatus_t deserializeConnack( MQTTConnectionProperties_t * pConnackProperties,
                                        const MQTTPacketInfo_t * pIncomingPacket,
                                        bool * pSessionPresent,
                                        MQTTPropBuilder_t * pPropBuffer );

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
static MQTTStatus_t deserializeSubUnsubAck( const MQTTPacketInfo_t * incomingPacket,
                                            uint16_t * pPacketId,
                                            MQTTReasonCodeInfo_t * pReasonCodes,
                                            MQTTPropBuilder_t * pPropBuffer );

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
                                                  uint32_t length,
                                                  uint8_t * pIndex,
                                                  MQTTPropBuilder_t * pPropBuffer );

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
static MQTTStatus_t deserializeSubUnsubAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                                      uint8_t * pIndex,
                                                      size_t * pSubackPropertyLength,
                                                      uint32_t remainingLength );

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
static MQTTStatus_t deserializePubAcks( const MQTTPacketInfo_t * pAck,
                                        uint16_t * pPacketIdentifier,
                                        MQTTReasonCodeInfo_t * pReasonCode,
                                        bool requestProblem,
                                        MQTTPropBuilder_t * pPropBuffer );

/**
 * @brief Validate the length and decode the publish ack properties.
 *
 * @param[out] pPropBuffer To store the decoded property.
 * @param[in] pIndex Pointer to the current index of the buffer.
 * @param[in] remainingLength Remaining length of properties in the incoming packet.
 *
 *
 * @return #MQTTSuccess, #MQTTBadResponse.
 **/
static MQTTStatus_t decodePubAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                            uint8_t * pIndex,
                                            uint32_t remainingLength );

/**
 * @brief Prints the appropriate message for the PUBREL, PUBACK response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard PUBREL, PUBACK response code.
 * @param[in] packetIdentifier Packet id of the ack packet.
 *
 * @return #MQTTSuccess, #MQTTServerRefused and #MQTTBadResponse.
 */
static MQTTStatus_t logAckResponse( MQTTSuccessFailReasonCode_t reasonCode,
                                    uint16_t packetIdentifier );

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
                                                  uint32_t remainingLength );

/**
 * @brief Prints and validates the appropriate message for the Disconnect response code if logs
 * are enabled.
 *
 * @param[in] reasonCode MQTT Verion 5 standard DISCONNECT response code.
 * @param[in] incoming To differentiate between outgoing and incoming disconnect.
 *
 * @return #MQTTSuccess, #MQTTBadParameter and #MQTTBadResponse.
 */
static MQTTStatus_t validateDisconnectResponse( MQTTSuccessFailReasonCode_t reasonCode,
                                                bool incoming );

/**
 * @brief Validates the reason codes for the given ACK packet type.
 *
 * @param[in] ackPacketType ACK packet type for which the reason code is being added.
 * @param[in] reasonCode The reason code being added to the ACK packet type.
 *
 * @return #MQTTSuccess or #MQTTBadParameter.
 */
static MQTTStatus_t validateReasonCodeForAck( uint8_t ackPacketType,
                                              MQTTSuccessFailReasonCode_t reasonCode );

/**
 * @brief Validate if a reason code is valid for CONNACK packets.
 *
 * This function checks if the provided reason code is a valid CONNACK
 * reason code according to the MQTT v5 specification.
 *
 * @param[in] reasonCode The reason code to validate.
 *
 * @return #MQTTSuccess if the reason code is valid for CONNACK;
 * #MQTTServerRefused if the reason code indicates server refusal;
 * #MQTTBadResponse if the reason code is invalid.
 */
static inline MQTTStatus_t isValidConnackReasonCode( uint8_t reasonCode );

/**
 * @brief Validate properties in the DISCONNECT packet received from the server.
 *
 * @note Incoming properties are different than the ones the client can send.
 *
 * @param[in] pIndex Pointer to the start of the properties.
 * @param[in] disconnectPropertyLength Length of the properties in the DISCONNECT packet.
 *
 * @return #MQTTSuccess if DISCONNECT is valid;
 * #MQTTBadResponse if the DISCONNECT packet is invalid.
 */
static MQTTStatus_t validateIncomingDisconnectProperties( uint8_t * pIndex,
                                                          uint32_t disconnectPropertyLength );

/*-----------------------------------------------------------*/

static MQTTStatus_t validateReasonCodeForAck( uint8_t ackPacketType,
                                              MQTTSuccessFailReasonCode_t reasonCode )
{
    MQTTStatus_t status = MQTTSuccess;

    switch( ackPacketType )
    {
        case MQTT_PACKET_TYPE_PUBACK:

            if( ( reasonCode != MQTT_REASON_PUBACK_SUCCESS ) &&
                ( reasonCode != MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS ) &&
                ( reasonCode != MQTT_REASON_PUBACK_UNSPECIFIED_ERROR ) &&
                ( reasonCode != MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR ) &&
                ( reasonCode != MQTT_REASON_PUBACK_NOT_AUTHORIZED ) &&
                ( reasonCode != MQTT_REASON_PUBACK_TOPIC_NAME_INVALID ) &&
                ( reasonCode != MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE ) &&
                ( reasonCode != MQTT_REASON_PUBACK_QUOTA_EXCEEDED ) &&
                ( reasonCode != MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID ) )
            {
                LogError( ( "Invalid reason code for PUBACK." ) );
                status = MQTTBadParameter;
            }

            break;

        case MQTT_PACKET_TYPE_PUBREC:

            if( ( reasonCode != MQTT_REASON_PUBREC_SUCCESS ) &&
                ( reasonCode != MQTT_REASON_PUBREC_NO_MATCHING_SUBSCRIBERS ) &&
                ( reasonCode != MQTT_REASON_PUBREC_UNSPECIFIED_ERROR ) &&
                ( reasonCode != MQTT_REASON_PUBREC_IMPLEMENTATION_SPECIFIC_ERROR ) &&
                ( reasonCode != MQTT_REASON_PUBREC_NOT_AUTHORIZED ) &&
                ( reasonCode != MQTT_REASON_PUBREC_TOPIC_NAME_INVALID ) &&
                ( reasonCode != MQTT_REASON_PUBREC_PACKET_IDENTIFIER_IN_USE ) &&
                ( reasonCode != MQTT_REASON_PUBREC_QUOTA_EXCEEDED ) &&
                ( reasonCode != MQTT_REASON_PUBREC_PAYLOAD_FORMAT_INVALID ) )
            {
                LogError( ( "Invalid reason code for PUBREC." ) );
                status = MQTTBadParameter;
            }

            break;

        case MQTT_PACKET_TYPE_PUBREL:

            if( ( reasonCode != MQTT_REASON_PUBREL_SUCCESS ) &&
                ( reasonCode != MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND ) )
            {
                LogError( ( "Invalid reason code for PUBREL." ) );
                status = MQTTBadParameter;
            }

            break;

        case MQTT_PACKET_TYPE_PUBCOMP:

            if( ( reasonCode != MQTT_REASON_PUBCOMP_SUCCESS ) &&
                ( reasonCode != MQTT_REASON_PUBCOMP_PACKET_IDENTIFIER_NOT_FOUND ) )
            {
                LogError( ( "Invalid reason code for PUBCOMP." ) );
                status = MQTTBadParameter;
            }

            break;

        default:
            LogError( ( "Unknown ACK packet type: %" PRIu8 ".", ackPacketType ) );
            status = MQTTBadParameter;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static uint32_t remainingLengthEncodedSize( uint32_t length )
{
    uint32_t encodedSize;

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

    LogDebug( ( "Encoded size for length %" PRIu32 " is %" PRIu32,
                length,
                encodedSize ) );

    return encodedSize;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t calculatePublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                                uint32_t * pRemainingLength,
                                                uint32_t * pPacketSize,
                                                uint32_t maxPacketSize,
                                                uint32_t publishPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t packetSize = 0;
    uint32_t propertyAndPayloadLimit = 0;

    assert( pPublishInfo != NULL );
    assert( pRemainingLength != NULL );
    assert( pPacketSize != NULL );
    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) );
    assert( !CHECK_SIZE_T_OVERFLOWS_32BIT( pPublishInfo->payloadLength ) );

    /* The variable header of a PUBLISH packet always contains the topic name.
     * The first 2 bytes of UTF-8 string contains length of the string.
     */
    packetSize += ( uint32_t ) ( pPublishInfo->topicNameLength + sizeof( uint16_t ) );

    /* The variable header of a QoS 1 or 2 PUBLISH packet contains a 2-byte
     * packet identifier. */
    if( pPublishInfo->qos > MQTTQoS0 )
    {
        packetSize += 2U;
    }

    packetSize += variableLengthEncodedSize( publishPropertyLength );

    /* Calculate the maximum allowed size of the properties and payload combined for
     * the given parameters. */
    propertyAndPayloadLimit = MQTT_MAX_REMAINING_LENGTH - packetSize;

    if( publishPropertyLength > propertyAndPayloadLimit )
    {
        LogError( ( "PUBLISH properties length of %lu cannot exceed "
                    "%lu so as not to exceed the maximum "
                    "remaining length of MQTT 5.0 packet( %" PRIu32 " ).",
                    ( unsigned long ) publishPropertyLength,
                    ( unsigned long ) propertyAndPayloadLimit,
                    ( uint32_t ) MQTT_MAX_REMAINING_LENGTH ) );
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
                        "remaining length of MQTT 5.0 packet( %" PRIu32 " ).",
                        ( unsigned long ) ( pPublishInfo->payloadLength + publishPropertyLength ),
                        ( unsigned long ) ( propertyAndPayloadLimit + publishPropertyLength ),
                        ( uint32_t ) MQTT_MAX_REMAINING_LENGTH ) );
            status = MQTTBadParameter;
        }
        else
        {
            packetSize += ( uint32_t ) pPublishInfo->payloadLength;
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
        else
        {
            *pPacketSize = packetSize;
        }
    }

    LogDebug( ( "PUBLISH packet remaining length=%lu and packet size=%lu.",
                ( unsigned long ) *pRemainingLength,
                ( unsigned long ) *pPacketSize ) );

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializePubAcks( const MQTTPacketInfo_t * pAck,
                                        uint16_t * pPacketIdentifier,
                                        MQTTReasonCodeInfo_t * pReasonCode,
                                        bool requestProblem,
                                        MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;

    assert( pAck != NULL );
    assert( pPacketIdentifier != NULL );
    assert( pAck->pRemainingData != NULL );
    assert( !CHECK_U32T_OVERFLOWS_SIZE_T( pAck->remainingLength ) );

    pIndex = pAck->pRemainingData;

    if( pReasonCode == NULL )
    {
        LogError( ( "pReasonCode cannot be NULL." ) );
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

        LogDebug( ( "Packet identifier is %hu.",
                    ( unsigned short ) *pPacketIdentifier ) );

        /* Packet identifier cannot be 0. */
        if( *pPacketIdentifier == 0U )
        {
            LogError( ( "Packet identifier cannot be 0." ) );
            status = MQTTBadResponse;
        }
    }

    /* If reason code is success, server can choose to send the reason code or not. */
    if( ( status == MQTTSuccess ) && ( pAck->remainingLength > 2U ) )
    {
        pReasonCode->reasonCode = pIndex;
        pReasonCode->reasonCodeLength = 1U;
        pIndex++;
    }

    if( ( status == MQTTSuccess ) && ( pAck->remainingLength > 3U ) )
    {
        /* Protocol error to send user property and reason string if client has set request problem to false. */
        if( requestProblem == false )
        {
            LogError( ( "User property and reason string not expected in ACK packet when requestProblem is false." ) );
            status = MQTTBadResponse;
        }
        else
        {
            /* 3 bytes have been used up by the packet ID (2) and reason code (1). */
            status = decodePubAckProperties( pPropBuffer, pIndex, pAck->remainingLength - 3U );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopic( const MQTTPublishInfo_t * pPublishInfo,
                                                      uint32_t remainingLength,
                                                      uint8_t * pBuffer,
                                                      size_t * headerSize )
{
    uint32_t headerLength;
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* The first byte of a PUBLISH packet contains the packet type and flags. */
    uint8_t publishFlags = MQTT_PACKET_TYPE_PUBLISH;

    if( pPublishInfo == NULL )
    {
        LogError( ( "pPublishInfo cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pBuffer == NULL )
    {
        LogError( ( "pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length must be less than 268435456." ) );
        status = MQTTBadParameter;
    }
    else if( headerSize == NULL )
    {
        LogError( ( "headerSize cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
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

        /* Get the start address of the buffer. */
        pIndex = pBuffer;

        /* Length of serialized packet = First byte
         *                               + Length of encoded remaining length
         *                               + Encoded topic length. */
        headerLength = 1U + remainingLengthEncodedSize( remainingLength ) + 2U;

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

        /* We needn't check whether header length will overflow this cast as header length always < 8 (bytes). */
        *headerSize = ( size_t ) headerLength;
    }

    return status;
}

/*-----------------------------------------------------------*/

static void serializePublishCommon( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    uint32_t remainingLength,
                                    uint16_t packetIdentifier,
                                    const MQTTFixedBuffer_t * pFixedBuffer,
                                    bool serializePayload )
{
    uint8_t * pIndex = NULL;
    uint32_t propertyLength = 0U;
    /* The first byte of a PUBLISH packet contains the packet type and flags. */
    uint8_t publishFlags = MQTT_PACKET_TYPE_PUBLISH;

    assert( pPublishInfo != NULL );
    assert( pFixedBuffer != NULL );
    assert( pFixedBuffer->pBuffer != NULL );
    /* Packet Id should be non zero for Qos 1 and Qos 2. */
    assert( ( pPublishInfo->qos == MQTTQoS0 ) || ( packetIdentifier != 0U ) );
    /* Duplicate flag should be set only for Qos 1 or Qos 2. */
    assert( ( pPublishInfo->dup != true ) || ( pPublishInfo->qos != MQTTQoS0 ) );
    /* The topic name length must fit in 16-bits. */
    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) );

    /* Get the start address of the buffer. */
    pIndex = pFixedBuffer->pBuffer;

    if( ( pPublishProperties != NULL ) && ( pPublishProperties->pBuffer != NULL ) )
    {
        assert( !CHECK_SIZE_T_OVERFLOWS_32BIT( pPublishProperties->currentIndex ) );
        propertyLength = ( uint32_t ) pPublishProperties->currentIndex;
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
        /* No need to set any bits in a QoS0 packet. */
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
                           ( uint16_t ) pPublishInfo->topicNameLength );

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
        pIndex = &pIndex[ ( size_t ) propertyLength ];
    }

    /* The payload is placed after the properties.
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

static uint32_t getRemainingLength( TransportRecv_t recvFunc,
                                    NetworkContext_t * pNetworkContext )
{
    uint32_t remainingLength = 0, multiplier = 1, bytesDecoded = 0, expectedSize = 0;
    uint8_t encodedByte = 0;
    int32_t bytesReceived = 0;

    /* This algorithm is copied from the MQTT v5.0 spec (same as v3.1.1). */
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
                /* Temporarily store the result before multiplying it with a multiplier. */
                uint8_t tempResult = encodedByte & 0x7FU;

                remainingLength += ( ( uint32_t ) tempResult ) * multiplier;
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
    uint32_t remainingLength = 0;
    uint32_t multiplier = 1;
    size_t bytesDecoded = 0;
    uint32_t expectedSize = 0;
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
                uint8_t tempResult;

                /* Get the next byte. It is at the next position after the bytes
                 * decoded till now since the header of one byte was read before. */
                encodedByte = pBuffer[ bytesDecoded + 1U ];

                /* Temporarily store the result before multiplying it with a multiplier. */
                tempResult = encodedByte & 0x7FU;

                remainingLength += ( ( uint32_t ) tempResult ) * multiplier;
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

        if( bytesDecoded != ( ( size_t ) expectedSize ) )
        {
            LogError( ( "Expected and actual length of decoded bytes do not match.\n" ) );
            status = MQTTBadResponse;
        }
        else if( CHECK_U32T_OVERFLOWS_SIZE_T( remainingLength ) )
        {
            LogError( ( "Remaining length %" PRIu32
                        " will overflow when stored in pIncomingPacket->remainingLength "
                        "(type: size_t).", remainingLength ) );

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
            /* coverity[misra_c_2012_rule_14_3_violation] */
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
        case MQTT_PACKET_TYPE_AUTH:
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

static MQTTStatus_t checkPublishRemainingLength( uint32_t remainingLength,
                                                 MQTTQoS_t qos,
                                                 uint32_t qos0Minimum )
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
    /* Log an error based on the CONNACK response code. */
    switch( responseCode )
    {
        case ( uint8_t ) MQTT_REASON_CONNACK_SUCCESS:
            LogDebug( ( "Connection accepted." ) );
            break;

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

        /* This branch will never be reached as there the caller checks the value before-hand. */
        default:
            /* LCOV_EXCL_START */
            LogError( ( "Invalid reason code received." ) );
            assert( false );
            break;
            /* LCOV_EXCL_STOP */
    }
}

/*-----------------------------------------------------------*/

static inline MQTTStatus_t isValidConnackReasonCode( uint8_t reasonCode )
{
    MQTTStatus_t status;

    switch( reasonCode )
    {
        case ( uint8_t ) MQTT_REASON_CONNACK_SUCCESS:
        case ( uint8_t ) MQTT_REASON_CONNACK_UNSPECIFIED_ERROR:
        case ( uint8_t ) MQTT_REASON_CONNACK_MALFORMED_PACKET:
        case ( uint8_t ) MQTT_REASON_CONNACK_PROTOCOL_ERROR:
        case ( uint8_t ) MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR:
        case ( uint8_t ) MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION:
        case ( uint8_t ) MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID:
        case ( uint8_t ) MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD:
        case ( uint8_t ) MQTT_REASON_CONNACK_NOT_AUTHORIZED:
        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_UNAVAILABLE:
        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_BUSY:
        case ( uint8_t ) MQTT_REASON_CONNACK_BANNED:
        case ( uint8_t ) MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD:
        case ( uint8_t ) MQTT_REASON_CONNACK_TOPIC_NAME_INVALID:
        case ( uint8_t ) MQTT_REASON_CONNACK_PACKET_TOO_LARGE:
        case ( uint8_t ) MQTT_REASON_CONNACK_QUOTA_EXCEEDED:
        case ( uint8_t ) MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID:
        case ( uint8_t ) MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED:
        case ( uint8_t ) MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED:
        case ( uint8_t ) MQTT_REASON_CONNACK_USE_ANOTHER_SERVER:
        case ( uint8_t ) MQTT_REASON_CONNACK_SERVER_MOVED:
        case ( uint8_t ) MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED:
            status = MQTTSuccess;
            break;

        default:
            LogError( ( "Invalid reason code received." ) );
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateConnackParams( const MQTTPacketInfo_t * pIncomingPacket,
                                           bool * pSessionPresent )
{
    MQTTStatus_t status = MQTTSuccess;
    const uint8_t * pRemainingData;
    uint8_t reasonCode;

    assert( pIncomingPacket != NULL );
    assert( pSessionPresent != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );
    assert( pIncomingPacket->type == MQTT_PACKET_TYPE_CONNACK );

    /* Remaining Length of the CONNACK cannot be less than 3.
     * 1 byte for each of the following:
     * - Connect Acknowledge Flags
     * - Connect Reason Code
     * - Properties (0x00) indicating no trailing properties. */
    if( pIncomingPacket->remainingLength < MQTT_PACKET_CONNACK_MINIMUM_SIZE )
    {
        LogError( ( "Incomplete Connack received" ) );

        status = MQTTBadResponse;
    }

    if( status == MQTTSuccess )
    {
        pRemainingData = pIncomingPacket->pRemainingData;
        reasonCode = pRemainingData[ 1 ];

        /* Reserved bits in CONNACK must be 0. */
        if( ( pRemainingData[ 0 ] | 0x01U ) != 0x01U )
        {
            LogError( ( "Reserved bits in CONNACK not set to 0." ) );

            status = MQTTBadResponse;
        }
        else
        {
            /* Determine if the "Session Present" bit is set. This is the
             * lowest bit of the first byte of variable header. */
            if( ( pRemainingData[ 0 ] & MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK ) ==
                MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK )
            {
                LogDebug( ( "CONNACK session present bit set." ) );
                *pSessionPresent = true;

                /* MQTT 5 specifies that the fourth byte in CONNACK must be 0 if the
                 * "Session Present" bit is set. */
                if( reasonCode != 0U )
                {
                    LogError( ( "Session Present bit is set, but connect return code in CONNACK is %u (nonzero).",
                                ( unsigned int ) reasonCode ) );
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
        if( isValidConnackReasonCode( reasonCode ) != MQTTSuccess )
        {
            status = MQTTBadResponse;
        }
        else
        {
            if( reasonCode != ( ( uint8_t ) MQTT_REASON_CONNACK_SUCCESS ) )
            {
                status = MQTTServerRefused;
            }

            logConnackResponse( reasonCode );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeConnack( MQTTConnectionProperties_t * pConnackProperties,
                                        const MQTTPacketInfo_t * pIncomingPacket,
                                        bool * pSessionPresent,
                                        MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pVariableHeader = NULL;
    MQTTStatus_t statusCopy = MQTTSuccess;

    /* Validate the arguments. */
    status = validateConnackParams( pIncomingPacket, pSessionPresent );

    if( status == MQTTServerRefused )
    {
        statusCopy = status;
    }

    if( ( status == MQTTSuccess ) || ( status == MQTTServerRefused ) )
    {
        pVariableHeader = pIncomingPacket->pRemainingData;

        /* Skip over flags and reason code. */
        pVariableHeader = &pVariableHeader[ 2U ];

        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        status = decodeVariableLength( pVariableHeader, ( size_t ) ( pIncomingPacket->remainingLength - 2U ), &propertyLength );
    }

    /* Validate the packet size if max packet size is set. */
    if( status == MQTTSuccess )
    {
        /* Validate the remaining length. */
        if( ( pIncomingPacket->remainingLength ) != ( 2U + propertyLength + variableLengthEncodedSize( propertyLength ) ) )
        {
            LogError( ( "Invalid Remaining Length" ) );
            status = MQTTBadResponse;
        }
        /* Deserialize the connack properties. */
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
                                                     uint32_t * pRemainingLength,
                                                     uint32_t * pPacketSize,
                                                     uint32_t subscribePropLen,
                                                     uint32_t maxPacketSize,
                                                     MQTTSubscriptionType_t subscriptionType )
{
    uint32_t packetSize = 0U;
    size_t i;
    MQTTStatus_t status = MQTTSuccess;

    assert( pSubscriptionList != NULL );
    assert( pRemainingLength != NULL );
    assert( pPacketSize != NULL );
    assert( subscriptionCount != 0U );
    assert( subscribePropLen < MQTT_REMAINING_LENGTH_INVALID );

    /* 2 byte packet id. */
    packetSize += 2U;

    packetSize += subscribePropLen;
    packetSize += variableLengthEncodedSize( subscribePropLen );

    for( i = 0; i < subscriptionCount; i++ )
    {
        if( CHECK_SIZE_T_OVERFLOWS_16BIT( pSubscriptionList[ i ].topicFilterLength ) )
        {
            LogError( ( "Topic filter length must be less than 65536. Length is %" PRIu32, ( uint32_t ) pSubscriptionList[ i ].topicFilterLength ) );
            status = MQTTBadParameter;
        }
        else
        {
            /* We need not worry about overflow here as even when the subscribe property length is
             * maximum and topic filter is maximum, the addition will not overflow a uint32_t. */
            packetSize += ( uint32_t ) ( pSubscriptionList[ i ].topicFilterLength + 2U );

            if( packetSize >= MQTT_REMAINING_LENGTH_INVALID )
            {
                LogError( ( "Packet size must be smaller than 268435456" ) );
                status = MQTTBadParameter;
            }
            else if( subscriptionType == MQTT_TYPE_SUBSCRIBE )
            {
                packetSize += 1U;
            }
            else
            {
                /* Unsubscribe packet does not have an additional byte for QoS. */
            }
        }

        if( status != MQTTSuccess )
        {
            break;
        }
    }

    /* At this point, the "Remaining length" has been calculated. Return error
     * if the "Remaining length" exceeds what is allowed by MQTT 5. Otherwise,
     * set the output parameter. */
    if( ( status == MQTTSuccess ) && ( packetSize > MQTT_MAX_REMAINING_LENGTH ) )
    {
        LogError( ( "Subscribe packet size %" PRIu32 " exceeds 268435456.",
                    packetSize ) );
        status = MQTTBadParameter;
    }

    if( status == MQTTSuccess )
    {
        *pRemainingLength = packetSize;

        /* This is the total packet size which is the sum of:
         * Remaining Length +
         * Bytes required to encode the remaining length +
         * 1 byte MQTT header
         */
        packetSize += 1U + variableLengthEncodedSize( packetSize );
        *pPacketSize = packetSize;

        if( packetSize > maxPacketSize )
        {
            LogError( ( "Packet size is greater than the allowed maximum packet size." ) );
            status = MQTTBadParameter;
        }
    }

    LogDebug( ( "%s packet remaining length=%lu and packet size=%lu.",
                ( subscriptionType == MQTT_TYPE_SUBSCRIBE ) ? "SUBSCRIBE" : "UNSUBSCRIBE",
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
                LogWarn( ( "Topic Filter Refused." ) );
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

static MQTTStatus_t deserializeSubUnsubAck( const MQTTPacketInfo_t * incomingPacket,
                                            uint16_t * pPacketId,
                                            MQTTReasonCodeInfo_t * pReasonCodes,
                                            MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    uint32_t remainingLength = 0U;
    size_t statusTotalBytes = 0U;
    const uint8_t * pStatusStart;
    size_t propertyLength = 0U;

    /* Validate input parameters using assert. */
    assert( incomingPacket != NULL );
    assert( pPacketId != NULL );
    assert( !CHECK_U32T_OVERFLOWS_SIZE_T( incomingPacket->remainingLength ) );

    pIndex = incomingPacket->pRemainingData;
    remainingLength = incomingPacket->remainingLength;

    if( pReasonCodes == NULL )
    {
        LogError( ( "pReasonCodes cannot be NULL for SUB/UNSUB ack packets." ) );
        status = MQTTBadParameter;
    }

    if( incomingPacket->remainingLength < 4U )
    {
        LogError( ( "Suback Packet Cannot have a remaining Length of less than 4." ) );
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
        status = deserializeSubUnsubAckProperties( pPropBuffer,
                                                   pIndex,
                                                   &propertyLength,
                                                   incomingPacket->remainingLength );
    }

    if( status == MQTTSuccess )
    {
        uint32_t propertyLengthU32 = ( uint32_t ) propertyLength;
        /* Total number of bytes used by the properties - the encoded length + the actual properties. */
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        size_t totalPropertiesLength = ( size_t ) ( propertyLengthU32 + variableLengthEncodedSize( propertyLengthU32 ) );

        /* Total bytes of status codes = length - packet ID - properties total length. */
        statusTotalBytes = remainingLength - sizeof( uint16_t ) - totalPropertiesLength;

        /* Status codes start just after the properties. */
        pStatusStart = &pIndex[ totalPropertiesLength ];
        status = readSubackStatus( statusTotalBytes, pStatusStart, pReasonCodes );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateSubscriptionSerializeParams( const MQTTSubscribeInfo_t * pSubscriptionList,
                                                         size_t subscriptionCount,
                                                         uint16_t packetId,
                                                         uint32_t remainingLength,
                                                         const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t packetSize = 0;

    /* Validate all the parameters. */
    if( ( pFixedBuffer == NULL ) || ( pSubscriptionList == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer=%p, "
                    "pSubscriptionList=%p.",
                    ( const void * ) pFixedBuffer,
                    ( const void * ) pSubscriptionList ) );
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
    else if( remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length must be less than 268435456." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* The serialized packet size = First byte
         * + length of encoded size of remaining length
         * + remaining length. */
        uint32_t serializedPacketSize = 1U + remainingLengthEncodedSize( remainingLength )
                                        + remainingLength;

        if( CHECK_U32T_OVERFLOWS_SIZE_T( serializedPacketSize ) )
        {
            LogError( ( "Serialized packet size will not fit in a size_t variable." ) );

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
            /* coverity[misra_c_2012_rule_14_3_violation] */
            status = MQTTBadParameter;
        }
        else
        {
            packetSize = ( size_t ) serializedPacketSize;

            if( packetSize > pFixedBuffer->size )
            {
                LogError( ( "Buffer size of %lu is not sufficient to hold "
                            "serialized packet of size of %lu.",
                            ( unsigned long ) pFixedBuffer->size,
                            ( unsigned long ) packetSize ) );
                status = MQTTNoMemory;
            }
        }

        if( status == MQTTSuccess )
        {
            size_t it;

            for( it = 0; it < subscriptionCount; it++ )
            {
                /* Check whether the topic filter and the topic filter length are non-zero. */
                if( ( pSubscriptionList[ it ].pTopicFilter == NULL ) || ( pSubscriptionList[ it ].topicFilterLength == 0U ) )
                {
                    LogError( ( "Topic filter length must be non-zero and the topic filter must be non-NULL." ) );
                    status = MQTTBadParameter;
                }
                else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pSubscriptionList[ it ].topicFilterLength ) )
                {
                    LogError( ( "Each topic filter must be less than 65535 characters long." ) );
                    status = MQTTBadParameter;
                }
                else
                {
                    /* Valid values. */
                }

                if( status != MQTTSuccess )
                {
                    break;
                }
            }
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
    const uint8_t * pPacketIdentifierHigh = NULL;
    uint8_t * pIndex = NULL;

    assert( pIncomingPacket != NULL );
    assert( pIncomingPacket->pRemainingData != NULL );
    assert( pPacketId != NULL );
    assert( pPublishInfo != NULL );

    pIndex = pIncomingPacket->pRemainingData;
    /* The flags are the lower 4 bits of the first byte in PUBLISH. */
    status = processPublishFlags( ( pIncomingPacket->type & 0x0FU ), pPublishInfo );

    if( status == MQTTSuccess )
    {
        /* Sanity checks for "Remaining length". A QoS 0 PUBLISH  must have a remaining
         * length of at least 4 to accommodate topic name length (2 bytes), topic name
         * (at least 1 byte) and, Property Length (at least 1 byte for 0 properties).
         * A QoS 1 or 2 PUBLISH must have a remaining length of at least 5 for the packet
         * identifier in addition to the topic name length and topic name. */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
                                              pPublishInfo->qos,
                                              4U );
    }

    if( status == MQTTSuccess )
    {
        /* Extract the topic name starting from the first byte of the variable header.
         * The topic name string starts at byte 3 in the variable header. */
        pPublishInfo->topicNameLength = UINT16_DECODE( pIndex );
        pIndex = &pIndex[ sizeof( uint16_t ) ];

        /* Sanity checks for topic name length and "Remaining length". The remaining
         * length must be at least as large as the variable length header:
         *   2 bytes to encode the Topic Length +
         *   length of the topic string +
         *   1 byte for the property length (when 0 properties).
         */
        status = checkPublishRemainingLength( pIncomingPacket->remainingLength,
                                              pPublishInfo->qos,
                                              2U + ( uint32_t ) pPublishInfo->topicNameLength + 1U );
    }

    if( status == MQTTSuccess )
    {
        /* Parse the topic. */
        pPublishInfo->pTopicName = ( char * ) pIndex;
        LogDebug( ( "Topic name: %hu.", ( unsigned short ) pPublishInfo->topicNameLength ) );

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
        status = deserializePublishProperties( pPublishInfo,
                                               pPropBuffer,
                                               pIndex,
                                               topicAliasMax,
                                               pIncomingPacket->remainingLength );
    }

    if( status == MQTTSuccess )
    {
        /* Calculate the length of the payload. QoS 1 or 2 PUBLISH packets contain
         * a packet identifier, but QoS 0 PUBLISH packets do not. */
        uint32_t payloadLengthU32 = pIncomingPacket->remainingLength -
                                    ( ( ( uint32_t ) pPublishInfo->topicNameLength ) + 2U ) -
                                    ( ( ( uint32_t ) pPublishInfo->propertyLength ) + variableLengthEncodedSize( ( uint32_t ) pPublishInfo->propertyLength ) );

        pIndex = &pIndex[ ( size_t ) variableLengthEncodedSize( ( uint32_t ) pPublishInfo->propertyLength ) ];
        pIndex = &pIndex[ pPublishInfo->propertyLength ];

        if( CHECK_U32T_OVERFLOWS_SIZE_T( payloadLengthU32 ) )
        {
            LogError( ( "Payload length will not fit in size_t." ) );

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
            /* coverity[misra_c_2012_rule_14_3_violation] */
            status = MQTTBadResponse;
        }
        else
        {
            pPublishInfo->payloadLength = payloadLengthU32;

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

/*-----------------------------------------------------------*/

/**
 * @brief Set a bit in pPropBuffer->fieldSet if pPropBuffer is not NULL.
 */
static void setConnackPropBit( MQTTPropBuilder_t * pPropBuffer,
                               uint8_t bitPos )
{
    if( pPropBuffer != NULL )
    {
        UINT32_SET_BIT( pPropBuffer->fieldSet, bitPos );
    }
}

/**
 * @brief Validate that a uint8 property is 0 or 1 (boolean).
 */
static MQTTStatus_t validateBoolProp( uint8_t value,
                                      const char * pPropName )
{
    MQTTStatus_t status = MQTTSuccess;

    if( value > 1U )
    {
        LogError( ( "Server set %s to %u (not 0 or 1). Invalid response.", pPropName, value ) );
        status = MQTTBadResponse;
    }

    return status;
}

/**
 * @brief Decode a uint8 boolean property, validate it, and set the fieldSet bit.
 */
static MQTTStatus_t decodeConnackBoolProp( uint8_t * pDest,
                                           uint32_t * pPropertyLength,
                                           bool * pSeen,
                                           uint8_t ** ppIndex,
                                           const char * pPropName,
                                           MQTTPropBuilder_t * pPropBuffer,
                                           uint8_t bitPos )
{
    MQTTStatus_t status = decodeUint8t( pDest, pPropertyLength, pSeen, ppIndex );

    if( status == MQTTSuccess )
    {
        status = validateBoolProp( *pDest, pPropName );
    }

    if( status == MQTTSuccess )
    {
        setConnackPropBit( pPropBuffer, bitPos );
    }

    return status;
}

static MQTTStatus_t deserializeConnackProperty( uint8_t propertyId,
                                                MQTTConnectionProperties_t * pConnackProperties,
                                                uint32_t * pPropertyLength,
                                                uint8_t ** ppVariableHeader,
                                                MQTTPropBuilder_t * pPropBuffer,
                                                bool * pSeenFlags )
{
    MQTTStatus_t status = MQTTSuccess;
    const char * data;
    size_t dataLength;

    /* pSeenFlags layout: [0]=sessionExpiry [1]=receiveMax [2]=maxQos [3]=retain
     * [4]=maxPacket [5]=clientId [6]=topicAlias [7]=wildcard [8]=subId
     * [9]=sharedsub [10]=keepAlive [11]=responseInfo [12]=serverRef
     * [13]=authMethod [14]=authData [15]=reasonString */

    switch( propertyId )
    {
        case MQTT_SESSION_EXPIRY_ID:
            status = decodeUint32t( &pConnackProperties->sessionExpiry, pPropertyLength,
                                    &pSeenFlags[ 0 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Session expiry from server: %" PRIu32, pConnackProperties->sessionExpiry ) );
                setConnackPropBit( pPropBuffer, MQTT_SESSION_EXPIRY_INTERVAL_POS );
            }

            break;

        case MQTT_RECEIVE_MAX_ID:
            status = decodeUint16t( &pConnackProperties->serverReceiveMax, pPropertyLength,
                                    &pSeenFlags[ 1 ], ppVariableHeader );

            if( ( status == MQTTSuccess ) && ( pConnackProperties->serverReceiveMax == 0U ) )
            {
                LogError( ( "Receive Maximum value set to 0 by the server." ) );
                status = MQTTBadResponse;
            }

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Server receive maximum: %" PRIu16, pConnackProperties->serverReceiveMax ) );
                setConnackPropBit( pPropBuffer, MQTT_RECEIVE_MAXIMUM_POS );
            }

            break;

        case MQTT_MAX_QOS_ID:
            status = decodeConnackBoolProp( &pConnackProperties->serverMaxQos, pPropertyLength,
                                            &pSeenFlags[ 2 ], ppVariableHeader, "maximum QoS",
                                            pPropBuffer, MQTT_MAX_QOS_POS );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Server maximum QoS: %" PRIu8, pConnackProperties->serverMaxQos ) );
            }

            break;

        case MQTT_RETAIN_AVAILABLE_ID:
            status = decodeConnackBoolProp( &pConnackProperties->retainAvailable, pPropertyLength,
                                            &pSeenFlags[ 3 ], ppVariableHeader, "retain available",
                                            pPropBuffer, MQTT_RETAIN_AVAILABLE_POS );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Retain available: %" PRIu8, pConnackProperties->retainAvailable ) );
            }

            break;

        case MQTT_MAX_PACKET_SIZE_ID:
            status = decodeUint32t( &pConnackProperties->serverMaxPacketSize, pPropertyLength,
                                    &pSeenFlags[ 4 ], ppVariableHeader );

            if( ( status == MQTTSuccess ) && ( pConnackProperties->serverMaxPacketSize == 0U ) )
            {
                LogError( ( "Server set maximum packet size to 0. Invalid response." ) );
                status = MQTTBadResponse;
            }

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Server maximum packet size: %" PRIu32, pConnackProperties->serverMaxPacketSize ) );
                setConnackPropBit( pPropBuffer, MQTT_MAX_PACKET_SIZE_POS );
            }

            break;

        case MQTT_ASSIGNED_CLIENT_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 5 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Assigned client ID: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_ASSIGNED_CLIENT_ID_POS );
            }

            break;

        case MQTT_TOPIC_ALIAS_MAX_ID:
            status = decodeUint16t( &pConnackProperties->serverTopicAliasMax, pPropertyLength,
                                    &pSeenFlags[ 6 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Topic alias max ID: %" PRIu16, pConnackProperties->serverTopicAliasMax ) );
                setConnackPropBit( pPropBuffer, MQTT_TOPIC_ALIAS_MAX_POS );
            }

            break;

        case MQTT_REASON_STRING_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 15 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogInfo( ( "Reason string from server: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_REASON_STRING_POS );
            }

            break;

        case MQTT_USER_PROPERTY_ID:
           {
               const char * key;
               const char * value;
               size_t keyLength;
               size_t valueLength;
               status = decodeUserProp( &key, &keyLength, &value, &valueLength, pPropertyLength, ppVariableHeader );

               if( status == MQTTSuccess )
               {
                   LogDebug( ( "User property: %.*s : %.*s", ( int ) keyLength, key, ( int ) valueLength, value ) );
                   setConnackPropBit( pPropBuffer, MQTT_USER_PROP_POS );
               }
           }
           break;

        case MQTT_WILDCARD_ID:
            status = decodeConnackBoolProp( &pConnackProperties->isWildcardAvailable, pPropertyLength,
                                            &pSeenFlags[ 7 ], ppVariableHeader, "wildcard",
                                            pPropBuffer, MQTT_WILDCARD_SUBSCRIPTION_AVAILABLE_POS );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Wildcard subscription available: %s",
                            pConnackProperties->isWildcardAvailable ? "Yes" : "No" ) );
            }

            break;

        case MQTT_SUB_AVAILABLE_ID:
            status = decodeConnackBoolProp( &pConnackProperties->isSubscriptionIdAvailable, pPropertyLength,
                                            &pSeenFlags[ 8 ], ppVariableHeader, "subscription ID availability",
                                            pPropBuffer, MQTT_SUBSCRIPTION_ID_AVAILABLE_POS );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Subscription ID available: %s",
                            pConnackProperties->isSubscriptionIdAvailable ? "Yes" : "No" ) );
            }

            break;

        case MQTT_SHARED_SUB_ID:
            status = decodeConnackBoolProp( &pConnackProperties->isSharedAvailable, pPropertyLength,
                                            &pSeenFlags[ 9 ], ppVariableHeader, "shared sub availability",
                                            pPropBuffer, MQTT_SHARED_SUBSCRIPTION_AVAILABLE_POS );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Shared subscription available: %s",
                            pConnackProperties->isSharedAvailable ? "Yes" : "No" ) );
            }

            break;

        case MQTT_SERVER_KEEP_ALIVE_ID:
            status = decodeUint16t( &pConnackProperties->serverKeepAlive, pPropertyLength,
                                    &pSeenFlags[ 10 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Server keep alive: %d", ( int ) pConnackProperties->serverKeepAlive ) );
                setConnackPropBit( pPropBuffer, MQTT_SERVER_KEEP_ALIVE_POS );
            }

            break;

        case MQTT_RESPONSE_INFO_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 11 ], ppVariableHeader );

            if( ( status == MQTTSuccess ) && ( pConnackProperties->requestResponseInfo == false ) )
            {
                LogError( ( "Client did not request information still server sent it. Protocol error." ) );
                status = MQTTBadResponse;
            }

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Response information: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_RESPONSE_INFORMATION_POS );
            }

            break;

        case MQTT_SERVER_REF_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 12 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Server reference: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_SERVER_REFERENCE_POS );
            }

            break;

        case MQTT_AUTH_METHOD_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 13 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Authentication method received: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_AUTHENTICATION_METHOD_POS );
            }

            break;

        case MQTT_AUTH_DATA_ID:
            status = decodeUtf8( &data, &dataLength, pPropertyLength, &pSeenFlags[ 14 ], ppVariableHeader );

            if( status == MQTTSuccess )
            {
                LogDebug( ( "Auth data received: %.*s", ( int ) dataLength, data ) );
                setConnackPropBit( pPropBuffer, MQTT_AUTHENTICATION_DATA_POS );
            }

            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

static MQTTStatus_t deserializeConnackProperties( MQTTConnectionProperties_t * pConnackProperties,
                                                  uint32_t length,
                                                  uint8_t * pIndex,
                                                  MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pVariableHeader = pIndex;
    uint32_t propertyLength = length;
    bool seenFlags[ 16 ] = { false };

    pVariableHeader = &pVariableHeader[ variableLengthEncodedSize( propertyLength ) ];

    if( pPropBuffer != NULL )
    {
        pPropBuffer->pBuffer = pVariableHeader;
        pPropBuffer->bufferLength = propertyLength;
        pPropBuffer->currentIndex = propertyLength;
        pPropBuffer->fieldSet = 0U;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pVariableHeader;

        pVariableHeader = &pVariableHeader[ 1 ];
        propertyLength -= 1U;

        status = deserializeConnackProperty( propertyId, pConnackProperties,
                                             &propertyLength, &pVariableHeader,
                                             pPropBuffer, seenFlags );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t logAckResponse( MQTTSuccessFailReasonCode_t reasonCode,
                                    uint16_t packetIdentifier )
{
    MQTTStatus_t status = MQTTSuccess;

    switch( reasonCode )
    {
        case MQTT_REASON_PUBACK_SUCCESS: /* Also equivalent to MQTT_REASON_PUBREL_SUCCESS. */
            ( void ) packetIdentifier;
            break;

        case MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS:
            LogDebug( ( "Publish accepted with packet id %hu: No matching subscribers.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_UNSPECIFIED_ERROR:
            LogDebug( ( "Publish refused with packet id %hu: Connection rate exceeded.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR:
            LogDebug( ( "Publish refused with packet id %hu:  The PUBLISH is valid but the receiver is not willing to accept it.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_NOT_AUTHORIZED:
            LogDebug( ( "Publish refused with packet id %hu: The PUBLISH is not authorized.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_TOPIC_NAME_INVALID:
            LogDebug( ( "Publish refused with packet id %hu: Topic Name not accepted.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE:
            LogDebug( ( "Publish refused with packet id %hu: The Packet Identifier is already in use. ",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_QUOTA_EXCEEDED:
            LogDebug( ( "Publish refused with packet id %hu: Quota exceeded.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID:
            LogDebug( ( "Publish refused with packet id %hu: Payload format indicator is invalid.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        case MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND:
            LogError( ( "Publish refused with packet id %hu: Packet identifier invalid.",
                        ( unsigned short ) packetIdentifier ) );
            break;

        default:
            status = MQTTBadResponse;
            break;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializeSubUnsubAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                                      uint8_t * pIndex,
                                                      size_t * pSubackPropertyLength,
                                                      uint32_t remainingLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pLocalIndex = pIndex;
    const char * pReasonString;
    size_t reasonStringLength;
    bool reasonString = false;

    assert( !CHECK_U32T_OVERFLOWS_SIZE_T( remainingLength ) );

    /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
    /* coverity[misra_c_2012_rule_10_8_violation] */
    status = decodeVariableLength( pLocalIndex, ( size_t ) ( remainingLength - 2U ), &propertyLength );

    if( status == MQTTSuccess )
    {
        if( CHECK_U32T_OVERFLOWS_SIZE_T( propertyLength ) )
        {
            LogError( ( "Property length will overflow size_t variable." ) );

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
            /* coverity[misra_c_2012_rule_14_3_violation] */
            status = MQTTBadParameter;
        }
        else
        {
            *pSubackPropertyLength = ( size_t ) propertyLength;
        }
    }

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
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeUtf8( &pReasonString, &reasonStringLength, &propertyLength,
                                     &reasonString, &pLocalIndex );

                if( status == MQTTSuccess )
                {
                    if( pReasonString != NULL )
                    {
                        LogInfo( ( "SubAck reason string sent by server: %.*s",
                                   ( int ) reasonStringLength, pReasonString ) );
                    }
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * propertyKey;
                   size_t propertyKeyLen;
                   const char * propertyValue;
                   size_t propertyValueLen;
                   status = decodeUserProp( &propertyKey, &propertyKeyLen, &propertyValue,
                                            &propertyValueLen, &propertyLength, &pLocalIndex );
               }
               break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static void serializeConnectPacket( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    uint32_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    uint8_t * pIndex = NULL;
    uint32_t connectPropertyLength = 0U;
    uint32_t willPropertyLength = 0U;

    assert( pConnectInfo != NULL );
    assert( pFixedBuffer != NULL );
    assert( pFixedBuffer->pBuffer != NULL );
    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->clientIdentifierLength ) );
    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->userNameLength ) );
    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->passwordLength ) );

    if( pWillInfo != NULL )
    {
        assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pWillInfo->topicNameLength ) );
    }

    pIndex = pFixedBuffer->pBuffer;

    /* Serialize the header. */
    pIndex = serializeConnectFixedHeader( pIndex,
                                          pConnectInfo,
                                          pWillInfo,
                                          remainingLength );

    if( ( pConnectProperties != NULL ) && ( pConnectProperties->pBuffer != NULL ) )
    {
        assert( !CHECK_SIZE_T_OVERFLOWS_32BIT( pConnectProperties->currentIndex ) );
        connectPropertyLength = ( uint32_t ) pConnectProperties->currentIndex;
    }

    if( ( pWillProperties != NULL ) && ( pWillProperties->pBuffer != NULL ) )
    {
        assert( !CHECK_SIZE_T_OVERFLOWS_32BIT( pWillProperties->currentIndex ) );
        willPropertyLength = ( uint32_t ) pWillProperties->currentIndex;
    }

    /* Write the properties length into the CONNECT packet. */
    pIndex = encodeVariableLength( pIndex, connectPropertyLength );

    if( connectPropertyLength > 0U )
    {
        ( void ) memcpy( ( void * ) pIndex, ( const void * ) pConnectProperties->pBuffer, connectPropertyLength );
        pIndex = &pIndex[ ( size_t ) connectPropertyLength ];
    }

    /* Write the client identifier into the CONNECT packet. */
    pIndex = encodeString( pIndex,
                           pConnectInfo->pClientIdentifier,
                           ( uint16_t ) pConnectInfo->clientIdentifierLength );

    /* Write the will topic name and message into the CONNECT packet if provided. */
    if( pWillInfo != NULL )
    {
        pIndex = encodeVariableLength( pIndex, willPropertyLength );

        if( willPropertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pWillProperties->pBuffer, willPropertyLength );
            pIndex = &pIndex[ ( size_t ) willPropertyLength ];
        }

        pIndex = encodeString( pIndex,
                               pWillInfo->pTopicName,
                               ( uint16_t ) pWillInfo->topicNameLength );

        pIndex = encodeString( pIndex,
                               pWillInfo->pPayload,
                               ( uint16_t ) pWillInfo->payloadLength );
    }

    /* Encode the user name if provided. */
    if( pConnectInfo->pUserName != NULL )
    {
        pIndex = encodeString( pIndex, pConnectInfo->pUserName, ( uint16_t ) pConnectInfo->userNameLength );
    }

    /* Encode the password if provided. */
    if( pConnectInfo->pPassword != NULL )
    {
        pIndex = encodeString( pIndex, pConnectInfo->pPassword, ( uint16_t ) pConnectInfo->passwordLength );
    }

    LogDebug( ( "Length of serialized CONNECT packet is %lu.",
                ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );

    /* Ensure that the difference between the end and beginning of the buffer
     * is less than the buffer size. */
    assert( ( ( size_t ) ( pIndex - pFixedBuffer->pBuffer ) ) <= pFixedBuffer->size );
}

/*-----------------------------------------------------------*/

static MQTTStatus_t deserializePublishProperties( MQTTPublishInfo_t * pPublishInfo,
                                                  MQTTPropBuilder_t * pPropBuffer,
                                                  uint8_t * pIndex,
                                                  uint16_t topicAliasMax,
                                                  uint32_t remainingLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pLocalIndex = pIndex;
    uint32_t subscriptionId;
    uint32_t remainingLengthForProperties;
    bool contentType = false;
    bool messageExpiryInterval = false;
    bool responseTopic = false;
    bool topicAlias = false;
    bool payloadFormatIndicator = false;
    bool correlationData = false;
    uint16_t topicAliasVal;

    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) );
    assert( !CHECK_U32T_OVERFLOWS_SIZE_T( remainingLength ) );

    /* Decode Property Length. */
    remainingLengthForProperties = remainingLength;

    if( remainingLengthForProperties < ( pPublishInfo->topicNameLength + sizeof( uint16_t ) ) )
    {
        LogError( ( "Topic name length is greater than the remaining length." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* For QoS > 0, 2 byte packet ID is included. See whether the remaining length can accommodate that. */
        uint32_t qosEncodedLength = ( pPublishInfo->qos > MQTTQoS0 ) ? 2U : 0U;

        /* Topic is encoded as 2 bytes for the length and topic string. */
        remainingLengthForProperties -= ( ( uint32_t ) pPublishInfo->topicNameLength ) + 2U;

        if( remainingLengthForProperties < qosEncodedLength )
        {
            LogError( ( "Topic name length and QoS cannot fit in the remaining length." ) );
            status = MQTTBadParameter;
        }
        else
        {
            remainingLengthForProperties -= ( pPublishInfo->qos > MQTTQoS0 ) ? 2U : 0U;
        }
    }

    if( status == MQTTSuccess )
    {
        status = decodeVariableLength( pLocalIndex, ( size_t ) remainingLengthForProperties, &propertyLength );

        if( status == MQTTSuccess )
        {
            if( CHECK_U32T_OVERFLOWS_SIZE_T( propertyLength ) )
            {
                LogError( ( "Property length cannot fit in size_t." ) );

                /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
                /* coverity[misra_c_2012_rule_14_3_violation] */
                status = MQTTBadResponse;
            }
            else
            {
                pPublishInfo->propertyLength = ( size_t ) propertyLength;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        status = checkPublishRemainingLength( remainingLength,
                                              pPublishInfo->qos,
                                              ( ( uint32_t ) pPublishInfo->topicNameLength ) + 2U +
                                              propertyLength + variableLengthEncodedSize( propertyLength ) );
    }

    if( status == MQTTSuccess )
    {
        pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( propertyLength ) ];
    }

    if( pPropBuffer != NULL )
    {
        pPropBuffer->pBuffer = pLocalIndex;
        pPropBuffer->bufferLength = propertyLength;
        pPropBuffer->currentIndex = propertyLength;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_PAYLOAD_FORMAT_ID:
               {
                   uint8_t property;
                   status = decodeUint8t( &property, &propertyLength, &payloadFormatIndicator, &pLocalIndex );

                   if( status == MQTTSuccess )
                   {
                       /* Payload format must only be 0 or 1. */
                       if( property > 1U )
                       {
                           status = MQTTBadResponse;
                           LogError( ( "Payload Format Indicator is not 0x00. " ) );
                       }
                   }
               }
               break;

            case MQTT_TOPIC_ALIAS_ID:
                status = decodeUint16t( &topicAliasVal, &propertyLength, &topicAlias, &pLocalIndex );

                if( status == MQTTSuccess )
                {
                    if( topicAliasVal == 0U )
                    {
                        status = MQTTBadResponse;
                        LogError( ( "Topic Alias value of 0 is not permitted." ) );
                    }
                }

                break;

            case MQTT_RESPONSE_TOPIC_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &responseTopic, &pLocalIndex );
               }
               break;

            case MQTT_CORRELATION_DATA_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &correlationData, &pLocalIndex );
               }
               break;

            case MQTT_MSG_EXPIRY_ID:
               {
                   uint32_t property;
                   status = decodeUint32t( &property, &propertyLength, &messageExpiryInterval, &pLocalIndex );
               }
               break;

            case MQTT_CONTENT_TYPE_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &contentType, &pLocalIndex );
               }
               break;

            case MQTT_SUBSCRIPTION_ID_ID:
                status = decodeVariableLength( pLocalIndex, ( size_t ) propertyLength, &subscriptionId );

                if( status == MQTTSuccess )
                {
                    pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( subscriptionId ) ];
                    propertyLength -= variableLengthEncodedSize( subscriptionId );
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * pPropertyKey;
                   size_t propertyKeyLen;
                   const char * pPropertyValue;
                   size_t propertyValueLen;
                   status = decodeUserProp( &pPropertyKey,
                                            &propertyKeyLen,
                                            &pPropertyValue,
                                            &propertyValueLen,
                                            &propertyLength,
                                            &pLocalIndex );
               }
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
        uint32_t propertyLength = 0U;
        uint8_t * pIndex;

        assert( !CHECK_SIZE_T_OVERFLOWS_32BIT( pPropBuilder->currentIndex ) );
        assert( pPropBuilder->currentIndex < MQTT_REMAINING_LENGTH_INVALID );

        propertyLength = ( uint32_t ) pPropBuilder->currentIndex;
        pIndex = pPropBuilder->pBuffer; /* Pointer to the buffer */

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
                    status = decodeUint32t( &pConnectProperties->sessionExpiry, &propertyLength,
                                            &sessionExpiry, &pIndex );
                    break;

                case MQTT_RECEIVE_MAX_ID:
                    status = decodeUint16t( &pConnectProperties->receiveMax, &propertyLength,
                                            &receiveMax, &pIndex );
                    break;

                case MQTT_MAX_PACKET_SIZE_ID:
                    status = decodeUint32t( &pConnectProperties->maxPacketSize, &propertyLength,
                                            &maxPacket, &pIndex );
                    break;

                case MQTT_TOPIC_ALIAS_MAX_ID:
                    status = decodeUint16t( &pConnectProperties->topicAliasMax, &propertyLength,
                                            &topicAlias, &pIndex );
                    break;

                case MQTT_REQUEST_PROBLEM_ID:
                case MQTT_REQUEST_RESPONSE_ID:
                   {
                       uint8_t property;
                       /* TODO: should this go in the context? */
                       status = decodeUint8t( &property, &propertyLength, &used, &pIndex );
                   }
                   break;

                case MQTT_AUTH_DATA_ID:
                case MQTT_AUTH_METHOD_ID:
                   {
                       const char * data;
                       size_t dataLength;
                       status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                   }
                   break;

                case MQTT_USER_PROPERTY_ID:
                   {
                       const char * key, * value;
                       size_t keyLength, valueLength;
                       status = decodeUserProp( &key,
                                                &keyLength,
                                                &value,
                                                &valueLength,
                                                &propertyLength,
                                                &pIndex );
                   }
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

MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        const MQTTPropBuilder_t * pConnectProperties,
                                        const MQTTPropBuilder_t * pWillProperties,
                                        uint32_t * pRemainingLength,
                                        uint32_t * pPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t remainingLength;
    uint32_t propertyLength = 0U;
    uint32_t willPropertyLength = 0U;

    /* The CONNECT packet will always include a 10-byte variable header without connect properties. */
    uint32_t connectPacketSize = MQTT_PACKET_CONNECT_HEADER_SIZE;

    /* Validate arguments. */
    if( ( pConnectInfo == NULL ) || ( pRemainingLength == NULL ) ||
        ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pConnectInfo=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( const void * ) pConnectInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( ( pConnectInfo->clientIdentifierLength == 0U ) !=
             ( ( pConnectInfo->pClientIdentifier == NULL ) || ( *( pConnectInfo->pClientIdentifier ) == '\0' ) ) )
    {
        LogError( ( "Client ID length and value mismatch." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->clientIdentifierLength ) )
    {
        LogError( ( "Client ID length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->userNameLength ) )
    {
        LogError( ( "User name length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->passwordLength ) )
    {
        LogError( ( "Password length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else if( ( pWillInfo != NULL ) && CHECK_SIZE_T_OVERFLOWS_16BIT( pWillInfo->payloadLength ) )
    {
        /* The MQTTPublishInfo_t is reused for the will message. The payload
         * length for any other message could be larger than 65,535, but
         * the will message length is required to be represented in 2 bytes. */
        LogError( ( "The Will Message length must not exceed %d. "
                    "pWillInfo->payloadLength=%lu.",
                    UINT16_MAX,
                    ( unsigned long ) pWillInfo->payloadLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pWillInfo != NULL ) && CHECK_SIZE_T_OVERFLOWS_16BIT( pWillInfo->topicNameLength ) )
    {
        LogError( ( "Will Topic name length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Do Nothing. */
    }

    if( ( status == MQTTSuccess ) && ( pConnectProperties != NULL ) && ( pConnectProperties->pBuffer != NULL ) )
    {
        /* The value must fit in a 32-bit variable all the while being small enough to
         * be properly encoded in a variable integer format. */
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pConnectProperties->currentIndex ) ||
            ( pConnectProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
        {
            LogError( ( "Connect properties must be less than 268435456 "
                        "to be able to fit in a MQTT packet." ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pConnectProperties->currentIndex;
        }
    }

    if( ( status == MQTTSuccess ) && ( pWillProperties != NULL ) && ( pWillProperties->pBuffer != NULL ) )
    {
        /* The value must fit in a 32-bit variable all the while being small enough to
         * be properly encoded in a variable integer format. */
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pWillProperties->currentIndex ) ||
            ( pWillProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
        {
            LogError( ( "Will properties must be less than 268435456 "
                        "to be able to fit in a MQTT packet." ) );
            status = MQTTBadParameter;
        }
        else
        {
            willPropertyLength = ( uint32_t ) pWillProperties->currentIndex;
        }
    }

    if( status == MQTTSuccess )
    {
        /* Since property length, client ID length, will property length, will topic name length
         * and will payload length are ensured to be below their maximum value, connectPacketSize
         * will not overflow.
         * Max property length                     268435455
         * Max encoded prop length               +         4
         * Max client ID len (+2 byte len)       +     65535 + 2
         * Max will property length              + 268435455
         * Max will topic name len (+2 byte len) +     65535 + 2
         * Max will payload len (+2 byte len)    +     65535 + 2
         * Max username length (+2 byte len)     +     65535 + 2
         * Max password length (+2 byte len)     +     65535 + 2
         * Total                                 = 537198599 ( < UINT32_MAX ) */

        /* Add the length of the properties. */
        connectPacketSize += propertyLength;
        connectPacketSize += variableLengthEncodedSize( propertyLength );

        /* Add the length of the client identifier. */
        connectPacketSize += ( uint32_t ) ( pConnectInfo->clientIdentifierLength + sizeof( uint16_t ) );

        /* Add the lengths of the will message, topic name and properties if provided. */
        if( pWillInfo != NULL )
        {
            connectPacketSize += willPropertyLength;
            connectPacketSize += variableLengthEncodedSize( willPropertyLength );
            connectPacketSize += ( uint32_t ) ( pWillInfo->topicNameLength + sizeof( uint16_t ) +
                                                pWillInfo->payloadLength + sizeof( uint16_t ) );
        }

        /* Add the lengths of the user name and password if provided. */
        if( pConnectInfo->pUserName != NULL )
        {
            connectPacketSize += ( uint32_t ) ( pConnectInfo->userNameLength + sizeof( uint16_t ) );
        }

        if( pConnectInfo->pPassword != NULL )
        {
            connectPacketSize += ( uint32_t ) ( pConnectInfo->passwordLength + sizeof( uint16_t ) );
        }

        /* At this point, the "Remaining Length" field of the MQTT CONNECT packet has
         * been calculated. */
        remainingLength = connectPacketSize;

        if( remainingLength > MQTT_MAX_REMAINING_LENGTH )
        {
            LogError( ( "Packet remaining length is greater than maximum allowed." ) );
            status = MQTTBadParameter;
        }
        else
        {
            /* Calculate the full size of the MQTT CONNECT packet by adding the size of
             * the "Remaining Length" field plus 1 byte for the "Packet Type" field. */
            connectPacketSize += 1U + variableLengthEncodedSize( connectPacketSize );
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

static MQTTStatus_t decodePubAckProperties( MQTTPropBuilder_t * pPropBuffer,
                                            uint8_t * pIndex,
                                            uint32_t remainingLength )
{
    uint32_t propertyLength = 0U;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = pIndex;
    bool reasonString = false;

    assert( !CHECK_U32T_OVERFLOWS_SIZE_T( remainingLength ) );

    /* Decode the property length */
    status = decodeVariableLength( pLocalIndex, ( size_t ) remainingLength, &propertyLength );

    if( status == MQTTSuccess )
    {
        /* Validate the remaining length. Since properties are the last in the MQTT packet
         * the length which is remaining must be:
         *     Bytes taken to encode the property length +
         *     The actual length of the properties
         */
        if( remainingLength != ( propertyLength + variableLengthEncodedSize( propertyLength ) ) )
        {
            status = MQTTBadResponse;
        }
        else
        {
            pLocalIndex = &pLocalIndex[ variableLengthEncodedSize( propertyLength ) ];
        }
    }

    if( ( pPropBuffer != NULL ) && ( status == MQTTSuccess ) )
    {
        pPropBuffer->pBuffer = pLocalIndex;
        pPropBuffer->bufferLength = propertyLength;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        /* Decode the property id. */
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &reasonString, &pLocalIndex );
                   break;
               }

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * pPropertyKey;
                   size_t propertyKeyLen;
                   const char * pPropertyValue;
                   size_t propertyValueLen;
                   status = decodeUserProp( &pPropertyKey,
                                            &propertyKeyLen,
                                            &pPropertyValue,
                                            &propertyValueLen,
                                            &propertyLength,
                                            &pLocalIndex );
                   break;
               }

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateDisconnectResponse( MQTTSuccessFailReasonCode_t reasonCode,
                                                bool incoming )
{
    MQTTStatus_t status;

    /* Validate the reason code. */
    /* coverity[misra_c_2012_rule_10_5_violation] */
    switch( reasonCode )
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

MQTTStatus_t MQTT_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    uint32_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t connectPacketSize = 0;

    /* Validate arguments. */
    if( ( pConnectInfo == NULL ) || ( pFixedBuffer == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pConnectInfo=%p, "
                    "pFixedBuffer=%p.",
                    ( const void * ) pConnectInfo,
                    ( const void * ) pFixedBuffer ) );
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
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->clientIdentifierLength ) )
    {
        LogError( ( "clientIdentifierLength must be less than 65536 to fit in 16-bits." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->userNameLength ) )
    {
        LogError( ( "userNameLength must be less than 65536 to fit in 16-bits." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pConnectInfo->passwordLength ) )
    {
        LogError( ( "passwordLength must be less than 65536 to fit in 16-bits." ) );
        status = MQTTBadParameter;
    }
    else if( ( pWillInfo != NULL ) && CHECK_SIZE_T_OVERFLOWS_16BIT( pWillInfo->topicNameLength ) )
    {
        LogError( ( "topicNameLength must be less than 65536 to fit in 16-bits." ) );
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
                                          uint32_t * pRemainingLength,
                                          uint32_t * pPacketSize,
                                          uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;

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
    else if( pRemainingLength == NULL )
    {
        LogError( ( "Pointer to remaining length cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        if( ( pSubscribeProperties != NULL ) && ( pSubscribeProperties->pBuffer != NULL ) )
        {
            /* The value must fit in a 32-bit variable all the while being small enough to
             * be properly encoded in a variable integer format. */
            if( CHECK_SIZE_T_OVERFLOWS_32BIT( pSubscribeProperties->currentIndex ) ||
                ( pSubscribeProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
            {
                LogError( ( "Subscription properties must be less than 268435456 "
                            "to be able to fit in a MQTT packet." ) );
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength = ( uint32_t ) pSubscribeProperties->currentIndex;
            }
        }

        if( status == MQTTSuccess )
        {
            status = calculateSubscriptionPacketSize( pSubscriptionList, subscriptionCount,
                                                      pRemainingLength, pPacketSize, propertyLength,
                                                      maxPacketSize, MQTT_TYPE_SUBSCRIBE );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                      size_t subscriptionCount,
                                      const MQTTPropBuilder_t * pSubscribeProperties,
                                      uint16_t packetId,
                                      uint32_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer )
{
    size_t i = 0;
    uint8_t * pIndex = NULL;
    uint32_t propertyLength = 0U;
    MQTTStatus_t status = MQTTSuccess;

    if( ( pSubscribeProperties != NULL ) && ( pSubscribeProperties->pBuffer != NULL ) )
    {
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pSubscribeProperties->currentIndex ) ||
            ( pSubscribeProperties->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
        {
            LogError( ( "Subscribe properties cannot have a length more than 268435455." ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pSubscribeProperties->currentIndex;
        }
    }

    if( status == MQTTSuccess )
    {
        /* Validate all the parameters. */
        status = validateSubscriptionSerializeParams( pSubscriptionList,
                                                      subscriptionCount,
                                                      packetId,
                                                      remainingLength,
                                                      pFixedBuffer );
    }

    if( status == MQTTSuccess )
    {
        pIndex = pFixedBuffer->pBuffer;

        pIndex = serializeSubscribeHeader( remainingLength,
                                           pIndex,
                                           packetId );
        /* Serialize properties. */
        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pSubscribeProperties->pBuffer, ( size_t ) propertyLength );
            pIndex = &pIndex[ ( size_t ) propertyLength ];
        }

        /* Serialize each subscription topic filter and QoS. */
        for( i = 0; i < subscriptionCount; i++ )
        {
            uint8_t subscriptionOptions = 0U;
            pIndex = encodeString( pIndex,
                                   pSubscriptionList[ i ].pTopicFilter,
                                   ( uint16_t ) pSubscriptionList[ i ].topicFilterLength );

            /* Place the subscription options. */
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
                                            uint32_t * pRemainingLength,
                                            uint32_t * pPacketSize,
                                            uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;

    /* Validate parameters. */
    if( ( pSubscriptionList == NULL ) || ( pRemainingLength == NULL ) ||
        ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pSubscriptionList=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( const void * ) pSubscriptionList,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        LogError( ( "Max Packet size cannot be 0" ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionCount == 0U )
    {
        LogError( ( "Subscription count is 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        if( ( pUnsubscribeProperties != NULL ) && ( pUnsubscribeProperties->pBuffer != NULL ) )
        {
            /* The value must fit in a 32-bit variable all the while being small enough to
             * be properly encoded in a variable integer format. */
            if( CHECK_SIZE_T_OVERFLOWS_32BIT( pUnsubscribeProperties->currentIndex ) ||
                ( pUnsubscribeProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
            {
                LogError( ( "Un-Subscription properties must be less than 268435456 "
                            "to be able to fit in a MQTT packet." ) );
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength = ( uint32_t ) pUnsubscribeProperties->currentIndex;
            }
        }

        if( status == MQTTSuccess )
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
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateUnsubscribeProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;


    if( ( pPropertyBuilder != NULL ) && ( pPropertyBuilder->pBuffer != NULL ) )
    {
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pPropertyBuilder->currentIndex ) ||
            ( pPropertyBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
        {
            LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pPropertyBuilder->currentIndex;
            pIndex = pPropertyBuilder->pBuffer;
        }
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        pIndex = &pIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key, &keyLength, &value, &valueLength, &propertyLength, &pIndex );

                   if( status == MQTTSuccess )
                   {
                       LogTrace( ( "Processing User Property %.*s:%.*s",
                                   ( int ) keyLength, key,
                                   ( int ) valueLength, value ) );
                   }
               }
               break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        const MQTTPropBuilder_t * pUnsubscribeProperties,
                                        uint16_t packetId,
                                        uint32_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t i = 0;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;

    if( ( pUnsubscribeProperties != NULL ) && ( pUnsubscribeProperties->pBuffer != NULL ) )
    {
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pUnsubscribeProperties->currentIndex ) ||
            ( pUnsubscribeProperties->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
        {
            LogError( ( "Unsubscribe properties cannot have a length more than 268435455." ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pUnsubscribeProperties->currentIndex;
        }
    }

    if( status == MQTTSuccess )
    {
        /* Validate all the parameters. */
        status = validateSubscriptionSerializeParams( pSubscriptionList,
                                                      subscriptionCount,
                                                      packetId,
                                                      remainingLength,
                                                      pFixedBuffer );
    }

    if( status == MQTTSuccess )
    {
        /* Get the start of the buffer to the iterator variable. */
        pIndex = pFixedBuffer->pBuffer;

        pIndex = serializeUnsubscribeHeader( remainingLength, pIndex, packetId );

        /* Serialize the properties. */
        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pUnsubscribeProperties->pBuffer, ( size_t ) propertyLength );
            pIndex = &pIndex[ ( size_t ) propertyLength ];
        }

        /* Serialize each subscription topic filter. */
        for( i = 0; i < subscriptionCount; i++ )
        {
            pIndex = encodeString( pIndex,
                                   pSubscriptionList[ i ].pTopicFilter,
                                   ( uint16_t ) pSubscriptionList[ i ].topicFilterLength );
        }

        LogDebug( ( "Length of serialized UNSUBSCRIBE packet is %lu.",
                    ( ( unsigned long ) ( pIndex - pFixedBuffer->pBuffer ) ) ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        const MQTTPropBuilder_t * pPublishProperties,
                                        uint32_t * pRemainingLength,
                                        uint32_t * pPacketSize,
                                        uint32_t maxPacketSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;

    if( ( pPublishProperties != NULL ) && ( pPublishProperties->pBuffer != NULL ) )
    {
        /* The value must fit in a 32-bit variable all the while being small enough to
         * be properly encoded in a variable integer format. */
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pPublishProperties->currentIndex ) ||
            ( pPublishProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
        {
            LogError( ( "Publish properties must be less than 268435456 "
                        "to be able to fit in a MQTT packet." ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pPublishProperties->currentIndex;
        }
    }

    if( ( pPublishInfo == NULL ) || ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pPublishInfo=%p, "
                    "pRemainingLength=%p, pPacketSize=%p.",
                    ( const void * ) pPublishInfo,
                    ( void * ) pRemainingLength,
                    ( void * ) pPacketSize ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) )
    {
        LogError( ( "Topic name length must be smaller than 65535." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_32BIT( pPublishInfo->payloadLength ) ||
             ( pPublishInfo->payloadLength >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Payload must be smaller than 268435455." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = calculatePublishPacketSize( pPublishInfo,
                                             pRemainingLength,
                                             pPacketSize,
                                             maxPacketSize,
                                             propertyLength );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    uint16_t packetId,
                                    uint32_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t packetSize = 0;

    if( ( pFixedBuffer == NULL ) || ( pPublishInfo == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pFixedBuffer=%p, "
                    "pPublishInfo=%p.",
                    ( const void * ) pFixedBuffer,
                    ( const void * ) pPublishInfo ) );
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
                    ( const void * ) pPublishInfo->pTopicName,
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
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) )
    {
        LogError( ( "topicNameLength must be less than 65535 to fit in 16-bits." ) );
        status = MQTTBadParameter;
    }
    else if( remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length cannot be greater than 268435455." ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishProperties != NULL ) &&
             ( pPublishProperties->pBuffer != NULL ) &&
             ( CHECK_SIZE_T_OVERFLOWS_32BIT( pPublishProperties->currentIndex ) ||
               ( pPublishProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) ) )
    {
        LogError( ( "Property length cannot be greater than 268435455." ) );
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
        LogError( ( "Buffer size of %" PRIu32 " is not sufficient to hold "
                                              "serialized PUBLISH packet of size of %" PRIu32 ".",
                    ( uint32_t ) pFixedBuffer->size,
                    packetSize ) );
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
                                          uint32_t remainingLength,
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
                    ( const void * ) pFixedBuffer,
                    ( const void * ) pPublishInfo,
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
                    ( const void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) )
    {
        LogError( ( "topicNameLength must be less than 65535 to fit in 16-bits." ) );
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
    else if( remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length cannot be greater than 268435455." ) );
        status = MQTTBadParameter;
    }
    else if( remainingLength < pPublishInfo->payloadLength )
    {
        LogError( ( "Remaining length cannot be less than the payload length." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Length of serialized packet = First byte
         *                               + Length of encoded remaining length
         *                               + Remaining length
         *                               - Payload Length.
         */
        uint32_t packetSizeU32 = 1U + variableLengthEncodedSize( remainingLength )
                                 + remainingLength
                                 - ( ( uint32_t ) pPublishInfo->payloadLength );

        if( CHECK_U32T_OVERFLOWS_SIZE_T( packetSizeU32 ) )
        {
            LogError( ( "Length of serialized packet will not fit in size_t variable." ) );

            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
            /* coverity[misra_c_2012_rule_14_3_violation] */
            status = MQTTBadParameter;
        }
        else
        {
            packetSize = ( size_t ) packetSizeU32;
        }
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

/*-----------------------------------------------------------*/

/**
 * @brief Serialize a publish ACK packet with properties into pFixedBuffer.
 * Called only when pReasonCode != NULL and pAckProperties != NULL.
 */
static MQTTStatus_t serializeAckWithProperties( const MQTTFixedBuffer_t * pFixedBuffer,
                                                uint8_t packetType,
                                                uint16_t packetId,
                                                const MQTTPropBuilder_t * pAckProperties,
                                                const MQTTSuccessFailReasonCode_t * pReasonCode )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t ackPacketRemainingLength;
    uint32_t ackPacketSize;
    uint8_t * pIndex;

    ackPacketRemainingLength = 3U +
                               variableLengthEncodedSize( ( uint32_t ) pAckProperties->currentIndex ) +
                               ( ( uint32_t ) pAckProperties->currentIndex );

    if( ackPacketRemainingLength > MQTT_MAX_REMAINING_LENGTH )
    {
        LogError( ( "Packet remaining length must be smaller than 268435456." ) );
        status = MQTTBadParameter;
    }
    else
    {
        ackPacketSize = 1U +
                        variableLengthEncodedSize( ackPacketRemainingLength ) +
                        ackPacketRemainingLength;

        if( pFixedBuffer->size < ackPacketSize )
        {
            LogError( ( "Not enough space in the buffer to encode properties." ) );
            status = MQTTBadParameter;
        }
        else
        {
            pFixedBuffer->pBuffer[ 0 ] = packetType;
            pIndex = encodeVariableLength( &pFixedBuffer->pBuffer[ 1 ], ackPacketRemainingLength );
            pIndex[ 0 ] = UINT16_HIGH_BYTE( packetId );
            pIndex[ 1 ] = UINT16_LOW_BYTE( packetId );
            pIndex[ 2 ] = ( uint8_t ) ( *pReasonCode );
            pIndex = encodeVariableLength( &pIndex[ 3 ], ( uint32_t ) pAckProperties->currentIndex );
            ( void ) memcpy( pIndex, pAckProperties->pBuffer, pAckProperties->currentIndex );
        }
    }

    return status;
}

/**
 * @brief Validate parameters and serialize a publish ACK packet body.
 * Handles the three cases: no reason code, reason code only, reason code + properties.
 */
static MQTTStatus_t serializeAckBody( const MQTTFixedBuffer_t * pFixedBuffer,
                                      uint8_t packetType,
                                      uint16_t packetId,
                                      const MQTTPropBuilder_t * pAckProperties,
                                      const MQTTSuccessFailReasonCode_t * pReasonCode )
{
    MQTTStatus_t status = MQTTSuccess;

    pFixedBuffer->pBuffer[ 0 ] = packetType;

    if( pReasonCode == NULL )
    {
        /* Simple 4-byte ACK: type + remaining(2) + packetId(2). */
        pFixedBuffer->pBuffer[ 1 ] = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH;
        pFixedBuffer->pBuffer[ 2 ] = UINT16_HIGH_BYTE( packetId );
        pFixedBuffer->pBuffer[ 3 ] = UINT16_LOW_BYTE( packetId );
    }
    else if( pFixedBuffer->size < ( MQTT_PUBLISH_ACK_PACKET_SIZE + 1U ) )
    {
        LogError( ( "Not enough space for reason code." ) );
        status = MQTTBadParameter;
    }
    else if( ( pAckProperties == NULL ) || ( pAckProperties->pBuffer == NULL ) )
    {
        /* Reason code only, no properties. */
        pFixedBuffer->pBuffer[ 1 ] = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH + 1U;
        pFixedBuffer->pBuffer[ 2 ] = UINT16_HIGH_BYTE( packetId );
        pFixedBuffer->pBuffer[ 3 ] = UINT16_LOW_BYTE( packetId );
        pFixedBuffer->pBuffer[ 4 ] = ( uint8_t ) ( *pReasonCode );
    }
    else
    {
        status = serializeAckWithProperties( pFixedBuffer, packetType, packetId,
                                             pAckProperties, pReasonCode );
    }

    return status;
}

MQTTStatus_t MQTT_SerializeAck( const MQTTFixedBuffer_t * pFixedBuffer,
                                uint8_t packetType,
                                uint16_t packetId,
                                const MQTTPropBuilder_t * pAckProperties,
                                const MQTTSuccessFailReasonCode_t * pReasonCode )
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
    else if( ( pReasonCode == NULL ) && ( pAckProperties != NULL ) )
    {
        LogError( ( "A reason code must be provided if there are properties." ) );
        status = MQTTBadParameter;
    }
    else if( ( pReasonCode != NULL ) && ( validateReasonCodeForAck( packetType, *pReasonCode ) != MQTTSuccess ) )
    {
        LogError( ( "Invalid reason code for the ACK type." ) );
        status = MQTTBadParameter;
    }
    else if( ( pAckProperties != NULL ) &&
             ( CHECK_SIZE_T_OVERFLOWS_32BIT( pAckProperties->currentIndex ) ||
               ( pAckProperties->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) ) )
    {
        LogError( ( "ACK properties must be smaller than 268435456." ) );
        status = MQTTBadParameter;
    }
    else
    {
        switch( packetType )
        {
            case MQTT_PACKET_TYPE_PUBACK:
            case MQTT_PACKET_TYPE_PUBREC:
            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                status = serializeAckBody( pFixedBuffer, packetType, packetId,
                                           pAckProperties, pReasonCode );
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
                                           uint32_t * pRemainingLength,
                                           uint32_t * pPacketSize,
                                           uint32_t maxPacketSize,
                                           const MQTTSuccessFailReasonCode_t * pReasonCode )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t length = 0U;
    uint32_t packetSize = 0U;
    uint32_t propertyLength = 0U;

    /* Validate the arguments. */
    if( ( pReasonCode == NULL ) && ( pDisconnectProperties != NULL ) )
    {
        LogError( ( "Reason code must be specified if the properties are non-NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
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
    else if( ( pReasonCode != NULL ) && ( validateDisconnectResponse( *pReasonCode, false ) != MQTTSuccess ) )
    {
        LogError( ( "Invalid reason code." ) );
        status = MQTTBadParameter;
    }
    else if( pReasonCode != NULL )
    {
        /* Reason code. */
        length += 1U;
    }
    else
    {
        /* No reason code provided. No need to update the length. */
    }

    if( status == MQTTSuccess )
    {
        if( ( pDisconnectProperties != NULL ) && ( pDisconnectProperties->pBuffer != NULL ) )
        {
            /* The value must fit in a 32-bit variable all the while being small enough to
             * be properly encoded in a variable integer format. */
            if( CHECK_SIZE_T_OVERFLOWS_32BIT( pDisconnectProperties->currentIndex ) ||
                ( pDisconnectProperties->currentIndex > MQTT_MAX_REMAINING_LENGTH ) )
            {
                LogError( ( "Disconnect properties must be less than 268435456 "
                            "to be able to fit in a MQTT packet." ) );
                status = MQTTBadParameter;
            }
            else
            {
                propertyLength = ( uint32_t ) pDisconnectProperties->currentIndex;
            }
        }

        if( status == MQTTSuccess )
        {
            /* Validate the length. The sum of:
             * Bytes required to encode the properties +
             * Actual properties +
             * Optional reason code (which is depicted by length)
             *
             * Must be less than the maximum allowed remaining length.
             */
            if( ( propertyLength + variableLengthEncodedSize( propertyLength ) + length ) < MQTT_MAX_REMAINING_LENGTH )
            {
                length += variableLengthEncodedSize( propertyLength ) + propertyLength;
                *pRemainingLength = length;
            }
            else
            {
                LogError( ( "The properties + reason code cannot fit in MQTT_MAX_REMAINING_LENGTH bytes." ) );
                status = MQTTBadParameter;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        /* Packet size should be less than max allowed by the server. It is calculated as:
         * MQTT Disconnect header byte +
         * Bytes required to encode the remaining length +
         * The remaining length (which includes properties and error code).
         */
        packetSize = 1U + variableLengthEncodedSize( length ) + length;

        if( packetSize > maxPacketSize )
        {
            LogError( ( "Packet Size greater than Max Packet Size specified in the CONNACK" ) );
            status = MQTTBadParameter;
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
                                       const MQTTSuccessFailReasonCode_t * pReasonCode,
                                       uint32_t remainingLength,
                                       const MQTTFixedBuffer_t * pFixedBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    uint32_t packetSize = 0U;
    uint32_t propertyLength = 0U;

    if( ( pDisconnectProperties != NULL ) && ( pDisconnectProperties->pBuffer != NULL ) )
    {
        if( CHECK_SIZE_T_OVERFLOWS_32BIT( pDisconnectProperties->currentIndex ) ||
            ( pDisconnectProperties->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
        {
            LogError( ( "Disconnect properties cannot have a length more than 268435455." ) );
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength = ( uint32_t ) pDisconnectProperties->currentIndex;
        }
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
    else if( ( pReasonCode == NULL ) && ( pDisconnectProperties != NULL ) )
    {
        LogError( ( "Reason code must be provided if the properties are non-NULL." ) );
        status = MQTTBadParameter;
    }
    else if( remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length cannot be greater than 268435455." ) );
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
        pIndex = serializeDisconnectFixed( pIndex,
                                           pReasonCode,
                                           remainingLength );

        pIndex = encodeVariableLength( pIndex, propertyLength );

        if( propertyLength > 0U )
        {
            ( void ) memcpy( ( void * ) pIndex, ( const void * ) pDisconnectProperties->pBuffer, ( size_t ) propertyLength );
            pIndex = &pIndex[ ( size_t ) propertyLength ];
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetPingreqPacketSize( uint32_t * pPacketSize )
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
        /* All parameters are good. */
    }

    if( status == MQTTSuccess )
    {
        if( pFixedBuffer->size < MQTT_PACKET_PINGREQ_SIZE )
        {
            LogError( ( "Buffer size of %lu is not sufficient to hold "
                        "serialized PINGREQ packet of size of %" PRIu32 ".",
                        ( unsigned long ) pFixedBuffer->size,
                        ( uint32_t ) MQTT_PACKET_PINGREQ_SIZE ) );
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
                                      MQTTPropBuilder_t * propBuffer,
                                      uint32_t maxPacketSize,
                                      uint16_t topicAliasMax )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( pIncomingPacket == NULL ) || ( pPacketId == NULL ) || ( pPublishInfo == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pIncomingPacket=%p, "
                    "pPacketId=%p, pPublishInfo=%p",
                    ( const void * ) pIncomingPacket,
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
    else if( pIncomingPacket->remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "Remaining length cannot be larger than MQTT maximum (268435456)." ) );
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->remainingLength +
               variableLengthEncodedSize( pIncomingPacket->remainingLength ) +
               1U ) > maxPacketSize )
    {
        LogError( ( "The incoming packet length is greater than the maximum packet size." ) );
        status = MQTTBadResponse;
    }
    else
    {
        status = deserializePublish( pIncomingPacket, pPacketId, pPublishInfo, propBuffer, topicAliasMax );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeConnAck( const MQTTPacketInfo_t * pIncomingPacket,
                                      bool * pSessionPresent,
                                      MQTTPropBuilder_t * pPropBuffer,
                                      MQTTConnectionProperties_t * pConnectProperties )
{
    MQTTStatus_t status = MQTTSuccess;

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
    else if( pIncomingPacket->type != MQTT_PACKET_TYPE_CONNACK )
    {
        LogError( ( "MQTT_DeserializeConnAck should only be used to deserialize CONNACK packet." ) );
        status = MQTTBadParameter;
    }
    /* Pointer for session present cannot be NULL for CONNACK. */
    else if( pSessionPresent == NULL )
    {
        LogError( ( "pSessionPresent cannot be NULL for CONNACK packet." ) );
        status = MQTTBadParameter;
    }

    /* Pointer for remaining data cannot be NULL for packets other
     * than PINGRESP. */
    else if( pIncomingPacket->pRemainingData == NULL )
    {
        LogError( ( "Remaining data of incoming CONNACK packet is NULL." ) );
        status = MQTTBadParameter;
    }
    /* Max packet size cannot be 0. */
    else if( pConnectProperties->maxPacketSize == 0U )
    {
        LogError( ( "Max packet size cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_U32T_OVERFLOWS_SIZE_T( pIncomingPacket->remainingLength ) )
    {
        LogError( ( "pIncomingPacket->remainingLength cannot be represented by size_t." ) );

        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
        /* coverity[misra_c_2012_rule_14_3_violation] */
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->remainingLength +
               variableLengthEncodedSize( pIncomingPacket->remainingLength ) +
               1U ) > pConnectProperties->maxPacketSize )
    {
        LogError( ( "Incoming CONNACK packet Size cannot be greater than max packet size. " ) );
        status = MQTTBadResponse;
    }
    else
    {
        status = deserializeConnack( pConnectProperties,
                                     pIncomingPacket,
                                     pSessionPresent,
                                     pPropBuffer );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId,
                                  MQTTReasonCodeInfo_t * pReasonCode,
                                  MQTTPropBuilder_t * pPropBuffer,
                                  const MQTTConnectionProperties_t * pConnectProperties )
{
    MQTTStatus_t status = MQTTSuccess;

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
    else if( pIncomingPacket->type == MQTT_PACKET_TYPE_CONNACK )
    {
        LogError( ( "Please use MQTT_DeserializeConnAck for CONNACK packet." ) );
        status = MQTTBadParameter;
    }

    /* Pointer for packet identifier cannot be NULL for packets other than
     * CONNACK and PINGRESP. This function must not be called for CONNACK
     * handling and thus only PINGRESP is checked below. */
    else if( ( pPacketId == NULL ) &&
             ( pIncomingPacket->type != MQTT_PACKET_TYPE_PINGRESP ) )
    {
        LogError( ( "pPacketId cannot be NULL for packet type %02x.",
                    ( unsigned int ) pIncomingPacket->type ) );
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
    /* Max packet size cannot be 0. */
    else if( pConnectProperties->maxPacketSize == 0U )
    {
        LogError( ( "Max packet size cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_U32T_OVERFLOWS_SIZE_T( pIncomingPacket->remainingLength ) )
    {
        LogError( ( "Incoming packet length cannot be represented in a size_t variable." ) );

        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
        /* coverity[misra_c_2012_rule_14_3_violation] */
        status = MQTTBadParameter;
    }
    else if( ( pIncomingPacket->remainingLength +
               variableLengthEncodedSize( pIncomingPacket->remainingLength ) +
               1U ) > pConnectProperties->maxPacketSize )
    {
        LogError( ( "Packet Size cannot be greater than max packet size." ) );
        status = MQTTBadResponse;
    }
    else
    {
        /* Make sure response packet is a valid ack. */
        switch( pIncomingPacket->type )
        {
            case MQTT_PACKET_TYPE_PUBACK:
            case MQTT_PACKET_TYPE_PUBREC:
            case MQTT_PACKET_TYPE_PUBREL:
            case MQTT_PACKET_TYPE_PUBCOMP:
                status = deserializePubAcks( pIncomingPacket,
                                             pPacketId,
                                             pReasonCode,
                                             pConnectProperties->requestProblemInfo,
                                             pPropBuffer );

                if( ( status == MQTTSuccess ) && ( pIncomingPacket->remainingLength > 2U ) )
                {
                    /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-105 */
                    /* coverity[misra_c_2012_rule_10_5_violation] */
                    status = logAckResponse( ( MQTTSuccessFailReasonCode_t ) ( *pReasonCode->reasonCode ),
                                             *pPacketId );
                }

                break;

            case MQTT_PACKET_TYPE_SUBACK:
            case MQTT_PACKET_TYPE_UNSUBACK:
                status = deserializeSubUnsubAck( pIncomingPacket, pPacketId, pReasonCode, pPropBuffer );
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
        /* Input was invalid. */
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
            LogTrace( ( "Incoming packet type: %s",
                        MQTT_GetPacketTypeString( pIncomingPacket->type ) ) );
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

MQTTStatus_t MQTTPropertyBuilder_Init( MQTTPropBuilder_t * pPropertyBuilder,
                                       uint8_t * buffer,
                                       size_t length )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( pPropertyBuilder == NULL ) || ( buffer == NULL ) || ( length == 0U ) )
    {
        LogError( ( "Invalid arguments passed to MQTTPropertyBuilder_Init. "
                    "pPropertyBuilder must be non-NULL; "
                    "buffer must be non-NULL; "
                    "and length must be non-zero. " ) );
        status = MQTTBadParameter;
    }
    else if( length >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "The length must be less than max MQTT packet size (268435456)." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Nothing to do. All values are valid. */
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

MQTTStatus_t MQTT_ValidateWillProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;
    uint32_t propertyBitMask = 0;

    if( ( pPropertyBuilder == NULL ) || ( pPropertyBuilder->pBuffer == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else if( ( CHECK_SIZE_T_OVERFLOWS_32BIT( pPropertyBuilder->currentIndex ) ) ||
             ( pPropertyBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        const char * data;
        size_t dataLength;

        pIndex = &pIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_WILL_DELAY_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_WILL_DELAY_POS ) != true )
                {
                    status = decodeUint32t( NULL, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_WILL_DELAY_POS );
                }
                else
                {
                    LogError( ( "Will Delay Interval included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_PAYLOAD_FORMAT_ID:
               {
                   uint8_t property;

                   if( UINT32_CHECK_BIT( propertyBitMask, MQTT_PAYLOAD_FORMAT_INDICATOR_POS ) != true )
                   {
                       status = decodeUint8t( &property, &propertyLength, &used, &pIndex );
                       UINT32_SET_BIT( propertyBitMask, MQTT_PAYLOAD_FORMAT_INDICATOR_POS );
                   }
                   else
                   {
                       LogError( ( "Payload format indicator included more than once in the properties." ) );
                       status = MQTTBadResponse;
                   }

                   if( status == MQTTSuccess )
                   {
                       if( property > 1U )
                       {
                           LogError( ( "Payload Format can only be 0 or 1 in will properties." ) );
                           status = MQTTBadResponse;
                       }
                   }
               }
               break;

            case MQTT_MSG_EXPIRY_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_MESSAGE_EXPIRY_INTERVAL_POS ) != true )
                {
                    status = decodeUint32t( NULL, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_MESSAGE_EXPIRY_INTERVAL_POS );
                }
                else
                {
                    LogError( ( "Message Expiry Interval included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_CONTENT_TYPE_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_CONTENT_TYPE_POS ) != true )
                {
                    status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_CONTENT_TYPE_POS );
                }
                else
                {
                    LogError( ( "Content Type included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_RESPONSE_TOPIC_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_RESPONSE_TOPIC_POS ) != true )
                {
                    status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_RESPONSE_TOPIC_POS );
                }
                else
                {
                    LogError( ( "Response topic included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_CORRELATION_DATA_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_CORRELATION_DATA_POS ) != true )
                {
                    status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_CORRELATION_DATA_POS );
                }
                else
                {
                    LogError( ( "Corelation Data included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key,
                                            &keyLength,
                                            &value,
                                            &valueLength,
                                            &propertyLength,
                                            &pIndex );
               }
               break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateConnectProperties( const MQTTPropBuilder_t * pPropertyBuilder,
                                             bool * isRequestProblemInfoSet,
                                             uint32_t * pPacketMaxSizeValue )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;
    uint32_t propertyBitMask = 0;

    if( ( pPropertyBuilder == NULL ) ||
        ( pPropertyBuilder->pBuffer == NULL ) ||
        ( isRequestProblemInfoSet == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else if( ( CHECK_SIZE_T_OVERFLOWS_32BIT( pPropertyBuilder->currentIndex ) ) ||
             ( pPropertyBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
        *isRequestProblemInfoSet = false;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        const char * data;
        size_t dataLength;

        pIndex = &pIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_SESSION_EXPIRY_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_SESSION_EXPIRY_INTERVAL_POS ) != true )
                {
                    status = decodeUint32t( NULL, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_SESSION_EXPIRY_INTERVAL_POS );
                }
                else
                {
                    LogError( ( "Session Expiry Interval included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_RECEIVE_MAX_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_RECEIVE_MAXIMUM_POS ) != true )
                {
                    uint16_t receiveMax;
                    status = decodeUint16t( &receiveMax, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_RECEIVE_MAXIMUM_POS );

                    if( status == MQTTSuccess )
                    {
                        if( receiveMax == 0U )
                        {
                            LogError( ( "Receive Maximum cannot be 0 in CONNECT properties." ) );
                            status = MQTTBadResponse;
                        }
                    }
                }
                else
                {
                    LogError( ( "Receive Maximum included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_MAX_PACKET_SIZE_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_MAX_PACKET_SIZE_POS ) != true )
                {
                    uint32_t maxPacketSize;
                    status = decodeUint32t( &maxPacketSize, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_MAX_PACKET_SIZE_POS );

                    if( status == MQTTSuccess )
                    {
                        if( maxPacketSize == 0U )
                        {
                            LogError( ( "Maximum Packet Size cannot be 0 in CONNECT properties." ) );
                            status = MQTTBadResponse;
                        }
                        else if( pPacketMaxSizeValue != NULL )
                        {
                            *pPacketMaxSizeValue = maxPacketSize;
                        }
                        else
                        {
                            /* Nothing to do. */
                        }
                    }
                }
                else
                {
                    LogError( ( "Maximum Packet Size included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_TOPIC_ALIAS_MAX_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_TOPIC_ALIAS_MAX_POS ) != true )
                {
                    status = decodeUint16t( NULL, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_TOPIC_ALIAS_MAX_POS );
                }
                else
                {
                    LogError( ( "Topic Alias Maximum included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_REQUEST_RESPONSE_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_REQUEST_RESPONSE_INFO_POS ) != true )
                {
                    uint8_t requestResponseInfo;
                    status = decodeUint8t( &requestResponseInfo, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_REQUEST_RESPONSE_INFO_POS );

                    if( status == MQTTSuccess )
                    {
                        if( ( requestResponseInfo != 0U ) && ( requestResponseInfo != 1U ) )
                        {
                            LogError( ( "Request Response Information can only be 0 or 1 in CONNECT properties." ) );
                            status = MQTTBadResponse;
                        }
                    }
                }
                else
                {
                    LogError( ( "Request Response Information included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_REQUEST_PROBLEM_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_REQUEST_PROBLEM_INFO_POS ) != true )
                {
                    uint8_t requestProblemInfo;
                    status = decodeUint8t( &requestProblemInfo, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_REQUEST_PROBLEM_INFO_POS );

                    if( status == MQTTSuccess )
                    {
                        if( ( requestProblemInfo != 0U ) && ( requestProblemInfo != 1U ) )
                        {
                            LogError( ( "Request Problem Information can only be 0 or 1 in CONNECT properties." ) );
                            status = MQTTBadResponse;
                        }
                        else
                        {
                            *isRequestProblemInfoSet = ( requestProblemInfo == 1U );
                        }
                    }
                }
                else
                {
                    LogError( ( "Request Problem Information included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_AUTH_METHOD_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_AUTHENTICATION_METHOD_POS ) != true )
                {
                    status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_AUTHENTICATION_METHOD_POS );
                }
                else
                {
                    LogError( ( "Authentication Method included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_AUTH_DATA_ID:

                if( UINT32_CHECK_BIT( propertyBitMask, MQTT_AUTHENTICATION_DATA_POS ) != true )
                {
                    status = decodeUtf8( &data, &dataLength, &propertyLength, &used, &pIndex );
                    UINT32_SET_BIT( propertyBitMask, MQTT_AUTHENTICATION_DATA_POS );
                }
                else
                {
                    LogError( ( "Authentication Data included more than once in the properties." ) );
                    status = MQTTBadResponse;
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key,
                                            &keyLength,
                                            &value,
                                            &valueLength,
                                            &propertyLength,
                                            &pIndex );
               }
               break;

            default:
                LogError( ( "Invalid property ID 0x%02x in CONNECT properties.", propertyId ) );
                status = MQTTBadResponse;
                break;
        }
    }

    if( status == MQTTSuccess )
    {
        if( ( UINT32_CHECK_BIT( propertyBitMask, MQTT_AUTHENTICATION_DATA_POS ) == true ) &&
            ( UINT32_CHECK_BIT( propertyBitMask, MQTT_AUTHENTICATION_METHOD_POS ) != true ) )
        {
            LogError( ( "Authentication data added but no authentication method present in CONNECT properties." ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidateSubscribeProperties( bool isSubscriptionIdAvailable,
                                               const MQTTPropBuilder_t * propBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pLocalIndex = NULL;
    uint32_t subscriptionId = 0;
    bool subscriptionIDIncluded = false;

    if( ( propBuilder == NULL ) || ( propBuilder->pBuffer == NULL ) )
    {
        status = MQTTBadParameter;
    }
    else if( ( CHECK_SIZE_T_OVERFLOWS_32BIT( propBuilder->currentIndex ) ) ||
             ( propBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) propBuilder->currentIndex;
        pLocalIndex = propBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_SUBSCRIPTION_ID_ID:

                if( subscriptionIDIncluded == true )
                {
                    status = MQTTBadParameter;
                    LogError( ( "Subscription ID included. Protocol Error to include twice." ) );
                }

                if( status == MQTTSuccess )
                {
                    status = decodeVariableLength( pLocalIndex, ( size_t ) propertyLength, &subscriptionId );
                }

                if( status == MQTTSuccess )
                {
                    propertyLength -= variableLengthEncodedSize( subscriptionId );

                    if( isSubscriptionIdAvailable == false )
                    {
                        LogError( ( "Protocol Error : Subscription Id not available" ) );
                        status = MQTTBadParameter;
                    }
                    else if( subscriptionId == 0U )
                    {
                        LogError( ( "Protocol Error : Subscription Id value set to 0" ) );
                        status = MQTTBadParameter;
                    }
                    else
                    {
                        subscriptionIDIncluded = true;
                        LogTrace( ( "Processing subscription ID %" PRIu32,
                                    subscriptionId ) );
                    }
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key, &keyLength, &value, &valueLength, &propertyLength, &pLocalIndex );

                   if( status == MQTTSuccess )
                   {
                       LogTrace( ( "Processing User Property %.*s:%.*s",
                                   ( int ) keyLength, key,
                                   ( int ) valueLength, value ) );
                   }
               }
               break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_ValidatePublishProperties( uint16_t serverTopicAliasMax,
                                             const MQTTPropBuilder_t * propBuilder,
                                             uint16_t * topicAlias )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pLocalIndex = NULL;
    bool topicAliasBool = false;

    if( ( propBuilder == NULL ) || ( propBuilder->pBuffer == NULL ) )
    {
        LogError( ( "Property Builder is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( topicAlias == NULL )
    {
        LogError( ( "Topic Alias is NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( CHECK_SIZE_T_OVERFLOWS_32BIT( propBuilder->currentIndex ) ) ||
             ( propBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) propBuilder->currentIndex;
        pLocalIndex = propBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pLocalIndex;
        bool used = false;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_PAYLOAD_FORMAT_ID:
               {
                   uint8_t property;
                   status = decodeUint8t( &property, &propertyLength, &used, &pLocalIndex );
               }
               break;

            case MQTT_MSG_EXPIRY_ID:
               {
                   uint32_t property;
                   status = decodeUint32t( &property, &propertyLength, &used, &pLocalIndex );
                   break;
               }

            case MQTT_CONTENT_TYPE_ID:
            case MQTT_CORRELATION_DATA_ID:
            case MQTT_RESPONSE_TOPIC_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &used, &pLocalIndex );
                   break;
               }

            case MQTT_TOPIC_ALIAS_ID:
                status = decodeUint16t( topicAlias, &propertyLength, &topicAliasBool, &pLocalIndex );

                if( ( status == MQTTSuccess ) && ( serverTopicAliasMax < *topicAlias ) )
                {
                    LogError( ( "Protocol Error: Topic Alias greater than Topic Alias Max" ) );
                    status = MQTTBadParameter;
                }

                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * pPropertyKey;
                   size_t propertyKeyLen;
                   const char * pPropertyValue;
                   size_t propertyValueLen;
                   status = decodeUserProp( &pPropertyKey,
                                            &propertyKeyLen,
                                            &pPropertyValue,
                                            &propertyValueLen,
                                            &propertyLength,
                                            &pLocalIndex );
               }
               break;

            default:
                status = MQTTBadParameter;
                break;
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
                    ( const void * ) pPublishInfo ) );
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
                    ( const void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pPublishInfo->pTopicName == NULL ) && ( pPublishInfo->topicNameLength != 0U ) )
    {
        LogError( ( "Invalid topic name for PUBLISH: pTopicName=%p, "
                    "topicNameLength=%hu.",
                    ( const void * ) pPublishInfo->pTopicName,
                    ( unsigned short ) pPublishInfo->topicNameLength ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( pPublishInfo->topicNameLength ) )
    {
        LogError( ( "topicNameLength must be less than 65535 to fit in 16-bits." ) );
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

MQTTStatus_t MQTT_ValidatePublishAckProperties( const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;
    bool used = false;

    if( ( pPropertyBuilder != NULL ) &&
        ( ( CHECK_SIZE_T_OVERFLOWS_32BIT( pPropertyBuilder->currentIndex ) ) ||
          ( pPropertyBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }

    if( ( status == MQTTSuccess ) && ( pPropertyBuilder != NULL ) && ( pPropertyBuilder->pBuffer != NULL ) )
    {
        propertyLength = ( uint32_t ) pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        pIndex = &pIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &used, &pIndex );
               }
               break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key,
                                            &keyLength,
                                            &value,
                                            &valueLength,
                                            &propertyLength,
                                            &pIndex );
               }
               break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetAckPacketSize( uint32_t * pRemainingLength,
                                    uint32_t * pPacketSize,
                                    uint32_t maxPacketSize,
                                    size_t ackPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t length = 0U;
    uint32_t propertyLength;
    uint32_t packetSize = 0U;

    /* Validate the parameters. */
    if( ( pRemainingLength == NULL ) || ( pPacketSize == NULL ) )
    {
        LogError( ( "pRemainingLength and pPacketSize cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        LogError( ( "maxPacketSize cannot be 0 as specified by MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_32BIT( ackPropertyLength ) ||
             ( ackPropertyLength > MQTT_MAX_REMAINING_LENGTH ) )
    {
        LogError( ( "ackPropertyLength must be smaller than 268435455 to fit in MQTT packet." ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) ackPropertyLength;

        /* 1 byte Reason code + 2 byte Packet Identifier. */
        length += 3U;

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
        /* Length of variable header + 1 byte ACK header +
         * bytes required to encode the remaining length. */
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

MQTTStatus_t MQTT_ValidateDisconnectProperties( uint32_t connectSessionExpiry,
                                                const MQTTPropBuilder_t * pPropertyBuilder )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t propertyLength = 0U;
    uint8_t * pIndex = NULL;
    uint32_t sessionExpiry;

    if( ( pPropertyBuilder == NULL ) || ( pPropertyBuilder->pBuffer == NULL ) )
    {
        LogError( ( "Arguments cannot be NULL : pPropertyBuilder=%p.", ( const void * ) pPropertyBuilder ) );
        status = MQTTBadParameter;
    }
    else if( ( CHECK_SIZE_T_OVERFLOWS_32BIT( pPropertyBuilder->currentIndex ) ) ||
             ( pPropertyBuilder->currentIndex >= MQTT_REMAINING_LENGTH_INVALID ) )
    {
        LogError( ( "Property length cannot have more than %" PRIu32 " bytes", MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        propertyLength = ( uint32_t ) pPropertyBuilder->currentIndex;
        pIndex = pPropertyBuilder->pBuffer;
    }

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        uint8_t propertyId = *pIndex;
        bool used = false;
        pIndex = &pIndex[ 1 ];
        propertyLength -= 1U;

        switch( propertyId )
        {
            case MQTT_SESSION_EXPIRY_ID:
                status = decodeUint32t( &sessionExpiry, &propertyLength, &used, &pIndex );

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
               {
                   const char * pProperty;
                   size_t length;
                   status = decodeUtf8( &pProperty, &length, &propertyLength, &used, &pIndex );
               }
               break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key, &keyLength, &value, &valueLength, &propertyLength, &pIndex );
               }
               break;

            default:
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_DeserializeDisconnect( const MQTTPacketInfo_t * pPacket,
                                         uint32_t maxPacketSize,
                                         MQTTReasonCodeInfo_t * pDisconnectInfo,
                                         MQTTPropBuilder_t * pPropBuffer )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex = NULL;
    uint32_t propertyLength = 0U;

    /* Validate the arguments. */
    if( ( pPacket == NULL ) || ( pPacket->pRemainingData == NULL ) )
    {
        LogError( ( "pPacket and pPacket->pRemainingData must not be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pDisconnectInfo == NULL )
    {
        LogError( ( "pDisconnectInfo must not be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( maxPacketSize == 0U )
    {
        LogError( ( "maxPacketSize must not be 0." ) );
        status = MQTTBadParameter;
    }
    else if( pPacket->remainingLength >= MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "pPacket->remainingLength must be less than 268435456." ) );
        status = MQTTBadResponse;
    }
    else if( CHECK_U32T_OVERFLOWS_SIZE_T( pPacket->remainingLength ) )
    {
        LogError( ( "pPacket->remainingLength cannot fit in size_t." ) );

        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
        /* coverity[misra_c_2012_rule_14_3_violation] */
        status = MQTTBadParameter;
    }

    /* Packet size should not be more than the max allowed by the client.
     * The length is calculated as: Remaining length +
     *       Bytes required to encode the remaining length +
     *       The disconnect header of 1 byte.
     */
    else if( ( pPacket->remainingLength + variableLengthEncodedSize( pPacket->remainingLength ) + 1U ) > maxPacketSize )
    {
        status = MQTTBadResponse;
    }
    else if( pPacket->remainingLength == 0U )
    {
        /* No properties or reason code provided. Nothing to do. */
    }
    else
    {
        /* Extract the reason code. */
        pIndex = pPacket->pRemainingData;
        pDisconnectInfo->reasonCode = pIndex;
        pDisconnectInfo->reasonCodeLength = 1U;
        pIndex++;

        /* Validate the reason code. */
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-105 */
        /* coverity[misra_c_2012_rule_10_5_violation] */
        status = validateDisconnectResponse( ( MQTTSuccessFailReasonCode_t ) ( *pDisconnectInfo->reasonCode ), true );
    }

    if( status == MQTTSuccess )
    {
        if( ( pPacket->remainingLength > 1U ) )
        {
            /* Extract the property length. */
            /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
            /* coverity[misra_c_2012_rule_10_8_violation] */
            status = decodeVariableLength( pIndex, ( size_t ) ( pPacket->remainingLength - 1U ), &propertyLength );

            if( status == MQTTSuccess )
            {
                /* Validate the remaining length. It must only have the reason code
                 * and the properties and the bytes needed to encode the property length. */
                if( pPacket->remainingLength != ( propertyLength + variableLengthEncodedSize( propertyLength ) + 1U ) )
                {
                    LogError( ( "Remaining length doesn't match the expected size." ) );
                    status = MQTTBadResponse;
                }
                else if( CHECK_U32T_OVERFLOWS_SIZE_T( propertyLength ) )
                {
                    LogError( ( "Incoming property length will overflow the property buffer." ) );

                    /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-143 */
                    /* coverity[misra_c_2012_rule_14_3_violation] */
                    status = MQTTBadResponse;
                }
                else
                {
                    pIndex = &pIndex[ ( size_t ) variableLengthEncodedSize( propertyLength ) ];

                    if( pPropBuffer != NULL )
                    {
                        pPropBuffer->bufferLength = propertyLength;
                        pPropBuffer->pBuffer = pIndex;
                    }
                }
            }
            else
            {
                LogError( ( "Failed to decode the property length. Malformed packet." ) );
                status = MQTTBadResponse;
            }
        }
    }

    if( status == MQTTSuccess )
    {
        status = validateIncomingDisconnectProperties( pIndex, propertyLength );

        if( status != MQTTSuccess )
        {
            LogError( ( "Failed to validate disconnect properties." ) );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t validateIncomingDisconnectProperties( uint8_t * pIndex,
                                                          uint32_t disconnectPropertyLength )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pLocalIndex = pIndex;
    const char * pReasonString;
    size_t reasonStringLength;
    const char * pServerRef;
    size_t pServerRefLength;
    uint32_t propertyLength = disconnectPropertyLength;
    bool reasonString = false;
    bool serverRef = false;

    while( ( propertyLength > 0U ) && ( status == MQTTSuccess ) )
    {
        /* Decode the property id. */
        uint8_t propertyId = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ 1 ];
        propertyLength -= 1U;

        /* Validate the property id and decode accordingly. */
        switch( propertyId )
        {
            case MQTT_REASON_STRING_ID:
                status = decodeUtf8( &pReasonString, &reasonStringLength, &propertyLength, &reasonString, &pLocalIndex );
                break;

            case MQTT_USER_PROPERTY_ID:
               {
                   const char * key, * value;
                   size_t keyLength, valueLength;
                   status = decodeUserProp( &key, &keyLength, &value, &valueLength, &propertyLength, &pLocalIndex );
               }
               break;

            case MQTT_SERVER_REF_ID:
                status = decodeUtf8( &pServerRef, &pServerRefLength, &propertyLength, &serverRef, &pLocalIndex );
                break;

            default:
                status = MQTTBadResponse;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/
