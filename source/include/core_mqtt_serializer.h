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
 * @file core_mqtt_serializer.h
 * @brief User-facing functions for serializing and deserializing MQTT 3.1.1
 * packets. This header should be included for building a lighter weight MQTT
 * client than the managed CSDK MQTT library API in core_mqtt.h, by using the
 * serializer and de-serializer functions exposed in this file's API.
 */
#ifndef CORE_MQTT_SERIALIZER_H
#define CORE_MQTT_SERIALIZER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON */

#include "transport_interface.h"
#include "core_mqtt_config_defaults.h"

/* MQTT packet types. */

/**
 * @addtogroup mqtt_constants
 * @{
 */
#define MQTT_PACKET_TYPE_CONNECT        ( ( uint8_t ) 0x10U )  /**< @brief CONNECT (client-to-server). */
#define MQTT_PACKET_TYPE_CONNACK        ( ( uint8_t ) 0x20U )  /**< @brief CONNACK (server-to-client). */
#define MQTT_PACKET_TYPE_PUBLISH        ( ( uint8_t ) 0x30U )  /**< @brief PUBLISH (bidirectional). */
#define MQTT_PACKET_TYPE_PUBACK         ( ( uint8_t ) 0x40U )  /**< @brief PUBACK (bidirectional). */
#define MQTT_PACKET_TYPE_PUBREC         ( ( uint8_t ) 0x50U )  /**< @brief PUBREC (bidirectional). */
#define MQTT_PACKET_TYPE_PUBREL         ( ( uint8_t ) 0x62U )  /**< @brief PUBREL (bidirectional). */
#define MQTT_PACKET_TYPE_PUBCOMP        ( ( uint8_t ) 0x70U )  /**< @brief PUBCOMP (bidirectional). */
#define MQTT_PACKET_TYPE_SUBSCRIBE      ( ( uint8_t ) 0x82U )  /**< @brief SUBSCRIBE (client-to-server). */
#define MQTT_PACKET_TYPE_SUBACK         ( ( uint8_t ) 0x90U )  /**< @brief SUBACK (server-to-client). */
#define MQTT_PACKET_TYPE_UNSUBSCRIBE    ( ( uint8_t ) 0xA2U )  /**< @brief UNSUBSCRIBE (client-to-server). */
#define MQTT_PACKET_TYPE_UNSUBACK       ( ( uint8_t ) 0xB0U )  /**< @brief UNSUBACK (server-to-client). */
#define MQTT_PACKET_TYPE_PINGREQ        ( ( uint8_t ) 0xC0U )  /**< @brief PINGREQ (client-to-server). */
#define MQTT_PACKET_TYPE_PINGRESP       ( ( uint8_t ) 0xD0U )  /**< @brief PINGRESP (server-to-client). */
#define MQTT_PACKET_TYPE_DISCONNECT     ( ( uint8_t ) 0xE0U )  /**< @brief DISCONNECT (client-to-server). */
/** @} */

/**
 * @ingroup mqtt_constants
 * @brief The size of MQTT PUBACK, PUBREC, PUBREL, and PUBCOMP packets, per MQTT spec.
 */
#define MQTT_PUBLISH_ACK_PACKET_SIZE    ( 4UL )

/* Structures defined in this file. */
struct MQTTFixedBuffer;
struct MQTTConnectInfo;
struct MQTTSubscribeInfo;
struct MQTTPublishInfo;
struct MQTTPacketInfo;
struct MQTTSubscribeProperties ; 
struct MQTTConnectProperties;
struct MQTTUserProperty;
struct MQTTUserProperties;
struct MQTTAuthInfo;
/**
 * @ingroup mqtt_enum_types
 * @brief Return codes from MQTT functions.
 */
typedef enum MQTTStatus
{
    MQTTSuccess = 0,      /**< Function completed successfully. */
    MQTTBadParameter,     /**< At least one parameter was invalid. */
    MQTTNoMemory,         /**< A provided buffer was too small. */
    MQTTSendFailed,       /**< The transport send function failed. */
    MQTTRecvFailed,       /**< The transport receive function failed. */
    MQTTBadResponse,      /**< An invalid packet was received from the server. */
    MQTTServerRefused,    /**< The server refused a CONNECT or SUBSCRIBE. */
    MQTTNoDataAvailable,  /**< No data available from the transport interface. */
    MQTTIllegalState,     /**< An illegal state in the state record. */
    MQTTStateCollision,   /**< A collision with an existing state record entry. */
    MQTTKeepAliveTimeout, /**< Timeout while waiting for PINGRESP. */
    MQTTNeedMoreBytes,     /**< MQTT_ProcessLoop/MQTT_ReceiveLoop has received
                          incomplete data; it should be called again (probably after
                          a delay). */
    MQTTStatusConnected,            /**< MQTT connection is established with the broker. */
    MQTTStatusNotConnected,         /**< MQTT connection is not established with the broker. */
    MQTTStatusDisconnectPending,    /**< Transport Interface has failed and MQTT connection needs to be closed. */
    MQTTPublishStoreFailed,         /**< User provided API to store a copy of outgoing publish for retransmission  purposes,
                                    has failed. */
    MQTTPublishRetrieveFailed,      /**< User provided API to retrieve the copy of a publish while reconnecting
                                    with an unclean session has failed. */

    MQTTMalformedPacket=0x81,/**A malformed packet was received from the server. */
    MQTTProtocolError=0x82 /**A packet with protocol error was received from the server. */

} MQTTStatus_t;

/**
 * @ingroup mqtt_enum_types
 * @brief MQTT Quality of Service values.
 */
typedef enum MQTTQoS
{
    MQTTQoS0 = 0, /**< Delivery at most once. */
    MQTTQoS1 = 1, /**< Delivery at least once. */
    MQTTQoS2 = 2  /**< Delivery exactly once. */
} MQTTQoS_t;

#define MQTT_SUBSCRIBE_QOS1                    ( 0 ) /**< @brief MQTT SUBSCRIBE QoS1 flag. */
#define MQTT_SUBSCRIBE_QOS2                    ( 1 ) /**< @brief MQTT SUBSCRIBE QoS2 flag. */
#define MQTT_SUBSCRIBE_NO_LOCAL                ( 2 ) /**< @brief MQTT SUBSCRIBE no local flag. */
#define MQTT_SUBSCRIBE_RETAIN_AS_PUBLISHED     ( 3 ) /**< @brief MQTT SUBSCRIBE retain as published flag. */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING1        ( 4 ) /**<@brief MQTT SUBSCRIBE Retain Handling Option 1 */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING2        ( 5 ) /**<@brief Retain Handling Option 2   -> in core_mqtt_serializer.c */

/**
 * @ingroup mqtt_struct_types
 * @brief Buffer passed to MQTT library.
 *
 * These buffers are not copied and must remain in scope for the duration of the
 * MQTT operation.
 */
typedef struct MQTTFixedBuffer
{
    uint8_t * pBuffer; /**< @brief Pointer to buffer. */
    size_t size;       /**< @brief Size of buffer. */
} MQTTFixedBuffer_t;

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT CONNECT packet parameters.
 */


typedef struct MQTTConnectInfo
{
    /**
     * @brief Whether to establish a new, clean session or resume a previous session.
     */
    bool cleanSession;

    /**
     * @brief MQTT keep alive period.
     */
    uint16_t keepAliveSeconds;

    /**
     * @brief MQTT client identifier. Must be unique per client.
     */
    const char * pClientIdentifier;

    /**
     * @brief Length of the client identifier.
     */
    uint16_t clientIdentifierLength;

    /**
     * @brief MQTT user name. Set to NULL if not used.
     */
    const char * pUserName;

    /**
     * @brief Length of MQTT user name. Set to 0 if not used.
     */
    uint16_t userNameLength;

    /**
     * @brief MQTT password. Set to NULL if not used.
     */
    const char * pPassword;

    /**
     * @brief Length of MQTT password. Set to 0 if not used.
     */
    uint16_t passwordLength;
} MQTTConnectInfo_t;

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT SUBSCRIBE packet parameters.
 */

typedef enum retainHandling{
    retainSendOnSub = 0,
    retainSendOnSubIfNotPresent = 1,
    retainDoNotSendonSub = 2 
}retainHandling_t; 

