/*
 * coreMQTT <DEVELOPMENT BRANCH>
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * @file core_mqtt_utest.c
 * @brief Unit tests for functions in core_mqtt.h.
 */
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "unity.h"

/* Include paths for public enums, structures, and macros. */
#include "core_mqtt.h"

/* #include "mock_core_mqttv5_serializer.h" */
/* #include "mock_core_mqttv5_state.h" */

#include "core_mqtt_config_defaults.h"
#include "mock_core_mqtt_serializer.h"
#include "mock_core_mqtt_state.h"
/* Set network context to double pointer to buffer (uint8_t**). */
struct NetworkContext
{
    uint8_t ** buffer;
};

/**
 * @brief MQTT client identifier.
 */
#define MQTT_CLIENT_IDENTIFIER                 "testclient"

/**
 * @brief A valid starting packet ID per MQTT spec. Start from 1.
 */
#define MQTT_FIRST_VALID_PACKET_ID             ( 1 )

/**
 * @brief A PINGREQ packet is always 2 bytes in size, defined by MQTT 3.1.1 spec.
 */
#define MQTT_PACKET_PINGREQ_SIZE               ( 2U )

/**
 * @brief A packet type not handled by MQTT_ProcessLoop.
 */
#define MQTT_PACKET_TYPE_INVALID               ( 0U )

/**
 * @brief Number of milliseconds in a second.
 */
#define MQTT_ONE_SECOND_TO_MS                  ( 1000U )

/**
 * @brief Length of the MQTT network buffer.
 */
#define MQTT_TEST_BUFFER_LENGTH                ( 128 )

/**
 * @brief Sample keep-alive interval that should be greater than 0.
 */
#define MQTT_SAMPLE_KEEPALIVE_INTERVAL_S       ( 1U )

/**
 * @brief Length of time spent for single test case with
 * multiple iterations spent in the process loop for coverage.
 */
#define MQTT_SAMPLE_PROCESS_LOOP_TIMEOUT_MS    ( 1U )

/**
 * @brief Zero timeout in the process loop implies one iteration.
 */
#define MQTT_NO_TIMEOUT_MS                     ( 0U )

/**
 * @brief Sample length of remaining serialized data.
 */
#define MQTT_SAMPLE_REMAINING_LENGTH           ( 64 )

/**
 * @brief Subtract this value from max value of global entry time
 * for the timer overflow test.
 */
#define MQTT_OVERFLOW_OFFSET                   ( 3 )

/**
 * @brief The number of times the "getTime()" function is called
 * within a single iteration of the #MQTT_ProcessLoop.
 *
 * This constant is used for the timer overflow test which checks
 * that the API can support normal behavior even if the timer
 * overflows.
 *
 * @note Currently, there are 6 calls within a single iteration.
 * This can change when the implementation changes which would be
 * caught through unit test failure.
 */
#define MQTT_TIMER_CALLS_PER_ITERATION         ( 6 )

/**
 * @brief Timeout for the timer overflow test.
 */
#define MQTT_TIMER_OVERFLOW_TIMEOUT_MS         ( 10 )

/**
 * @brief A sample network context that we set to NULL.
 */
#define MQTT_SAMPLE_NETWORK_CONTEXT            ( NULL )

/**
 * @brief Sample topic filter to subscribe to.
 */
#define MQTT_SAMPLE_TOPIC_FILTER               "iot"

/**
 * @brief Length of sample topic filter.
 */
#define MQTT_SAMPLE_TOPIC_FILTER_LENGTH        ( sizeof( MQTT_SAMPLE_TOPIC_FILTER ) - 1 )

/**
 * @brief Sample topic filter to subscribe to.
 */
#define MQTT_SAMPLE_TOPIC_FILTER1              "TopicFilter2"

/**
 * @brief Length of sample topic filter.
 */
#define MQTT_SAMPLE_TOPIC_FILTER_LENGTH1       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER1 ) - 1 )

/**
 * @brief Sample topic filter to subscribe to.
 */
#define MQTT_SAMPLE_TOPIC_FILTER2              "SomeTopic"

/**
 * @brief Length of sample topic filter.
 */
#define MQTT_SAMPLE_TOPIC_FILTER_LENGTH2       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER2 ) - 1 )

