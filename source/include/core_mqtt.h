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
 * @file core_mqtt.h
 * @brief User-facing functions of the MQTT 5.0 library.
 */
#ifndef CORE_MQTT_H
#define CORE_MQTT_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/* Include MQTT serializer library. */
#include "core_mqtt_serializer.h"

/* Include transport interface. */
#include "transport_interface.h"

/**
 * @cond DOXYGEN_IGNORE
 * The current version of this library.
 *
 * If MQTT_LIBRARY_VERSION ends with + it represents the version in development
 * after the numbered release.
 */
#define MQTT_LIBRARY_VERSION    "v2.3.1+"
/** @endcond */

/**
 * @ingroup mqtt_constants
 * @brief Invalid packet identifier.
 *
 * Zero is an invalid packet identifier as per MQTT 5.0 spec.
 */
#define MQTT_PACKET_ID_INVALID    ( ( uint16_t ) 0U )

/* Structures defined in this file. */
struct MQTTPubAckInfo;
struct MQTTContext;
struct MQTTDeserializedInfo;

/**
 * @ingroup mqtt_struct_types
 * @brief An opaque structure provided by the library to the #MQTTStorePacketForRetransmit function when using #MQTTStorePacketForRetransmit.
 */
typedef struct MQTTVec MQTTVec_t;

/**
 * @ingroup mqtt_callback_types
 * @brief Application provided function to query the time elapsed since a given
 * epoch in milliseconds.
 *
 * @note The timer should be a monotonic timer. It just needs to provide an
 * incrementing count of milliseconds elapsed since a given epoch.
 *
 * @note As the timer is supposed to be a millisecond timer returning a 32-bit
 * value, it will overflow in just under 50 days. But it will not cause any issues
 * in the library as the time function is only used for calculating durations for
 * timeouts and keep alive periods. The difference in unsigned numbers is
 * used where unsigned wrap around is defined. Unless the timeout is bigger than
 * 100 days (50*2) where the numbers can wrap around more than once the code
 * should work properly.
 *
 * @return The time elapsed in milliseconds.
 */
typedef uint32_t (* MQTTGetCurrentTimeFunc_t )( void );

/**
 * @ingroup mqtt_callback_types
 * @brief Application callback for receiving incoming publishes and incoming
 * acks, as well as adding properties to outgoing publish acks.
 *
 * @note This callback will be called only if packets are deserialized with a
 * result of #MQTTSuccess or #MQTTServerRefused. The latter can be obtained
 * when deserializing a SUBACK indicating a broker's rejection of a subscribe,
 * or a CONNACK indicating a broker's rejection of a connection.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pPacketInfo Information on the type of incoming MQTT packet.
 * @param[in] pDeserializedInfo Deserialized information from incoming packet.
 * @param[out] pReasonCode Reason code for the incoming packet.
 * @param[out] pSendPropsBuffer Properties to be sent in the outgoing packet.
 * @param[in] pGetPropsBuffer Properties to be received in the incoming packet.
 *
 * @note Get optional properties of incoming packets by calling these functions:
 *
 *
 * - Connack Properties:
 *  - #MQTTPropGet_SessionExpiry
 *  - #MQTTPropGet_ConnReceiveMax
 *  - #MQTTPropGet_ConnMaxQos
 *  - #MQTTPropGet_ConnRetainAvailable
 *  - #MQTTPropGet_ConnMaxPacketSize
 *  - #MQTTPropGet_ConnClientId
 *  - #MQTTPropGet_ConnTopicAliasMax
 *  - #MQTTPropGet_ReasonString
 *  - #MQTTPropGet_UserProp
 *  - #MQTTPropGet_ConnWildcard
 *  - #MQTTPropGet_ConnSubId
 *  - #MQTTPropGet_ConnSharedSubAvailable
 *  - #MQTTPropGet_ConnServerKeepAlive
 *  - #MQTTPropGet_ConnResponseInfo
 *  - #MQTTPropGet_ServerRef
 *  - #MQTTPropGet_ConnAuthMethod
 *  - #MQTTPropGet_ConnAuthData
 *
 * - Publish Properties:
 *  - #MQTTPropGet_PubTopicAlias
 *  - #MQTTPropGet_PubPayloadFormatIndicator
 *  - #MQTTPropGet_PubResponseTopic
 *  - #MQTTPropGet_PubCorrelationData
 *  - #MQTTPropGet_PubMessageExpiryInterval
 *  - #MQTTPropGet_PubContentType
 *  - #MQTTPropGet_PubSubscriptionId
 *  - #MQTTPropGet_UserProp
 *
 * - Ack Properties (PUBACK, PUBREC, PUBREL, PUBCOMP, SUBACK, UNSUBACK):
 *  - #MQTTPropGet_ReasonString
 *  - #MQTTPropGet_UserProp
 *
 * - Disconnect Properties:
 *  - #MQTTPropGet_SessionExpiry
 *  - #MQTTPropGet_ReasonString
 *  - #MQTTPropGet_UserProp
 *  - #MQTTPropGet_ServerRef
 *
 * @note Add optional properties to outgoing publish ack packets by calling these functions:
 *
 * - #MQTTPropAdd_UserProp
 * - #MQTTPropAdd_ReasonString
 * @return
 * - true Event callback was able to process the packet
 * - false This is not an error but just a flag that tells
 *          the user that the eventcallback was unable to process
 *          a packet due to application specific reasons.
 *          The application should recall the processloop after
 *          making sure that it would be able to process the
 *          received packet again.
 */
typedef bool (* MQTTEventCallback_t )( struct MQTTContext * pContext,
                                       struct MQTTPacketInfo * pPacketInfo,
                                       struct MQTTDeserializedInfo * pDeserializedInfo,
                                       enum MQTTSuccessFailReasonCode * pReasonCode,
                                       struct MQTTPropBuilder * pSendPropsBuffer,
                                       struct MQTTPropBuilder * pGetPropsBuffer );

/**
 * @brief User defined callback used to store outgoing publishes. Used to track any publish
 * retransmit on an unclean session connection.
 *
 * @param[in] pContext Initialised MQTT Context.
 * @param[in] packetId Outgoing publish packet identifier.
 * @param[in] pMqttVec Pointer to the opaque mqtt vector structure. Users should use MQTT_GetBytesInMQTTVec
 *                and MQTT_SerializeMQTTVec functions to get the memory required and to serialize the
 *                MQTTVec_t in the provided memory respectively.
 *
 * @return True if the copy is successful else false.
 */
/* @[define_mqtt_retransmitstorepacket] */
typedef bool ( * MQTTStorePacketForRetransmit)( struct MQTTContext * pContext,
                                                uint16_t packetId,
                                                MQTTVec_t * pMqttVec );
/* @[define_mqtt_retransmitstorepacket] */

/**
 * @brief User defined callback used to retreive a copied publish for resend operation. Used to
 * track any publish retransmit on an unclean session connection.
 *
 * @param[in] pContext Initialised MQTT Context.
 * @param[in] packetId Copied publish packet identifier.
 * @param[out] pSerializedMqttVec Output parameter to store the pointer to the serialized MQTTVec_t
 *                  using MQTT_SerializeMQTTVec.
 * @param[out] pSerializedMqttVecLen Output parameter to return the number of bytes used to store the
 *                  MQTTVec_t. This value should be the same as the one received from MQTT_GetBytesInMQTTVec
 *                  when storing the packet.
 *
 * @return True if the retreive is successful else false.
 */