typedef struct MQTTSubscribeInfo
{
    /**
     * @brief Quality of Service for subscription. Include protocol error of qos > 2 
     */
    MQTTQoS_t qos;

    /**
     * @brief Topic filter to subscribe to.
     */
    const char * pTopicFilter;

    /**
     * @brief Length of subscription topic filter - unsigned long
     */
    uint16_t ultopicFilterLength;
    /**
     * @brief no local option for subscription. Include protocol error if noLocalOption = 1 in a shared subscription
     */
    #if (MQTT_VERSION_5_ENABLED)
        bool noLocalOption;
        bool retainAsPublishedOption;
        retainHandling_t retainHandlingOption; 
    #endif
} MQTTSubscribeInfo_t;



   /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold authentication method and authentication data.
 */
typedef struct MQTTAuthInfo
{
    /**
     * @brief Authentication method used for authentication.
     */
    const char* pAuthMethod;
    /**
     * @brief Length of the authentication mathod.
     */
    uint16_t authMethodLength;
    /**
     * @brief Authentication data used for authentication.
     */
    const char* pAuthData;
     /**
     * @brief Length of the authentication data.
     */
    uint16_t authDataLength;
} MQTTAuthInfo_t;

#if(MQTT_USER_PROPERTY_ENABLED)
   /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold user property.
 */
typedef struct MQTTUserProperty
{
    /**
     * @brief key.
     */
    const char* pKey;
    /**
     * @brief Length of the key.
     */
    uint16_t keyLength;
    /**
     * @brief value.
     */
    const char* pValue;
    /**
     * @brief Length of the value.
     */
    uint16_t valueLength;
} MQTTUserProperty_t;

   /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold user property.
 */
typedef struct MQTTUserProperties
{
    /**
     * @brief Array to store the user properties.
     */
    MQTTUserProperty_t userProperty[MAX_USER_PROPERTY];
    /**
     * @brief Number of user property;
     */
    uint32_t count;

} MQTTUserProperties_t;

#endif
   /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold connect and connack properties.
 */
typedef struct MQTTConnectProperties
{
     /**
     * @brief Four Byte Integer representing the Session Expiry Interval in seconds.
     */
    uint32_t sessionExpiry;
     /**
     * @brief Maximum number of unacknowledged PUBLISH packets client is willing to receive.
     */
    uint16_t receiveMax;
     /**
     * @brief Four Byte Integer representing the Maximum Packet Size the Client is willing to accept.
     */
    uint32_t maxPacketSize;
     /**
     * @brief Two Byte Integer representing the Topic Alias Maximum value.
     */
    uint16_t topicAliasMax;
     /**
     * @brief  A value of 0 indicates that the Server MUST NOT return Response Information.
     */
    bool  requestResponseInfo;
     /**
     * @brief The Client uses this value to indicate whether the Reason String or User Properties
     *  are sent in the case of failures
     */
    bool  requestProblemInfo;
     /**
     * @brief Length of the connect properties.
     */
    size_t propertyLength;
     /**
     * @brief  Pointer to the incoming authentication information.
     */
    MQTTAuthInfo_t *pOutgoingAuth;

    /**
     * @brief Maximum number of unacknowledged PUBLISH packets client is willing to receive.
     */
    uint16_t serverReceiveMax;
     /**
     * @brief  Max qos supported by the server.
     */
    uint8_t serverMaxQos;
     /**
     * @brief Byte declares whether the Server supports retained messages.
     */
    uint8_t retainAvailable;
     /**
     * @brief Four Byte Integer representing the Maximum Packet Size the Server is willing to accept.
     */
    uint32_t serverMaxPacketSize;
     /**
     * @brief Client identifier assigned by the client.
     */
    const char* pClientIdentifier;
     /**
     * @brief Length of the assigned client identifier.
     */
    uint16_t clientIdLength;
     /**
     * @brief Two Byte Integer representing the Topic Alias Maximum value.
     */
    uint16_t serverTopicAliasMax;
     /**
     * @brief Reason String is a human readable string designed for diagnostics.
     */
    const char* pReasonString;
     /**
     * @brief Length of reason string.
     */
    uint16_t reasonStringLength;
     /**
     * @brief Whether wildcard subscription is available.
     */
    uint8_t isWildcardAvaiable;
     /**
     * @brief Whether the Server supports Subscription Identifiers.
     */
    uint8_t subscriptionId;
     /**
     * @brief Whether the Server supports Shared Subscription.
     */
    uint8_t isSharedAvailable;
     /**
     * @brief Keep Alive value given by the server.
     */
    uint16_t serverKeepAlive;
     /**
     * @brief UTF-8 Encoded String which is used as the basis for creating a Response Topic.
     */
    const char* pResponseInfo;
     /**
     * @brief Length of the response information.
     */
    uint16_t responseInfoLength;
     /**
     * @brief UTF-8 Encoded String which can be used by the Client to identify another Server to use
     */
    const char* pServerRef;
     /**
     * @brief Length of the server reference.
     */
    uint16_t serverRefLength;
     /**
     * @brief  Pointer to the incoming authentication information.
     */
    MQTTAuthInfo_t *pIncomingAuth;
    #if(MQTT_USER_PROPERTY_ENABLED)
     /**
     * @brief   Pointer to the outgoing user properties.
     */
    MQTTUserProperties_t *pOutgoingUserProperty;
     /**
     * @brief Pointer to the incoming user properties.
     */
    MQTTUserProperties_t *pIncomingUserProperty;
    #endif

} MQTTConnectProperties_t;

 /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold acknowledgment properties.
 */
typedef struct MQTTAckInfo
{
    /**
     * @brief Response code;
     */
    uint8_t reasonCode;
    /**
     * @brief Property Length.
     */
    size_t propertyLength;
    #if(MQTT_USER_PROPERTY_ENABLED)
     /**
     * @brief To store a key value pair.
     */
    MQTTUserProperties_t* pUserProperty;
    #endif
     /**
     * @brief Reason String is a human readable string designed for diagnostics.
     */
    const char* pReasonString;
     /**
     * @brief Length of reason string.
     */
    uint16_t reasonStringLength;
} MQTTAckInfo_t;


/**
 * @ingroup mqtt_struct_types
 * @brief MQTT PUBLISH packet parameters.
 */
typedef struct MQTTPublishInfo
{
    /**
     * @brief Quality of Service for message.
     */
    MQTTQoS_t qos;

    /**
     * @brief Whether this is a retained message.
     */
    bool retain;

    /**
     * @brief Whether this is a duplicate publish message.
     */
    bool dup;

    /**
     * @brief Topic name on which the message is published.
     */
    const char * pTopicName;

    /**
     * @brief Length of topic name.
     */
    uint16_t topicNameLength;

    /**
     * @brief Message payload.
     */
    const void * pPayload;

    /**
     * @brief Message payload length.
     */
    size_t payloadLength;

#if (MQTT_VERSION_5_ENABLED)
     /**
     * @brief Length of the properties.
     */
    size_t propertyLength;
     /**
     * @brief  Four Byte Integer representing the Will Delay Interval in seconds.
     */
    uint32_t willDelay;
     /**
     * @brief Payload Format Indicator.
     **/
      uint8_t payloadFormat;
     /**
     * @brief Topic alias value.
     **/
    uint16_t topicAlias;
     /**
     * @brief Four Byte Integer representing the Message Expiry Interval.
     */
    uint32_t msgExpiryInterval;
     /**
     * @brief Whether the message expiry is specified.
     */
    bool msgExpiryPresent;
     /**
     * @brief Length of the content type.
     */
    uint16_t contentTypeLength;
     /**
     * @brief UTF-8 Encoded String describing the content of the Will Message.
     */
    const char *pContentType;
     /**
     * @brief Length of the response topic.
     */
    uint16_t responseTopicLength;
     /**
     * @brief UTF-8 Encoded String which is used as the Topic Name for a response message.
     */
    const char *pResponseTopic;
     /**
     * @brief Length of the correlation data.
     */
    uint16_t correlationLength;
     /**
     * @brief To identify which request the Response Message is for.
     */
    const void *pCorrelationData;
    #if(MQTT_USER_PROPERTY_ENABLED)
     /**
     * @brief Pointer to the user properties.
     */
    MQTTUserProperties_t* pUserProperty;
    #endif
#endif

} MQTTPublishInfo_t;

/**
 * @ingroup mqtt_struct_types
 * @brief MQTT incoming packet parameters.
 */
