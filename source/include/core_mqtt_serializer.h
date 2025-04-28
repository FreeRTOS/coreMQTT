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
struct MQTTAuthInfo;
/**
 * @ingroup mqtt_enum_types
 * @brief Return codes from MQTT functions.
 */
typedef enum MQTTStatus
{
    MQTTSuccess = 0,                /**< Function completed successfully. */
    MQTTBadParameter,               /**< At least one parameter was invalid. */
    MQTTNoMemory,                   /**< A provided buffer was too small. */
    MQTTSendFailed,                 /**< The transport send function failed. */
    MQTTRecvFailed,                 /**< The transport receive function failed. */
    MQTTBadResponse,                /**< An invalid packet was received from the server. */
    MQTTServerRefused,              /**< The server refused a CONNECT or SUBSCRIBE. */
    MQTTNoDataAvailable,            /**< No data available from the transport interface. */
    MQTTIllegalState,               /**< An illegal state in the state record. */
    MQTTStateCollision,             /**< A collision with an existing state record entry. */
    MQTTKeepAliveTimeout,           /**< Timeout while waiting for PINGRESP. */
    MQTTNeedMoreBytes,              /**< MQTT_ProcessLoop/MQTT_ReceiveLoop has received
                                    incomplete data; it should be called again (probably after
                                    a delay). */
    MQTTEndOfProperties,

    MQTTStatusConnected,            /**< MQTT connection is established with the broker. */
    MQTTStatusNotConnected,         /**< MQTT connection is not established with the broker. */
    MQTTStatusDisconnectPending,    /**< Transport Interface has failed and MQTT connection needs to be closed. */
    MQTTPublishStoreFailed,         /**< User provided API to store a copy of outgoing publish for retransmission  purposes,
                                    has failed. */
    MQTTPublishRetrieveFailed       /**< User provided API to retrieve the copy of a publish while reconnecting
                                    with an unclean session has failed. */
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

/**
 * @ingroup mqtt_enum_types
 * @brief MQTT reason codes.
 *
 * These values are defined in the MQTT 5.0 specification.
 */
typedef enum MQTTSuccessFailReasonCode
{
    /* PUBACK reason codes*/
    MQTT_REASON_PUBACK_SUCCESS = 0x00,
    MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS = 0x10,
    MQTT_REASON_PUBACK_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_PUBACK_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_PUBACK_TOPIC_NAME_INVALID = 0x90,
    MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE = 0x91,
    MQTT_REASON_PUBACK_QUOTA_EXCEEDED = 0x97,
    MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID = 0x99,

    /*PUBREC reason codes*/
    MQTT_REASON_PUBREC_SUCCESS = 0x00,
    MQTT_REASON_PUBREC_NO_MATCHING_SUBSCRIBERS = 0x10,
    MQTT_REASON_PUBREC_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_PUBREC_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_PUBREC_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_PUBREC_TOPIC_NAME_INVALID = 0x90,
    MQTT_REASON_PUBREC_PACKET_IDENTIFIER_IN_USE = 0x91,
    MQTT_REASON_PUBREC_QUOTA_EXCEEDED = 0x97,
    MQTT_REASON_PUBREC_PAYLOAD_FORMAT_INVALID = 0x99,

    /*PUBREL reason codes*/
    MQTT_REASON_PUBREL_SUCCESS = 0x00,
    MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND = 0x92,

    /*PUBCOMP reason codes*/
    MQTT_REASON_PUBCOMP_SUCCESS = 0x00,
    MQTT_REASON_PUBCOMP_PACKET_IDENTIFIER_NOT_FOUND = 0x92,

    /* CONNACK reason codes */
    MQTT_REASON_CONNACK_SUCCESS = 0x00,
    MQTT_REASON_CONNACK_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_CONNACK_MALFORMED_PACKET = 0x81,
    MQTT_REASON_CONNACK_PROTOCOL_ERROR = 0x82,
    MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION = 0x84,
    MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID = 0x85,
    MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD = 0x86,
    MQTT_REASON_CONNACK_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_CONNACK_SERVER_UNAVAILABLE = 0x88,
    MQTT_REASON_CONNACK_SERVER_BUSY = 0x89,
    MQTT_REASON_CONNACK_BANNED = 0x8A,
    MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD = 0x8C,
    MQTT_REASON_CONNACK_TOPIC_NAME_INVALID = 0x90,
    MQTT_REASON_CONNACK_PACKET_TOO_LARGE = 0x95,
    MQTT_REASON_CONNACK_QUOTA_EXCEEDED = 0x97,
    MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID = 0x99,
    MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED = 0x9A,
    MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED = 0x9B,
    MQTT_REASON_CONNACK_USE_ANOTHER_SERVER = 0x9C,
    MQTT_REASON_CONNACK_SERVER_MOVED = 0x9D,
    MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED = 0x9F,


    /*SUBACK reason codes*/
    MQTT_REASON_SUBACK_GRANTED_QOS0 = 0x00,
    MQTT_REASON_SUBACK_GRANTED_QOS1 = 0x01,
    MQTT_REASON_SUBACK_GRANTED_QOS2 = 0x02,
    MQTT_REASON_SUBACK_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_SUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_SUBACK_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_SUBACK_TOPIC_FILTER_INVALID = 0x8F,
    MQTT_REASON_SUBACK_PACKET_IDENTIFIER_IN_USE = 0x91,
    MQTT_REASON_SUBACK_QUOTA_EXCEEDED = 0x97,
    MQTT_REASON_SUBACK_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9E,
    MQTT_REASON_SUBACK_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 0xA1,
    MQTT_REASON_SUBACK_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xA2,

    /*UNSUBACK reason codes*/

    MQTT_REASON_UNSUBACK_SUCCESS = 0x00,
    MQTT_REASON_UNSUBACK_NO_SUBSCRIPTION_EXISTED = 0x11,
    MQTT_REASON_UNSUBACK_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_UNSUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_UNSUBACK_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_UNSUBACK_TOPIC_FILTER_INVALID = 0x8F,
    MQTT_REASON_UNSUBACK_PACKET_IDENTIFIER_IN_USE = 0x91,

    /*DISCONNECT reason codes*/
    MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION = 0x00,
    MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE = 0x04,
    MQTT_REASON_DISCONNECT_UNSPECIFIED_ERROR = 0x80,
    MQTT_REASON_DISCONNECT_MALFORMED_PACKET = 0x81,
    MQTT_REASON_DISCONNECT_PROTOCOL_ERROR = 0x82,
    MQTT_REASON_DISCONNECT_IMPLEMENTATION_SPECIFIC_ERROR = 0x83,
    MQTT_REASON_DISCONNECT_NOT_AUTHORIZED = 0x87,
    MQTT_REASON_DISCONNECT_SERVER_BUSY = 0x89,
    MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN = 0x8B,
    MQTT_REASON_DISCONNECT_BAD_AUTHENTICATION_METHOD = 0x8C,
    MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT = 0x8D,
    MQTT_REASON_DISCONNECT_SESSION_TAKEN_OVER = 0x8E,
    MQTT_REASON_DISCONNECT_TOPIC_FILTER_INVALID = 0x8F,
    MQTT_REASON_DISCONNECT_TOPIC_NAME_INVALID = 0x90,
    MQTT_REASON_DISCONNECT_RECEIVE_MAXIMUM_EXCEEDED = 0x93,
    MQTT_REASON_DISCONNECT_TOPIC_ALIAS_INVALID = 0x94,
    MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE = 0x95,
    MQTT_REASON_DISCONNECT_MESSAGE_RATE_TOO_HIGH = 0x96,
    MQTT_REASON_DISCONNECT_QUOTA_EXCEEDED = 0x97,
    MQTT_REASON_DISCONNECT_ADMINISTRATIVE_ACTION = 0x98,
    MQTT_REASON_DISCONNECT_PAYLOAD_FORMAT_INVALID = 0x99,
    MQTT_REASON_DISCONNECT_RETAIN_NOT_SUPPORTED = 0x9A,
    MQTT_REASON_DISCONNECT_QOS_NOT_SUPPORTED = 0x9B,
    MQTT_REASON_DISCONNECT_USE_ANOTHER_SERVER = 0x9C,
    MQTT_REASON_DISCONNECT_SERVER_MOVED = 0x9D,
    MQTT_REASON_DISCONNECT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9E,
    MQTT_REASON_DISCONNECT_CONNECTION_RATE_EXCEEDED = 0x9F,
    MQTT_REASON_DISCONNECT_MAXIMUM_CONNECT_TIME = 0xA0,
    MQTT_REASON_DISCONNECT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 0xA1,
    MQTT_REASON_DISCONNECT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xA2

}MQTTSuccessFailReasonCode_t;


#define MQTT_SUBSCRIBE_QOS1                    ( 0 ) /**< @brief MQTT SUBSCRIBE QoS1 flag. */
#define MQTT_SUBSCRIBE_QOS2                    ( 1 ) /**< @brief MQTT SUBSCRIBE QoS2 flag. */
#define MQTT_SUBSCRIBE_NO_LOCAL                ( 2 ) /**< @brief MQTT SUBSCRIBE no local flag. */
#define MQTT_SUBSCRIBE_RETAIN_AS_PUBLISHED     ( 3 ) /**< @brief MQTT SUBSCRIBE retain as published flag. */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING1        ( 4 ) /**<@brief MQTT SUBSCRIBE Retain Handling Option 1 */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING2        ( 5 ) /**<@brief Retain Handling Option 2   -> in core_mqtt_serializer.c */

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
* @brief Subscription ID id
*/
#define MQTT_SUBSCRIPTION_ID_ID          ( 0x0B )


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


/**
 * @brief MQTT Subscription packet types.
 */
typedef enum MQTTSubscriptionType
{
    MQTT_TYPE_SUBSCRIBE,  /**< @brief The type is a SUBSCRIBE packet. */
    MQTT_TYPE_UNSUBSCRIBE /**< @brief The type is a UNSUBSCRIBE packet. */
} MQTTSubscriptionType_t;


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
    uint16_t topicFilterLength;
    /**
     * @brief no local option for subscription. Include protocol error if noLocalOption = 1 in a shared subscription
     */

        bool noLocalOption;
        bool retainAsPublishedOption;
        retainHandling_t retainHandlingOption;

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

    uint8_t reasonCode ;
} MQTTAuthInfo_t;


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
     * @brief Maximum number of unacknowledged PUBLISH packets server is willing to receive.
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
     * @brief Whether wildcard subscription is available.
     */
    uint8_t isWildcardAvaiable;

     /**
     * @brief Whether the Server supports Subscription Identifiers.
     */
    uint8_t isSubscriptionIdAvailable;

     /**
     * @brief Whether the Server supports Shared Subscription.
     */
    uint8_t isSharedAvailable;

     /**
     * @brief Keep Alive value given by the server.
     */
    uint16_t serverKeepAlive;


} MQTTConnectProperties_t;

 /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold reason codes.
 */