/* @[define_mqtt_retransmitretrievepacket] */
typedef bool ( * MQTTRetrievePacketForRetransmit)( struct MQTTContext * pContext,
                                                   uint16_t packetId,
                                                   uint8_t ** pSerializedMqttVec,
                                                   size_t * pSerializedMqttVecLen );
/* @[define_mqtt_retransmitretrievepacket] */

/**
 * @brief User defined callback used to clear a particular copied publish packet. Used to
 * track any publish retransmit on an unclean session connection.
 *
 * @param[in] pContext Initialised MQTT Context.
 * @param[in] packetId Copied publish packet identifier.
 */
/* @[define_mqtt_retransmitclearpacket] */
typedef void (* MQTTClearPacketForRetransmit)( struct MQTTContext * pContext,
                                               uint16_t packetId );
/* @[define_mqtt_retransmitclearpacket] */

/**
 * @ingroup mqtt_enum_types
 * @brief Values indicating if an MQTT connection exists.
 */
typedef enum MQTTConnectionStatus
{
    MQTTNotConnected,     /**< @brief MQTT Connection is inactive. */
    MQTTConnected,        /**< @brief MQTT Connection is active. */
    MQTTDisconnectPending /**< @brief MQTT Connection needs to be disconnected as a transport error has occurred. */
} MQTTConnectionStatus_t;

/**
 * @ingroup mqtt_enum_types
 * @brief The state of QoS 1 or QoS 2 MQTT publishes, used in the state engine.
 */
typedef enum MQTTPublishState
{
    MQTTStateNull = 0,  /**< @brief An empty state with no corresponding PUBLISH. */
    MQTTPublishSend,    /**< @brief The library will send an outgoing PUBLISH packet. */
    MQTTPubAckSend,     /**< @brief The library will send a PUBACK for a received PUBLISH. */
    MQTTPubRecSend,     /**< @brief The library will send a PUBREC for a received PUBLISH. */
    MQTTPubRelSend,     /**< @brief The library will send a PUBREL for a received PUBREC. */
    MQTTPubCompSend,    /**< @brief The library will send a PUBCOMP for a received PUBREL. */
    MQTTPubAckPending,  /**< @brief The library is awaiting a PUBACK for an outgoing PUBLISH. */
    MQTTPubRecPending,  /**< @brief The library is awaiting a PUBREC for an outgoing PUBLISH. */
    MQTTPubRelPending,  /**< @brief The library is awaiting a PUBREL for an incoming PUBLISH. */
    MQTTPubCompPending, /**< @brief The library is awaiting a PUBCOMP for an outgoing PUBLISH. */
    MQTTPublishDone     /**< @brief The PUBLISH has been completed. */
} MQTTPublishState_t;

/**
 * @ingroup mqtt_enum_types
 * @brief Packet types used in acknowledging QoS 1 or QoS 2 publishes.
 */
typedef enum MQTTPubAckType
{
    MQTTPuback, /**< @brief PUBACKs are sent in response to a QoS 1 PUBLISH. */
    MQTTPubrec, /**< @brief PUBRECs are sent in response to a QoS 2 PUBLISH. */
    MQTTPubrel, /**< @brief PUBRELs are sent in response to a PUBREC. */
    MQTTPubcomp /**< @brief PUBCOMPs are sent in response to a PUBREL. */
} MQTTPubAckType_t;

/**
 * @ingroup mqtt_enum_types
 * @brief The status codes in the SUBACK response to a subscription request.
 */
typedef enum MQTTSubAckStatus
{
    MQTTSubAckSuccessQos0 = 0x00, /**< @brief Success with a maximum delivery at QoS 0. */
    MQTTSubAckSuccessQos1 = 0x01, /**< @brief Success with a maximum delivery at QoS 1. */
    MQTTSubAckSuccessQos2 = 0x02, /**< @brief Success with a maximum delivery at QoS 2. */
    MQTTSubAckFailure = 0x80      /**< @brief Failure. */
} MQTTSubAckStatus_t;

/**
 * @ingroup mqtt_struct_types
 * @brief An element of the state engine records for QoS 1 or Qos 2 publishes.
 */
typedef struct MQTTPubAckInfo
{
    uint16_t packetId;               /**< @brief The packet ID of the original PUBLISH. */
    MQTTQoS_t qos;                   /**< @brief The QoS of the original PUBLISH. */
    MQTTPublishState_t publishState; /**< @brief The current state of the publish process. */
} MQTTPubAckInfo_t;


/**
 * @ingroup mqtt_struct_types
 * @brief A struct representing an MQTT connection.
 */
typedef struct MQTTContext
{
    /**
     * @brief State engine records for outgoing publishes.
     */
    MQTTPubAckInfo_t * outgoingPublishRecords;

    /**
     * @brief State engine records for incoming publishes.
     */
    MQTTPubAckInfo_t * incomingPublishRecords;

    /**
     * @brief The maximum number of outgoing publish records.
     */
    size_t outgoingPublishRecordMaxCount;

    /**
     * @brief The maximum number of incoming publish records.
     */
    size_t incomingPublishRecordMaxCount;

    /**
     * @brief The transport interface used by the MQTT connection.
     */
    TransportInterface_t transportInterface;

    /**
     * @brief The buffer used in sending and receiving packets from the network.
     */
    MQTTFixedBuffer_t networkBuffer;

    /**
     * @brief The buffer used to store properties for outgoing ack packets.
     */
    MQTTPropBuilder_t ackPropsBuffer;

    /**
     * @brief The next available ID for outgoing MQTT packets.
     */
    uint16_t nextPacketId;

    /**
     * @brief Whether the context currently has a connection to the broker.
     */
    MQTTConnectionStatus_t connectStatus;

    /**
     * @brief Function used to get millisecond timestamps.
     */
    MQTTGetCurrentTimeFunc_t getTime;

    /**
     * @brief Callback function used to give deserialized MQTT packets to the application.
     */
    MQTTEventCallback_t appCallback;

    /**
     * @brief Timestamp of the last packet sent by the library.
     */
    uint32_t lastPacketTxTime;

    /**
     * @brief Timestamp of the last packet received by the library.
     */
    uint32_t lastPacketRxTime;

    /**
     * @brief Whether the library sent a packet during a call of #MQTT_ProcessLoop or
     * #MQTT_ReceiveLoop.
     */
    bool controlPacketSent;

    /**
     * @brief Index to keep track of the number of bytes received in network buffer.
     */
    size_t index;

    /* Keep alive members. */
    uint16_t keepAliveIntervalSec; /**< @brief Keep Alive interval. */
    uint32_t pingReqSendTimeMs;    /**< @brief Timestamp of the last sent PINGREQ. */
    bool waitingForPingResp;       /**< @brief If the library is currently awaiting a PINGRESP. */

    /**
     * @brief Persistent Connection Properties, populated in the CONNECT and the CONNACK.
     */
    MQTTConnectionProperties_t connectionProperties;

    /**
     * @brief User defined API used to store outgoing publishes.
     */
    MQTTStorePacketForRetransmit storeFunction;

    /**
     * @brief User defined API used to retreive a copied publish for resend operation.
     */
    MQTTRetrievePacketForRetransmit retrieveFunction;

    /**
     * @brief User defined API used to clear a particular copied publish packet.
     */
    MQTTClearPacketForRetransmit clearFunction;
} MQTTContext_t;

/**
 * @ingroup mqtt_struct_types
 * @brief Struct to hold deserialized packet information for an #MQTTEventCallback_t
 * callback.
 */