typedef struct MQTTPacketInfo
{
    /**
     * @brief Type of incoming MQTT packet.
     */
    uint8_t type;

    /**
     * @brief Remaining serialized data in the MQTT packet.
     */
    uint8_t * pRemainingData;

    /**
     * @brief Length of remaining serialized data.
     */
    size_t remainingLength;

    /**
     * @brief The length of the MQTT header including the type and length.
     */
    size_t headerLength;
} MQTTPacketInfo_t;
// struct for subscribe properties -> have it in mqtt context ? 

typedef struct MQTTSubscribeProperties
{
    /**
     * Length of the property field - variable byte int
     */
    size_t propertyLength;
    /**
     * User properties - list of key value pairs
     */
    MQTTUserProperties_t* pUserProperties;
    /**
     * Subscription ID - variable byte int
     */

    size_t subscriptionId;

} MQTTSubscribeProperties_t;
/**
 * @brief Get the size and Remaining Length of an MQTT CONNECT packet.
 *
 * This function must be called before #MQTT_SerializeConnect in order to get
 * the size of the MQTT CONNECT packet that is generated from #MQTTConnectInfo_t
 * and optional #MQTTPublishInfo_t. The size of the #MQTTFixedBuffer_t supplied
 * to #MQTT_SerializeConnect must be at least @p pPacketSize. The provided
 * @p pConnectInfo and @p pWillInfo are valid for serialization with
 * #MQTT_SerializeConnect only if this function returns #MQTTSuccess. The
 * remaining length returned in @p pRemainingLength and the packet size returned
 * in @p pPacketSize are valid only if this function returns #MQTTSuccess.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[out] pRemainingLength The Remaining Length of the MQTT CONNECT packet.
 * @param[out] pPacketSize The total size of the MQTT CONNECT packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTConnectInfo_t connectInfo = { 0 };
 * MQTTPublishInfo_t willInfo = { 0 };
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the connection info, the details are out of scope for this example.
 * initializeConnectInfo( &connectInfo );
 *
 * // Initialize the optional will info, the details are out of scope for this example.
 * initializeWillInfo( &willInfo );
 *
 * // Get the size requirement for the connect packet.
 * status = MQTT_GetConnectPacketSize(
 *      &connectInfo, &willInfo, &remainingLength, &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the connect request.
 * }
 * @endcode
 */
/* @[declare_mqtt_getconnectpacketsize] */
static MQTTStatus_t MQTT_GetUserPropertySize(const MQTTUserProperty_t* pUserProperty,
    uint32_t number,
    size_t* pSize); 
MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize );
/* @[declare_mqtt_getconnectpacketsize] */

/**
 * @brief Serialize an MQTT CONNECT packet in the given fixed buffer @p pFixedBuffer.
 *
 * #MQTT_GetConnectPacketSize should be called with @p pConnectInfo and
 * @p pWillInfo before invoking this function to get the size of the required
 * #MQTTFixedBuffer_t and @p remainingLength. The @p remainingLength must be
 * the same as returned by #MQTT_GetConnectPacketSize. The #MQTTFixedBuffer_t
 * must be at least as large as the size returned by #MQTT_GetConnectPacketSize.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetConnectPacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTConnectInfo_t connectInfo = { 0 };
 * MQTTPublishInfo_t willInfo = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Assume connectInfo and willInfo are initialized. Get the size requirement for
 * // the connect packet.
 * status = MQTT_GetConnectPacketSize(
 *      &connectInfo, &willInfo, &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the connect packet into the fixed buffer.
 * status = MQTT_SerializeConnect( &connectInfo, &willInfo, remainingLength, &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The connect packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializeconnect] */
MQTTStatus_t MQTT_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializeconnect] */

/**
 * @brief Get packet size and Remaining Length of an MQTT SUBSCRIBE packet.
 *
 * This function must be called before #MQTT_SerializeSubscribe in order to get
 * the size of the MQTT SUBSCRIBE packet that is generated from the list of
 * #MQTTSubscribeInfo_t. The size of the #MQTTFixedBuffer_t supplied
 * to #MQTT_SerializeSubscribe must be at least @p pPacketSize. The provided
 * @p pSubscriptionList is valid for serialization with #MQTT_SerializeSubscribe
 * only if this function returns #MQTTSuccess. The remaining length returned in
 * @p pRemainingLength and the packet size returned in @p pPacketSize are valid
 * only if this function returns #MQTTSuccess.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[out] pRemainingLength The Remaining Length of the MQTT SUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT SUBSCRIBE packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * size_t remainingLength = 0, packetSize = 0;
 * // This is assumed to be a list of filters we want to subscribe to.
 * const char * filters[ NUMBER_OF_SUBSCRIPTIONS ];
 *
 * // Set each subscription.
 * for( int i = 0; i < NUMBER_OF_SUBSCRIPTIONS; i++ )
 * {
 *      subscriptionList[ i ].qos = MQTTQoS0;
 *      // Each subscription needs a topic filter.
 *      subscriptionList[ i ].pTopicFilter = filters[ i ];
 *      subscriptionList[ i ].topicFilterLength = strlen( filters[ i ] );
 * }
 *
 * // Get the size requirement for the subscribe packet.
 * status = MQTT_GetSubscribePacketSize(
 *      &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, &remainingLength, &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the subscribe request.
 * }
 * @endcode
 */
/* @[declare_mqtt_getsubscribepacketsize] */
MQTTStatus_t MQTTV5_GetSubscribePacketSize(MQTTSubscribeInfo_t* pSubscriptionList,
    MQTTSubscribeProperties_t* pSubscribeProperties,
    size_t subscriptionCount,
    size_t* pRemainingLength,
    size_t* pPacketSize); 

MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                          size_t subscriptionCount,
                                          size_t * pRemainingLength,
                                          size_t * pPacketSize );
/* @[declare_mqtt_getsubscribepacketsize] */

/**
 * @brief Serialize an MQTT SUBSCRIBE packet in the given buffer.
 *
 * #MQTT_GetSubscribePacketSize should be called with @p pSubscriptionList
 * before invoking this function to get the size of the required
 * #MQTTFixedBuffer_t and @p remainingLength. The @p remainingLength must be
 * the same as returned by #MQTT_GetSubscribePacketSize. The #MQTTFixedBuffer_t
 * must be at least as large as the size returned by #MQTT_GetSubscribePacketSize.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetSubscribePacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 * uint16_t packetId;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Function to return a valid, unused packet identifier. The details are out of
 * // scope for this example.
 * packetId = getNewPacketId();
 *
 * // Assume subscriptionList has been initialized. Get the subscribe packet size.
 * status = MQTT_GetSubscribePacketSize(
 *      &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the subscribe packet into the fixed buffer.
 * status = MQTT_SerializeSubscribe(
 *      &subscriptionList[ 0 ],
 *      NUMBER_OF_SUBSCRIPTIONS,
 *      packetId,
 *      remainingLength,
 *      &fixedBuffer
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The subscribe packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializesubscribe] */
MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                      size_t subscriptionCount,
                                      uint16_t packetId,
                                      size_t remainingLength,
                                      const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializesubscribe] */

/**
 * @brief Get packet size and Remaining Length of an MQTT UNSUBSCRIBE packet.
 *
 * This function must be called before #MQTT_SerializeUnsubscribe in order to
 * get the size of the MQTT UNSUBSCRIBE packet that is generated from the list
 * of #MQTTSubscribeInfo_t. The size of the #MQTTFixedBuffer_t supplied
 * to #MQTT_SerializeUnsubscribe must be at least @p pPacketSize. The provided
 * @p pSubscriptionList is valid for serialization with #MQTT_SerializeUnsubscribe
 * only if this function returns #MQTTSuccess. The remaining length returned in
 * @p pRemainingLength and the packet size returned in @p pPacketSize are valid
 * only if this function returns #MQTTSuccess.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[out] pRemainingLength The Remaining Length of the MQTT UNSUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT UNSUBSCRIBE packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the subscribe info. The details are out of scope for this example.
 * initializeSubscribeInfo( &subscriptionList[ 0 ] );
 *
 * // Get the size requirement for the unsubscribe packet.
 * status = MQTT_GetUnsubscribePacketSize(
 *      &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, &remainingLength, &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the unsubscribe request.
 * }
 * @endcode
 */