typedef struct MQTTReasonCodeInfo
{
    const uint8_t * reasonCode;
    size_t reasonCodeLength ;

} MQTTReasonCodeInfo_t;

/**
 * @brief The generic structure used to build a 'property' section of the packet
 * into.
 * @member pBuffer pointer to the user provided buffer.
 * @member bufferLength length of the user provided buffer.
 * @member currentIndex Index of the location which is yet to be written to in the
 * buffer.
 * @member fieldsSet This is a bitmask to track which fields have already been set.
 * This allows the library to throw an error in case certain fields are included more
 * than once as it is a protocol error to include certain fields more than once.
 */
typedef struct MqttPropBuilder
{
    uint8_t* pBuffer;
    size_t bufferLength;
    size_t currentIndex;
    uint32_t fieldSet;
} MqttPropBuilder_t;

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

    size_t subscriptionId ;

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


MQTTStatus_t MQTT_GetSubscribePacketSize(const MQTTSubscribeInfo_t* pSubscriptionList,
    size_t subscriptionCount,
    size_t* pRemainingLength,
    size_t* pPacketSize,
    size_t subscribePropLen,
    uint32_t maxPacketSize);


/* @[declare_mqtt_getsubscribepacketsize] */

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


/* @[declare_mqtt_getunsubscribepacketsize] */

