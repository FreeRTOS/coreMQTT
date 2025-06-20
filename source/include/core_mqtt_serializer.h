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
 * @brief User-facing functions for serializing and deserializing MQTT 5.0
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


/* Structures defined in this file. */
struct MQTTFixedBuffer;
struct MQTTConnectInfo;
struct MQTTSubscribeInfo;
struct MQTTPublishInfo;
struct MQTTPacketInfo;
struct MQTTConnectProperties;
struct MQTTUserProperty;

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
    MQTTEndOfProperties,            /**< End of properties reached while parsing MQTT packet. */
    MQTTStatusConnected,            /**< MQTT connection is established with the broker. */
    MQTTStatusNotConnected,         /**< MQTT connection is not established with the broker. */
    MQTTStatusDisconnectPending,    /**< Transport Interface has failed and MQTT connection needs to be closed. */
    MQTTPublishStoreFailed,         /**< User provided API to store a copy of outgoing publish for retransmission  purposes,
                                    has failed. */
    MQTTPublishRetrieveFailed,       /**< User provided API to retrieve the copy of a publish while reconnecting
                                    with an unclean session has failed. */
    MQTTEventCallbackFailed         /**< Error in the user provided event callback function. */
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
    /* PUBACK reason codes */
    MQTT_REASON_PUBACK_SUCCESS = 0x00,                    /**< Publish was successfully received and accepted. */
    MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS = 0x10,    /**< Publish was accepted but there are no subscribers. */
    MQTT_REASON_PUBACK_UNSPECIFIED_ERROR = 0x80,         /**< Unspecified error occurred for the PUBACK. */
    MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error for the PUBACK. */
    MQTT_REASON_PUBACK_NOT_AUTHORIZED = 0x87,            /**< Client is not authorized to publish. */
    MQTT_REASON_PUBACK_TOPIC_NAME_INVALID = 0x90,        /**< Topic name is not valid. */
    MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE = 0x91,  /**< Packet identifier is already in use. */
    MQTT_REASON_PUBACK_QUOTA_EXCEEDED = 0x97,            /**< Implementation or system quota exceeded. */
    MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID = 0x99,    /**< Payload format is invalid. */

    /* PUBREC reason codes */
    MQTT_REASON_PUBREC_SUCCESS = 0x00,                   /**< Publish was successfully received for QoS 2. */
    MQTT_REASON_PUBREC_NO_MATCHING_SUBSCRIBERS = 0x10,   /**< Publish received but no matching subscribers. */
    MQTT_REASON_PUBREC_UNSPECIFIED_ERROR = 0x80,        /**< Unspecified error occurred for the PUBREC. */
    MQTT_REASON_PUBREC_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error for the PUBREC. */
    MQTT_REASON_PUBREC_NOT_AUTHORIZED = 0x87,           /**< Client is not authorized to publish. */
    MQTT_REASON_PUBREC_TOPIC_NAME_INVALID = 0x90,       /**< Topic name is not valid. */
    MQTT_REASON_PUBREC_PACKET_IDENTIFIER_IN_USE = 0x91, /**< Packet identifier is already in use. */
    MQTT_REASON_PUBREC_QUOTA_EXCEEDED = 0x97,           /**< Implementation or system quota exceeded. */
    MQTT_REASON_PUBREC_PAYLOAD_FORMAT_INVALID = 0x99,   /**< Payload format is invalid. */

    /* PUBREL reason codes */
    MQTT_REASON_PUBREL_SUCCESS = 0x00,                  /**< Publish release was successful. */
    MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND = 0x92, /**< Packet identifier was not found. */

    /* PUBCOMP reason codes */
    MQTT_REASON_PUBCOMP_SUCCESS = 0x00,                 /**< Publish complete was successful. */
    MQTT_REASON_PUBCOMP_PACKET_IDENTIFIER_NOT_FOUND = 0x92, /**< Packet identifier was not found. */

    /* CONNACK reason codes */
    MQTT_REASON_CONNACK_SUCCESS = 0x00,                 /**< Connection accepted. */
    MQTT_REASON_CONNACK_UNSPECIFIED_ERROR = 0x80,       /**< Unspecified error occurred during connection. */
    MQTT_REASON_CONNACK_MALFORMED_PACKET = 0x81,        /**< Received packet was malformed. */
    MQTT_REASON_CONNACK_PROTOCOL_ERROR = 0x82,          /**< Protocol error occurred. */
    MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error. */
    MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION = 0x84, /**< Protocol version not supported. */
    MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID = 0x85, /**< Client identifier is not valid. */
    MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD = 0x86, /**< Username or password is malformed. */
    MQTT_REASON_CONNACK_NOT_AUTHORIZED = 0x87,          /**< Client is not authorized to connect. */
    MQTT_REASON_CONNACK_SERVER_UNAVAILABLE = 0x88,      /**< Server is unavailable. */
    MQTT_REASON_CONNACK_SERVER_BUSY = 0x89,             /**< Server is busy. */
    MQTT_REASON_CONNACK_BANNED = 0x8A,                  /**< Client has been banned. */
    MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD = 0x8C, /**< Authentication method is not supported. */
    MQTT_REASON_CONNACK_TOPIC_NAME_INVALID = 0x90,      /**< Topic name is invalid. */
    MQTT_REASON_CONNACK_PACKET_TOO_LARGE = 0x95,        /**< Packet size exceeds maximum allowed. */
    MQTT_REASON_CONNACK_QUOTA_EXCEEDED = 0x97,          /**< Implementation or system quota exceeded. */
    MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID = 0x99,  /**< Payload format is invalid. */
    MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED = 0x9A,    /**< Retain is not supported. */
    MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED = 0x9B,       /**< QoS level is not supported. */
    MQTT_REASON_CONNACK_USE_ANOTHER_SERVER = 0x9C,      /**< Client should temporarily use another server. */
    MQTT_REASON_CONNACK_SERVER_MOVED = 0x9D,            /**< Client should permanently use another server. */
    MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED = 0x9F, /**< Connection rate limit exceeded. */

    /* SUBACK reason codes */
    MQTT_REASON_SUBACK_GRANTED_QOS0 = 0x00,             /**< Subscription accepted with maximum QoS 0. */
    MQTT_REASON_SUBACK_GRANTED_QOS1 = 0x01,             /**< Subscription accepted with maximum QoS 1. */
    MQTT_REASON_SUBACK_GRANTED_QOS2 = 0x02,             /**< Subscription accepted with maximum QoS 2. */
    MQTT_REASON_SUBACK_UNSPECIFIED_ERROR = 0x80,        /**< Unspecified error occurred for the subscription. */
    MQTT_REASON_SUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error. */
    MQTT_REASON_SUBACK_NOT_AUTHORIZED = 0x87,           /**< Client is not authorized to subscribe. */
    MQTT_REASON_SUBACK_TOPIC_FILTER_INVALID = 0x8F,     /**< Topic filter is not valid. */
    MQTT_REASON_SUBACK_PACKET_IDENTIFIER_IN_USE = 0x91, /**< Packet identifier is already in use. */
    MQTT_REASON_SUBACK_QUOTA_EXCEEDED = 0x97,           /**< Implementation or system quota exceeded. */
    MQTT_REASON_SUBACK_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9E, /**< Shared subscriptions are not supported. */
    MQTT_REASON_SUBACK_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 0xA1, /**< Subscription identifiers are not supported. */
    MQTT_REASON_SUBACK_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xA2, /**< Wildcard subscriptions are not supported. */

    /* UNSUBACK reason codes */
    MQTT_REASON_UNSUBACK_SUCCESS = 0x00,                /**< Unsubscribe was successful. */
    MQTT_REASON_UNSUBACK_NO_SUBSCRIPTION_EXISTED = 0x11, /**< No matching subscription existed. */
    MQTT_REASON_UNSUBACK_UNSPECIFIED_ERROR = 0x80,      /**< Unspecified error occurred for the unsubscribe. */
    MQTT_REASON_UNSUBACK_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error. */
    MQTT_REASON_UNSUBACK_NOT_AUTHORIZED = 0x87,         /**< Client is not authorized to unsubscribe. */
    MQTT_REASON_UNSUBACK_TOPIC_FILTER_INVALID = 0x8F,   /**< Topic filter is not valid. */
    MQTT_REASON_UNSUBACK_PACKET_IDENTIFIER_IN_USE = 0x91, /**< Packet identifier is already in use. */

    /* DISCONNECT reason codes */
    MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION = 0x00,  /**< Normal client-initiated disconnect. */
    MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE = 0x04, /**< Client disconnecting with Will Message. */
    MQTT_REASON_DISCONNECT_UNSPECIFIED_ERROR = 0x80,     /**< Unspecified error occurred. */
    MQTT_REASON_DISCONNECT_MALFORMED_PACKET = 0x81,      /**< Received packet was malformed. */
    MQTT_REASON_DISCONNECT_PROTOCOL_ERROR = 0x82,        /**< Protocol error occurred. */
    MQTT_REASON_DISCONNECT_IMPLEMENTATION_SPECIFIC_ERROR = 0x83, /**< Implementation specific error. */
    MQTT_REASON_DISCONNECT_NOT_AUTHORIZED = 0x87,        /**< Client is not authorized. */
    MQTT_REASON_DISCONNECT_SERVER_BUSY = 0x89,           /**< Server is busy. */
    MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN = 0x8B,  /**< Server is shutting down. */
    MQTT_REASON_DISCONNECT_BAD_AUTHENTICATION_METHOD = 0x8C, /**< Authentication method is invalid. */
    MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT = 0x8D,    /**< Keep alive timeout occurred. */
    MQTT_REASON_DISCONNECT_SESSION_TAKEN_OVER = 0x8E,    /**< Another connection using same client ID. */
    MQTT_REASON_DISCONNECT_TOPIC_FILTER_INVALID = 0x8F,  /**< Topic filter is not valid. */
    MQTT_REASON_DISCONNECT_TOPIC_NAME_INVALID = 0x90,    /**< Topic name is not valid. */
    MQTT_REASON_DISCONNECT_RECEIVE_MAXIMUM_EXCEEDED = 0x93, /**< Receive maximum value exceeded. */
    MQTT_REASON_DISCONNECT_TOPIC_ALIAS_INVALID = 0x94,   /**< Topic alias is invalid. */
    MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE = 0x95,      /**< Packet size exceeds maximum allowed. */
    MQTT_REASON_DISCONNECT_MESSAGE_RATE_TOO_HIGH = 0x96, /**< Message rate too high. */
    MQTT_REASON_DISCONNECT_QUOTA_EXCEEDED = 0x97,        /**< Implementation or system quota exceeded. */
    MQTT_REASON_DISCONNECT_ADMINISTRATIVE_ACTION = 0x98,  /**< Disconnected due to administrative action. */
    MQTT_REASON_DISCONNECT_PAYLOAD_FORMAT_INVALID = 0x99, /**< Payload format is invalid. */
    MQTT_REASON_DISCONNECT_RETAIN_NOT_SUPPORTED = 0x9A,   /**< Retain is not supported. */
    MQTT_REASON_DISCONNECT_QOS_NOT_SUPPORTED = 0x9B,      /**< QoS level is not supported. */
    MQTT_REASON_DISCONNECT_USE_ANOTHER_SERVER = 0x9C,     /**< Client should temporarily use another server. */
    MQTT_REASON_DISCONNECT_SERVER_MOVED = 0x9D,           /**< Client should permanently use another server. */
    MQTT_REASON_DISCONNECT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED = 0x9E, /**< Shared subscriptions are not supported. */
    MQTT_REASON_DISCONNECT_CONNECTION_RATE_EXCEEDED = 0x9F, /**< Connection rate limit exceeded. */
    MQTT_REASON_DISCONNECT_MAXIMUM_CONNECT_TIME = 0xA0,    /**< Maximum connection time authorized exceeded. */
    MQTT_REASON_DISCONNECT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED = 0xA1, /**< Subscription identifiers are not supported. */
    MQTT_REASON_DISCONNECT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED = 0xA2,    /**< Wildcard subscriptions are not supported. */

    MQTT_INVALID_REASON_CODE = 0xFF /**< @brief Invalid reason code. */

} MQTTSuccessFailReasonCode_t;