/* @[declare_mqtt_getunsubscribepacketsize] */
MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize );
/* @[declare_mqtt_getunsubscribepacketsize] */

/**
 * @brief Serialize an MQTT UNSUBSCRIBE packet in the given buffer.
 *
 * #MQTT_GetUnsubscribePacketSize should be called with @p pSubscriptionList
 * before invoking this function to get the size of the required
 * #MQTTFixedBuffer_t and @p remainingLength. The @p remainingLength must be
 * the same as returned by #MQTT_GetUnsubscribePacketSize. The #MQTTFixedBuffer_t
 * must be at least as large as the size returned by #MQTT_GetUnsubscribePacketSize.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetUnsubscribePacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 * uint16_t packetId;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Function to return a valid, unused packet identifier. The details are out of
 * // scope for this example.
 * packetId = getNewPacketId();
 *
 * // Assume subscriptionList has been initialized. Get the unsubscribe packet size.
 * status = MQTT_GetUnsubscribePacketSize(
 *      &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the unsubscribe packet into the fixed buffer.
 * status = MQTT_SerializeUnsubscribe(
 *      &subscriptionList[ 0 ],
 *      NUMBER_OF_SUBSCRIPTIONS,
 *      packetId,
 *      remainingLength,
 *      &fixedBuffer
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The unsubscribe packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializeunsubscribe] */
MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializeunsubscribe] */

/**
 * @brief Get the packet size and remaining length of an MQTT PUBLISH packet.
 *
 * This function must be called before #MQTT_SerializePublish in order to get
 * the size of the MQTT PUBLISH packet that is generated from #MQTTPublishInfo_t.
 * The size of the #MQTTFixedBuffer_t supplied to #MQTT_SerializePublish must be
 * at least @p pPacketSize. The provided @p pPublishInfo is valid for
 * serialization with #MQTT_SerializePublish only if this function returns
 * #MQTTSuccess. The remaining length returned in @p pRemainingLength and the
 * packet size returned in @p pPacketSize are valid only if this function
 * returns #MQTTSuccess.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT PUBLISH packet.
 * @param[out] pPacketSize The total size of the MQTT PUBLISH packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or if invalid parameters are passed; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo = { 0 };
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the publish info.
 * publishInfo.qos = MQTTQoS0;
 * publishInfo.pTopicName = "/some/topic/name";
 * publishInfo.topicNameLength = strlen( publishInfo.pTopicName );
 * publishInfo.pPayload = "Hello World!";
 * publishInfo.payloadLength = strlen( "Hello World!" );
 *
 * // Get the size requirement for the publish packet.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo, &remainingLength, &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the publish.
 * }
 * @endcode
 */
/* @[declare_mqtt_getpublishpacketsize] */
MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize );
/* @[declare_mqtt_getpublishpacketsize] */

/**
 * @brief Serialize an MQTT PUBLISH packet in the given buffer.
 *
 * This function will serialize complete MQTT PUBLISH packet into
 * the given buffer. If the PUBLISH payload can be sent separately,
 * consider using #MQTT_SerializePublishHeader, which will serialize
 * only the PUBLISH header into the buffer.
 *
 * #MQTT_GetPublishPacketSize or #MQTTV5_GetPublishPacketSize (for version 5 users) should be called with @p pPublishInfo before
 * invoking this function to get the size of the required #MQTTFixedBuffer_t and
 * @p remainingLength. The @p remainingLength must be the same as returned by
 * #MQTT_GetPublishPacketSize. The #MQTTFixedBuffer_t must be at least as large
 * as the size returned by #MQTT_GetPublishPacketSize.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetPublishPacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 * uint16_t packetId;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // A packet identifier is unused for QoS 0 publishes. Otherwise, a valid, unused packet
 * // identifier must be used.
 * packetId = 0;
 *
 * // Assume publishInfo has been initialized. Get publish packet size.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo, &remainingLength, &packetSize
 * );
 *  // or call MQTTV5_GetPublishPacketSize
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the publish packet into the fixed buffer.
 * status = MQTT_SerializePublish(
 *      &publishInfo,
 *      packetId,
 *      remainingLength,
 *      &fixedBuffer
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The publish packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializepublish] */
MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
                                    uint16_t packetId,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializepublish] */

/**
 * @brief Serialize an MQTT PUBLISH packet header without the topic string in the
 * given buffer. This function will add the topic string length to the provided
 * buffer. This helps reduce an unnecessary copy of the topic string into the
 * buffer.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetPublishPacketSize.
 * @param[out] pBuffer Buffer for packet serialization.
 * @param[out] headerSize Size of the serialized MQTT PUBLISH header.
 *
 * @return #MQTTSuccess if the serialization is successful. Otherwise, #MQTTBadParameter.
 */
MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopic( const MQTTPublishInfo_t * pPublishInfo,
                                                      size_t remainingLength,
                                                      uint8_t * pBuffer,
                                                      size_t * headerSize );

/**
 * @brief Serialize an MQTT PUBLISH packet header in the given buffer.
 *
 * This function serializes PUBLISH header in to the given buffer. The payload
 * for PUBLISH will not be copied over to the buffer. This will help reduce
 * the memory needed for the buffer and avoid an unwanted copy operation of the
 * PUBLISH payload into the buffer. If the payload also would need to be part of
 * the serialized buffer, consider using #MQTT_SerializePublish.
 *
 * #MQTT_GetPublishPacketSize should be called with @p pPublishInfo before
 * invoking this function to get the size of the required #MQTTFixedBuffer_t and
 * @p remainingLength. The @p remainingLength must be the same as returned by
 * #MQTT_GetPublishPacketSize. The #MQTTFixedBuffer_t must be at least as large
 * as the size returned by #MQTT_GetPublishPacketSize.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetPublishPacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 * @param[out] pHeaderSize Size of the serialized MQTT PUBLISH header.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0, headerSize = 0;
 * uint16_t packetId;
 * int32_t bytesSent;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // A packet identifier is unused for QoS 0 publishes. Otherwise, a valid, unused packet
 * // identifier must be used.
 * packetId = 0;
 *
 * // Assume publishInfo has been initialized. Get the publish packet size.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo, &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * // The payload will not be serialized, so the the fixed buffer does not need to hold it.
 * assert( ( packetSize - publishInfo.payloadLength ) <= BUFFER_SIZE );
 *
 * // Serialize the publish packet header into the fixed buffer.
 * status = MQTT_SerializePublishHeader(
 *      &publishInfo,
 *      packetId,
 *      remainingLength,
 *      &fixedBuffer,
 *      &headerSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The publish header and payload can now be sent to the broker.
 *      // mqttSocket here is a socket descriptor created and connected to the MQTT
 *      // broker outside of this function.
 *      bytesSent = send( mqttSocket, ( void * ) fixedBuffer.pBuffer, headerSize, 0 );
 *      assert( bytesSent == headerSize );
 *      bytesSent = send( mqttSocket, publishInfo.pPayload, publishInfo.payloadLength, 0 );
 *      assert( bytesSent == publishInfo.payloadLength );
 * }
 * @endcode
 */
/* @[declare_mqtt_serializepublishheader] */
MQTTStatus_t MQTT_SerializePublishHeader( const MQTTPublishInfo_t * pPublishInfo,
                                          uint16_t packetId,
                                          size_t remainingLength,
                                          const MQTTFixedBuffer_t * pFixedBuffer,
                                          size_t * pHeaderSize );
/* @[declare_mqtt_serializepublishheader] */

/**
 * @brief Serialize an MQTT PUBACK, PUBREC, PUBREL, or PUBCOMP into the given
 * buffer.
 *
 * @param[out] pFixedBuffer Buffer for packet serialization.
 * @param[in] packetType Byte of the corresponding packet fixed header per the
 * MQTT spec.
 * @param[in] packetId Packet ID of the publish.
 *
 * @return #MQTTBadParameter, #MQTTNoMemory, or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * uint16_t packetId;
 * uint8_t packetType;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 * // The fixed buffer must be large enough to hold 4 bytes.
 * assert( BUFFER_SIZE >= MQTT_PUBLISH_ACK_PACKET_SIZE );
 *
 * // The packet ID must be the same as the original publish packet.
 * packetId = publishPacketId;
 *
 * // The byte representing a packet of type ACK. This function accepts PUBACK, PUBREC, PUBREL, or PUBCOMP.
 * packetType = MQTT_PACKET_TYPE_PUBACK;
 *
 * // Serialize the publish acknowledgment into the fixed buffer.
 * status = MQTT_SerializeAck( &fixedBuffer, packetType, packetId );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The publish acknowledgment can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializeack] */