typedef struct MQTTDeserializedInfo
{
    uint16_t packetIdentifier;          /**< @brief Packet ID of deserialized packet. */
    MQTTPublishInfo_t * pPublishInfo;   /**< @brief Pointer to deserialized publish info. */
    MQTTStatus_t deserializationResult; /**< @brief Return code of deserialization. */
    MQTTReasonCodeInfo_t * pReasonCode; /**< @brief Pointer to deserialized ack info. */
} MQTTDeserializedInfo_t;

/**
 * @brief Initialize an MQTT context.
 *
 * This function must be called on an #MQTTContext_t before any other function.
 *
 * @note The #MQTTGetCurrentTimeFunc_t function for querying time must be defined. If
 * there is no time implementation, it is the responsibility of the application
 * to provide a dummy function to always return 0, provide 0 timeouts for
 * all calls to #MQTT_Connect, #MQTT_ProcessLoop, and #MQTT_ReceiveLoop and configure
 * the #MQTT_RECV_POLLING_TIMEOUT_MS and #MQTT_SEND_TIMEOUT_MS configurations
 * to be 0. This will result in loop functions running for a single iteration, and
 * #MQTT_Connect relying on #MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT to receive the CONNACK packet.
 *
 * @param[in] pContext The context to initialize.
 * @param[in] pTransportInterface The transport interface to use with the context.
 * @param[in] getTimeFunction The time utility function which can return the amount of time
 *    (in milliseconds) elapsed since a given epoch. This function will be used to ensure that
 *    timeouts in the API calls are met and keep-alive messages are sent on time.
 * @param[in] userCallback The user callback to use with the context to notify about incoming
 *     packet events.
 * @param[in] pNetworkBuffer Network buffer provided for the context. This buffer will be used
 *     to receive incoming messages from the broker. This buffer must remain valid and in scope
 *     for the entire lifetime of the @p pContext and must not be used by another context and/or
 *     application.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSuccess otherwise.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Function for obtaining a timestamp.
 * uint32_t getTimeStampMs();
 * // Callback function for receiving packets.
 * void eventCallback(
 *      MQTTContext_t * pContext,
 *      MQTTPacketInfo_t * pPacketInfo,
 *      MQTTDeserializedInfo_t * pDeserializedInfo
 * );
 * // Network send.
 * int32_t networkSend( NetworkContext_t * pContext, const void * pBuffer, size_t bytes );
 * // Network receive.
 * int32_t networkRecv( NetworkContext_t * pContext, void * pBuffer, size_t bytes );
 *
 * MQTTContext_t mqttContext;
 * TransportInterface_t transport;
 * MQTTFixedBuffer_t fixedBuffer;
 * // Create a globally accessible buffer which remains in scope for the entire duration
 * // of the MQTT context.
 * uint8_t buffer[ 1024 ];
 *
 * // Clear context.
 * memset( ( void * ) &mqttContext, 0x00, sizeof( MQTTContext_t ) );
 *
 * // Set transport interface members.
 * transport.pNetworkContext = &someTransportContext;
 * transport.send = networkSend;
 * transport.recv = networkRecv;
 *
 * // Set buffer members.
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = 1024;
 *
 * status = MQTT_Init( &mqttContext, &transport, getTimeStampMs, eventCallback, &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // Do something with mqttContext. The transport and fixedBuffer structs were
 *      // copied into the context, so the original structs do not need to stay in scope.
 *      // However, the memory pointed to by the fixedBuffer.pBuffer must remain in scope.
 * }
 * @endcode
 */
/* @[declare_mqtt_init] */
MQTTStatus_t MQTT_Init( MQTTContext_t * pContext,
                        const TransportInterface_t * pTransportInterface,
                        MQTTGetCurrentTimeFunc_t getTimeFunction,
                        MQTTEventCallback_t userCallback,
                        const MQTTFixedBuffer_t * pNetworkBuffer );
/* @[declare_mqtt_init] */

/**
 * @brief Initialize an MQTT context for QoS > 0.
 *
 * This function must be called on an #MQTTContext_t after MQTT_Init and before any other function.
 *
 * @param[in] pContext The context to initialize.
 * @param[in] pOutgoingPublishRecords Pointer to memory which will be used to store state of outgoing
 * publishes.
 * @param[in] outgoingPublishCount Maximum number of records which can be kept in the memory
 * pointed to by @p pOutgoingPublishRecords.
 * @param[in] pIncomingPublishRecords Pointer to memory which will be used to store state of incoming
 * publishes.
 * @param[in] incomingPublishCount Maximum number of records which can be kept in the memory
 * pointed to by @p pIncomingPublishRecords.
 * @param[in] pAckPropsBuf Pointer to memory which will be used to store properties of outgoing publish-ACKS.
 * @param[in] ackPropsBufLength Length of the buffer pointed to by @p pBuffer.
 * @return #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSuccess otherwise.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Function for obtaining a timestamp.
 * uint32_t getTimeStampMs();
 * // Callback function for receiving packets.
 * void eventCallback(
 *      MQTTContext_t * pContext,
 *      MQTTPacketInfo_t * pPacketInfo,
 *      MQTTDeserializedInfo_t * pDeserializedInfo
 * );
 * // Network send.
 * int32_t networkSend( NetworkContext_t * pContext, const void * pBuffer, size_t bytes );
 * // Network receive.
 * int32_t networkRecv( NetworkContext_t * pContext, void * pBuffer, size_t bytes );
 *
 * MQTTContext_t mqttContext;
 * TransportInterface_t transport;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ 1024 ];
 * const size_t outgoingPublishCount = 30;
 * MQTTPubAckInfo_t outgoingPublishes[ outgoingPublishCount ];
 *
 * // Clear context.
 * memset( ( void * ) &mqttContext, 0x00, sizeof( MQTTContext_t ) );
 *
 * // Set transport interface members.
 * transport.pNetworkContext = &someTransportContext;
 * transport.send = networkSend;
 * transport.recv = networkRecv;
 *
 * // Set buffer members.
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = 1024;
 *
 * status = MQTT_Init( &mqttContext, &transport, getTimeStampMs, eventCallback, &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // We do not expect any incoming publishes in this example, therefore the incoming
 *      // publish pointer is NULL and the count is zero.
 *      // The buffer is used to store properties of outgoing publish-ACKS.
 *      uint8_t ackPropsBuf[ 500 ];
 *      size_t ackPropsBufLength = 500 ;
 *      status = MQTT_InitStatefulQoS( &mqttContext, outgoingPublishes, outgoingPublishCount, NULL, 0 , ackPropsBuf, ackPropsBufLength );
 *
 *      // Now QoS1 and/or QoS2 publishes can be sent with this context.
 * }
 * @endcode
 */
/* @[declare_mqtt_initstatefulqos] */
MQTTStatus_t MQTT_InitStatefulQoS( MQTTContext_t * pContext,
                                   MQTTPubAckInfo_t * pOutgoingPublishRecords,
                                   size_t outgoingPublishCount,
                                   MQTTPubAckInfo_t * pIncomingPublishRecords,
                                   size_t incomingPublishCount,
                                   uint8_t * pAckPropsBuf,
                                   size_t ackPropsBufLength );
/* @[declare_mqtt_initstatefulqos] */