/**
 * @ingroup mqtt_enum_types
 * @brief Retain Handling types.
 */
typedef enum MQTTRetainHandling{
    retainSendOnSub = 0, /**< Send retained messages at the time of subscription. */
    retainSendOnSubIfNotPresent = 1,  /**< Send retained messages at subscription only if subscription does not currently exist. */
    retainDoNotSendonSub = 2 /**< Do not send retained messages at the time of subscription. */
}MQTTRetainHandling_t;

/**
 * @ingroup mqtt_enum_types
 * @brief MQTT Subscription packet types.
 */
typedef enum MQTTSubscriptionType
{
    MQTT_TYPE_SUBSCRIBE,  /**< @brief The type is a SUBSCRIBE packet. */
    MQTT_TYPE_UNSUBSCRIBE /**< @brief The type is a UNSUBSCRIBE packet. */
} MQTTSubscriptionType_t;

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

    /**
     * @brief If true, Application Messages that are published to this subscription
     * will not be forwarded to the Client that published them.
     */
    bool noLocalOption;

    /**
     *  @brief If true, Application Messages forwarded using this subscription keep the RETAIN
     * flag they were published with.
     */
    bool retainAsPublishedOption;

    /**
     * @brief Specifies whether retained messages are sent
     * when the subscription is established.
     */
    MQTTRetainHandling_t retainHandlingOption;

} MQTTSubscribeInfo_t;


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
     * @brief Two Byte Integer representing the Topic Alias Maximum value.
     */
    uint16_t serverTopicAliasMax;

     /**
     * @brief Whether wildcard subscription is available.
     */
    uint8_t isWildcardAvailable;

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


} MQTTConnectionProperties_t;

 /**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold reason codes.
 */
typedef struct MQTTReasonCodeInfo
{
    /** @brief Pointer to the reason code array. */
    const uint8_t * reasonCode;

    /** @brief Length of the reason code array. */
    size_t reasonCodeLength;

} MQTTReasonCodeInfo_t;

/**
 * @ingroup mqtt_struct_types
 * @brief Property builder for MQTT packets.
 */
typedef struct MQTTPropBuilder
{
    uint8_t * pBuffer;           /**< @brief Pointer to the buffer for storing properties. */
    size_t bufferLength;         /**< @brief Total length of the buffer available for properties. */
    size_t currentIndex;         /**< @brief Current position in the buffer where next property will be written. */
    uint32_t fieldSet;           /**< @brief Bitfield tracking which properties have been added. */
} MQTTPropBuilder_t;

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
 * #MQTTSubscribeInfo_t and #MQTTPropBuilder_t (optional subscribe properties).
 * The size of the #MQTTFixedBuffer_t supplied to #MQTT_SerializeSubscribe must
 * be at least @p pPacketSize. The provided @p pSubscriptionList is valid for
 * serialization with #MQTT_SerializeSubscribe
 * only if this function returns #MQTTSuccess. The remaining length returned in
 * @p pRemainingLength and the packet size returned in @p pPacketSize are valid
 * only if this function returns #MQTTSuccess.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] pSubscribeProperties MQTT SUBSCRIBE properties builder. Pass NULL if not used.
 * @param[out] pRemainingLength The Remaining Length of the MQTT SUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT SUBSCRIBE packet.
 * @param[in] maxPacketSize Maximum packet size.
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
 * MQTTPropBuilder_t subscribeProperties = { 0 };
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
 *      subscriptionList[ i ].noLocalOption = false;
 *      subscriptionList[ i ].retainAsPublishedOption = false;
 *      subscriptionList[ i ].retainHandlingOption = retainSendOnSub;
 * }
 *
 * // Initialize subscribe properties (if needed)
 * initializeSubscribeProperties( &subscribeProperties );
 *
 * // Get the size requirement for the subscribe packet.
 * status = MQTT_GetSubscribePacketSize(
 *      &subscriptionList[ 0 ],
 *      NUMBER_OF_SUBSCRIPTIONS,
 *      &subscribeProperties,
 *      &remainingLength,
 *      &packetSize,
 *      maxPacketSize
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
MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            const MQTTPropBuilder_t * pSubscribeProperties,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize );
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
 * @param[in] pSubscribeProperties MQTT v5.0 properties for the SUBSCRIBE packet. Can be NULL
 * if no properties are needed.
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
 * MQTTPropBuilder_t subscribeProperties = { 0 };
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
 * // Assume subscriptionList and subscribeProperties have been initialized.
 * Get the subscribe packet size.
 * status = MQTT_GetSubscribePacketSize(
 *      &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, &subscribeProperties,
 *      &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the subscribe packet into the fixed buffer.
 * status = MQTT_SerializeSubscribe(
 *      &subscriptionList[ 0 ],
 *      NUMBER_OF_SUBSCRIPTIONS,
 *      &subscribeProperties,
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
    const MQTTPropBuilder_t * pSubscribeProperties,
    uint16_t packetId,
    size_t remainingLength,
    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializesubscribe] */

/**
 * @brief Get packet size and Remaining Length of an MQTT UNSUBSCRIBE packet.
 *
 * This function must be called before #MQTT_SerializeUnsubscribe in order to
 * get the size of the MQTT UNSUBSCRIBE packet that is generated from the list
 * of #MQTTSubscribeInfo_t and #MQTTPropBuilder_t (optional unsubscribe properties).
 * The size of the #MQTTFixedBuffer_t supplied to #MQTT_SerializeUnsubscribe must be
 * at least @p pPacketSize. The provided @p pSubscriptionList is valid for serialization
 * with #MQTT_SerializeUnsubscribe only if this function returns #MQTTSuccess.
 * The remaining length returned in @p pRemainingLength and the packet size returned
 * in @p pPacketSize are valid only if this function returns #MQTTSuccess.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] pUnsubscribeProperties MQTT UNSUBSCRIBE properties builder. Pass NULL if not used.
 * @param[out] pRemainingLength The Remaining Length of the MQTT UNSUBSCRIBE packet.
 * @param[out] pPacketSize The total size of the MQTT UNSUBSCRIBE packet.
 * @param[in] maxPacketSize Maximum packet size.
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
 * MQTTPropBuilder_t unsubscribeProperties = { 0 };
 * size_t maxPacketSize = 0;
 *
 * // Initialize maxPacketSize. The details are out of scope for this example.
 * initializeMaxPacketSize( &maxPacketSize );
 *
 * // Initialize the subscribe info. The details are out of scope for this example.
 * initializeSubscribeInfo( &subscriptionList[ 0 ] );
 *
 * //Initialize the property buffer. The details are out of scope for this example.
 * initializePropertyBuffer( &unsubscribeProperties );
 *
 * // Get the size requirement for the unsubscribe packet.
 * status = MQTT_GetUnsubscribePacketSize(
 *      &subscriptionList[ 0 ],
 *      NUMBER_OF_SUBSCRIPTIONS,
 *      &unsubscribeProperties,
 *      &remainingLength,
 *      &packetSize,
 *      maxPacketSize);
 *
 * if( status == MQTTSuccess )
 * {
 *      // The unsubscribe packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_getunsubscribepacketsize] */
MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t* pSubscriptionList,
                                            size_t subscriptionCount,
                                            const MQTTPropBuilder_t * pUnsubscribeProperties,
                                            size_t* pRemainingLength,
                                            size_t* pPacketSize,
                                            uint32_t maxPacketSize);