MQTTStatus_t MQTT_GetUnsubscribePacketSize(const MQTTSubscribeInfo_t* pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t* pRemainingLength,
                                            size_t* pPacketSize,
                                            uint32_t maxPacketSize,
                                            size_t propLen);


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

MQTTStatus_t MQTT_DeserializePublish(const MQTTPacketInfo_t* pIncomingPacket,
    uint16_t* pPacketId,
    MQTTPublishInfo_t* pPublishInfo,
    MqttPropBuilder_t* propBuffer,
    uint32_t maxPacketSize);
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
MQTTStatus_t MQTT_DeserializePing( const MQTTPacketInfo_t * pIncomingPacket,
                                   const uint16_t * pPacketId,
                                   const bool * pSessionPresent );
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
 * @brief Update the duplicate publish flag within the given header of the publish packet.
 *
 * @param[in] pHeader The buffer holding the header content
 * @param[in] set If true then the flag will be set else cleared
 *
 * @return #MQTTSuccess on successful setting of the duplicate flag,
 * #MQTTBadParameter for invalid parameters
 */
 /* @[declare_mqtt_updateduplicatepublishflag] */
MQTTStatus_t MQTT_UpdateDuplicatePublishFlag( uint8_t * pHeader , bool set);
/* @[declare_mqtt_updateduplicatepublishflag] */

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
 * @brief Deserialize an MQTT CONNACK packet.
 *
 * @param[out] pConnackProperties To store the deserialized connack properties.
 * @param[in]  pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out]  pSessionPresent Whether a previous session was present.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTSuccess, #MQTTBadResponse, #MQTTBadResponse, #MQTTServerRefused
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
MQTTStatus_t MQTT_DeserializeConnack(MQTTConnectProperties_t* pConnackProperties,
                                        const MQTTPacketInfo_t* pIncomingPacket,
                                        bool* pSessionPresent,
                                        MqttPropBuilder_t* propBuffer);