/**
 * @brief Sample topic filter to subscribe to.
 */
#define MQTT_SAMPLE_TOPIC_FILTER3              "iotTopicFilter"

/**
 * @brief Length of sample topic filter.
 */
#define MQTT_SAMPLE_TOPIC_FILTER_LENGTH3       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER3 ) - 1 )


#define   TEST_TOPIC_ALIAS                     ( 2U )
#define   TEST_MSG_EXPIRY                      ( 100U )
#define   TEST_CONTENT_TYPE_LENGTH             ( 2 )
#define   TEST_CONTENT_TYPE                    ( "ab" )
#define   TEST_RESPONSE_TOPIC_LENGTH           ( 10 )
#define   TEST_RESPONSE_TOPIC                  ( "aaaaaaaaaa" )
#define   TEST_CORRELATION_DATA_LENGTH         ( 5 )
#define   TEST_CORRELATION_DATA                ( "abcde" )

/**
 * @brief Return values of mocked calls in MQTT_ProcessLoop(). Used by
 * `expectProcessLoopCalls`
 */
typedef struct ProcessLoopReturns
{
    MQTTStatus_t deserializeStatus;           /**< @brief Status after deserializing incoming packet. */
    MQTTPublishState_t stateAfterDeserialize; /**< @brief Publish state after deserializing incoming packet. */
    MQTTStatus_t updateStateStatus;           /**< @brief Status after updating publish state. */
    MQTTStatus_t serializeStatus;             /**< @brief Status after serializing a publish ack to send. */
    MQTTPublishState_t stateAfterSerialize;   /**< @brief Publish state after serializing an ack to send. */
    MQTTStatus_t processLoopStatus;           /**< @brief Return value of the process loop. */
    bool incomingPublish;                     /**< @brief Whether the incoming packet is a publish. */
    MQTTPublishInfo_t * pPubInfo;             /**< @brief Publish information to be returned by the deserializer. */
    uint32_t timeoutMs;                       /**< @brief The timeout value to call MQTT_ProcessLoop API with. */
} ProcessLoopReturns_t;


/**
 * @brief Time at the beginning of each test. Note that this is not updated with
 * a real clock. Instead, we simply increment this variable.
 */
static uint32_t globalEntryTime = 0;

/**
 * @brief A static buffer used by the MQTT library for storing packet data.
 */
static uint8_t mqttBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

/**
 * @brief A flag to indicate whether event callback is called from
 * MQTT_ProcessLoop.
 */
static bool isEventCallbackInvoked = false;
static bool receiveOnce = false;
static MQTTStatus_t modifyIncomingPacketStatus = MQTTSuccess;


/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp()
{
    memset( mqttBuffer, 0x0, sizeof( mqttBuffer ) );
    MQTT_State_strerror_IgnoreAndReturn( "DUMMY_MQTT_STATE" );

    globalEntryTime = 0;
}

/* Called after each test method. */
void tearDown()
{
}

/* Called at the beginning of the whole suite. */
void suiteSetUp()
{
    receiveOnce = 0;
}

/* Called at the end of the whole suite. */
int suiteTearDown( int numFailures )
{
    return numFailures;
}


/* ========================================================================== */

/**
 * @brief A mocked timer query function that increments on every call. This
 * guarantees that only a single iteration runs in the ProcessLoop for ease
 * of testing.
 */
static uint32_t getTime( void )
{
    return globalEntryTime++;
}

/**
 * @brief Mocked failed transport read.
 */
static int32_t transportRecvFailure( NetworkContext_t * pNetworkContext,
                                     void * pBuffer,
                                     size_t bytesToRead )
{
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToRead;
    return -1;
}

/**
 * @brief Mocked MQTT event callback.
 *
 * @param[in] pContext MQTT context pointer.
 * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
 * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
 */
static void eventCallback( MQTTContext_t * pContext,
                           MQTTPacketInfo_t * pPacketInfo,
                           MQTTDeserializedInfo_t * pDeserializedInfo )
{
    ( void ) pContext;
    ( void ) pPacketInfo;
    ( void ) pDeserializedInfo;

    /* Update the global state to indicate that event callback is invoked. */
    isEventCallbackInvoked = true;
    pDeserializedInfo->pNextAckInfo = NULL;
}