/* @[declare_mqtt_getunsubscribepacketsize] */

/**
 * @brief Serialize an MQTT UNSUBSCRIBE packet with properties in the given buffer.
 *
 * #MQTT_GetUnsubscribePacketSize should be called with @p pSubscriptionList
 * and @p pUnsubscribeProperties before invoking this function to get the size of the required
 * #MQTTFixedBuffer_t and @p remainingLength. The @p remainingLength must be
 * the same as returned by #MQTT_GetUnsubscribePacketSize. The #MQTTFixedBuffer_t
 * must be at least as large as the size returned by #MQTT_GetUnsubscribePacketSize.
 *
 * @param[in] pSubscriptionList List of MQTT subscription info to unsubscribe from.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] pUnsubscribeProperties MQTT 5.0 properties for the UNSUBSCRIBE packet. Can be NULL if no properties are needed.
 * @param[in] packetId Packet identifier used for the UNSUBSCRIBE packet.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetUnsubscribePacketSize.
 * @param[out] pFixedBuffer Buffer where the serialized UNSUBSCRIBE packet will be written.
 *
 * @return #MQTTNoMemory if pFixedBuffer is too small to hold the MQTT packet;
 * #MQTTBadParameter if any of the parameters are invalid (NULL pSubscriptionList or pFixedBuffer, zero subscriptionCount);
 * #MQTTSuccess if the packet was serialized successfully.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[2];
 * MQTTPropBuilder_t unsubscribeProperties;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[100];
 * size_t remainingLength = 0, packetSize = 0;
 * uint16_t packetId = 1;
 *
 * // Initialize the fixed buffer.
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = sizeof( buffer );
 *
 * // Initialize subscription list.
 * subscriptionList[0].pTopicFilter = "topic/1";
 * subscriptionList[0].topicFilterLength = strlen("topic/1");
 * subscriptionList[1].pTopicFilter = "topic/2";
 * subscriptionList[1].topicFilterLength = strlen("topic/2");
 *
 * // Initialize properties (optional)
 *
 * // Get size requirement for the unsubscribe packet.
 * status = MQTT_GetUnsubscribePacketSize(
 *      subscriptionList,
 *      2,
 *      &unsubscribeProperties,
 *      &remainingLength,
 *      &packetSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // Serialize unsubscribe packet.
 *      status = MQTT_SerializeUnsubscribe(
 *          subscriptionList,
 *          2,
 *          &unsubscribeProperties,
 *          packetId,
 *          remainingLength,
 *          &fixedBuffer
 *      );
 * }
 *
 * if( status == MQTTSuccess )
 * {
 *      // The unsubscribe packet has been serialized successfully.
 *      // The serialized packet is now ready to be sent to the broker.
 * }
 *
 * @endcode
 */
/* @[declare_mqtt_serializeunsubscribe] */
MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
    size_t subscriptionCount,
    const MQTTPropBuilder_t * pUnsubscribeProperties,
    uint16_t packetId,
    size_t remainingLength,
    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializeunsubscribe] */

/**
 * @brief Serialize an MQTT PUBLISH packet in the given buffer.
 *
 * This function will serialize complete MQTT PUBLISH packet into
 * the given buffer. If the PUBLISH payload can be sent separately,
 * consider using #MQTT_SerializePublishHeader, which will serialize
 * only the PUBLISH header into the buffer.
 *
 * #MQTT_GetPublishPacketSize should be called with @p pPublishInfo before
 * invoking this function to get the size of the required #MQTTFixedBuffer_t and
 * @p remainingLength. The @p remainingLength must be the same as returned by
 * #MQTT_GetPublishPacketSize. The #MQTTFixedBuffer_t must be at least as large
 * as the size returned by #MQTT_GetPublishPacketSize.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] pPublishProperties MQTT v5.0 properties for the PUBLISH packet. Can be NULL
 * if no properties are needed.
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
 * MQTTPropBuilder_t publishProperties = { 0 };
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
 * // Assume publishInfo and publishProperties have been initialized. Get publish packet size.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo, &publishProperties, &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the publish packet into the fixed buffer.
 * status = MQTT_SerializePublish(
 *      &publishInfo,
 *      &publishProperties,
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
                                    const MQTTPropBuilder_t * pPublishProperties,
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
/* @[declare_mqtt_serializepublishheaderwithouttopic] */
MQTTStatus_t MQTT_SerializePublishHeaderWithoutTopic( const MQTTPublishInfo_t * pPublishInfo,
                                                      size_t remainingLength,
                                                      uint8_t * pBuffer,
                                                      size_t * headerSize );
/* @[declare_mqtt_serializepublishheaderwithouttopic] */

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
 * @param[in] pPublishProperties MQTT v5.0 properties for the PUBLISH packet. Can be NULL
 * if no properties are needed.
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
 * MQTTPropBuilder_t publishProperties ;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0, headerSize = 0;
 * uint16_t packetId;
 * int32_t bytesSent;
 * uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // A packet identifier is unused for QoS 0 publishes. Otherwise, a valid, unused packet
 * // identifier must be used.
 * packetId = 0;
 *
 * // Assume publishInfo and publishProperties have been initialized. Get the publish packet size.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo, &publishProperties, &remainingLength, &packetSize, maxPacketSize
);
 * assert( status == MQTTSuccess );
 * // The payload will not be serialized, so the the fixed buffer does not need to hold it.
 * assert( ( packetSize - publishInfo.payloadLength ) <= BUFFER_SIZE );
 *
 * // Serialize the publish packet header into the fixed buffer.
 * status = MQTT_SerializePublishHeader(
 *      &publishInfo,
 *      &publishProperties,
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
    const MQTTPropBuilder_t * pPublishProperties,
    uint16_t packetId,
    size_t remainingLength,
    const MQTTFixedBuffer_t * pFixedBuffer,
    size_t * pHeaderSize );
/* @[declare_mqtt_serializepublishheader] */

/**
 * @brief Serialize an MQTT PUBACK, PUBREC, PUBREL, or PUBCOMP into the given
 * buffer. These PUBLISH ACKS are serialized without reason codes or properties.
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
 * @param[in] propBuffer Buffer to hold the properties.
 * @param[in] maxPacketSize Maximum packet size.
 * @param[in] topicAliasMax Maximum topic alias specified in the CONNECT packet.
 *
 * @return
 * - #MQTTBadParameter if invalid parameters are passed
 * - #MQTTBadResponse if invalid packet is read
 * - #MQTTSuccess otherwise.
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
 * MQTTPropBuilder_t propBuffer ;
 * uint16_t packetId;
 * uint32_t maxPacketSize = pContext->connectionProperties.maxPacketSize;
 * uint16_t topicAliasMax = pContext->connectionProperties.topicAliasMax;
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
 *      status = MQTT_DeserializePublish( &incomingPacket, &packetId, &publishInfo,
 *                                        &propBuffer, maxPacketSize, topicAliasMax );
 *      if( status == MQTTSuccess )
 *      {
 *          // The deserialized publish information can now be used from `publishInfo`.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_deserializepublish] */
MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t* pIncomingPacket,
                                      uint16_t* pPacketId,
                                      MQTTPublishInfo_t* pPublishInfo,
                                      MQTTPropBuilder_t* propBuffer,
                                      uint32_t maxPacketSize,
                                      uint16_t topicAliasMax );
/* @[declare_mqtt_deserializepublish] */

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
 * @brief Get the size and Remaining Length of an MQTT Version 5 CONNECT packet.
 *
 * This function must be called before #MQTT_SerializeConnect in order to get
 * the size of the MQTT CONNECT packet that is generated from #MQTTConnectInfo_t, #MQTTPublishInfo_t
 * and optional MQTTPropBuilder_t. The size of the #MQTTFixedBuffer_t supplied
 * to #MQTT_SerializeConnect must be at least @p pPacketSize. The provided
 * @p pConnectInfo and @p pWillInfo are valid for serialization with
 * #MQTT_SerializeConnect only if this function returns #MQTTSuccess. The
 * remaining length returned in @p pRemainingLength and the packet size returned
 * in @p pPacketSize are valid only if this function returns #MQTTSuccess.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT properties builder. Pass NULL if not used.
 * @param[in] pWillProperties MQTT Will properties builder. Pass NULL if not used.
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
 * MQTTPropBuilder_t connectionProperties = { 0 };
 * MQTTPropBuilder_t willProperties = { 0 };
 * size_t remainingLength = 0, packetSize = 0;
 *
 * // Initialize the connection info, the details are out of scope for this example.
 * initializeConnectInfo( &connectInfo );
 *
 * // Initialize the optional will info, the details are out of scope for this example.
 * initializeWillInfo( &willInfo );
 *
 * // Initialize connect properties and will properties, the details are out of scope for this example.
 * initializeConnectProperties( &connectionProperties );
 * initializeWillProperties( &willProperties );
 *
 * // Get the size requirement for the connect packet.
 * status = MQTT_GetConnectPacketSize(
 *      &connectInfo,
 *      &willInfo,
 *      &connectionProperties,
 *      &willProperties,
 *      &remainingLength,
 *      &packetSize
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
MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        const MQTTPropBuilder_t *pConnectProperties,
                                        const MQTTPropBuilder_t *pWillProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize );
/* @[declare_mqtt_getconnectpacketsize] */

/**
 * @brief Serialize an MQTT CONNECT packet in the given fixed buffer @p pFixedBuffer.
 *
 * #MQTT_GetConnectPacketSize should be called with @p pConnectInfo, @p pWillInfo,
 * @p pConnectProperties, and @p pWillProperties before invoking this function to get
 * the size of the required #MQTTFixedBuffer_t and @p remainingLength. The
 * @p remainingLength must be the same as returned by #MQTT_GetConnectPacketSize.
 * The #MQTTFixedBuffer_t must be at least as large as the size returned by
 * #MQTT_GetConnectPacketSize.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if not used.
 * @param[in] pConnectProperties MQTT CONNECT properties builder. Pass NULL if not used.
 * @param[in] pWillProperties MQTT Will properties builder. Pass NULL if not used.
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
 * MQTTPropBuilder_t connectionProperties = { 0 };
 * MQTTPropBuilder_t willProperties = { 0 };
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Assume connectInfo, willInfo, and properties are initialized.
 * // Get the size requirement for the connect packet.
 * status = MQTT_GetConnectPacketSize(
 *      &connectInfo, &willInfo, &connectionProperties, &willProperties,
 *      &remainingLength, &packetSize
 * );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the connect packet into the fixed buffer.
 * status = MQTT_SerializeConnect(
 *      &connectInfo,
 *      &willInfo,
 *      &connectionProperties,
 *      &willProperties,
 *      remainingLength,
 *      &fixedBuffer
 * );
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
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializeconnect] */

/**
 * @brief Validate the publish parameters present in the given publish structure @p pPublishInfo.
 *
 * This function must be called before #MQTT_GetPublishPacketSize in order to validate the publish parameters.
 *
 * @param[in] pPublishInfo MQTT publish packet parameters.
 * @param[in] retainAvailable Whether server allows retain or not.
 * @param[in] maxQos Maximum QoS supported by the server.
 * @param[in] topicAlias  Topic alias in the PUBLISH packet.
 * @param[in] maxPacketSize Maximum packet size allowed by the server.
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
 * uint16_t topicAlias;
 * uint8_t retainAvailable;
 * uint8_t maxQos;
 * // Set in the CONNACK packet.
 * uint32_t maxPacketSize ;
 *
 * //Set the publish info parameters.
 *
 * //Validate the publish packet
 * status = MQTT_ValidatePublishParams(&publishInfo, retainAvailable, maxQos, topicAlias, maxPacketSize);
 *
 * if( status == MQTTSuccess )
 * {
 *      // Get the packet size and serialize the publish packet.
 * }
 * @endcode
 */
/* @[declare_mqtt_validatepublishparams] */
MQTTStatus_t MQTT_ValidatePublishParams(const MQTTPublishInfo_t* pPublishInfo,
                                        uint8_t retainAvailable,
                                        uint8_t maxQos,
                                        uint16_t topicAlias,
                                        uint32_t maxPacketSize);
/* @[declare_mqtt_validatepublishparams] */


/**
 * @brief Get the packet size and remaining length of an MQTT PUBLISH packet.
 *
 * #MQTT_ValidatePublishParams should be called with @p pPublishInfo before invoking this function
 * to validate the publish parameters. This function must be called before #sendPublishWithoutCopy
 * in order to get the size of the MQTT PUBLISH packet that is generated from #MQTTPublishInfo_t
 * and optional publish properties. The remaining length returned in @p pRemainingLength and the
 * packet size returned in @p pPacketSize are valid only if this function
 * returns #MQTTSuccess.
 *
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] pPublishProperties MQTT PUBLISH properties builder. Pass NULL if not used.
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
 * MQTTPropBuilder_t publishProperties = { 0 };
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
 * // Initialize publish properties (if needed)
 * initializePublishProperties( &publishProperties );
 *
 * // Validate publish parameters
 * status = MQTT_ValidatePublishParams(&publishInfo, topicAliasMax, retainAvailable, maxQos);
 *
 * // Get the size requirement for the publish packet.
 * status = MQTT_GetPublishPacketSize(
 *      &publishInfo,
 *      &publishProperties,
 *      &remainingLength,
 *      &packetSize,
 *      maxPacketSize
 * );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The publish packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_getpublishpacketsize] */
MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        const MQTTPropBuilder_t * pPublishProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize,
                                        uint32_t maxPacketSize);
/* @[declare_mqtt_getpublishpacketsize] */


/**
 * @brief Deserialize an MQTT PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK, PINGRESP or CONNACK.
 *
 * @param[in] pIncomingPacket #MQTTPacketInfo_t containing the buffer.
 * @param[out] pPacketId The packet ID obtained from the buffer.
 * @param[out] pSessionPresent Boolean flag indicating if a session is present (only valid for CONNACK packets).
 *                            For CONNACK: true if session is present, false otherwise.
 *                            For other packet types: this parameter is ignored.
 * @param[out] pReasonCode Struct to store reason code(s) from the acknowledgment packet.
 *                        Contains the success/failure status of the corresponding request.
 * @param[out] pPropBuffer Struct to store the deserialized acknowledgment properties.
 *                       Will contain any MQTT v5.0 properties included in the ack packet.
 * @param[in,out] pConnectProperties Struct to store the deserialized connect/connack properties.
 *                                   This parameter cannot be NULL.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the packet was successfully deserialized
 * - #MQTTBadParameter if invalid parameters are passed
 * - #MQTTServerRefused if the server explicitly rejected the request, either in the CONNACK or a SUBACK.
 * - #MQTTBadResponse if the packet type is invalid or packet parsing fails
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * uint16_t packetId;
 * bool sessionPresent;
 * MQTTReasonCodeInfo_t reasonCode ; // Can be set to NULL if the incoming packet is CONNACK or PINGRESP
 * MQTTPropBuilder_t propBuffer; // Can be set to NULL if the user does not want any incoming properties.
 * MQTTConnectionProperties_t connectionProperties = pContext->connectionProperties;  // Cannot be set to NULL.
 *
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket(&incomingPacket);
 *
 * // Deserialize ack information if the incoming packet is a publish ack.
 * status = MQTT_DeserializeAck(&incomingPacket,
 *                             &packetId,
 *                             &sessionPresent,
 *                             &reasonCode,
 *                             &propBuffer,
 *                             &connectionProperties);
 * if(status == MQTTSuccess)
 * {s
 *     // Ack information is now available.
 * }
 * @endcode
 */
/* @[declare_mqtt_deserializeack] */
MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                uint16_t * pPacketId,
                                bool * pSessionPresent,
                                MQTTReasonCodeInfo_t * pReasonCode,
                                MQTTPropBuilder_t * pPropBuffer,
                                MQTTConnectionProperties_t * pConnectProperties );
/* @[declare_mqtt_deserializeack] */

/**
 * @fn uint8_t* MQTT_SerializeAckFixed(uint8_t* pIndex,
                                        uint8_t packetType,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        MQTTSuccessFailReasonCode_t reasonCode);
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
 * @brief Get the size of an outgoing PUBLISH ACK packet.
 *
 * @note If no reason code is sent and property length is zero then #MQTT_SerializeAck can be used directly.
 *
 * @param[out]  pRemainingLength The remaining length of the packet to be serialized.
 * @param[out]  pPacketSize The size of the packet to be serialized.
 * @param[in]  maxPacketSize Maximum packet size allowed by the server.
 * @param[in]  ackPropertyLength The length of the properties.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;
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
 * size_t remainingLength =0;
 * size_t packetSize = 0;
 * size_t ackPropertyLength = 0;
 * uint32_t maxPacketSize;
 * //set the parameters.
 * // Get the size requirement for the ack packet.
 * status = MQTT_GetAckPacketSize(&remainingLength,&packetSize,maxPacketSize, ackPropertyLength);
 * }
 * @endcode
 */
/* @[declare_mqtt_getackpacketsize] */
MQTTStatus_t MQTT_GetAckPacketSize(size_t* pRemainingLength,
    size_t* pPacketSize,
    uint32_t maxPacketSize,
    size_t ackPropertyLength);
/* @[declare_mqtt_getackpacketsize] */