/**
 * @brief Initialize an MQTT context for publish retransmits for QoS > 0.
 *
 * This function must be called on an #MQTTContext_t after MQTT_InitstatefulQoS and before any other function.
 *
 * @param[in] pContext The context to initialize.
 * @param[in] storeFunction User defined API used to store outgoing publishes.
 * @param[in] retrieveFunction User defined API used to retreive a copied publish for resend operation.
 * @param[in] clearFunction User defined API used to clear a particular copied publish packet.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSuccess otherwise.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Function for obtaining a timestamp.
 * uint32_t getTimeStampMs();
 * // Callback function for receiving packets.
 * void eventCallback(
 *      MQTTContext_t * pContext,
 *      MQTTPacketInfo_t * pPacketInfo,
 *      MQTTDeserializedInfo_t * pDeserializedInfo
 * );
 * // Network send.
 * int32_t networkSend( NetworkContext_t * pContext, const void * pBuffer, size_t bytes );
 * // Network receive.
 * int32_t networkRecv( NetworkContext_t * pContext, void * pBuffer, size_t bytes );
 * // User defined callback used to store outgoing publishes
 * bool publishStoreCallback(struct MQTTContext* pContext,
 *                           uint16_t packetId,
 *                           MQTTVec_t* pIoVec);
 * // User defined callback used to retreive a copied publish for resend operation
 * bool publishRetrieveCallback(struct MQTTContext* pContext,
 *                              uint16_t packetId,
 *                              TransportOutVector_t** pIoVec,
 *                              size_t* ioVecCount);
 * // User defined callback used to clear a particular copied publish packet
 * bool publishClearCallback(struct MQTTContext* pContext,
 *                           uint16_t packetId);
 * // User defined callback used to clear all copied publish packets
 * bool publishClearAllCallback(struct MQTTContext* pContext);
 *
 * MQTTContext_t mqttContext;
 * TransportInterface_t transport;
 * MQTTFixedBuffer_t fixedBuffer;
 * uint8_t buffer[ 1024 ];
 * const size_t outgoingPublishCount = 30;
 * MQTTPubAckInfo_t outgoingPublishes[ outgoingPublishCount ];
 *
 * // Clear context.
 * memset( ( void * ) &mqttContext, 0x00, sizeof( MQTTContext_t ) );
 *
 * // Set transport interface members.
 * transport.pNetworkContext = &someTransportContext;
 * transport.send = networkSend;
 * transport.recv = networkRecv;
 *
 * // Set buffer members.
 * fixedBuffer.pBuffer = buffer;
 * fixedBuffer.size = 1024;
 *
 * status = MQTT_Init( &mqttContext, &transport, getTimeStampMs, eventCallback, &fixedBuffer );
 *
 * if( status == MQTTSuccess )
 * {
 *      // We do not expect any incoming publishes in this example, therefore the incoming
 *      // publish pointer is NULL and the count is zero.
 *      status = MQTT_InitStatefulQoS( &mqttContext, outgoingPublishes, outgoingPublishCount, NULL, 0 );
 *
 *      // Now QoS1 and/or QoS2 publishes can be sent with this context.
 * }
 *
 * if( status == MQTTSuccess )
 * {
 *      status = MQTT_InitRetransmits( &mqttContext, publishStoreCallback,
 *                                                   publishRetrieveCallback,
 *                                                   publishClearCallback,
 *                                                   publishClearAllCallback );
 *
 *      // Now unacked Publishes can be resent on an unclean session resumption.
 * }
 * @endcode
 */

/* @[declare_mqtt_initretransmits] */
MQTTStatus_t MQTT_InitRetransmits( MQTTContext_t * pContext,
                                   MQTTStorePacketForRetransmit storeFunction,
                                   MQTTRetrievePacketForRetransmit retrieveFunction,
                                   MQTTClearPacketForRetransmit clearFunction );
/* @[declare_mqtt_initretransmits] */

/**
 * @brief Checks the MQTT connection status with the broker.
 *
 * @param[in] pContext Initialized MQTT context.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTStatusConnected if the MQTT connection is established with the broker.<br>
 * #MQTTStatusNotConnected if the MQTT connection is broker.<br>
 * #MQTTStatusDisconnectPending if Transport Interface has failed and MQTT connection needs to be closed.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * @endcode
 */
/* @[declare_mqtt_checkconnectstatus] */
MQTTStatus_t MQTT_CheckConnectStatus( const MQTTContext_t * pContext );
/* @[declare_mqtt_checkconnectstatus] */

/**
 * @brief Establish an MQTT session.
 *
 * This function will send MQTT CONNECT packet and receive a CONNACK packet. The
 * send and receive from the network is done through the transport interface.
 *
 * The maximum time this function waits for a CONNACK is decided in one of the
 * following ways:
 * 1. If @p timeoutMs is greater than 0:
 *    #MQTTContext_t.getTime is used to ensure that the function does not wait
 *    more than @p timeoutMs for CONNACK.
 * 2. If @p timeoutMs is 0:
 *    The network receive for CONNACK is retried up to the number of times
 *    configured by #MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT.
 *
 * @note If a dummy #MQTTGetCurrentTimeFunc_t was passed to #MQTT_Init, then a
 * timeout value of 0 MUST be passed to the API, and the #MQTT_RECV_POLLING_TIMEOUT_MS
 * and #MQTT_SEND_TIMEOUT_MS timeout configurations MUST be set to 0.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pConnectInfo MQTT CONNECT packet information.
 * @param[in] pWillInfo Last Will and Testament. Pass NULL if Last Will and
 * Testament is not used.
 * @param[in] timeoutMs Maximum time in milliseconds to wait for a CONNACK packet.
 * A zero timeout makes use of the retries for receiving CONNACK as configured with
 * #MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT.
 * @param[in] pPropertyBuilder Properties to be sent in the outgoing packet.
 * @param[in] pWillPropertyBuilder Will Properties to be sent in the outgoing packet.
 * @param[out] pSessionPresent This value will be set to true if a previous session
 * was present; otherwise it will be set to false. It is only relevant if not
 * establishing a clean session.
 *
 * @return
 * #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSendFailed if transport send failed;<br>
 * #MQTTRecvFailed if transport receive failed for CONNACK;<br>
 * #MQTTNoDataAvailable if no data available to receive in transport until
 * the @p timeoutMs for CONNACK;<br>
 * #MQTTStatusConnected if the connection is already established<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTPublishRetrieveFailed if on an unclean session connection, the copied
 * publishes are not retrieved successfully for retransmission<br>
 * #MQTTBadResponse if the received CONNACK packet is malformed<br>
 * #MQTTServerRefused if the server refuses the connection in the CONNACK.<br>
 * #MQTTEventCallbackFailed if the user defined callback fails.<br>
 * #MQTTSuccess otherwise.<br>
 *
 * @note This API may spend more time than provided in the timeoutMS parameters in
 * certain conditions as listed below:
 *
 * 1. Timeouts are incorrectly configured - If the timeoutMS is less than the
 *    transport receive timeout and if a CONNACK packet is not received within
 *    the transport receive timeout, the API will spend the transport receive
 *    timeout (which is more time than the timeoutMs). It is the case of incorrect
 *    timeout configuration as the timeoutMs parameter passed to this API must be
 *    greater than the transport receive timeout. Please refer to the transport
 *    interface documentation for more details about timeout configurations.
 *
 * 2. Partial CONNACK packet is received right before the expiry of the timeout - It
 *    is possible that first two bytes of CONNACK packet (packet type and remaining
 *    length) are received right before the expiry of the timeoutMS. In that case,
 *    the API makes one more network receive call in an attempt to receive the remaining
 *    2 bytes. In the worst case, it can happen that the remaining 2 bytes are never
 *    received and this API will end up spending timeoutMs + transport receive timeout.
 *
 * Functions to add optional properties to the CONNECT packet are:
 *
 * Connect Properties:
 * - #MQTTPropAdd_SessionExpiry
 * - #MQTTPropAdd_ConnReceiveMax
 * - #MQTTPropAdd_ConnMaxPacketSize
 * - #MQTTPropAdd_ConnTopicAliasMax
 * - #MQTTPropAdd_ConnRequestRespInfo
 * - #MQTTPropAdd_ConnRequestProbInfo
 * - #MQTTPropAdd_UserProp
 * - #MQTTPropAdd_ConnAuthMethod
 * - #MQTTPropAdd_ConnAuthData
 *
 * Will Properties:
 * - #MQTTPropAdd_WillDelayInterval
 * - #MQTTPropAdd_PubPayloadFormat
 * - #MQTTPropAdd_PubMessageExpiry
 * - #MQTTPropAdd_PubResponseTopic
 * - #MQTTPropAdd_PubCorrelationData
 * - #MQTTPropAdd_PubContentType
 * - #MQTTPropAdd_UserProp
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTConnectInfo_t connectInfo = { 0 };
 * MQTTPublishInfo_t willInfo = { 0 };
 * bool sessionPresent;
 * // This is assumed to have been initialized before calling this function.
 * MQTTContext_t * pContext;
 *
 * // True for creating a new session with broker, false if we want to resume an old one.
 * connectInfo.cleanSession = true;
 * // Client ID must be unique to broker. This field is required.
 * connectInfo.pClientIdentifier = "someClientID";
 * connectInfo.clientIdentifierLength = strlen( connectInfo.pClientIdentifier );
 *
 * // The following fields are optional.
 * // Value for keep alive.
 * connectInfo.keepAliveSeconds = 60;
 * // Optional username and password.
 * connectInfo.pUserName = "someUserName";
 * connectInfo.userNameLength = strlen( connectInfo.pUserName );
 * connectInfo.pPassword = "somePassword";
 * connectInfo.passwordLength = strlen( connectInfo.pPassword );
 *  // Optional properties to be sent in the CONNECT packet.
 * MQTTPropBuilder_t connectPropsBuilder;
 * uint8_t connectPropsBuffer[ 100 ];
 * size_t connectPropsBufferLength = sizeof( connectPropsBuffer );
 * status = MQTTPropertyBuilder_Init( &connectPropsBuilder, connectPropsBuffer, connectPropsBufferLength );
 *
 *   // Set a property in the connectPropsBuilder
 * uint32_t maxPacketSize = 100 ;
 * status = MQTTPropAdd_ConnMaxPacketSize(&connectPropsBuilder, maxPacketSize);
 *
 * // The last will and testament is optional, it will be published by the broker
 * // should this client disconnect without sending a DISCONNECT packet.
 * willInfo.qos = MQTTQoS0;
 * willInfo.pTopicName = "/lwt/topic/name";
 * willInfo.topicNameLength = strlen( willInfo.pTopicName );
 * willInfo.pPayload = "LWT Message";
 * willInfo.payloadLength = strlen( "LWT Message" );
 *  // Optional Will Properties to be sent in the CONNECT packet.
 * MQTTPropBuilder_t willPropsBuilder;
 * uint8_t willPropsBuffer[ 100 ];
 * size_t willPropsBufferLength = sizeof( willPropsBuffer );
 * status = MQTTPropertyBuilder_Init( &willPropsBuilder, willPropsBuffer, willPropsBufferLength );
 *
 * // Set a property in the willPropsBuilder
 * status = MQTTPropAdd_PubPayloadFormat( &willPropsBuilder, 1);
 *
 * // Send the connect packet. Use 100 ms as the timeout to wait for the CONNACK packet.
 * status = MQTT_Connect( pContext, &connectInfo, &willInfo, 100, &sessionPresent, &connectPropsBuilder, &willPropsBuilder );
 *
 * if( status == MQTTSuccess )
 * {
 *      // Since we requested a clean session, this must be false
 *      assert( sessionPresent == false );
 *
 *      // Do something with the connection.
 * }
 * @endcode
 */