/**
 * @brief Mocked MQTT event callback.
 *
 * @param[in] pContext MQTT context pointer.
 * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
 * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
 */
static void eventCallback1( MQTTContext_t * pContext,
                            MQTTPacketInfo_t * pPacketInfo,
                            MQTTDeserializedInfo_t * pDeserializedInfo )
{
    ( void ) pContext;
    ( void ) pPacketInfo;
    ( void ) pDeserializedInfo;
    MQTTAckInfo_t ackInfo;
    MQTTUserProperties_t user;
    memset( &user, 0x0, sizeof( user ) );
    memset( &ackInfo, 0x0, sizeof( ackInfo ) );
    /* Update the global state to indicate that event callback is invoked. */
    isEventCallbackInvoked = true;
    pDeserializedInfo->pNextAckInfo = &ackInfo;

    if( pDeserializedInfo->packetIdentifier == 1 )
    {
        ackInfo.reasonStringLength = 2;
        ackInfo.pReasonString = "ab";
        ackInfo.pUserProperty = &user;
        user.count = 0;
    }
}


/**
 * @brief Mocked transport send that always returns 0 bytes sent.
 */
static int32_t transportSendNoBytes( NetworkContext_t * pNetworkContext,
                                     const void * pBuffer,
                                     size_t bytesToWrite )
{
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToWrite;
    return 0;
}

static uint8_t * MQTTV5_SerializeAckFixed_cb( uint8_t * pIndex,
                                              uint8_t packetType,
                                              uint16_t packetId,
                                              size_t remainingLength,
                                              size_t propertyLength,
                                              int numcallbacks )
{
    ( void ) packetType;
    ( void ) packetId;
    ( void ) remainingLength;
    ( void ) propertyLength;
    ( void ) numcallbacks;

    return pIndex;
}

static uint8_t * MQTTV5_SerializeDisconnectFixed_cb( uint8_t * pIndex,
                                                     const MQTTAckInfo_t * pAckInfo,
                                                     size_t remainingLength,
                                                     uint32_t sessionExpiry,
                                                     int numcallbacks )
{
    ( void ) pIndex;
    ( void ) pAckInfo;
    ( void ) remainingLength;
    ( void ) sessionExpiry;
    ( void ) numcallbacks;

    return pIndex;
}



static uint8_t * MQTT_SerializeConnectFixedHeader_cb( uint8_t * pIndex,
                                                      const MQTTConnectInfo_t * pConnectInfo,
                                                      const MQTTPublishInfo_t * pWillInfo,
                                                      size_t remainingLength,
                                                      int numcallbacks )
{
    ( void ) pConnectInfo;
    ( void ) pWillInfo;
    ( void ) remainingLength;
    ( void ) numcallbacks;

    return pIndex;
}

static uint8_t * MQTTV5_SerializeConnectProperties_cb( uint8_t * pIndex,
                                                       const MQTTConnectProperties_t * pConnectProperties
                                                       ,
                                                       int numcallbacks )
{
    ( void ) pConnectProperties;
    ( void ) numcallbacks;

    return pIndex;
}

static uint8_t * MQTT_SerializePublishProperties_cb( const MQTTPublishInfo_t * pPublishInfo,
                                                     uint8_t * pIndex
                                                     ,
                                                     int numcallbacks )
{
    ( void ) pPublishInfo;
    ( void ) numcallbacks;

    return pIndex;
}


/**
 * @brief Mocked successful transport writev.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[in] pMessage vectors to send
 * @param[in] bytesToWrite number of vectors
 *
 * @return Number of bytes sent; negative value on error;
 * 0 for timeout or 0 bytes sent.
 */
static int32_t transportWritevSuccess( NetworkContext_t * pNetworkContext,
                                       TransportOutVector_t * pIoVectorIterator,
                                       size_t vectorsToBeSent )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    int32_t bytesToWrite = 0;
    size_t i;

    for( i = 0; i < vectorsToBeSent; ++i )
    {
        bytesToWrite += pIoVectorIterator->iov_len;
        pIoVectorIterator++;
    }

    return bytesToWrite;
}


/**
 * @brief Mocked successful transport send.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[in] pMessage Data to send.
 * @param[in] bytesToWrite Length of data to send.
 *
 * @return Number of bytes sent; negative value on error;
 * 0 for timeout or 0 bytes sent.
 */