/**
 * @brief Get the size of an MQTT DISCONNECT packet.
 *
 * @param[in] pDisconnectProperties MQTT DISCONNECT properties builder. Pass NULL if not used.
 * @param[out] pRemainingLength The Remaining Length of the MQTT DISCONNECT packet.
 * @param[out] pPacketSize The size of the MQTT DISCONNECT packet.
 * @param[in] maxPacketSize Maximum packet size allowed by the server.
 * @param[in] reasonCode The reason code for the disconnect.
 *
 * @return #MQTTSuccess, or #MQTTBadParameter if parameters are invalid
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * size_t remainingLength = 0;
 * size_t packetSize = 0;
 * uint32_t maxPacketSize;
 * MQTTPropBuilder_t disconnectProperties ;
 * MQTTSuccessFailReasonCode_t reasonCode;
 *
 * //Set property builder. The details are out of scope for this example.
 * initializePropertyBuilder( &disconnectProperties );
 *
 * //Set the parameters.
 * // Get the size requirement for the disconnect packet.
 * status = MQTT_GetDisconnectPacketSize(&disconnectProperties, &remainingLength,&packetSize,maxPacketSize, reasonCode);
 *
 * if( status == MQTTSuccess )
 * {
 *      // Send the disconnect packet.
 * }
 * @endcode
 */
/* @[declare_mqtt_getdisconnectpacketsize] */
MQTTStatus_t MQTT_GetDisconnectPacketSize(  const MQTTPropBuilder_t * pDisconnectProperties,
                                            size_t* pRemainingLength,
                                            size_t* pPacketSize,
                                            uint32_t maxPacketSize,
                                            MQTTSuccessFailReasonCode_t reasonCode);
/* @[declare_mqtt_getdisconnectpacketsize] */

/**
 * @brief Serialize an MQTT DISCONNECT packet into the given buffer.
 *
 * The input #MQTTFixedBuffer_t.size must be at least as large as the size
 * returned by #MQTT_GetDisconnectPacketSize. This function should only be called
 * after #MQTT_GetDisconnectPacketSize to ensure proper buffer sizing.
 *
 * @param[in] pDisconnectProperties MQTT v5.0 properties for the DISCONNECT packet. Can be NULL
 * if no properties are needed.
 * @param[in] reasonCode The reason code for the disconnect. For MQTT v5.0, this indicates
 * why the connection is being terminated.
 * @param[in] remainingLength Remaining Length provided by #MQTT_GetDisconnectPacketSize.
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
 * MQTTPropBuilder_t disconnectProperties = { 0 };
 * uint8_t buffer[ BUFFER_SIZE ];
 * size_t remainingLength = 0, packetSize = 0;
 *
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = BUFFER_SIZE;
 *
 * // Get the disconnect packet size.
 * status = MQTT_GetDisconnectPacketSize( &disconnectProperties,
 *                                        MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION,
 *                                        &remainingLength,
 *                                        &packetSize );
 * assert( status == MQTTSuccess );
 * assert( packetSize <= BUFFER_SIZE );
 *
 * // Serialize the disconnect into the fixed buffer.
 * status = MQTT_SerializeDisconnect( &disconnectProperties,
 *                                   MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION,
 *                                   remainingLength,
 *                                   &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The disconnect packet can now be sent to the broker.
 * }
 * @endcode
 */
/* @[declare_mqtt_serializedisconnect] */
MQTTStatus_t MQTT_SerializeDisconnect( const MQTTPropBuilder_t *pDisconnectProperties,
    MQTTSuccessFailReasonCode_t reasonCode,
    size_t remainingLength,
    const MQTTFixedBuffer_t * pFixedBuffer );
/* @[declare_mqtt_serializedisconnect] */

/**
 * @fn uint8_t * MQTT_SerializeDisconnectFixed(uint8_t * pIndex,
                                        MQTTSuccessFailReasonCode_t reasonCode,
                                        size_t remainingLength);
 * @brief Serialize the fixed part of the disconnect packet header.
 *
 * @param[out] pIndex Pointer to the buffer where the fixed size parameters is to be serialized.
 * @param[in] reasonCode The disconnect reason code.
 * @param[in] remainingLength The remaining length of the packet to be serialized.
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
 * @param[in] maxPacketSize Maximum packet size allowed by the client.
 * @param[out] pDisconnectInfo Struct containing disconnect reason code
 * @param[out] pPropBuffer MQTTPropBuilder_t to store the deserialized properties.
 *
 * @return #MQTTBadParameter, #MQTTBadResponse or #MQTTSuccess.
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPacketInfo_t incomingPacket;
 * MQTTReasonCodeInfo_t disconnectInfo;
 * uint32_t maxPacketSize;
 * MQTTPropBuilder_t propBuffer; // Assume this is initialized properly
 * // Receive an incoming packet and populate all fields. The details are out of scope
 * // for this example.
 * receiveIncomingPacket( &incomingPacket );
 *
 * // Deserialize disconnect information.
 * if( ( incomingPacket.type) == MQTT_PACKET_TYPE_DISCONNECT )
 * {
 *      status = MQTT_DeserializeDisconnect(&incomingPacket,
*                                           maxPacketSize,
                                            &disconnectInfo,
                                            &propBuffer);
 *      if( status == MQTTSuccess )
 *      {
 *          // Disconnect information is available.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_deserializedisconnect] */
MQTTStatus_t MQTT_DeserializeDisconnect(const MQTTPacketInfo_t* pPacket,
                                          uint32_t maxPacketSize,
                                          MQTTReasonCodeInfo_t* pDisconnectInfo,
                                          MQTTPropBuilder_t* pPropBuffer);
/* @[declare_mqtt_deserializedisconnect] */


/**
 * @brief Updates the MQTT context with connect properties from the property builder.
 *
 * This function processes the property builder and updates the connect properties
 * in the MQTT context. It handles the conversion and validation of properties from
 * the property builder to the connect properties structure.
 *
 * @param[in] pPropBuilder Pointer to the property builder containing MQTT properties.
 *                         Must not be NULL.
 * @param[out] pConnectProperties Pointer to the connection properties structure to be updated.
 *                               Must not be NULL.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if properties were successfully updated
 * - #MQTTBadParameter, MQTTBadResponse if invalid parameters are passed
 *
 * <b>Example</b>
 * @code{c}
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPropBuilder_t propBuilder = { 0 };
 * MQTTConnectionProperties_t connectionProperties = { 0 };
 *
 * // Initialize property builder with desired properties
 * // ...
 *
 * // Update connect properties
 * status = updateContextWithConnectProps(&propBuilder, &connectionProperties);
 *
 * if(status == MQTTSuccess)
 * {
 *     // Properties successfully updated in the context
 * }
 * @endcode
 */

MQTTStatus_t updateContextWithConnectProps(const MQTTPropBuilder_t* pPropBuilder, MQTTConnectionProperties_t* pConnectProperties);


/**
 * @brief Adds a Subscription Identifier property to the MQTT property builder.
 *
 * This function adds a Subscription Identifier property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure where
 *                                the Subscription Identifier will be added.
 *                                Must not be NULL.
 * @param[in] subscriptionId The Subscription Identifier value to be added.
 *                          Must be greater than 0.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Subscription Identifier was successfully added
 * - #MQTTBadParameter if pPropertyBuilder is NULL or subscriptionId is 0
 * - #MQTTNoMemory if the property builder has insufficient space
 *
 * <b>Example</b>
 * @code{c}
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPropBuilder_t propertyBuilder ; // Assume this is initialized properly
 * size_t subscriptionId = 12345;
 *
 * // Add Subscription Identifier to property builder
 * status = MQTTPropAdd_SubscribeId(&propertyBuilder, subscriptionId);
 *
 * if(status == MQTTSuccess)
 * {
 *     // Subscription Identifier successfully added
 * }
 * @endcode
 *
 * @note This property is only valid for MQTT v5.0 and above.
 * @note The Subscription Identifier can be used in SUBSCRIBE packets and
 *       will be returned in matched PUBLISH packets.
 */

/* @[declare_mqttpropadd_subscribeid] */
MQTTStatus_t MQTTPropAdd_SubscribeId(MQTTPropBuilder_t* pPropertyBuilder, size_t subscriptionId);
/* @[declare_mqttpropadd_subscribeid] */

/**
 * @brief Adds User Property to the MQTT property builder.
 *
 * This function adds User Property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  userProperty       The User Property to be added.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Subscription Identifier was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_userprop] */
MQTTStatus_t MQTTPropAdd_UserProp(MQTTPropBuilder_t* pPropertyBuilder, const MQTTUserProperty_t* userProperty);
/* @[declare_mqttpropadd_userprop] */

/**
 * @brief Adds Session Expiry Interval property to the MQTT property builder.
 *
 * This function adds Session Expiry Interval property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  sessionExpiry     The Session Expiry Interval in seconds.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Session Expiry Interval was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_sessionexpiry] */
MQTTStatus_t MQTTPropAdd_SessionExpiry(MQTTPropBuilder_t* pPropertyBuilder, uint32_t sessionExpiry);
/* @[declare_mqttpropadd_sessionexpiry] */

/**
 * @brief Adds Receive Maximum property to the MQTT property builder.
 *
 * This function adds Receive Maximum property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  receiveMax        The maximum number of QoS 1 and QoS 2 messages allowed to be received simultaneously.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Receive Maximum was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connreceivemax] */
MQTTStatus_t MQTTPropAdd_ConnReceiveMax(MQTTPropBuilder_t* pPropertyBuilder, uint16_t receiveMax);
/* @[declare_mqttpropadd_connreceivemax] */

/**
 * @brief Adds Maximum Packet Size property to the MQTT property builder.
 *
 * This function adds Maximum Packet Size property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  maxPacketSize     The maximum packet size the client is willing to accept.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Maximum Packet Size was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connmaxpacketsize] */