/* @[declare_mqtt_connect] */
MQTTStatus_t MQTT_Connect( MQTTContext_t * pContext,
                           const MQTTConnectInfo_t * pConnectInfo,
                           const MQTTPublishInfo_t * pWillInfo,
                           uint32_t timeoutMs,
                           bool * pSessionPresent,
                           const MQTTPropBuilder_t * pPropertyBuilder,
                           const MQTTPropBuilder_t * pWillPropertyBuilder );
/* @[declare_mqtt_connect] */

/**
 * @brief Sends MQTT SUBSCRIBE for the given list of topic filters to
 * the broker.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pSubscriptionList Array of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in @ pSubscriptionList
 * array.
 * @param[in] packetId Packet ID generated by #MQTT_GetPacketId.
 * @param[in] pPropertyBuilder Properties to be sent in the outgoing packet.
 * @return
 * #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTBadResponse if there is an error in property parsing;<br>
 * #MQTTSendFailed if transport write failed;<br>
 * #MQTTStatusNotConnected if the connection is not established yet<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTSuccess otherwise.<br>
 *
 * Functions to add optional properties to the SUBSCRIBE packet are:
 *
 * - #MQTTPropAdd_SubscribeId
 * - #MQTTPropAdd_UserProp
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t subscriptionList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * uint16_t packetId;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
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
 * // Optional Properties to be sent in the SUBSCRIBE packet
 * MQTTPropBuilder_t propertyBuilder;
 * uint8_t propertyBuffer[ 100 ];
 * size_t propertyBufferLength = sizeof( propertyBuffer );
 * status = MQTTPropertyBuilder_Init( &propertyBuilder, propertyBuffer, propertyBufferLength );
 *
 * status = MQTTPropAdd_SubscribeId(&propertyBuilder, 1);
 *
 * // Obtain a new packet id for the subscription.
 * packetId = MQTT_GetPacketId( pContext );
 *
 * status = MQTT_Subscribe( pContext, &subscriptionList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, packetId, propertyBuilder );
 *
 * if( status == MQTTSuccess )
 * {
 *      // We must now call MQTT_ReceiveLoop() or MQTT_ProcessLoop() to receive the SUBACK.
 *      // If the broker accepts the subscription we can now receive publishes
 *      // on the requested topics.
 * }
 * @endcode
 */
/* @[declare_mqtt_subscribe] */

MQTTStatus_t MQTT_Subscribe( MQTTContext_t * pContext,
                             const MQTTSubscribeInfo_t * pSubscriptionList,
                             size_t subscriptionCount,
                             uint16_t packetId,
                             const MQTTPropBuilder_t * pPropertyBuilder );

/* @[declare_mqtt_subscribe] */


/**
 * @brief Publishes a message to the given topic name.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pPublishInfo MQTT PUBLISH packet parameters.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] pPropertyBuilder Properties to be sent in the outgoing packet.
 *
 * @return
 * #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTBadResponse if there is an error in property parsing;<br>
 * #MQTTSendFailed if transport write failed;<br>
 * #MQTTStatusNotConnected if the connection is not established yet<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTPublishStoreFailed if the user provided callback to copy and store the
 * outgoing publish packet fails<br>
 * #MQTTSuccess otherwise.<br>
 *
 * Functions to add optional properties to the PUBLISH packet are:
 *
 * - #MQTTPropAdd_PubPayloadFormat
 * - #MQTTPropAdd_PubMessageExpiry
 * - #MQTTPropAdd_PubTopicAlias
 * - #MQTTPropAdd_PubResponseTopic
 * - #MQTTPropAdd_PubCorrelationData
 * - #MQTTPropAdd_PubContentType
 * - #MQTTPropAdd_UserProp
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTPublishInfo_t publishInfo;
 * uint16_t packetId;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
 *
 * // QoS of publish.
 * publishInfo.qos = MQTTQoS1;
 * publishInfo.pTopicName = "/some/topic/name";
 * publishInfo.topicNameLength = strlen( publishInfo.pTopicName );
 * publishInfo.pPayload = "Hello World!";
 * publishInfo.payloadLength = strlen( "Hello World!" );
 * // Optional properties to be sent in the PUBLISH packet.
 * MQTTPropBuilder_t propertyBuilder;
 * uint8_t propertyBuffer[ 100 ];
 * size_t propertyBufferLength = sizeof( propertyBuffer );
 * status = MQTTPropertyBuilder_Init( &propertyBuilder, propertyBuffer, propertyBufferLength );
 *
 * // Set a property in the propertyBuilder
 * status = MQTTPropAdd_PubPayloadFormat( &propertyBuilder, 1);
 *
 * // Packet ID is needed for QoS > 0.
 * packetId = MQTT_GetPacketId( pContext );
 *
 * status = MQTT_Publish( pContext, &publishInfo, packetId, &propertyBuilder );
 *
 * if( status == MQTTSuccess )
 * {
 *      // Since the QoS is > 0, we will need to call MQTT_ReceiveLoop()
 *      // or MQTT_ProcessLoop() to process the publish acknowledgments.
 * }
 * @endcode
 */