MQTTStatus_t MQTT_SerializeAck( const MQTTFixedBuffer_t * pFixedBuffer,
                                uint8_t packetType,
                                uint16_t packetId );
/* @[declare_mqtt_serializeack] */

/**
 * @brief Get the size of an MQTT DISCONNECT packet.
 *
 * @param[out] pPacketSize The size of the MQTT DISCONNECT packet.
 *
 * @return #MQTTSuccess, or #MQTTBadParameter if @p pPacketSize is NULL.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * size_t packetSize = 0;
 *
 * // Get the size requirement for the disconnect packet.
 * status = MQTT_GetDisconnectPacketSize( &packetSize );
 * assert( status == MQTTSuccess );
 * assert( packetSize == 2 );
 *
 * // The application should allocate or use a static #MQTTFixedBuffer_t of
 * // size >= 2 to serialize the disconnect packet.
 *
 * @endcode
 */
/* @[declare_mqtt_getdisconnectpacketsize] */
MQTTStatus_t MQTT_GetDisconnectPacketSize( size_t * pPacketSize );
/* @[declare_mqtt_getdisconnectpacketsize] */

/**
 * @brief Serialize an MQTT DISCONNECT packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTT_GetDisconnectPacketSize.
 *
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Get the disconnect packet size.
 * status = MQTT_GetDisconnectPacketSize( &packetSize );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the disconnect into the fixed buffer.
 * status = MQTT_SerializeDisconnect( &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The disconnect packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializedisconnect] */
MQTTStatus_t MQTT_SerializeDisconnect( const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializedisconnect] */

/**
 * @brief Get the size of an MQTT PINGREQ packet.
 *
 * @param[out] pPacketSize The size of the MQTT PINGREQ packet.
 *
 * @return  #MQTTSuccess or #MQTTBadParameter if pPacketSize is NULL.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * size_t packetSize = 0;
 *
 * // Get the size requirement for the ping request packet.
 * status = MQTT_GetPingreqPacketSize( &packetSize );
 * assert( status == MQTTSuccess );
 * assert( packetSize == 2 );
 *
 * // The application should allocate or use a static #MQTTFixedBuffer_t of
 * // size >= 2 to serialize the ping request.
 *
 * @endcode
 */
/* @[declare_mqtt_getpingreqpacketsize] */
MQTTStatus_t MQTT_GetPingreqPacketSize( size_t * pPacketSize );
/* @[declare_mqtt_getpingreqpacketsize] */

/**
 * @brief Serialize an MQTT PINGREQ packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTT_GetPingreqPacketSize.
 *
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Get the ping request packet size.
 * status = MQTT_GetPingreqPacketSize( &packetSize );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the ping request into the fixed buffer.
 * status = MQTT_SerializePingreq( &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The ping request can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializepingreq] */
MQTTStatus_t MQTT_SerializePingreq( const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializepingreq] */

/**
 * @brief Deserialize an MQTT PUBLISH packet.
 *
 * @param[in] pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pPacketId The packet ID obtained from the buffer.
 * @param[out] pPublishInfo Struct containing information about the publish.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // TransportRecv_t function for reading from the network.
 * int32_t socket_recv(
 *      NetworkContext_t * pNetworkContext,
 *      void * pBuffer,
 *      size_t bytesToRecv
 * );
 * // Some context to be used with the above transport receive function.
 * NetworkContext_t networkContext;
 *
 * // Other variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * MQTTPublishInfo_t publishInfo = { 0 };
 * uint16_t packetId;
 *
 * int32_t bytesRecvd;
 * // A buffer to hold remaining data of the incoming packet.
 * uint8_t buffer[ BUFFER_SIZE ];
 *
 * // Populate all fields of the incoming packet.
 * status = MQTT_GetIncomingPacketTypeAndLength(
 *      socket_recv,
 *      &networkContext,
 *      &incomingPacket
 * );
 * assert( status == MQTTSuccess );
 * assert( incomingPacket.remainingLength <= BUFFER_SIZE );
 * bytesRecvd = socket_recv(
 *      &networkContext,
 *      ( void * ) buffer,
 *      incomingPacket.remainingLength
 * );
 * incomingPacket.pRemainingData = buffer;
 *
 * // Deserialize the publish information if the incoming packet is a publish.
 * if( ( incomingPacket.type & 0xF0 ) == MQTT_PACKET_TYPE_PUBLISH )
 * {
 *      status = MQTT_DeserializePublish( &incomingPacket, &packetId, &publishInfo );
 *      if( status == MQTTSuccess )
 *      {
 *          // The deserialized publish information can now be used from `publishInfo`.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_deserializepublish] */
MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                      uint16_t * pPacketId,
                                      MQTTPublishInfo_t * pPublishInfo );
/* @[declare_mqtt_deserializepublish] */

/**
 * @brief Deserialize an MQTT CONNACK, SUBACK, UNSUBACK, PUBACK, PUBREC, PUBREL,
 * PUBCOMP, or PINGRESP.
 *
 * @param[in] pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pPacketId The packet ID of obtained from the buffer. Not used
 * in CONNACK or PINGRESP.
 * @param[out] pSessionPresent Boolean flag from a CONNACK indicating present session.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTServerRefused, or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * // Used for SUBACK, UNSUBACK, PUBACK, PUBREC, PUBREL, and PUBCOMP.
 * uint16_t packetId;
 * // Used for CONNACK.
 * bool sessionPresent;
 *
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket( &incomingPacket );
 *
 * // Deserialize ack information if the incoming packet is not a publish.
 * if( ( incomingPacket.type & 0xF0 ) != MQTT_PACKET_TYPE_PUBLISH )
 * {
 *      status = MQTT_DeserializeAck( &incomingPacket, &packetId, &sessionPresent );
 *      if( status == MQTTSuccess )
 *      {
 *          // The packet ID or session present flag information is available. For
 *          // ping response packets, the only information is the status code.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_deserializeack] */
MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId,
                                  bool * pSessionPresent );
/* @[declare_mqtt_deserializeack] */

/**
 * @brief Extract the MQTT packet type and length from incoming packet.
 *
 * This function must be called for every incoming packet to retrieve the
 * #MQTTPacketInfo_t.type and #MQTTPacketInfo_t.remainingLength. A
 * #MQTTPacketInfo_t is not valid until this routine has been invoked.
 *
 * @param[in] readFunc Transport layer read function pointer.
 * @param[in] pNetworkContext The network context pointer provided by the application.
 * @param[out] pIncomingPacket Pointer to MQTTPacketInfo_t structure. This is
 * where type, remaining length and packet identifier are stored.
 *
 * @return #MQTTSuccess on successful extraction of type and length,
 * #MQTTBadParameter if @p pIncomingPacket is invalid,
 * #MQTTRecvFailed on transport receive failure,
 * #MQTTBadResponse if an invalid packet is read, and
 * #MQTTNoDataAvailable if there is nothing to read.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // TransportRecv_t function for reading from the network.
 * int32_t socket_recv(
 *      NetworkContext_t * pNetworkContext,
 *      void * pBuffer,
 *      size_t bytesToRecv
 * );
 * // Some context to be used with above transport receive function.
 * NetworkContext_t networkContext;
 *
 * // Struct to hold the incoming packet information.
 * MQTTPacketInfo_t incomingPacket;
 * MQTTStatus_t status = MQTTSuccess;
 * int32_t bytesRecvd;
 * // Buffer to hold the remaining data of the incoming packet.
 * uint8_t buffer[ BUFFER_SIZE ];
 *
 * // Loop until data is available to be received.
 * do{
 *      status = MQTT_GetIncomingPacketTypeAndLength(
 *          socket_recv,
 *          &networkContext,
 *          &incomingPacket
 *      );
 * } while( status == MQTTNoDataAvailable );
 *
 * assert( status == MQTTSuccess );
 *
 * // Receive the rest of the incoming packet.
 * assert( incomingPacket.remainingLength <= BUFFER_SIZE );
 * bytesRecvd = socket_recv(
 *      &networkContext,
 *      ( void * ) buffer,
 *      incomingPacket.remainingLength
 * );
 *
 * // Set the remaining data field.
 * incomingPacket.pRemainingData = buffer;
 * @endcode
 */