MQTTStatus_t MQTTPropAdd_ConnMaxPacketSize(MQTTPropBuilder_t* pPropertyBuilder, uint32_t maxPacketSize);
/* @[declare_mqttpropadd_connmaxpacketsize] */

/**
 * @brief Adds Topic Alias Maximum property to the MQTT property builder.
 *
 * This function adds Topic Alias Maximum property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  topicAliasMax     The maximum value of topic alias accepted by the client.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Topic Alias Maximum was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_conntopicaliasmax] */
MQTTStatus_t MQTTPropAdd_ConnTopicAliasMax(MQTTPropBuilder_t* pPropertyBuilder, uint16_t topicAliasMax);
/* @[declare_mqttpropadd_conntopicaliasmax] */

/**
 * @brief Adds Request Response Information property to the MQTT property builder.
 *
 * This function adds Request Response Information property to the property builder.
 *
 * @param[out] pPropertyBuilder       Pointer to the property builder structure.
 * @param[in]  requestResponseInfo    Boolean indicating whether response information is requested.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Request Response Information was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connrequestrespinfo] */
MQTTStatus_t MQTTPropAdd_ConnRequestRespInfo(MQTTPropBuilder_t* pPropertyBuilder, bool requestResponseInfo);
/* @[declare_mqttpropadd_connrequestrespinfo] */

/**
 * @brief Adds Request Problem Information property to the MQTT property builder.
 *
 * This function adds Request Problem Information property to the property builder.
 *
 * @param[out] pPropertyBuilder       Pointer to the property builder structure.
 * @param[in]  requestProblemInfo    Boolean indicating whether problem information is requested.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Request Problem Information was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connrequestprobinfo] */
MQTTStatus_t MQTTPropAdd_ConnRequestProbInfo(MQTTPropBuilder_t* pPropertyBuilder, bool requestProblemInfo);
/* @[declare_mqttpropadd_connrequestprobinfo] */

/**
 * @brief Adds Authentication Method property to the MQTT property builder.
 *
 * This function adds Authentication Method property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  authMethod        Pointer to the authentication method string.
 * @param[in]  authMethodLength  Length of the authentication method string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Authentication Method was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connauthmethod] */
MQTTStatus_t MQTTPropAdd_ConnAuthMethod(MQTTPropBuilder_t* pPropertyBuilder,
                                        const char* authMethod,
                                        uint16_t authMethodLength);
/* @[declare_mqttpropadd_connauthmethod] */

/**
 * @brief Adds Authentication Data property to the MQTT property builder.
 *
 * This function adds Authentication Data property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  authData          Pointer to the authentication data.
 * @param[in]  authDataLength    Length of the authentication data.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Authentication Data was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_connauthdata] */
MQTTStatus_t MQTTPropAdd_ConnAuthData( MQTTPropBuilder_t* pPropertyBuilder,
                                       const char* authData,
                                       uint16_t authDataLength);
/* @[declare_mqttpropadd_connauthdata] */

/**
 * @brief Adds Payload Format Indicator property to the MQTT property builder.
 *
 * This function adds Payload Format Indicator property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  payloadFormat     Boolean indicating the payload format (true for UTF-8, false for unspecified bytes).
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Payload Format Indicator was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubpayloadformat] */
MQTTStatus_t MQTTPropAdd_PubPayloadFormat(MQTTPropBuilder_t* pPropertyBuilder, bool payloadFormat);
/* @[declare_mqttpropadd_pubpayloadformat] */

/**
 * @brief Adds Message Expiry Interval property to the MQTT property builder.
 *
 * This function adds Message Expiry Interval property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  messageExpiry     The message expiry interval in seconds.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Message Expiry Interval was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubmessageexpiry] */
MQTTStatus_t MQTTPropAdd_PubMessageExpiry(MQTTPropBuilder_t* pPropertyBuilder, uint32_t messageExpiry);
/* @[declare_mqttpropadd_pubmessageexpiry] */

/**
 * @brief Adds Will Delay Interval property to the MQTT property builder.
 *
 * This function adds Message Expiry Interval property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  willDelayInterval  Will Delay Interval in seconds.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Message Expiry Interval was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_willdelayinterval] */
MQTTStatus_t MQTTPropAdd_WillDelayInterval( MQTTPropBuilder_t * pPropertyBuilder,
                                            uint32_t willDelayInterval );
/* @[declare_mqttpropadd_willdelayinterval] */

/**
 * @brief Adds Topic Alias property to the MQTT property builder.
 *
 * This function adds Topic Alias property to the property builder.
 *
 * @param[out] pPropertyBuilder   Pointer to the property builder structure.
 * @param[in]  topicAlias        The topic alias value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Topic Alias was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubtopicalias] */
MQTTStatus_t MQTTPropAdd_PubTopicAlias(MQTTPropBuilder_t* pPropertyBuilder, uint16_t topicAlias);
/* @[declare_mqttpropadd_pubtopicalias] */

/**
 * @brief Adds Response Topic property to the MQTT property builder.
 *
 * This function adds Response Topic property to the property builder.
 *
 * @param[out] pPropertyBuilder      Pointer to the property builder structure.
 * @param[in]  responseTopic        Pointer to the response topic string.
 * @param[in]  responseTopicLength  Length of the response topic string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Response Topic was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubresponsetopic] */
MQTTStatus_t MQTTPropAdd_PubResponseTopic( MQTTPropBuilder_t* pPropertyBuilder,
                                           const char* responseTopic,
                                           uint16_t responseTopicLength);
/* @[declare_mqttpropadd_pubresponsetopic] */
/**
 * @brief Adds Correlation Data property to the MQTT property builder.
 *
 * This function adds Correlation Data property to the property builder.
 *
 * @param[out] pPropertyBuilder      Pointer to the property builder structure.
 * @param[in]  pCorrelationData     Pointer to the correlation data.
 * @param[in]  correlationLength    Length of the correlation data.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Correlation Data was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubcorrelationdata] */
MQTTStatus_t MQTTPropAdd_PubCorrelationData(MQTTPropBuilder_t* pPropertyBuilder,
                                            const void* pCorrelationData,
                                            uint16_t correlationLength);
/* @[declare_mqttpropadd_pubcorrelationdata] */

/**
 * @brief Adds Content Type property to the MQTT property builder.
 *
 * This function adds Content Type property to the property builder.
 *
 * @param[out] pPropertyBuilder     Pointer to the property builder structure.
 * @param[in]  contentType         Pointer to the content type string.
 * @param[in]  contentTypeLength   Length of the content type string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Content Type was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_pubcontenttype] */
MQTTStatus_t MQTTPropAdd_PubContentType(MQTTPropBuilder_t* pPropertyBuilder,
    const char* contentType,
    uint16_t contentTypeLength);
/* @[declare_mqttpropadd_pubcontenttype] */

/**
 * @brief Adds Reason String property to the MQTT property builder.
 *
 * This function adds Reason String property to the property builder.
 *
 * @param[out] pPropertyBuilder      Pointer to the property builder structure.
 * @param[in]  pReasonString        Pointer to the reason string.
 * @param[in]  reasonStringLength   Length of the reason string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Reason String was successfully added
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTNoMemory if the property builder has insufficient space
 */
/* @[declare_mqttpropadd_reasonstring] */
MQTTStatus_t MQTTPropAdd_ReasonString(MQTTPropBuilder_t* pPropertyBuilder,
                                    const char* pReasonString,
                                    uint16_t reasonStringLength);
/* @[declare_mqttpropadd_reasonstring] */

/**
 * @brief Validates the properties of a PUBLISH packet.
 *
 * This function validates the properties in the property builder for a PUBLISH packet.
 *
 * @param[in]  serverTopicAliasMax  Maximum topic alias value allowed by the server.
 * @param[in]  propBuilder          Pointer to the property builder structure.
 * @param[out] topicAlias          Pointer to store the topic alias value if present.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the properties are valid
 * - #MQTTBadParameter if invalid parameters are passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqtt_validatepublishproperties] */
MQTTStatus_t MQTT_ValidatePublishProperties(uint16_t serverTopicAliasMax, const MQTTPropBuilder_t* propBuilder, uint16_t *topicAlias);
/* @[declare_mqtt_validatepublishproperties] */

/**
 * @brief Validates the properties of a SUBSCRIBE packet.
 *
 * This function validates the properties in the property builder for a SUBSCRIBE packet.
 *
 * @param[in] isSubscriptionIdAvailable  Boolean indicating if subscription identifiers are supported.
 * @param[in] propBuilder               Pointer to the property builder structure.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the properties are valid
 * - #MQTTBadParameter if an invalid parameter is passed
 */
/* @[declare_mqtt_validatesubscribeproperties] */
MQTTStatus_t MQTT_ValidateSubscribeProperties(uint8_t isSubscriptionIdAvailable, const MQTTPropBuilder_t* propBuilder);
/* @[declare_mqtt_validatesubscribeproperties] */

/**
 * @brief Validates the properties specified for an MQTT DISCONNECT packet.
 *
 * @param[in] connectSessionExpiry The session expiry interval that was specified
 *                                in the CONNECT packet. Used to validate that the
 *                                DISCONNECT session expiry is not non-zero while
 *                                connectSessionExpiry is zero.
 * @param[in] pPropertyBuilder Pointer to the property builder structure containing subscribe properties.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess , #MQTTBadParameter or #MQTTBadResponse.
 */