/* @[declare_mqttv5_deserializeconnack] */


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
MQTTStatus_t MQTT_GetConnectPacketSize(const MQTTConnectInfo_t* pConnectInfo,
    const MQTTPublishInfo_t* pWillInfo,
    size_t propLen,
    size_t willPropLen,
    size_t* pRemainingLength,
    size_t* pPacketSize);
/* @[declare_mqttv5_getconnectpacketsize] */


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
MQTTStatus_t MQTT_ValidatePublishParams(const MQTTPublishInfo_t* pPublishInfo, uint8_t retainAvailable, uint8_t maxQos,uint16_t topicAlias,  uint32_t maxPacketSize);
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
MQTTStatus_t MQTT_GetPublishPacketSize(const MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize ,
                                        uint32_t maxPacketSize,
                                        size_t publishPropertyLength);
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
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTServerRefused, #MQTTBadResponse, #MQTTBadResponse  or #MQTTSuccess.
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
 *    status = MQTT_DeserializePublishAck(&incomingPacket,
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
/* @[declare_MQTT_DeserializePublishAck] */
MQTTStatus_t MQTT_DeserializePublishAck(const MQTTPacketInfo_t* pIncomingPacket,
                                    uint16_t* pPacketId,
                                    MQTTReasonCodeInfo_t* pReasonCode,
                                    bool requestProblem,
                                    uint32_t maxPacketSize,
                                    MqttPropBuilder_t* propBuffer);

/* @[declare_MQTT_DeserializePublishAck] */
MQTTStatus_t MQTT_DeserializeSuback(MQTTReasonCodeInfo_t* subackReasonCodes,
                                        const MQTTPacketInfo_t* pSuback,
                                        uint16_t* pPacketId,
                                        MqttPropBuilder_t* propBuffer,
                                        uint32_t maxPacketSize);
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
 * status = MQTT_GetAckPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize);
 *
 * if( status == MQTTSuccess )
 * {
 *      // The application should allocate or use a static #MQTTFixedBuffer_t
 *      // of size >= packetSize to serialize the ack packet.
 * }
 * @endcode
 */
/* @[declare_MQTT_GetAckPacketSize] */
// MQTTStatus_t MQTT_GetAckPacketSize(MQTTContext_t* pContext,
//    MQTTAckInfo_t* pAckInfo,
//    size_t* pRemainingLength,
//    size_t* pPacketSize,
//    uint32_t maxPacketSize);
// /* @[declare_MQTT_GetAckPacketSize] */

/**
 * @fn uint8_t * MQTT_SerializeAckFixed(uint8_t * pIndex,
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
 *
 *
 * @return A pointer to the end of the encoded string.
 */

/**
 * @cond DOXYGEN_IGNORE
 * Doxygen should ignore this definition, this function is private.
 */
uint8_t* MQTT_SerializeAckFixed(uint8_t* pIndex,
    uint8_t packetType,
    uint16_t packetId,
    size_t remainingLength,
    MQTTSuccessFailReasonCode_t reasonCode);
/** @endcond */

/**
 * @brief Serialize an MQTT DISCONNECT packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTT_GetAckPacketSize.
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
 * status = MQTT_GetAckPacketSize(&disconnectInfo,&remainingLength,&packetSize,maxPacketSize);
 *
 */