/* @[declare_mqtt_getincomingpackettypeandlength] */
MQTTStatus_t MQTT_GetIncomingPacketTypeAndLength( TransportRecv_t readFunc,
                                                  NetworkContext_t * pNetworkContext,
                                                  MQTTPacketInfo_t * pIncomingPacket );
/* @[declare_mqtt_getincomingpackettypeandlength] */

/**
 * @brief Extract the MQTT packet type and length from incoming packet.
 *
 * This function must be called for every incoming packet to retrieve the
 * #MQTTPacketInfo_t.type and #MQTTPacketInfo_t.remainingLength. A
 * #MQTTPacketInfo_t is not valid until this routine has been invoked.
 *
 * @param[in] pBuffer The buffer holding the raw data to be processed
 * @param[in] pIndex Pointer to the index within the buffer to marking the end
 *            of raw data available.
 * @param[out] pIncomingPacket Structure used to hold the fields of the
 *            incoming packet.
 *
 * @return #MQTTSuccess on successful extraction of type and length,
 * #MQTTBadParameter if @p pIncomingPacket is invalid,
 * #MQTTBadResponse if an invalid packet is read, and
 * #MQTTNoDataAvailable if there is nothing to read.
 */
 /* @[declare_mqtt_processincomingpackettypeandlength] */
MQTTStatus_t MQTT_ProcessIncomingPacketTypeAndLength( const uint8_t * pBuffer,
                                                      const size_t * pIndex,
                                                      MQTTPacketInfo_t * pIncomingPacket );
/* @[declare_mqtt_processincomingpackettypeandlength] */

/**
 * @fn uint8_t * MQTT_SerializeConnectFixedHeader( uint8_t * pIndex, const MQTTConnectInfo_t * pConnectInfo, const MQTTPublishInfo_t * pWillInfo, size_t remainingLength );
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

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTT_SerializeConnectFixedHeader( uint8_t * pIndex,
                                            const MQTTConnectInfo_t * pConnectInfo,
                                            const MQTTPublishInfo_t * pWillInfo,
                                            size_t remainingLength );
/** @endcond */

/**
 * @fn  uint8_t * MQTT_SerializeSubscribeHeader( size_t remainingLength, uint8_t * pIndex, uint16_t packetId );
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

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTT_SerializeSubscribeHeader( size_t remainingLength,
                                         uint8_t * pIndex,
                                         uint16_t packetId );
/** @endcond */

/**
 * @fn uint8_t * MQTT_SerializeUnsubscribeHeader( size_t remainingLength, uint8_t * pIndex, uint16_t packetId );
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

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTT_SerializeUnsubscribeHeader( size_t remainingLength,
                                           uint8_t * pIndex,
                                           uint16_t packetId );
/** @endcond */

/**
 * @fn uint8_t * MQTTV5_SerializeConnectProperties(uint8_t * pIndex,const MQTTConnectProperties_t * pConnectProperties);
 * @brief Serialize the connect properties of the connect packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] pConnectProperties The connect properties information.
 *
 * @return A pointer to the end of the encoded string.
 */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTTV5_SerializeConnectProperties( uint8_t * pIndex,
                                           const MQTTConnectProperties_t * pConnectProperties);
/** @endcond */


/**
 * @fn uint8_t * MQTT_SerializePublishProperties(const MQTTPublishInfo_t * pPublishInfo, uint8_t * pIndex);
 * @brief Serialize the will properties of the connect packet.
 * @param[in] pPublishInfo The publish/will properties information.
 * @param[out] pIndex Pointer to the buffer where the header is to
 * be serialized.
 *
 * @return A pointer to the end of the encoded string.
 */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */

uint8_t * MQTT_SerializePublishProperties( const MQTTPublishInfo_t * pPublishInfo,
                                           uint8_t * pIndex);
/** @endcond */


/**
 * @brief Deserialize an MQTT CONNACK packet.
 *
 * @param[out] pConnackProperties To store the deserialized connack properties.
 * @param[in]  pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out]  pSessionPresent Whether a previous session was present.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTSuccess, #MQTTMalformedPacket, #MQTTProtocolError, #MQTTServerRefused
 *
 * <b>Example</b>
 * @code{c}
 *
 * // TransportRecv_t function for reading from the network.
 * int32_t socket_recv(
 *      NetworkContext_t * pNetworkContext,
 *      void * pBuffer,
 *      size_t bytesToRecv
 * );
 * // Some context to be used with the above transport receive function.
 * NetworkContext_t networkContext;
 *
 * // Other variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * MQTTConnectProperties_t properties = {0};
 * uint16_t packetId;
 *
 * int32_t bytesRecvd;
 * // A buffer to hold remaining data of the incoming packet.
 * uint8_t buffer[ BUFFER_SIZE ];
 *
 * // Populate all fields of the incoming packet.
 * status = MQTT_GetIncomingPacketTypeAndLength(
 *      socket_recv,
 *      &networkContext,
 *      &incomingPacket
 * );
 * assert( status == MQTTSuccess );
 * assert( incomingPacket.remainingLength <= BUFFER_SIZE );
 * bytesRecvd = socket_recv(
 *      &networkContext,
 *      ( void * ) buffer,
 *      incomingPacket.remainingLength
 * );
 * incomingPacket.pRemainingData = buffer;
 *
 * // Deserialize the publish information if the incoming packet is a publish.
 * if( ( incomingPacket.type & 0xF0 ) == MQTT_PACKET_TYPE_CONNACK )
 * {
 *      status = MQTT_DeserializeConnack(&properties, &incomingPacket, &session);
 *      if( status == MQTTSuccess )
 *      {
 *          // The deserialized connack information can now be used from `properties`.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqttv5_deserializeconnack] */
MQTTStatus_t MQTTV5_DeserializeConnack( MQTTConnectProperties_t *pConnackProperties,
                                        const MQTTPacketInfo_t * pIncomingPacket,
                                        bool * pSessionPresent );
/* @[declare_mqttv5_deserializeconnack] */

/**
 * @brief Initialize an MQTTConnectProperties_t.
 *
 * This function can be called on an #MQTTConnectProperties_t to set the properties to their default value before calling MQTT_Connect or #MQTTV5_GetConnectPacketSize.
 *
 * @param[in] pConnectProperties The connect properties to initialize.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * MQTTConnectProperties_t connectProperties;
 *
 * // Clear context.
 * memset( ( void * ) &connectProperties, 0x00, sizeof( MQTTConnectProperties_t ) );
 *
 * status = MQTTV5_InitConnect(&connectProperties);
 *
 * if( status == MQTTSuccess )
 * {
 * //Set the values for connect properties.
 * }
 * @endcode
 */
/* @[declare_mqttv5_initconnect] */
MQTTStatus_t MQTTV5_InitConnect(MQTTConnectProperties_t *pConnectProperties);
/* @[declare_mqttv5_initconnect] */

/**
 * @brief Get the size and Remaining Length of an MQTT Version 5 CONNECT packet .
 *
 * This function must be called before #MQTTV5_SerializeConnect in order to get
 * the size of the MQTT CONNECT packet that is generated from #MQTTConnectInfo_t,
 * MQTTConnectProperties_t and  optional #MQTTPublishInfo_t. The size of the #MQTTFixedBuffer_t supplied
 * to #MQTT_SerializeConnect must be at least @p pPacketSize. The provided
 * @p pConnectInfo, @p pConnectProperties and  @p pWillInfo are valid for serialization with
 * #MQTT_SerializeConnect only if this function returns #MQTTSuccess. The
 * remaining length returned in @p pRemainingLength and the packet size returned
 * in @p pPacketSize are valid only if this function returns #MQTTSuccess.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT properties parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT CONNECT packet.
 * @param[out] pPacketSize The total size of the MQTT CONNECT packet.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTConnectInfo_t connectInfo = { 0 };
 * MQTTPublishInfo_t willInfo = { 0 };
 * MQTTConnectProperties_t connectProperties ={0};
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the connection info, the details are out of scope for this example.
 * initializeConnectInfo( &connectInfo );
 *
 * // Initialize the optional will info, the details are out of scope for this example.
 * initializeWillInfo( &willInfo );
 *
 * // Initialize the connection properties, the details are out of scope for this example.
 * initializeConnectProperties( &connectProperties );
 *
 * // Get the size requirement for the connect packet.
 * status = MQTTV5_GetConnectPacketSize(
 *      &connectInfo, &willInfo, &connectProperties, &remainingLength, &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the connect request.
 * }
 * @endcode
 */