/* @[declare_mqtt_publish] */
MQTTStatus_t MQTT_Publish( MQTTContext_t * pContext,
                           const MQTTPublishInfo_t * pPublishInfo,
                           uint16_t packetId,
                           const MQTTPropBuilder_t * pPropertyBuilder );
/* @[declare_mqtt_publish] */

/**
 * @brief Cancels an outgoing publish callback (only for QoS > QoS0) by
 * removing it from the pending ACK list.
 *
 * @note This cannot cancel the actual publish as that might have already
 * been sent to the broker. This only removes the details of the given packet
 * ID from the list of unACKed packet. That allows the caller to free any memory
 * associated with the publish payload, topic string etc. Also, after this API
 * call, the user provided callback will not be invoked when the ACK packet is
 * received.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] packetId packet ID corresponding to the outstanding publish.
 *
 * @return #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSuccess otherwise.<br>
 */
/* @[declare_mqtt_cancelcallback] */
MQTTStatus_t MQTT_CancelCallback( const MQTTContext_t * pContext,
                                  uint16_t packetId );
/* @[declare_mqtt_cancelcallback] */

/**
 * @brief Sends an MQTT PINGREQ to broker.
 *
 * @param[in] pContext Initialized and connected MQTT context.
 *
 * @return
 * #MQTTNoMemory if pBuffer is too small to hold the MQTT packet;<br>
 * #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTSendFailed if transport write failed;<br>
 * #MQTTStatusNotConnected if the connection is not established yet<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTSuccess otherwise.<br>
 */
/* @[declare_mqtt_ping] */
MQTTStatus_t MQTT_Ping( MQTTContext_t * pContext );
/* @[declare_mqtt_ping] */

/**
 * @brief Sends MQTT UNSUBSCRIBE for the given list of topic filters to
 * the broker.
 *
 * @param[in] pContext Initialized MQTT context.
 * @param[in] pSubscriptionList List of MQTT subscription info.
 * @param[in] subscriptionCount The number of elements in pSubscriptionList.
 * @param[in] packetId packet ID generated by #MQTT_GetPacketId.
 * @param[in] pPropertyBuilder Properties to be sent in the outgoing packet.
 *
 * @return
 * - #MQTTBadParameter if invalid parameters are passed;<br>
 * - #MQTTBadResponse if invalid properties are parsed;<br>
 * - #MQTTSendFailed if transport write failed;<br>
 * - #MQTTStatusNotConnected if the connection is not established yet<br>
 * - #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * - #MQTTSuccess otherwise.<br>
 *
 * Functions to add optional properties to the UNSUBSCRIBE packet are:
 *
 * - #MQTTPropAdd_UserProp
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * MQTTSubscribeInfo_t unsubscribeList[ NUMBER_OF_SUBSCRIPTIONS ] = { 0 };
 * uint16_t packetId;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
 * // This is assumed to be a list of filters we want to unsubscribe from.
 * const char * filters[ NUMBER_OF_SUBSCRIPTIONS ];
 *
 * // Set information for each unsubscribe request.
 * for( int i = 0; i < NUMBER_OF_SUBSCRIPTIONS; i++ )
 * {
 *      unsubscribeList[ i ].pTopicFilter = filters[ i ];
 *      unsubscribeList[ i ].topicFilterLength = strlen( filters[ i ] );
 *
 *      // The QoS field of MQTT_SubscribeInfo_t is unused for unsubscribing.
 * }
 *
 * // Obtain a new packet id for the unsubscribe request.
 * packetId = MQTT_GetPacketId( pContext );
 * // Optional properties to be sent in the UNSUBSCRIBE packet.
 * MQTTPropBuilder_t propertyBuilder;
 * uint8_t propertyBuffer[ 100 ];
 * size_t propertyBufferLength = sizeof( propertyBuffer );
 * status = MQTTPropertyBuilder_Init( &propertyBuilder, propertyBuffer, propertyBufferLength );
 *
 * // Set a property in the propertyBuilder
 * MQTTUserProperty_t userProperty;
 * userProperty.pKey = "key";
 * userProperty.keyLength = strlen( userProperty.pKey );
 * userProperty.pValue = "value";
 * *userProperty.valueLength = strlen( userProperty.pValue );
 *
 * status = MQTTPropAdd_UserProp( &propertyBuilder,&userProperty);
 *
 * status = MQTT_Unsubscribe( pContext, &unsubscribeList[ 0 ], NUMBER_OF_SUBSCRIPTIONS, packetId, &propertyBuilder );
 *
 * if( status == MQTTSuccess )
 * {
 *      // We must now call MQTT_ReceiveLoop() or MQTT_ProcessLoop() to receive the UNSUBACK.
 *      // After this the broker should no longer send publishes for these topics.
 * }
 * @endcode
 */
/* @[declare_mqtt_unsubscribe] */
MQTTStatus_t MQTT_Unsubscribe( MQTTContext_t * pContext,
                               const MQTTSubscribeInfo_t * pSubscriptionList,
                               size_t subscriptionCount,
                               uint16_t packetId,
                               const MQTTPropBuilder_t * pPropertyBuilder );
/* @[declare_mqtt_unsubscribe] */

/**
 * @brief Sends MQTT DISCONNECT for a given reason code
 *
 * @param[in] pContext Initialized and connected MQTT context.
 * @param[in] pPropertyBuilder Properties to be sent in the outgoing packet.
 * @param[in] reasonCode Reason code to be sent in the DISCONNECT packet.
 *
 * @return
 * #MQTTBadParameter if invalid parameters are passed;<br>
 * #MQTTBadResponse if invalid properties are parsed;<br>
 * #MQTTSendFailed if transport send failed;<br>
 * #MQTTStatusNotConnected if the connection is not established yet and a PING
 * or an ACK is being sent.<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTSuccess otherwise.<br>
 *
 * Functions to add optional properties  to the DISCONNECT packet are:
 *
 * - #MQTTPropAdd_SessionExpiry
 * - #MQTTPropAdd_ReasonString
 * - #MQTTPropAdd_UserProp
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
 * // Optional properties to be sent in the DISCONNECT packet.
 * MQTTPropBuilder_t propertyBuilder;
 * uint8_t propertyBuffer[ 100 ];
 * size_t propertyBufferLength = sizeof( propertyBuffer );
 * status = MQTTPropertyBuilder_Init( &propertyBuilder, propertyBuffer, propertyBufferLength );
 *
 * // Set a property in the propertyBuilder
 * status = MQTTPropAdd_ReasonString( &propertyBuilder, "Disconnecting", 12);
 *
 * status = MQTT_Disconnect( pContext, &propertyBuilder, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
 *
 * if( status == MQTTSuccess )
 * {
 *      // The DISCONNECT packet was sent successfully. The connection is now closed.
 * }
 * @endcode
 */