MQTTStatus_t MQTT_GetAckPacketSize(size_t* pRemainingLength,
    size_t* pPacketSize,
    uint32_t maxPacketSize,
    size_t ackPropertyLength);

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
MQTTStatus_t MQTT_GetDisconnectPacketSize(size_t* pRemainingLength,
    size_t* pPacketSize,
    uint32_t maxPacketSize,
    size_t disconnectPropLen,
    MQTTSuccessFailReasonCode_t reasonCode);
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
uint8_t * MQTT_SerializeDisconnectFixed(uint8_t * pIndex,
                                        MQTTSuccessFailReasonCode_t reasonCode,
                                        size_t remainingLength);
/** @endcond */

/**
 * @brief Deserialize an MQTT Disconnect packet.
 *
 * @param[in] pPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pDisconnectInfo Struct containing information about the disconnect.
 * @param[in] maxPacketSize MAximum packet size allowed by the client.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse, #MQTTServerRefused, #MQTTBadResponse, #MQTTBadResponse or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * MQTTAckInfo_t disconnectInfo;
 * uint32_t maxPacketSize;
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket( &incomingPacket );
 *
 * // Deserialize disconnect information.
 * if( ( incomingPacket.type) == MQTT_PACKET_TYPE_DISCONNECT )
 * {
 *      status = MQTT_DeserializeDisconnect(&incomingPacket,
                                             &disconnectInfo,
                                            maxPacketSize );
 *      if( status == MQTTSuccess )
 *      {
 *          // Disconnect information is available.
 *      }
 * }
 * @endcode
 */
/* @[declare_MQTT_DeserializeDisconnect] */
MQTTStatus_t MQTT_DeserializeDisconnect(const MQTTPacketInfo_t* pPacket,
                                          uint32_t maxPacketSize,
                                          MQTTReasonCodeInfo_t* pDisconnectInfo,
                                          MqttPropBuilder_t* propBuffer);
/* @[declare_MQTT_DeserializeDisconnect] */


/*
* Updating context with connect properties sent by the user
*/
MQTTStatus_t updateContextWithConnectProps(const MqttPropBuilder_t* pPropBuilder, MQTTConnectProperties_t* pConnectProperties);
/*
* API calls for Optional Subscribe Properties
*/
MQTTStatus_t MQTTPropAdd_SubscribeId(MqttPropBuilder_t* pPropertyBuilder, size_t subscriptionId);

/*
* API call for sending User Properties
*/
MQTTStatus_t MQTTPropAdd_UserProp(MqttPropBuilder_t* pPropertyBuilder, const MQTTUserProperty_t* userProperty);

MQTTStatus_t MQTTPropAdd_ConnSessionExpiry(MqttPropBuilder_t* pPropertyBuilder, uint32_t sessionExpiry);

MQTTStatus_t MQTTPropAdd_ConnReceiveMax(MqttPropBuilder_t* pPropertyBuilder, uint16_t receiveMax);

MQTTStatus_t MQTTPropAdd_ConnMaxPacketSize(MqttPropBuilder_t* pPropertyBuilder, uint32_t maxPacketSize);

MQTTStatus_t MQTTPropAdd_ConnTopicAliasMax(MqttPropBuilder_t* pPropertyBuilder, uint16_t topicAliasMax);

MQTTStatus_t MQTTPropAdd_ConnRequestRespInfo(MqttPropBuilder_t* pPropertyBuilder, bool requestResponseInfo);

MQTTStatus_t MQTTPropAdd_ConnRequestProbInfo(MqttPropBuilder_t* pPropertyBuilder, bool requestProblemInfo);

MQTTStatus_t MQTTPropAdd_ConnAuthMethod(MqttPropBuilder_t* pPropertyBuilder,
                                        const char* authMethod,
                                        uint16_t authMethodLength);

MQTTStatus_t MQTTPropAdd_ConnAuthData( MqttPropBuilder_t* pPropertyBuilder,
                                       const char* authData,
                                       uint16_t authDataLength);

MQTTStatus_t MQTTPropAdd_PubPayloadFormat(MqttPropBuilder_t* pPropertyBuilder, bool payloadFormat);

MQTTStatus_t MQTTPropAdd_PubMessageExpiry(MqttPropBuilder_t* pPropertyBuilder, uint32_t messageExpiry);

MQTTStatus_t MQTTPropAdd_PubTopicAlias(MqttPropBuilder_t* pPropertyBuilder, uint16_t topicAlias);