/* @[declare_mqttv5_getconnectpacketsize] */
MQTTStatus_t MQTTV5_GetConnectPacketSize(const MQTTConnectInfo_t* pConnectInfo,
    MQTTPublishInfo_t* pWillInfo,
    MQTTConnectProperties_t* pConnectProperties,
    size_t* pRemainingLength,
    size_t* pPacketSize);
/* @[declare_mqttv5_getconnectpacketsize] */

/**
 * @brief Serialize an MQTT CONNECT packet in the given fixed buffer @p pFixedBuffer.
 *
 * #MQTTV5_GetConnectPacketSize should be called with @p pConnectInfo, @p pConnectProperties and
 * @p pWillInfo before invoking this function to get the size of the required
 * #MQTTFixedBuffer_t and @p remainingLength. The @p remainingLength must be
 * the same as returned by #MQTTV5_GetConnectPacketSize. The #MQTTFixedBuffer_t
 * must be at least as large as the size returned by #MQTTV5_GetConnectPacketSize.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT properties parameters.
 * @param[in] remainingLength Remaining Length provided by #MQTTV5_GetConnectPacketSize.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTConnectInfo_t connectInfo = { 0 };
 * MQTTPublishInfo_t willInfo = { 0 };
 * MQTTConnectProperties_t connectProperties ={0};
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Assume connectInfo and willInfo are initialized. Get the size requirement for
 * // the connect packet.
 * status = MQTTV5_GetConnectPacketSize(
 *      &connectInfo, &willInfo,&connectProperties &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the connect packet into the fixed buffer.
 * status = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&connectProperties,remainingLength, &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The connect packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqttv5_serializeconnect] */
MQTTStatus_t MQTTV5_SerializeConnect(const MQTTConnectInfo_t* pConnectInfo,
    const MQTTPublishInfo_t* pWillInfo,
    const MQTTConnectProperties_t *pConnectProperties,
    size_t remainingLength,
    const MQTTFixedBuffer_t* pFixedBuffer);
/* @[declare_mqttv5_serializeconnect] */


/**
 * @brief Validate the publish parameters present in the given publish structure @p pPublishInfo.
 *
 * This function must be called before #MQTTV5_GetPublishPacketSize in order to validate the publish parameters.
 *
 * @param[in] pPublishInfo MQTT publish packet parameters.
 * @param[in] topicAliasMax Maximum topic alias allowed by the server.
 * @param[in] retainAvailable Whether server allows retain or not.
 * @param[in]  maxQos Maximum QoS supported by the server.
 *
 * @return  #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo = {0};
 * uint16_t topicAliasMax;
 * uint8_t retainAvailable;
 * uint8_t maxQos;
 *
 * //Set the publish info parameters.
 *
 * //Validate the publish packet
 * status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retainAvailable,maxQos);
 *
 * if( status == MQTTSuccess )
 * {
 *      // Get the packet size and serialize the publish packet.
 * }
 * @endcode
 */
/* @[declare_mqttv5_validatepublishparams] */



MQTTStatus_t MQTTV5_ValidatePublishParams(const MQTTPublishInfo_t* pPublishInfo, uint16_t topicAliasMax, uint8_t retainAvailable, uint8_t maxQos);
/* @[declare_mqttv5_validatepublishparams] */


/**
 * @brief Get the packet size and remaining length of an MQTT PUBLISH packet.
 *
 * #MQTTV5_ValidatePublishParams should be called with @p pPublishInfo before invoking this function
 * to validate the publish parameters.This function must be called before #MQTT_SerializePublish
 * in order to get the size of the MQTT PUBLISH packet that is generated from #MQTTPublishInfo_t.
 * The size of the #MQTTFixedBuffer_t supplied to #MQTT_SerializePublish must be
 * at least @p pPacketSize. The provided @p pPublishInfo is valid for
 * serialization with #MQTT_SerializePublish only if this function returns
 * #MQTTSuccess. The remaining length returned in @p pRemainingLength and the
 * packet size returned in @p pPacketSize are valid only if this function
 * returns #MQTTSuccess.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT PUBLISH packet.
 * @param[out] pPacketSize The total size of the MQTT PUBLISH packet.
 * @param[in] maxPacketSize Maximum packet size allowed by the server.
 *
 * @return #MQTTBadParameter if the packet would exceed the size allowed by the
 * MQTT spec or if invalid parameters are passed; #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo = { 0 };
 * uint16_t topicAliasMax;
 * uint8_t retainAvailable;
 * uint8_t maxQos;
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the publish info.
 * publishInfo.qos = MQTTQoS0;
 * publishInfo.pTopicName = "/some/topic/name";
 * publishInfo.topicNameLength = strlen( publishInfo.pTopicName );
 * publishInfo.pPayload = "Hello World!";
 * publishInfo.payloadLength = strlen( "Hello World!" );
 *
 * //Set the publish properties.
 *
 * status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retainAvailable,maxQos);

 * // Get the size requirement for the publish packet.
 * status = MQTTV5_GetPublishPacketSize(
 *      &publishInfo, &remainingLength, &packetSize, maxPacketSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the publish.
 * }
 * @endcode
 */
/* @[declare_mqttv5_getpublishpacketsize] */
MQTTStatus_t MQTTV5_GetPublishPacketSize(MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize ,
                                        uint32_t maxPacketSize);
/* @[declare_mqttv5_getpublishpacketsize] */


/**
 * @brief Deserialize an MQTT PUBACK, PUBREC, PUBREL, PUBCOMP, or PINGRESP.
 *
 * @param[in] pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pPacketId The packet ID of obtained from the buffer.
 * @param[out] pAckInfo Struct to store the deserialized ack information.
 * @param[in] requestProblem Request problem value set in the connect packet.
 * @param[in]  maxPacketSize Maximum packet size allowed by the client.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTServerRefused, #MQTTProtocolError, #MQTTMalformedPacket  or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * uint16_t packetId;
 * MQTTAckInfo_t ackInfo = {0};
 * bool requestProblem;
 * uint32_t maxPacketSize;
 * bool sessionPresent;
 *
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket( &incomingPacket );
 *
 * // Deserialize ack information if the incoming packet is a publish ack.
 *    status = MQTTV5_DeserializeAck(&incomingPacket,
                                            &packetId,
                                            &ackInfo,
                                            requestProblem,
                                            maxPacketSize);
 *      if( status == MQTTSuccess )
 *      {
 *       // Ack information is now available.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqttv5_deserializeack] */
MQTTStatus_t MQTTV5_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                  uint16_t * pPacketId, MQTTAckInfo_t *pAckInfo, bool requestProblem,uint32_t maxPacketSize);
/* @[declare_mqttv5_deserializeack] */

/**
 * @brief Get the size of an MQTT DISCONNECT packet.
 *
 * @param[in] pAckInfo MQTT PUBLISH ACK packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT DISCONNECT packet.
 * @param[out] pPacketSize The size of the MQTT DISCONNECT packet.
 * @param[in] maxPacketSize Maximum packet size allowed by the server.
 *
 * @return #MQTTSuccess, or #MQTTBadParameter if parameters are invalid
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTAckInfo_t disconnectInfo ={0};
 * size_t remainingLength =0;
 * size_t packetSize = 0;
 * uint32_t maxPacketSize;
 *
 * //Set the parameters.
 * // Get the size requirement for the disconnect packet.
 * status = MQTTV5_GetAckPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize);
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the ack packet.
 * }
 * @endcode
 */
/* @[declare_mqttv5_getackpacketsize] */
MQTTStatus_t MQTTV5_GetAckPacketSize( MQTTAckInfo_t *pAckInfo, size_t* pRemainingLength,size_t * pPacketSize, uint32_t maxPacketSize);
/* @[declare_mqttv5_getackpacketsize] */