/* @[declare_mqtt_disconnect] */
MQTTStatus_t MQTT_Disconnect( MQTTContext_t * pContext,
                              const MQTTPropBuilder_t * pPropertyBuilder,
                              MQTTSuccessFailReasonCode_t reasonCode );
/* @[declare_mqtt_disconnect] */

/**
 * @brief Loop to receive packets from the transport interface. Handles keep
 * alive.
 *
 * @note If a dummy timer function, #MQTTGetCurrentTimeFunc_t, is passed to the library,
 * then the keep-alive mechanism is not supported by the #MQTT_ProcessLoop API.
 * In that case, the #MQTT_ReceiveLoop API function should be used instead.
 *
 * @param[in] pContext Initialized and connected MQTT context.
 *
 * @note Calling this function blocks the calling context for a time period that
 * depends on the passed the configuration macros, #MQTT_RECV_POLLING_TIMEOUT_MS
 * and #MQTT_SEND_TIMEOUT_MS, and the underlying transport interface implementation
 * timeouts, unless an error occurs. The blocking period also depends on the execution time of the
 * #MQTTEventCallback_t callback supplied to the library. It is recommended that the supplied
 * #MQTTEventCallback_t callback does not contain blocking operations to prevent potential
 * non-deterministic blocking period of the #MQTT_ProcessLoop API call.
 *
 * @return #MQTTBadParameter if context is NULL;<br>
 * #MQTTRecvFailed if a network error occurs during reception;<br>
 * #MQTTSendFailed if a network error occurs while sending an ACK or PINGREQ;<br>
 * #MQTTBadResponse if an invalid packet is received;<br>
 * #MQTTKeepAliveTimeout if the server has not sent a PINGRESP before
 * #MQTT_PINGRESP_TIMEOUT_MS milliseconds;<br>
 * #MQTTIllegalState if an incoming QoS 1/2 publish or ack causes an
 * invalid transition for the internal state machine;<br>
 * #MQTTNeedMoreBytes if MQTT_ProcessLoop has received
 * incomplete data; it should be called again (probably after a delay);<br>
 * #MQTTStatusNotConnected if the connection is not established yet and a PING
 * or an ACK is being sent.<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTEventCallbackFailed if the user provided #MQTTEventCallback_t callback fails to process the received packet;<br>
 * #MQTTSuccess on success.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
 *
 * while( true )
 * {
 *      status = MQTT_ProcessLoop( pContext );
 *
 *      if( status != MQTTSuccess && status != MQTTNeedMoreBytes )
 *      {
 *          // Determine the error. It's possible we might need to disconnect
 *          // the underlying transport connection.
 *      }
 *      else
 *      {
 *          // Other application functions.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_processloop] */
MQTTStatus_t MQTT_ProcessLoop( MQTTContext_t * pContext );
/* @[declare_mqtt_processloop] */

/**
 * @brief Loop to receive packets from the transport interface. Does not handle
 * keep alive.
 *
 * @note If a dummy #MQTTGetCurrentTimeFunc_t was passed to #MQTT_Init, then the
 * #MQTT_RECV_POLLING_TIMEOUT_MS and #MQTT_SEND_TIMEOUT_MS timeout configurations
 * MUST be set to 0.
 *
 * @param[in] pContext Initialized and connected MQTT context.
 *
 * @note Calling this function blocks the calling context for a time period that
 * depends on the the configuration macros, #MQTT_RECV_POLLING_TIMEOUT_MS and
 * #MQTT_SEND_TIMEOUT_MS, and the underlying transport interface implementation
 * timeouts, unless an error occurs. The blocking period also depends on the execution time of the
 * #MQTTEventCallback_t callback supplied to the library. It is recommended that the supplied
 * #MQTTEventCallback_t callback does not contain blocking operations to prevent potential
 * non-deterministic blocking period of the #MQTT_ReceiveLoop API call.
 *
 * @return #MQTTBadParameter if context is NULL;<br>
 * #MQTTRecvFailed if a network error occurs during reception;<br>
 * #MQTTSendFailed if a network error occurs while sending an ACK or PINGREQ;<br>
 * #MQTTBadResponse if an invalid packet is received;<br>
 * #MQTTKeepAliveTimeout if the server has not sent a PINGRESP before
 * #MQTT_PINGRESP_TIMEOUT_MS milliseconds;<br>
 * #MQTTIllegalState if an incoming QoS 1/2 publish or ack causes an
 * invalid transition for the internal state machine;<br>
 * #MQTTNeedMoreBytes if MQTT_ProcessLoop has received
 * incomplete data; it should be called again (probably after a delay);<br>
 * #MQTTStatusNotConnected if the connection is not established yet and a PING
 * or an ACK is being sent.<br>
 * #MQTTStatusDisconnectPending if the user is expected to call MQTT_Disconnect
 * before calling any other API<br>
 * #MQTTEventCallbackFailed if the user provided #MQTTEventCallback_t callback fails to process the received packet;<br>
 * #MQTTSuccess on success.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * MQTTStatus_t status;
 * uint32_t keepAliveMs = 60 * 1000;
 * // This context is assumed to be initialized and connected.
 * MQTTContext_t * pContext;
 *
 * while( true )
 * {
 *      status = MQTT_ReceiveLoop( pContext );
 *
 *      if( status != MQTTSuccess && status != MQTTNeedMoreBytes )
 *      {
 *          // Determine the error. It's possible we might need to disconnect
 *          // the underlying transport connection.
 *      }
 *      else
 *      {
 *          // Since this function does not send pings, the application may need
 *          // to in order to comply with keep alive.
 *          if( ( pContext->getTime() - pContext->lastPacketTxTime ) > keepAliveMs )
 *          {
 *              status = MQTT_Ping( pContext );
 *          }
 *
 *          // Other application functions.
 *      }
 * }
 * @endcode
 */
/* @[declare_mqtt_receiveloop] */
MQTTStatus_t MQTT_ReceiveLoop( MQTTContext_t * pContext );
/* @[declare_mqtt_receiveloop] */

/**
 * @brief Get a packet ID that is valid according to the MQTT 5.0 spec.
 *
 * @param[in] pContext Initialized MQTT context.
 *
 * @return A non-zero number.
 */
/* @[declare_mqtt_getpacketid] */
uint16_t MQTT_GetPacketId( MQTTContext_t * pContext );
/* @[declare_mqtt_getpacketid] */