/* @[declare_mqtt_validatedisconnectproperties] */
MQTTStatus_t MQTT_ValidateDisconnectProperties( uint32_t connectSessionExpiry, const MQTTPropBuilder_t * pPropertyBuilder);
/* @[declare_mqtt_validatedisconnectproperties] */

/**
 * @brief Validates the properties specified for WILL Properties in the MQTT CONNECT packet.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder structure containing will properties.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess , #MQTTBadParameter or #MQTTBadResponse.
 */
/* @[declare_mqtt_validatewillproperties] */
MQTTStatus_t MQTT_ValidateWillProperties( const MQTTPropBuilder_t * pPropertyBuilder);
/* @[declare_mqtt_validatewillproperties] */

/**
 * @brief Validates the properties specified for an MQTT UNSUBSCRIBE packet.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder structure containing unsubscribe properties.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess , #MQTTBadParameter or #MQTTBadResponse.
 */
/* @[declare_mqtt_validateunsubscribeproperties] */
MQTTStatus_t MQTT_ValidateUnsubscribeProperties( const MQTTPropBuilder_t * pPropertyBuilder);
/* @[declare_mqtt_validateunsubscribeproperties] */

/**
 * @brief Validates the properties specified for an MQTT PUBLISH ACK packet.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder structure containing unsubscribe properties.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess , #MQTTBadParameter or #MQTTBadResponse.
 */
/* @[declare_mqtt_validatepublishackproperties] */
MQTTStatus_t MQTT_ValidatePublishAckProperties( const MQTTPropBuilder_t * pPropertyBuilder );
/* @[declare_mqtt_validatepublishackproperties] */

/**
 * @brief Gets the Topic Alias property from the MQTT property builder.
 *
 * This function retrieves the Topic Alias property from the property builder.
 *
 * @param[in]  propBuffer    Pointer to the property builder structure.
 * @param[out] topicAlias   Pointer to store the retrieved topic alias value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Topic Alias was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubtopicalias] */
MQTTStatus_t MQTTPropGet_PubTopicAlias(MQTTPropBuilder_t* propBuffer, uint16_t* topicAlias);
/* @[declare_mqttpropget_pubtopicalias] */

/**
 * @brief Gets the Payload Format Indicator property from the MQTT property builder.
 *
 * This function retrieves the Payload Format Indicator property from the property builder.
 *
 * @param[in]  propBuffer    Pointer to the property builder structure.
 * @param[out] payloadFormat   Pointer to store the retrieved Payload Format Indicator value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Topic Alias was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubpayloadformat] */
MQTTStatus_t MQTTPropGet_PubPayloadFormatIndicator(MQTTPropBuilder_t* propBuffer, uint8_t* payloadFormat);
/* @[declare_mqttpropget_pubpayloadformat] */

/**
 * @brief Gets the Response Topic property from the MQTT property builder.
 *
 * This function retrieves the Response Topic property from the property builder.
 *
 * @param[in]  propBuffer           Pointer to the property builder structure.
 * @param[out] responseTopic        Pointer to store the response topic string.
 * @param[out] responseTopicLength  Pointer to store the length of the response topic string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Response Topic was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubresponsetopic] */
MQTTStatus_t MQTTPropGet_PubResponseTopic(MQTTPropBuilder_t* propBuffer, const char** responseTopic, uint16_t* responseTopicLength);
/* @[declare_mqttpropget_pubresponsetopic] */

/**
 * @brief Gets the Correlation Data property from the MQTT property builder.
 *
 * This function retrieves the Correlation Data property from the property builder.
 *
 * @param[in]  propBuffer         Pointer to the property builder structure.
 * @param[out] correlationData    Pointer to store the correlation data.
 * @param[out] correlationLength  Pointer to store the length of the correlation data.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Correlation Data was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubcorrelationdata] */
MQTTStatus_t MQTTPropGet_PubCorrelationData(MQTTPropBuilder_t* propBuffer, const void** correlationData, uint16_t* correlationLength);
/* @[declare_mqttpropget_pubcorrelationdata] */

/**
 * @brief Gets the Message Expiry Interval property from the MQTT property builder.
 *
 * This function retrieves the Message Expiry Interval property from the property builder.
 *
 * @param[in]  propBuffer         Pointer to the property builder structure.
 * @param[out] msgExpiryInterval  Pointer to store the message expiry interval value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Message Expiry Interval was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubmessageexpiryinterval] */
MQTTStatus_t MQTTPropGet_PubMessageExpiryInterval(MQTTPropBuilder_t* propBuffer, uint32_t* msgExpiryInterval);
/* @[declare_mqttpropget_pubmessageexpiryinterval] */

/**
 * @brief Gets the Content Type property from the MQTT property builder.
 *
 * This function retrieves the Content Type property from the property builder.
 *
 * @param[in]  propBuffer         Pointer to the property builder structure.
 * @param[out] pContentType       Pointer to store the content type string.
 * @param[out] contentTypeLength  Pointer to store the length of the content type string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Content Type was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubcontenttype] */
MQTTStatus_t MQTTPropGet_PubContentType(MQTTPropBuilder_t* propBuffer, const char** pContentType, uint16_t* contentTypeLength);
/* @[declare_mqttpropget_pubcontenttype] */

/**
 * @brief Gets the Subscription Identifier property from the MQTT property builder.
 *
 * This function retrieves the Subscription Identifier property from the property builder.
 *
 * @param[in]  propBuffer      Pointer to the property builder structure.
 * @param[out] subscriptionId  Pointer to store the subscription identifier value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Subscription Identifier was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_pubsubscriptionid] */
MQTTStatus_t MQTTPropGet_PubSubscriptionId(MQTTPropBuilder_t* propBuffer, size_t* subscriptionId);
/* @[declare_mqttpropget_pubsubscriptionid] */

/**
 * @brief Gets the User Property from the MQTT property builder.
 *
 * This function retrieves the User Property from the property builder.
 *
 * @param[in]  propBuffer       Pointer to the property builder structure.
 * @param[out] pUserPropKey     Pointer to store the user property key string.
 * @param[out] pUserPropKeyLen  Pointer to store the length of the key string.
 * @param[out] pUserPropVal     Pointer to store the user property value string.
 * @param[out] pUserPropValLen  Pointer to store the length of the value string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the User Property was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_userprop] */
MQTTStatus_t MQTTPropGet_UserProp(MQTTPropBuilder_t* propBuffer,
    const char** pUserPropKey,
    uint16_t* pUserPropKeyLen,
    const char** pUserPropVal,
    uint16_t* pUserPropValLen);
/* @[declare_mqttpropget_userprop] */

/**
 * @brief Gets the Reason String property from the MQTT property builder.
 *
 * This function retrieves the Reason String property from the property builder.
 *
 * @param[in]  propBuffer         Pointer to the property builder structure.
 * @param[out] pReasonString      Pointer to store the reason string.
 * @param[out] reasonStringLength Pointer to store the length of the reason string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Reason String was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_reasonstring] */
MQTTStatus_t MQTTPropGet_ReasonString(MQTTPropBuilder_t* propBuffer, const char** pReasonString, uint16_t* reasonStringLength);
/* @[declare_mqttpropget_reasonstring] */

/**
 * @brief Gets the Server Reference property from the MQTT DISCONNECT packet properties.
 *
 * This function retrieves the Server Reference property from the property builder.
 *
 * @param[in]  propBuffer       Pointer to the property builder structure.
 * @param[out] pServerRef       Pointer to store the server reference string.
 * @param[out] serverRefLength  Pointer to store the length of the server reference string.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Server Reference was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_disconnectserverref] */
MQTTStatus_t MQTTPropGet_ServerRef(MQTTPropBuilder_t* propBuffer, const char** pServerRef, uint16_t* serverRefLength);
/* @[declare_mqttpropget_disconnectserverref] */

/**
 * @brief Gets the Session Expiry Interval property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Session Expiry Interval property from the property builder.
 *
 * @param[in]  propBuffer     Pointer to the property builder structure.
 * @param[out] sessionExpiry  Pointer to store the session expiry interval value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Session Expiry Interval was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_sessionexpiry] */
MQTTStatus_t MQTTPropGet_SessionExpiry(MQTTPropBuilder_t* propBuffer, uint32_t* sessionExpiry);
/* @[declare_mqttpropget_sessionexpiry] */

/**
 * @brief Gets the Topic Alias Maximum property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Topic Alias Maximum property from the property builder.
 *
 * @param[in]  propBuffer     Pointer to the property builder structure.
 * @param[out] topicAliasMax  Pointer to store the topic alias maximum value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Topic Alias Maximum was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_conntopicaliasmax] */
MQTTStatus_t MQTTPropGet_ConnTopicAliasMax(MQTTPropBuilder_t* propBuffer, uint16_t* topicAliasMax);
/* @[declare_mqttpropget_conntopicaliasmax] */

/**
 * @brief Gets the Receive Maximum property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Receive Maximum property from the property builder.
 *
 * @param[in]  propBuffer   Pointer to the property builder structure.
 * @param[out] receiveMax   Pointer to store the receive maximum value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Receive Maximum was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connreceivemax] */
MQTTStatus_t MQTTPropGet_ConnReceiveMax(MQTTPropBuilder_t* propBuffer, uint16_t* receiveMax);
/* @[declare_mqttpropget_connreceivemax] */