/**
 * @fn uint8_t * MQTTV5_SerializeAckFixed(uint8_t * pIndex,
                                uint8_t packetType,
                                uint16_t packetId,
                                size_t remainingLength,
                                size_t propertyLength);
 * @brief Serialize the fixed size part of the ack packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the header is to
 * be serialized.
 * @param[in] packetType Type of publish ack
 * @param[in] packetId Packed identifier of the ack packet.
 * @param[in] remainingLength Remaining length of the ack packet.
 * @param[in] propertyLength Property length of the ack packet.
 *
 *
 * @return A pointer to the end of the encoded string.
 */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTTV5_SerializeAckFixed(uint8_t * pIndex,
                                uint8_t packetType,
                                uint16_t packetId,
                                size_t remainingLength,
                                size_t propertyLength);
/** @endcond */

/**
 * @brief Serialize an MQTT DISCONNECT packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTTV5_GetAckPacketSize.
 *
 * @note If reason code is success and property length is zero then #MQTT_SerializeAck can also be used.
 *
 * @param[in]  pAckInfo Struct containing information about the publish ack.
 * @param[in]  remainingLength The remaining length of the packet to be
 * serialized.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 * @param[in] packetType Type of publish ack.
 * @param[in] packetId Packed identifier of the ack packet.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * MQTTAckInfo_t  ackInfo;
 * uint16_t sessionExpiry;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTAckInfo_t disconnectInfo ={0};
 * size_t remainingLength =0;
 * size_t packetSize = 0;
 * uint8_t packetType;
 * uint8_t packedId;
 * uint32_t maxPacketSize;
 * //set the parameters.
 * // Get the size requirement for the ack packet.
 * status = MQTTV5_GetAckPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize);
 *
 *
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the disconnect into the fixed buffer.
 * MQTTV5_SerializePubAckWithProperty( &ackInfo,
                                    remainingLength,
                                    &fixedBuffer,
                                    packetType,
                                    packetId);
 * * if( status == MQTTSuccess )
 * {
 *      // The ack packet can now be sent to the broker.
 * }
 *
 * @endcode
 */
/* @[declare_mqttv5_serializepubackwithproperty] */
MQTTStatus_t MQTTV5_SerializePubAckWithProperty( const MQTTAckInfo_t *pAckInfo,
                                                 size_t remainingLength,
                                                 const MQTTFixedBuffer_t * pFixedBuffer,
                                                 uint8_t packetType,
                                                 uint16_t packetId);
/* @[declare_mqttv5_serializepubackwithproperty] */


/**
 * @brief Get the size of an MQTT DISCONNECT packet.
 *
 * @param[in] pDisconnectInfo MQTT DISCONNECT packet parameters.
 * @param[out] pRemainingLength The Remaining Length of the MQTT DISCONNECT packet.
 * @param[out] pPacketSize The size of the MQTT DISCONNECT packet.
 * @param[in] maxPacketSize Maximum packet size allowed by the server.
 * @param[in] sessionExpiry Session expiry interval in the disconnect packet.
 * @param[in] prevSessionExpiry Session expiry interval set in the connect packet.
 *
 * @return #MQTTSuccess, or #MQTTBadParameter if parameters are invalid
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTAckInfo_t disconnectInfo ={0};
 * size_t remainingLength =0;
 * size_t packetSize = 0;
 * uint32_t maxPacketSize;
 * uint32_t sessionExpiry;
 * uint32_t prevSessionExpiry
 *
 * //Set the parameters.
 * // Get the size requirement for the disconnect packet.
 * status = MQTTV5_GetDisconnectPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize, sessionExpiry,prevSessionExpiry );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the disconnect.
 * }
 * @endcode
 */
/* @[declare_mqttv5_getdisconnectpacketsize] */
MQTTStatus_t MQTTV5_GetDisconnectPacketSize( MQTTAckInfo_t* pDisconnectInfo, size_t * pRemainingLength, size_t * pPacketSize,uint32_t maxPacketSize, uint32_t sessionExpiry,uint32_t prevSessionExpiry );
/* @[declare_mqttv5_getdisconnectpacketsize] */


/**
 * @fn uint8_t * MQTTV5_SerializeDisconnectFixed(uint8_t * pIndex,
                                        const MQTTAckInfo_t * pDisconnectInfo,
                                        size_t remainingLength,
                                        uint32_t sessionExpiry);
 * @brief Serialize the fixed part of the disconnect packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the fixed size parameters is to
 * be serialized.
 * @param[in] pDisconnectInfo The disconnect information.
 * @param[in] remainingLength The remaining length of the packet to be
 * serialized.
 * @param[in] sessionExpiry Session expiry to be serialized.
 *
 * @return A pointer to the end of the encoded string.
 */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t * MQTTV5_SerializeDisconnectFixed(uint8_t * pIndex,
                                        const MQTTAckInfo_t * pDisconnectInfo,
                                        size_t remainingLength,
                                        uint32_t sessionExpiry);
/** @endcond */

/**
 * @brief Deserialize an MQTT Disconnect packet.
 *
 * @param[in] pPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pDisconnectInfo Struct containing information about the disconnect.
 * @param[out] pServerRef The server reference obtained from the buffer.
 * @param[out] pServerRefLength The server reference length obtained from the buffer.
 * @param[in] maxPacketSize MAximum packet size allowed by the client.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTServerRefused, #MQTTProtocolError, #MQTTMalformedPacket or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * MQTTAckInfo_t disconnectInfo;
 * const char * serverRef;
 * uint16_t serverRefLength;
 * uint32_t maxPacketSize;
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket( &incomingPacket );
 *
 * // Deserialize disconnect information.
 * if( ( incomingPacket.type) == MQTT_PACKET_TYPE_DISCONNECT )
 * {
 *      status = MQTTV5_DeserializeDisconnect(&incomingPacket,
                                             &disconnectInfo,
                                             &serverRef,
                                             &serverRefLength,
                                            maxPacketSize );
 *      if( status == MQTTSuccess )
 *      {
 *          // Disconnect information is available.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqttv5_deserializedisconnect] */
 MQTTStatus_t MQTTV5_DeserializeDisconnect( const MQTTPacketInfo_t * pPacket,
                                            MQTTAckInfo_t *pDisconnectInfo,
                                            const char ** pServerRef,
                                            uint16_t * pServerRefLength,
                                            uint32_t maxPacketSize );
/* @[declare_mqttv5_deserializedisconnect] */


/**
 * @brief Serialize an MQTT DISCONNECT packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTTV5_GetDisconnectPacketSize.
 *
 * @param[in]  pDisconnectInfo Struct containing information about the disconnect.
 * @param[in]  remainingLength The remaining length of the packet to be
 * serialized.
 * @param[out] pFixedBuffer Buffer for packet serialization.
 * @param[in]  sessionExpiry Session expiry to be serialized.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if invalid parameters are passed;
 * #MQTTSuccess otherwise.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * MQTTAckInfo_t disconnectInfo;
 * uint16_t sessionExpiry;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTAckInfo_t disconnectInfo ={0};
 * size_t remainingLength =0;
 * size_t packetSize = 0;
 * uint32_t maxPacketSize;
 * uint32_t sessionExpiry;
 * uint32_t prevSessionExpiry
 *
 * //set the parameters.
 * // Get the size requirement for the disconnect packet.
 * status = MQTTV5_GetDisconnectPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize, sessionExpiry,prevSessionExpiry );
 *
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the disconnect into the fixed buffer.
 * MQTTV5_SerializeDisconnectWithProperty( &disconnectInfo,
                                           remainingLength,
                                           &fixedBuffer,
                                           sessionExpiry);
 *
 * if( status == MQTTSuccess )
 * {
 *      // The disconnect packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqttv5_serializedisconnectwithproperty] */
MQTTStatus_t MQTTV5_SerializeDisconnectWithProperty( const MQTTAckInfo_t *pDisconnectInfo,
                                                     size_t remainingLength,
                                                     const MQTTFixedBuffer_t * pFixedBuffer,
                                                     uint32_t sessionExpiry);
/* @[declare_mqttv5_serializedisconnectwithproperty] */

static MQTTStatus_t MQTT_GetSubscribePropertiesSize(MQTTSubscribeProperties_t* pSubscribeProperties); 

static MQTTStatus_t calculateSubscriptionPacketSizeV5(MQTTSubscribeInfo_t* pSubscriptionList,
    MQTTSubscribeProperties_t* pSubscribeProperties,
    size_t subscriptionCount,
    size_t* pRemainingLength,
    size_t* pPacketSize);




/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_SERIALIZER_H */