/**
 * @brief A utility function that determines whether the passed topic filter and
 * topic name match according to the MQTT 5.0 protocol specification.
 *
 * @param[in] pTopicName The topic name to check.
 * @param[in] topicNameLength Length of the topic name.
 * @param[in] pTopicFilter The topic filter to check.
 * @param[in] topicFilterLength Length of topic filter.
 * @param[out] pIsMatch If the match is performed without any error, that is if the
 * return value is MQTTSuccess, then and only then the value in this parameter is valid
 * and updated. In such a case, if the topic filter and the topic name match, then this
 * value is set to true; otherwise if there is no match then it is set to false.
 *
 * @note The API assumes that the passed topic name is valid to meet the
 * requirements of the MQTT 5.0 specification. Invalid topic names (for example,
 * containing wildcard characters) should not be passed to the function.
 * Also, the API checks validity of topic filter for wildcard characters ONLY if
 * the passed topic name and topic filter do not have an exact string match.
 *
 * @return Returns one of the following:
 * - #MQTTBadParameter, if any of the input parameters is invalid.<br>
 * - #MQTTSuccess, if the matching operation was performed.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Variables used in this example.
 * const char * pTopic = "topic/match/1";
 * const char * pFilter = "topic/#";
 * MQTTStatus_t status = MQTTSuccess;
 * bool match = false;
 *
 * status = MQTT_MatchTopic( pTopic, strlen( pTopic ), pFilter, strlen( pFilter ), &match );
 * // Our parameters were valid, so this will return success.
 * assert( status == MQTTSuccess );
 *
 * // For this specific example, we already know this value is true. This
 * // check is placed here as an example for use with variable topic names.
 * if( match )
 * {
 *      // Application can decide what to do with the matching topic name.
 * }
 * @endcode
 */
MQTTStatus_t MQTT_MatchTopic( const char * pTopicName,
                              const uint16_t topicNameLength,
                              const char * pTopicFilter,
                              const uint16_t topicFilterLength,
                              bool * pIsMatch );

/**
 * @brief Parses the payload of an MQTT SUBACK packet that contains status codes
 * corresponding to topic filter subscription requests from the original
 * subscribe packet.
 *
 * Each return code in the SUBACK packet corresponds to a topic filter in the
 * SUBSCRIBE Packet being acknowledged.
 * The status codes can be one of the following:
 *  - 0x00 - Success - Maximum QoS 0
 *  - 0x01 - Success - Maximum QoS 1
 *  - 0x02 - Success - Maximum QoS 2
 *  These are the reason codes when the server refuses the request-
 *  - 0x80 - Topic Filter Refused
 *  - 0x83 - Implementation specific error.
 *  - 0x87 - Not authorized.
 *  - 0x8F - Invalid Topic Filter.
 *  - 0x91 - Packet identifier in use.
 *  - 0x97 - Quota exceeded.
 *  - 0x9E - Shared subscriptions not supported.
 *  - 0xA1 - Subscription identifiers not supported.
 *  - 0xA2 - Wildcard subscriptions not supported.
 *
 * Refer to #MQTTSubAckStatus_t for the status codes.
 *
 * @param[in] pSubackPacket The SUBACK packet whose payload is to be parsed.
 * @param[out] pPayloadStart This is populated with the starting address
 * of the payload (or return codes for topic filters) in the SUBACK packet.
 * @param[out] pPayloadSize This is populated with the size of the payload
 * in the SUBACK packet. It represents the number of topic filters whose
 * SUBACK status is present in the packet.
 *
 * @return Returns one of the following:
 * - #MQTTBadParameter if the input SUBACK packet is invalid.<br>
 * - #MQTTSuccess if parsing the payload was successful.<br>
 *
 * <b>Example</b>
 * @code{c}
 *
 * // Global variable used in this example.
 * // This is assumed to be the subscription list in the original SUBSCRIBE packet.
 * MQTTSubscribeInfo_t pSubscribes[ NUMBER_OF_SUBSCRIPTIONS ];
 *
 * // MQTT_GetSubAckStatusCodes is intended to be used from the application
 * // callback that is called by the library in MQTT_ProcessLoop or MQTT_ReceiveLoop.
 * bool eventCallback(
 *      MQTTContext_t * pContext,
 *      MQTTPacketInfo_t * pPacketInfo,
 *      MQTTDeserializedInfo_t * pDeserializedInfo
 *      MQTTSuccessFailReasonCode_t * pReasonCode,
 *      MQTTPropBuilder_t * pSendPropsBuffer,
 *      MQTTPropBuilder_t * pGetPropsBuffer
 * )
 * {
 *      MQTTStatus_t status = MQTTSuccess;
 *      uint8_t * pCodes;
 *      size_t numCodes;
 *
 *      if( pPacketInfo->type == MQTT_PACKET_TYPE_SUBACK )
 *      {
 *          status = MQTT_GetSubAckStatusCodes( pPacketInfo, &pCodes, &numCodes );
 *
 *          // Since the pointers to the payload and payload size are not NULL, and
 *          // we use the packet info struct passed to the app callback (verified
 *          // to be valid by the library), this function must return success.
 *          assert( status == MQTTSuccess );
 *          // The server must send a response code for each topic filter in the
 *          // original SUBSCRIBE packet.
 *          assert( numCodes == NUMBER_OF_SUBSCRIPTIONS );
 *
 *          for( int i = 0; i < numCodes; i++ )
 *          {
 *              // The only failure code is 0x80 = MQTTSubAckFailure.
 *              if( pCodes[ i ] == MQTTSubAckFailure )
 *              {
 *                  // The subscription failed, we may want to retry the
 *                  // subscription in pSubscribes[ i ] outside of this callback.
 *              }
 *              else
 *              {
 *                  // The subscription was granted, but the maximum QoS may be
 *                  // lower than what was requested. We can verify the granted QoS.
 *                  if( pSubscribes[ i ].qos != pCodes[ i ] )
 *                  {
 *                      LogWarn( (
 *                          "Requested QoS %u, but granted QoS %u for %s",
 *                          pSubscribes[ i ].qos, pCodes[ i ], pSubscribes[ i ].pTopicFilter
 *                      ) );
 *                  }
 *              }
 *          }
 *      }
 *      // Handle other packet types.
 * }
 * @endcode
 */
/* @[declare_mqtt_getsubackstatuscodes] */

MQTTStatus_t MQTT_GetSubAckStatusCodes( const MQTTPacketInfo_t * pSubackPacket,
                                        uint8_t ** pPayloadStart,
                                        size_t * pPayloadSize );
/* @[declare_mqtt_getsubackstatuscodes] */

/**
 * @brief Error code to string conversion for MQTT statuses.
 *
 * @param[in] status The status to convert to a string.
 *
 * @return The string representation of the status.
 */
/* @[declare_mqtt_status_strerror] */
const char * MQTT_Status_strerror( MQTTStatus_t status );
/* @[declare_mqtt_status_strerror] */

/**
 * @brief Get the bytes in a #MQTTVec pointer which can store the whole array as a an MQTT packet when calling MQTT_SerializeMQTTVec( void * pAllocatedMem, MQTTVec_t *pVec ) function.
 *
 * @param[in] pVec The #MQTTVec pointer given as input to the user defined #MQTTStorePacketForRetransmit callback function. Must not be NULL.
 *
 * @return The bytes in the provided #MQTTVec array which can then be used to set aside memory to be used with MQTT_SerializeMQTTVec( void * pAllocatedMem, MQTTVec_t *pVec ) function.
 */
/* @[declare_mqtt_getbytesinmqttvec] */
size_t MQTT_GetBytesInMQTTVec( const MQTTVec_t * pVec );
/* @[declare_mqtt_getbytesinmqttvec] */

/**
 * @brief Serialize the bytes in an array of #MQTTVec in the provided \p pAllocatedMem
 *
 * @param[in] pAllocatedMem Memory in which to serialize the data in the #MQTTVec array. It must be of size provided by MQTT_GetBytesInMQTTVec( MQTTVec_t *pVec ). Should not be NULL.
 * @param[in] pVec The #MQTTVec pointer given as input to the user defined #MQTTStorePacketForRetransmit callback function. Must not be NULL.
 */
/* @[declare_mqtt_serializemqttvec] */
void MQTT_SerializeMQTTVec( uint8_t * pAllocatedMem,
                            const MQTTVec_t * pVec );
/* @[declare_mqtt_serializemqttvec] */

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_H */