MQTTStatus_t MQTTPropAdd_PubResponseTopic( MqttPropBuilder_t* pPropertyBuilder,
                                           const char* responseTopic,
                                           uint16_t responseTopicLength);


MQTTStatus_t MQTTPropAdd_PubCorrelationData(MqttPropBuilder_t* pPropertyBuilder,
                                            const void* pCorrelationData,

                                            uint16_t correlationLength);
MQTTStatus_t MQTTPropAdd_PubContentType(MqttPropBuilder_t* pPropertyBuilder,
    const char* contentType,
    uint16_t contentTypeLength);

MQTTStatus_t MQTTPropAdd_ReasonString(MqttPropBuilder_t* pPropertyBuilder,
    const char* pReasonString,
    uint16_t reasonStringLength);



MQTTStatus_t validatePublishProperties(uint16_t serverTopicAliasMax, const MqttPropBuilder_t* propBuilder, uint16_t *topicAlias);
MQTTStatus_t validateSubscribeProperties(uint8_t isSubscriptionIdAvailable, const MqttPropBuilder_t* propBuilder);

MQTTStatus_t MQTTPropGet_PubTopicAlias(MqttPropBuilder_t* propBuffer, uint16_t* topicAlias);

MQTTStatus_t MQTTPropGet_PubPayloadFormatIndicator(MqttPropBuilder_t* propBuffer, uint8_t* payloadFormat);

MQTTStatus_t MQTTPropGet_PubResponseTopic(MqttPropBuilder_t* propBuffer, const char** responseTopic, uint16_t* responseTopicLength);

MQTTStatus_t MQTTPropGet_PubCorrelationData(MqttPropBuilder_t* propBuffer, const void** correlationData, uint16_t* correlationLength);

MQTTStatus_t MQTTPropGet_PubMessageExpiryInterval(MqttPropBuilder_t* propBuffer, uint32_t* msgExpiryInterval);

MQTTStatus_t MQTTPropGet_PubContentType(MqttPropBuilder_t* propBuffer, const char** pContentType, uint16_t* contentTypeLength);

MQTTStatus_t MQTTPropGet_PubSubscriptionId(MqttPropBuilder_t* propBuffer, size_t* subscriptionId);

MQTTStatus_t MQTTPropGet_UserProp(MqttPropBuilder_t* propBuffer,
    const char** pUserPropKey,
    uint16_t* pUserPropKeyLen,
    const char** pUserPropVal,
    uint16_t* pUserPropValLen);

MQTTStatus_t MQTTPropGet_ReasonString(MqttPropBuilder_t* propBuffer, const char** pReasonString, uint16_t* reasonStringLength);

MQTTStatus_t MQTTPropGet_DisconnectServerRef(MqttPropBuilder_t* propBuffer, const char** pServerRef, uint16_t* serverRefLength);
MQTTStatus_t MQTTPropGet_ConnSessionExpiry(MqttPropBuilder_t* propBuffer, uint32_t* sessionExpiry);
MQTTStatus_t MQTTPropGet_ConnTopicAliasMax(MqttPropBuilder_t* propBuffer, uint16_t* topicAliasMax);
MQTTStatus_t MQTTPropGet_ConnReceiveMax(MqttPropBuilder_t* propBuffer, uint16_t* receiveMax);
MQTTStatus_t MQTTPropGet_ConnMaxQos(MqttPropBuilder_t* propBuffer, uint8_t* maxQos);
MQTTStatus_t MQTTPropGet_ConnRetainAvailable(MqttPropBuilder_t* propBuffer, uint8_t* retainAvailable);
MQTTStatus_t MQTTPropGet_ConnMaxPacketSize(MqttPropBuilder_t* propBuffer, uint32_t* maxPacketSize);
MQTTStatus_t MQTTPropGet_ConnClientId(MqttPropBuilder_t* propBuffer, const char** pClientId, uint16_t* clientIdLength);
MQTTStatus_t MQTTPropGet_ConnWildcard(MqttPropBuilder_t* propBuffer, uint8_t* isWildCardAvailable);
MQTTStatus_t MQTTPropGet_ConnSubId(MqttPropBuilder_t* propBuffer, uint8_t* isSubIdAvailable);

MQTTStatus_t MQTT_IncomingGetNextProp(MqttPropBuilder_t* propBuffer, uint8_t* propertyId);

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_SERIALIZER_H */