static int32_t transportSendSuccess( NetworkContext_t * pNetworkContext,
                                     const void * pBuffer,
                                     size_t bytesToWrite )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    ( void ) pBuffer;
    return bytesToWrite;
}

/**
 * @brief Mocked failed transport send.
 */
static int32_t transportSendFailure( NetworkContext_t * pNetworkContext,
                                     const void * pBuffer,
                                     size_t bytesToWrite )
{
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToWrite;
    return -1;
}

/**
 * @brief Mocked successful transport read.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[out] pBuffer Buffer for receiving data.
 * @param[in] bytesToRead Size of pBuffer.
 *
 * @return Number of bytes received; negative value on error.
 */
static int32_t transportRecvSuccess( NetworkContext_t * pNetworkContext,
                                     void * pBuffer,
                                     size_t bytesToRead )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    ( void ) pBuffer;
    return bytesToRead;
}


/**
 * @brief Initialize the transport interface with the mocked functions for
 * send and receive.
 *
 * @brief param[in] pTransport The transport interface to use with the context.
 */
static void setupTransportInterface( TransportInterface_t * pTransport )
{
    pTransport->pNetworkContext = MQTT_SAMPLE_NETWORK_CONTEXT;
    pTransport->send = transportSendSuccess;
    pTransport->recv = transportRecvSuccess;
    pTransport->writev = transportWritevSuccess;
}

/**
 * @brief Initialize pNetworkBuffer using static buffer.
 *
 * @param[in] pNetworkBuffer Network buffer provided for the context.
 */
static void setupNetworkBuffer( MQTTFixedBuffer_t * const pNetworkBuffer )
{
    pNetworkBuffer->pBuffer = mqttBuffer;
    pNetworkBuffer->size = MQTT_TEST_BUFFER_LENGTH;
}

static void setupPublishProperties( MQTTPublishInfo_t * pPublishInfo )
{
    pPublishInfo->payloadFormat = 1;
    pPublishInfo->topicAlias = TEST_TOPIC_ALIAS;
    pPublishInfo->msgExpiryInterval = TEST_MSG_EXPIRY;
    pPublishInfo->msgExpiryPresent = 1;
    pPublishInfo->contentTypeLength = TEST_CONTENT_TYPE_LENGTH;
    pPublishInfo->pContentType = TEST_CONTENT_TYPE;
    pPublishInfo->responseTopicLength = TEST_RESPONSE_TOPIC_LENGTH;
    pPublishInfo->pResponseTopic = TEST_RESPONSE_TOPIC;
    pPublishInfo->correlationLength = TEST_CORRELATION_DATA_LENGTH;
    pPublishInfo->pCorrelationData = TEST_CORRELATION_DATA;
}


void test_MQTT_Connect()
{
    MQTTContext_t mqttContext = { 0 };
    MQTTConnectInfo_t connectInfo = { 0 };
    bool sessionPresent = false;
    MQTTStatus_t status;
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );

    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );

/*  No connect Properties */
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0U, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_Connect_error_path()
{
    MQTTContext_t mqttContext = { 0 };
    MQTTConnectInfo_t connectInfo = { 0 };
    MQTTPublishInfo_t willInfo = { 0 };
    uint32_t timeout = 2;
    bool sessionPresent;
    MQTTStatus_t status;
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTAuthInfo_t authInfo;
    MQTTAuthInfo_t authInfo2;
    MQTTUserProperties_t userProperties;

    memset( &properties, 0x0, sizeof( properties ) );
    memset( &authInfo, 0x0, sizeof( authInfo ) );
    memset( &authInfo2, 0x0, sizeof( authInfo2 ) );
    memset( &userProperties, 0x0, sizeof( userProperties ) );
    userProperties.userProperty[ 0 ].pKey = "ab";
    userProperties.userProperty[ 0 ].pValue = "ab";
    userProperties.userProperty[ 0 ].keyLength = 2;
    userProperties.userProperty[ 0 ].valueLength = 2;
    userProperties.count = 1;
    authInfo.pAuthMethod = "2";
    authInfo.authMethodLength = 1;
    authInfo.pAuthData = "ab";
    authInfo.authDataLength = 2;
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    memset( &willInfo, 0x0, sizeof( MQTTPublishInfo_t ) );
    memset( &connectInfo, 0x0, sizeof( MQTTConnectInfo_t ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );

    /* With non-NULL Will. */
    mqttContext.connectStatus = MQTTNotConnected;
    mqttContext.keepAliveIntervalSec = 0;
    mqttContext.pConnectProperties = &properties;
    properties.sessionExpiry = 13;
    properties.receiveMax = 12;
    properties.maxPacketSize = 1000;
    properties.topicAliasMax = 13;
    properties.requestProblemInfo = 0;
    properties.requestResponseInfo = 1;
    properties.pOutgoingUserProperty = &userProperties;
    properties.pOutgoingAuth = &authInfo;
    properties.pIncomingAuth = &authInfo2;

    willInfo.pTopicName = "test";
    willInfo.topicNameLength = 4;
    willInfo.pPayload = "Payload";
    willInfo.payloadLength = 7;
    willInfo.payloadFormat = 1;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval = 10;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval = 10;
    willInfo.contentTypeLength = 2;
    willInfo.pContentType = "ab";
    willInfo.responseTopicLength = 2;
    willInfo.pResponseTopic = "ab";
    willInfo.correlationLength = 2;
    willInfo.pCorrelationData = "ab";
    willInfo.willDelay = 3;
    willInfo.pUserProperty = &userProperties;
    connectInfo.pUserName = "abcd";
    connectInfo.userNameLength = 3;
    connectInfo.passwordLength = 4;
    connectInfo.pPassword = "1234";
    mqttContext.transportInterface.send = transportSendSuccess;
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

    properties.pOutgoingUserProperty = NULL;
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
    status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );
    properties.pOutgoingAuth->authDataLength = 0;
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
    status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

    properties.pOutgoingAuth = NULL;
    willInfo.contentTypeLength = 0;
    willInfo.responseTopicLength = 0;
    willInfo.correlationLength = 0;
    willInfo.pUserProperty = NULL;
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
    status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

    willInfo.pTopicName = NULL;
    mqttContext.transportInterface.send = transportSendSuccess;
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_Connect_receiveConnack( void )
{
    MQTTContext_t mqttContext = { 0 };
    MQTTConnectInfo_t connectInfo = { 0 };
    uint32_t timeout = 0;
    bool sessionPresent;
    MQTTStatus_t status;
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties = { 0 };
    MQTTPacketInfo_t incomingPacket = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    transport.recv = transportRecvFailure;

    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    memset( &properties, 0x0, sizeof( properties ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );
    mqttContext.pConnectProperties = &properties;
    /* Everything before receiving the CONNACK should succeed. */
    MQTTV5_SerializeConnect_IgnoreAndReturn( MQTTSuccess );
    MQTTV5_GetConnectPacketSize_IgnoreAndReturn( MQTTSuccess );

    /* Nothing received from transport interface. Set timeout to 2 for branch coverage. */
    timeout = 2;
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTNoDataAvailable );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTNoDataAvailable );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTTV5_SerializeConnectProperties_Stub( MQTTV5_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTNoDataAvailable, status );

    /* Did not receive a CONNACK. */
    incomingPacket.type = MQTT_PACKET_TYPE_PINGRESP;
    incomingPacket.remainingLength = 0;
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* Transport receive failure when receiving rest of packet. */
    incomingPacket.type = MQTT_PACKET_TYPE_CONNACK;
    incomingPacket.remainingLength = 2;
    timeout = 2;
    mqttContext.transportInterface.recv = transportRecvFailure;
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

    mqttContext.transportInterface.recv = transportRecvSuccess;
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeConnack_ExpectAnyArgsAndReturn( MQTTBadResponse );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTT_Publish( void )
{
    MQTTContext_t mqttContext = { 0 };
    MQTTPublishInfo_t publishInfo = { 0 };
    MQTTConnectProperties_t properties = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTStatus_t status;

    const uint16_t PACKET_ID = 1;

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    transport.send = transportSendFailure;

    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    memset( &publishInfo, 0x0, sizeof( publishInfo ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );

    /*Connect properties not defined*/
    status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    mqttContext.pConnectProperties = &properties;

    properties.maxPacketSize = 10000;
    MQTTV5_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    MQTTV5_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Send With properties*/
    setupPublishProperties( &publishInfo );
    MQTTV5_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Happy_Paths( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTPacketInfo_t incomingPacket = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    /* Modify incoming packet depending on type to be tested. */
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelSend;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths1( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTPublishState_t stateAfterSerialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTPacketInfo_t incomingPacket = { 0 };

    /*Update state returns bad response.*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelSend;
    stateAfterSerialize = MQTTPubCompPending;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTBadResponse );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths2( void )
{
    MQTTStatus_t status;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTPacketInfo_t incomingPacket = { 0 };

    /*Update state returns bad response.*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths3( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    uint16_t packetId = 2;
    MQTTPacketInfo_t incomingPacket = { 0 };

    /*Invalid packet parameters.*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelSend;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Happy_Paths2( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTPublishState_t stateAfterSerialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    uint16_t packetId = 1;
    MQTTPacketInfo_t incomingPacket = { 0 };

    /*Using event call back to set reason string and user properties,*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelSend;
    stateAfterSerialize = MQTTPubCompPending;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths4( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTPacketInfo_t incomingPacket = { 0 };

    /*Invalid packet parameters.*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelPending;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths5( void )
{
    MQTTStatus_t status;
    MQTTPublishState_t stateAfterDeserialize;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTPacketInfo_t incomingPacket = { 0 };
    uint16_t packetId = 1;

    /*Unable to send the packet using transport interface.*/
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    context.transportInterface.send = transportSendNoBytes;
    context.transportInterface.writev = NULL;
    properties.requestProblemInfo = 1;
    context.pConnectProperties = &properties;
    modifyIncomingPacketStatus = MQTTSuccess;
    stateAfterDeserialize = MQTTPubRelSend;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
    MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
    MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
}

void test_MQTTV5_Disconnect()
{
    uint32_t sessionExpiry = 0U;
    MQTTStatus_t status;
    MQTTConnectProperties_t properties = { 0 };
    MQTTAckInfo_t ackInfo = { 0 };
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTUserProperties_t userProperties = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid parameters*/
    status = MQTTV5_Disconnect( NULL, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_Disconnect( &context, NULL, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Connect properties not initialized*/
    status = MQTTV5_Disconnect( &context, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Bad Parameters*/
    context.pConnectProperties = &properties;
    MQTTV5_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTTV5_Disconnect( &context, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Valid parameters*/
    properties.maxPacketSize = 100U;
    properties.sessionExpiry = 10U;
    MQTTV5_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeDisconnectFixed_Stub( MQTTV5_SerializeDisconnectFixed_cb );
    status = MQTTV5_Disconnect( &context, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*With reason string and user property*/
    ackInfo.pUserProperty = &userProperties;
    ackInfo.pReasonString = "test";
    ackInfo.reasonStringLength = 4;
    MQTTV5_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeDisconnectFixed_Stub( MQTTV5_SerializeDisconnectFixed_cb );
    status = MQTTV5_Disconnect( &context, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Send failed*/
    context.transportInterface.send = transportSendFailure;
    context.transportInterface.writev = NULL;
    MQTTV5_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTTV5_SerializeDisconnectFixed_Stub( MQTTV5_SerializeDisconnectFixed_cb );
    status = MQTTV5_Disconnect( &context, &ackInfo, sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
}

void test_MQTT_ProcessLoop_handleIncomingDisconnect( void )
{
    MQTTStatus_t status;
    MQTTContext_t context = { 0 };
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTConnectProperties_t properties;
    MQTTAckInfo_t disconnectInfo;
    MQTTPacketInfo_t incomingPacket = { 0 };

    memset( &disconnectInfo, 0x0, sizeof( disconnectInfo ) );
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );
    status = MQTT_Init( &context, &transport, getTime, eventCallback1, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    context.pDisconnectInfo = &disconnectInfo;
    context.pConnectProperties = &properties;
    incomingPacket.type = MQTT_PACKET_TYPE_DISCONNECT;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeDisconnect_IgnoreAndReturn( MQTTSuccess );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid packet parameters.*/
    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
    MQTTV5_DeserializeDisconnect_IgnoreAndReturn( MQTTProtocolError );
    status = MQTT_ProcessLoop( &context );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );
}