/**
 * @brief Gets the Maximum QoS property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Maximum QoS property from the property builder.
 *
 * @param[in]  propBuffer   Pointer to the property builder structure.
 * @param[out] maxQos       Pointer to store the maximum QoS value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Maximum QoS was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connmaxqos] */
MQTTStatus_t MQTTPropGet_ConnMaxQos(MQTTPropBuilder_t* propBuffer, uint8_t* maxQos);
/* @[declare_mqttpropget_connmaxqos] */

/**
 * @brief Gets the Retain Available property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Retain Available property from the property builder.
 *
 * @param[in]  propBuffer       Pointer to the property builder structure.
 * @param[out] retainAvailable  Pointer to store the retain available flag.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Retain Available was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connretainavailable] */
MQTTStatus_t MQTTPropGet_ConnRetainAvailable(MQTTPropBuilder_t* propBuffer, uint8_t* retainAvailable);
/* @[declare_mqttpropget_connretainavailable] */

/**
 * @brief Gets the Maximum Packet Size property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Maximum Packet Size property from the property builder.
 *
 * @param[in]  propBuffer      Pointer to the property builder structure.
 * @param[out] maxPacketSize   Pointer to store the maximum packet size value.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Maximum Packet Size was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connmaxpacketsize] */
MQTTStatus_t MQTTPropGet_ConnMaxPacketSize(MQTTPropBuilder_t* propBuffer, uint32_t* maxPacketSize);
/* @[declare_mqttpropget_connmaxpacketsize] */

/**
 * @brief Gets the Client Identifier property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Client Identifier property from the property builder.
 *
 * @param[in]  propBuffer      Pointer to the property builder structure.
 * @param[out] pClientId       Pointer to store the client identifier string.
 * @param[out] clientIdLength  Pointer to store the length of the client identifier.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Client Identifier was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connclientid] */
MQTTStatus_t MQTTPropGet_ConnClientId(MQTTPropBuilder_t* propBuffer, const char** pClientId, uint16_t* clientIdLength);
/* @[declare_mqttpropget_connclientid] */

/**
 * @brief Gets the Wildcard Subscription Available property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Wildcard Subscription Available property from the property builder.
 *
 * @param[in]  propBuffer           Pointer to the property builder structure.
 * @param[out] isWildCardAvailable  Pointer to store the wildcard subscription available flag.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Wildcard Subscription Available was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connwildcard] */
MQTTStatus_t MQTTPropGet_ConnWildcard(MQTTPropBuilder_t* propBuffer, uint8_t* isWildCardAvailable);
/* @[declare_mqttpropget_connwildcard] */

/**
 * @brief Gets the Subscription Identifier Available property from the MQTT CONNECT packet properties.
 *
 * This function retrieves the Subscription Identifier Available property from the property builder.
 *
 * @param[in]  propBuffer         Pointer to the property builder structure.
 * @param[out] isSubIdAvailable   Pointer to store the subscription identifier available flag.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the Subscription Identifier Available was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTBadResponse if an invalid packet is read
 */
/* @[declare_mqttpropget_connsubid] */
MQTTStatus_t MQTTPropGet_ConnSubId(MQTTPropBuilder_t* propBuffer, uint8_t* isSubIdAvailable);
/* @[declare_mqttpropget_connsubid] */


/**
 * @brief Get the Shared Subscriptions Available property from CONNACK properties
 *
 * @param[in] propBuffer The property buffer containing CONNACK properties
 * @param[out] isSharedSubAvailable Pointer to store whether shared subscriptions are supported
 *                                 1 if available, 0 if not available
 *
 * @return #MQTTSuccess if property was found and retrieved successfully
 *         #MQTTBadParameter if propBuffer or isSharedSubAvailable is NULL
 *         #MQTTBadResponse if property value is invalid in buffer
 */
/* @[declare_mqttpropget_connsharedsubavailable] */
MQTTStatus_t MQTTPropGet_ConnSharedSubAvailable( MQTTPropBuilder_t * propBuffer,
                                                uint8_t * isSharedSubAvailable );
/* @[declare_mqttpropget_connsharedsubavailable] */
/**
 * @brief Get the Server Keep Alive property from CONNACK properties
 *
 * @param[in] propBuffer The property buffer containing CONNACK properties
 * @param[out] serverKeepAlive Pointer to store the server-specified keep alive interval in seconds
 *
 * @return #MQTTSuccess if property was found and retrieved successfully
 *         #MQTTBadParameter if propBuffer or serverKeepAlive is NULL
 *         #MQTTBadResponse if property value is invalid in buffer
 */
/* @[declare_mqttpropget_connserverkeepalive] */
MQTTStatus_t MQTTPropGet_ConnServerKeepAlive( MQTTPropBuilder_t * propBuffer,
                                            uint16_t * serverKeepAlive );
/* @[declare_mqttpropget_connserverkeepalive] */

/**
 * @brief Get the Response Information property from CONNACK properties
 *
 * @param[in] propBuffer The property buffer containing CONNACK properties
 * @param[out] pResponseInfo Pointer to store the response information string
 * @param[out] responseInfoLength Pointer to store length of response information
 *
 * @return #MQTTSuccess if property was found and retrieved successfully
 *         #MQTTBadParameter if propBuffer, pResponseInfo, or responseInfoLength is NULL
 *         #MQTTBadResponse if property value is invalid in buffer
 */
/* @[declare_mqttpropget_connresponseinfo] */
MQTTStatus_t MQTTPropGet_ConnResponseInfo( MQTTPropBuilder_t * propBuffer,
                                            const char ** pResponseInfo,
                                            uint16_t * responseInfoLength );
/* @[declare_mqttpropget_connresponseinfo] */

/**
 * @brief Get the Authentication Method property from CONNECT/CONNACK properties
 *
 * @param[in] propBuffer The property buffer containing CONNECT/CONNACK properties
 * @param[out] pAuthMethod Pointer to store the authentication method string
 * @param[out] authMethodLength Pointer to store length of authentication method
 *
 * @return #MQTTSuccess if property was found and retrieved successfully
 *         #MQTTBadParameter if propBuffer, pAuthMethod, or authMethodLength is NULL
 *         #MQTTBadResponse if property value is invalid in buffer
 */

/* @[declare_mqttpropget_connauthmethod] */
MQTTStatus_t MQTTPropGet_ConnAuthMethod(MQTTPropBuilder_t * propBuffer,
                                        const char ** pAuthMethod,
                                        uint16_t * authMethodLength);
/* @[declare_mqttpropget_connauthmethod] */

/**
 * @brief Get the Authentication Data property from CONNECT/CONNACK properties
 *
 * @param[in] propBuffer The property buffer containing CONNECT/CONNACK properties
 * @param[out] pAuthData Pointer to store the authentication data
 * @param[out] authDataLength Pointer to store length of authentication data
 *
 * @return #MQTTSuccess if property was found and retrieved successfully
 *         #MQTTBadParameter if propBuffer, pAuthData, or authDataLength is NULL
 *         #MQTTBadResponse if property value is invalid in buffer
 */

/* @[declare_mqttpropget_connauthdata] */
MQTTStatus_t MQTTPropGet_ConnAuthData(MQTTPropBuilder_t * propBuffer,
                                        const char ** pAuthData,
                                        uint16_t * authDataLength);

/* @[declare_mqttpropget_connauthdata] */

/**
 * @brief Gets the next property identifier from the incoming MQTT packet properties.
 *
 * This function retrieves the next property identifier from the property builder.
 *
 * @param[in]  propBuffer   Pointer to the property builder structure.
 * @param[out] propertyId   Pointer to store the next property identifier.
 *
 * @return Returns one of the following:
 * - #MQTTSuccess if the next property identifier was successfully retrieved
 * - #MQTTBadParameter if an invalid parameter is passed
 * - #MQTTEndOfProperties if there are no more properties to retrieve
 */
/* @[declare_mqtt_incominggetnextprop] */
MQTTStatus_t MQTT_IncomingGetNextProp(MQTTPropBuilder_t* propBuffer, uint8_t* propertyId);
/* @[declare_mqtt_incominggetnextprop] */

/**
 * @brief Initialize the property builder.
 *
 * @param[out] pPropertyBuilder Property builder to initialize.
 * @param[in] buffer Buffer to store the properties.
 * @param[in] length Length of the buffer.
 *
 * @return
 * - #MQTTBadParameter if invalid parameters are passed.
 * - #MQTTSuccess otherwise.
 */
/* @[declare_mqttpropertybuilder_init] */
MQTTStatus_t MQTTPropertyBuilder_Init( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint8_t * buffer,
                                        size_t length );
/* @[declare_mqttpropertybuilder_init] */

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
MQTTStatus_t decodeSubackPropertyLength( const uint8_t * pIndex, size_t remainingLength, size_t * subackPropertyLength );

/**
 * @brief Initialize an MQTTConnectionProperties_t.
 *
 * @note This function initializes the connect properties to default values.
 *       This function should only be used if using only serializer functions
 *       throughout the connection. It is also important to only call this function
 *       before sending the connect packet.
 *
 * @param[in] pConnectProperties The connect properties to initialize.
 *
 * @return
 * - #MQTTBadParameter if pConnectProperties is NULL.
 * - #MQTTSuccess otherwise.
 */
/* @[declare_mqtt_initconnect] */
MQTTStatus_t MQTT_InitConnect( MQTTConnectionProperties_t * pConnectProperties );
/* @[declare_mqtt_initconnect] */

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_SERIALIZER_H */
