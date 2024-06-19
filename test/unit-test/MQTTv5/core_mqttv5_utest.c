/*
 * coreMQTT v2.3.0
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

// #include "mock_core_mqttv5_serializer.h"
// #include "mock_core_mqttv5_state.h"

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
 * @brief The packet type to be received by the process loop.
 * IMPORTANT: Make sure this is set before calling expectProcessLoopCalls(...).
 */
static uint8_t currentPacketType = MQTT_PACKET_TYPE_INVALID;

/**
 * @brief The return value of modifyIncomingPacket(...) CMock callback that
 * replaces a call to MQTT_GetIncomingPacketTypeAndLength.
 * IMPORTANT: Make sure this is set before calling expectProcessLoopCalls(...).
 */
static MQTTStatus_t modifyIncomingPacketStatus = MQTTSuccess;

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

static const uint8_t SubscribeHeader[] =
{
    MQTT_PACKET_TYPE_SUBSCRIBE,                  /* Subscribe header. */
    0x10, 0x10,                                  /* Packet Length. */
    ( MQTT_FIRST_VALID_PACKET_ID >> 8 ),
    ( ( MQTT_FIRST_VALID_PACKET_ID ) & 0x00ffU ) /* Packet ID. */
};
static const size_t SubscribeHeaderLength = 5U;

static const uint8_t UnsubscribeHeader[] =
{
    MQTT_PACKET_TYPE_UNSUBSCRIBE,                /* Subscribe header. */
    0x12, 0x15,                                  /* Packet Length. */
    ( MQTT_FIRST_VALID_PACKET_ID >> 8 ),
    ( ( MQTT_FIRST_VALID_PACKET_ID ) & 0x00ffU ) /* Packet ID. */
};
static const size_t UnsubscribeHeaderLength = 5U;

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
 * @brief Mock successful transport send, and write data into buffer for
 * verification.
 */
static int32_t mockSend( NetworkContext_t * pNetworkContext,
                         const void * pMessage,
                         size_t bytesToSend )
{
    const uint8_t * buffer = ( const uint8_t * ) pMessage;
    /* Treat network context as pointer to buffer for mocking. */
    uint8_t * mockNetwork = *( pNetworkContext->buffer );
    size_t bytesSent = 0;

    while( bytesSent++ < bytesToSend )
    {
        /* Write single byte and advance buffer. */
        *mockNetwork++ = *buffer++;
    }

    /* Move stream by bytes sent. */
    *( pNetworkContext->buffer ) = mockNetwork;

    return bytesToSend;
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
}

/**
 * @brief A mocked timer query function that increments on every call. This
 * guarantees that only a single iteration runs in the ProcessLoop for ease
 * of testing.
 */
static uint32_t getTime( void )
{
    return globalEntryTime++;
}

static int32_t getTimeMockCallLimit = -1;

/**
 * @brief A mocked timer query function that increments on every call. This
 * guarantees that only a single iteration runs in the ProcessLoop for ease
 * of testing. Additionally, this tests whether the number of calls to this
 * function have exceeded than the set limit and asserts.
 */
static uint32_t getTimeMock( void )
{
    TEST_ASSERT_GREATER_THAN_INT32( -1, getTimeMockCallLimit-- );
    return globalEntryTime++;
}

static int32_t getTimeMockBigTimeStepCallLimit = -1;

/**
 * @brief A mocked timer query function that increments by MQTT_SEND_TIMEOUT_MS
 * to simulate the time consumed by a long running high priority task on every
 * call. Additionally, this tests whether the number of calls to this function
 * have exceeded than the set limit and asserts.
 */
static uint32_t getTimeMockBigTimeStep( void )
{
    TEST_ASSERT_GREATER_THAN_INT32( -1, getTimeMockBigTimeStepCallLimit-- );

    globalEntryTime += MQTT_SEND_TIMEOUT_MS;
    return globalEntryTime;
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

static uint8_t * MQTT_SerializeConnectProperties_cb( uint8_t* pIndex, const MQTTConnectProperties_t* pConnectProperties
                                                      ,int numcallbacks )
{
    ( void ) pConnectProperties;
    ( void ) numcallbacks;

    return pIndex;
}

static uint8_t * MQTT_SerializePublishProperties_cb( const MQTTPublishInfo_t* pPublishInfo, uint8_t* pIndex
                                                      ,int numcallbacks )
{
    ( void ) pPublishInfo;
    ( void ) numcallbacks;

    return pIndex;
}



/**
 * @brief A mocked timer function that could be used on a device with no system
 * time.
 */
static uint32_t getTimeDummy( void )
{
    return 0;
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

static void verifyEncodedTopicString( TransportOutVector_t * pIoVectorIterator,
                                      char * pTopicFilter,
                                      size_t topicFilterLength,
                                      MQTTQoS_t topicQos,
                                      int32_t * pBytesToWrite )
{
    /* Encoded topic length. */
    uint8_t serializedLength[ 2 ];
    const uint8_t * buffer;
    size_t length;

    serializedLength[ 0 ] = ( uint8_t ) ( topicFilterLength >> 8 );
    serializedLength[ 1 ] = ( uint8_t ) ( topicFilterLength & 0x00ffU );

    buffer = pIoVectorIterator[ 0 ].iov_base;
    length = pIoVectorIterator[ 0 ].iov_len;

    TEST_ASSERT_EQUAL( 2U, length );
    TEST_ASSERT_EQUAL_UINT8_ARRAY( serializedLength, buffer, 2U );
    ( *pBytesToWrite ) += length;

    buffer = pIoVectorIterator[ 1 ].iov_base;
    length = pIoVectorIterator[ 1 ].iov_len;

    /* Encoded topic string. */
    TEST_ASSERT_EQUAL( topicFilterLength, length );
    TEST_ASSERT_EQUAL_UINT8_ARRAY( pTopicFilter, buffer, topicFilterLength );
    ( *pBytesToWrite ) += length;

    buffer = pIoVectorIterator[ 2 ].iov_base;
    length = pIoVectorIterator[ 2 ].iov_len;

    /* Encoded QoS. */
    TEST_ASSERT_EQUAL( 1U, length );
    TEST_ASSERT_EQUAL_UINT8( topicQos, *buffer );
    ( *pBytesToWrite ) += length;
}

static void verifyEncodedTopicStringUnsubscribe( TransportOutVector_t * pIoVectorIterator,
                                                 char * pTopicFilter,
                                                 size_t topicFilterLength,
                                                 int32_t * pBytesToWrite )
{
    /* Encoded topic length. */
    uint8_t serializedLength[ 2 ];
    const uint8_t * buffer;
    size_t length;

    serializedLength[ 0 ] = ( uint8_t ) ( topicFilterLength >> 8 );
    serializedLength[ 1 ] = ( uint8_t ) ( topicFilterLength & 0x00ffU );

    buffer = pIoVectorIterator[ 0 ].iov_base;
    length = pIoVectorIterator[ 0 ].iov_len;

    TEST_ASSERT_EQUAL( 2U, length );
    TEST_ASSERT_EQUAL_UINT8_ARRAY( serializedLength, buffer, 2U );
    ( *pBytesToWrite ) += length;

    buffer = pIoVectorIterator[ 1 ].iov_base;
    length = pIoVectorIterator[ 1 ].iov_len;

    /* Encoded topic string. */
    TEST_ASSERT_EQUAL( topicFilterLength, length );
    TEST_ASSERT_EQUAL_UINT8_ARRAY( pTopicFilter, buffer, topicFilterLength );
    ( *pBytesToWrite ) += length;
}

/**
 * @brief Mocked transport writev to verify correct subscription
 * packet.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[in] pMessage vectors to send
 * @param[in] bytesToWrite number of vectors
 *
 * @return Number of bytes sent; negative value on error;
 * 0 for timeout or 0 bytes sent.
 */
static int32_t transportWritevSubscribeSuccess( NetworkContext_t * pNetworkContext,
                                                TransportOutVector_t * pIoVectorIterator,
                                                size_t vectorsToBeSent )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    int32_t bytesToWrite = 0;
    static int writeCount = 0;
    const uint8_t * buffer;
    size_t length;

    /* The header. */
    if( writeCount == 0 )
    {
        TEST_ASSERT_EQUAL( 4, vectorsToBeSent );

        buffer = pIoVectorIterator->iov_base;
        length = pIoVectorIterator->iov_len;

        TEST_ASSERT_EQUAL( length, SubscribeHeaderLength );
        TEST_ASSERT_EQUAL_UINT8_ARRAY( ( uint8_t * ) SubscribeHeader, buffer, SubscribeHeaderLength );

        bytesToWrite += length;
        pIoVectorIterator++;

        /* First topic filter. */
        verifyEncodedTopicString( pIoVectorIterator,
                                  MQTT_SAMPLE_TOPIC_FILTER,
                                  MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
                                  MQTTQoS1,
                                  &bytesToWrite );

        writeCount++;

        pIoVectorIterator += 4;
    }
    else if( writeCount == 1 )
    {
        TEST_ASSERT_EQUAL( 6, vectorsToBeSent );

        /* Second topic filter. */
        verifyEncodedTopicString( pIoVectorIterator,
                                  MQTT_SAMPLE_TOPIC_FILTER1,
                                  MQTT_SAMPLE_TOPIC_FILTER_LENGTH1,
                                  MQTTQoS2,
                                  &bytesToWrite );

        pIoVectorIterator += 3;

        /* Third topic filter. */
        verifyEncodedTopicString( pIoVectorIterator,
                                  MQTT_SAMPLE_TOPIC_FILTER2,
                                  MQTT_SAMPLE_TOPIC_FILTER_LENGTH2,
                                  MQTTQoS0,
                                  &bytesToWrite );

        writeCount++;
    }
    else if( writeCount == 2 )
    {
        TEST_ASSERT_EQUAL( 3, vectorsToBeSent );

        /* Fourth topic filter. */
        verifyEncodedTopicString( pIoVectorIterator,
                                  MQTT_SAMPLE_TOPIC_FILTER3,
                                  MQTT_SAMPLE_TOPIC_FILTER_LENGTH3,
                                  MQTTQoS1,
                                  &bytesToWrite );

        writeCount++;
    }
    else
    {
        bytesToWrite = -1;
    }

    return bytesToWrite;
}

/**
 * @brief Mocked transport writev to verify correct subscription
 * packet.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[in] pMessage vectors to send
 * @param[in] bytesToWrite number of vectors
 *
 * @return Number of bytes sent; negative value on error;
 * 0 for timeout or 0 bytes sent.
 */
static int32_t transportWritevUnsubscribeSuccess( NetworkContext_t * pNetworkContext,
                                                  TransportOutVector_t * pIoVectorIterator,
                                                  size_t vectorsToBeSent )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    int32_t bytesToWrite = 0;
    static int writeCount = 0;
    const uint8_t * buffer;
    size_t length;

    /* The header. */
    if( writeCount == 0 )
    {
        TEST_ASSERT_EQUAL( 5, vectorsToBeSent );

        buffer = pIoVectorIterator->iov_base;
        length = pIoVectorIterator->iov_len;

        TEST_ASSERT_EQUAL( length, UnsubscribeHeaderLength );
        TEST_ASSERT_EQUAL_UINT8_ARRAY( ( uint8_t * ) UnsubscribeHeader, buffer, UnsubscribeHeaderLength );

        bytesToWrite += length;
        pIoVectorIterator++;

        /* First topic filter. */
        verifyEncodedTopicStringUnsubscribe( pIoVectorIterator,
                                             MQTT_SAMPLE_TOPIC_FILTER,
                                             MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
                                             &bytesToWrite );

        pIoVectorIterator += 2;

        /* Second topic filter. */
        verifyEncodedTopicStringUnsubscribe( pIoVectorIterator,
                                             MQTT_SAMPLE_TOPIC_FILTER1,
                                             MQTT_SAMPLE_TOPIC_FILTER_LENGTH1,
                                             &bytesToWrite );

        writeCount++;
    }
    else if( writeCount == 1 )
    {
        TEST_ASSERT_EQUAL( 4, vectorsToBeSent );

        /* Third topic filter. */
        verifyEncodedTopicStringUnsubscribe( pIoVectorIterator,
                                             MQTT_SAMPLE_TOPIC_FILTER2,
                                             MQTT_SAMPLE_TOPIC_FILTER_LENGTH2,
                                             &bytesToWrite );

        pIoVectorIterator += 2;

        /* Fourth topic filter. */
        verifyEncodedTopicStringUnsubscribe( pIoVectorIterator,
                                             MQTT_SAMPLE_TOPIC_FILTER3,
                                             MQTT_SAMPLE_TOPIC_FILTER_LENGTH3,
                                             &bytesToWrite );

        writeCount++;
    }
    else
    {
        bytesToWrite = -1;
    }

    return bytesToWrite;
}

/**
 * @brief Mocked failed transport writev.
 *
 * @param[in] tcpSocket TCP socket.
 * @param[in] pMessage vectors to send
 * @param[in] bytesToWrite number of vectors
 *
 * @return Number of bytes sent; negative value on error;
 * 0 for timeout or 0 bytes sent.
 */
static int32_t transportWritevFail( NetworkContext_t * pNetworkContext,
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

    return bytesToWrite + 3;
}

static int32_t transportWritevError( NetworkContext_t * pNetworkContext,
                                     TransportOutVector_t * pIoVectorIterator,
                                     size_t vectorsToBeSent )
{
    ( void ) vectorsToBeSent;
    ( void ) pIoVectorIterator;

    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );

    return -1;
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

/**
 * @brief Mocked transport send that succeeds then fails.
 */
static int32_t transportSendSucceedThenFail( NetworkContext_t * pNetworkContext,
                                             const void * pMessage,
                                             size_t bytesToSend )
{
    int32_t retVal = bytesToSend;
    static int counter = 0;

    ( void ) pNetworkContext;
    ( void ) pMessage;

    if( counter++ )
    {
        retVal = -1;
        counter = 0;
    }

    return retVal;
}

/**
 * @brief Mocked transport send that only sends half of the bytes.
 */
static int32_t transportWritevPartialByte( NetworkContext_t * pNetworkContext,
                                           TransportOutVector_t * pIoVectorIterator,
                                           size_t vectorsToBeSent )
{
    int32_t bytesToWrite = 0;
    size_t i;

    ( void ) pNetworkContext;

    for( i = 0; i < vectorsToBeSent; ++i )
    {
        bytesToWrite += pIoVectorIterator->iov_len;
        pIoVectorIterator++;
    }

    if( bytesToWrite > 1 )
    {
        bytesToWrite /= 2;
    }

    return bytesToWrite;
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

static int32_t transportRecvOneSuccessOneFail( NetworkContext_t * pNetworkContext,
                                               void * pBuffer,
                                               size_t bytesToRead )
{
    TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
    ( void ) pBuffer;

    if( receiveOnce == false )
    {
        receiveOnce = true;
        return bytesToRead;
    }
    else
    {
        return -1;
    }
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
 * @brief Mocked transport reading one byte at a time.
 */
static int32_t transportRecvOneByte( NetworkContext_t * pNetworkContext,
                                     void * pBuffer,
                                     size_t bytesToRead )
{
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToRead;
    return 1;
}

/**
 * @brief Mocked transport returning zero bytes to simulate reception
 * of no data over network.
 */
static int32_t transportRecvNoData( NetworkContext_t * pNetworkContext,
                                    void * pBuffer,
                                    size_t bytesToRead )
{
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToRead;
    return 0;
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
 * @brief Initialize pSubscribeInfo using test-defined macros.
 *
 * @param[in] pSubscribeInfo Pointer to MQTT subscription info.
 */
static void setupSubscriptionInfo( MQTTSubscribeInfo_t * pSubscribeInfo )
{
    pSubscribeInfo->qos = MQTTQoS1;
    pSubscribeInfo->pTopicFilter = MQTT_SAMPLE_TOPIC_FILTER;
    pSubscribeInfo->topicFilterLength = MQTT_SAMPLE_TOPIC_FILTER_LENGTH;
}

/**
 * @brief Zero out a #ProcessLoopReturns_t.
 *
 * @param[in] pExpectParams Pointer to struct to reset.
 */
static void resetProcessLoopParams( ProcessLoopReturns_t * pExpectParams )
{
    pExpectParams->deserializeStatus = MQTTSuccess;
    pExpectParams->stateAfterDeserialize = MQTTStateNull;
    pExpectParams->updateStateStatus = MQTTSuccess;
    pExpectParams->serializeStatus = MQTTSuccess;
    pExpectParams->stateAfterSerialize = MQTTStateNull;
    pExpectParams->processLoopStatus = MQTTSuccess;
    pExpectParams->incomingPublish = false;
    pExpectParams->pPubInfo = NULL;
    pExpectParams->timeoutMs = MQTT_NO_TIMEOUT_MS;
}

/**
 * @brief create default context
 *
 * @param[out] context to initialize
 */
static void setUPContext( MQTTContext_t * mqttContext )
{
    MQTTStatus_t mqttStatus;
    static TransportInterface_t transport = { 0 };
    static MQTTFixedBuffer_t networkBuffer = { 0 };
    static MQTTPubAckInfo_t incomingRecords[ 10 ] = { 0 };
    static MQTTPubAckInfo_t outgoingRecords[ 10 ] = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );

    memset( mqttContext, 0x0, sizeof( MQTTContext_t ) );

    mqttStatus = MQTT_Init( mqttContext,
                            &transport,
                            getTime,
                            eventCallback,
                            &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

    mqttStatus = MQTT_InitStatefulQoS( mqttContext,
                                       outgoingRecords,
                                       10,
                                       incomingRecords,
                                       10 );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
}

/**
 * @brief This helper function is used to expect any calls from the process loop
 * to mocked functions belonging to an external header file. Its parameters
 * are used to provide return values for these mocked functions.
 */
static void expectProcessLoopCalls( MQTTContext_t * const pContext,
                                    ProcessLoopReturns_t * pExpectParams )
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTPacketInfo_t incomingPacket = { 0 };
    size_t pingreqSize = MQTT_PACKET_PINGREQ_SIZE;
    bool expectMoreCalls = true;
    /* Copy values passed in the parameter struct. */
    MQTTStatus_t deserializeStatus = pExpectParams->deserializeStatus;
    MQTTPublishState_t stateAfterDeserialize = pExpectParams->stateAfterDeserialize;
    MQTTStatus_t updateStateStatus = pExpectParams->updateStateStatus;
    MQTTStatus_t serializeStatus = pExpectParams->serializeStatus;
    MQTTPublishState_t stateAfterSerialize = pExpectParams->stateAfterSerialize;
    MQTTStatus_t processLoopStatus = pExpectParams->processLoopStatus;
    bool incomingPublish = pExpectParams->incomingPublish;
    MQTTPublishInfo_t * pPubInfo = pExpectParams->pPubInfo;
    uint32_t packetTxTimeoutMs = 0U;

    /* Modify incoming packet depending on type to be tested. */
    incomingPacket.type = currentPacketType;
    incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
    incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

    MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

    /* More calls are expected only with the following packet types. */
    if( ( currentPacketType != MQTT_PACKET_TYPE_PUBLISH ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_PUBACK ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_PUBREC ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_PUBREL ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_PUBCOMP ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_PINGRESP ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_SUBACK ) &&
        ( currentPacketType != MQTT_PACKET_TYPE_UNSUBACK ) )
    {
        expectMoreCalls = false;
    }

    /* When no data is available, the process loop tries to send a PINGREQ. */
    if( modifyIncomingPacketStatus == MQTTNoDataAvailable )
    {
        packetTxTimeoutMs = 1000U * ( uint32_t ) pContext->keepAliveIntervalSec;

        if( PACKET_TX_TIMEOUT_MS < packetTxTimeoutMs )
        {
            packetTxTimeoutMs = PACKET_TX_TIMEOUT_MS;
        }

        if( pContext->waitingForPingResp == false )
        {
            if( ( ( packetTxTimeoutMs != 0U ) &&
                  ( ( globalEntryTime - pContext->lastPacketTxTime ) >= packetTxTimeoutMs ) ) ||
                ( ( PACKET_RX_TIMEOUT_MS != 0U ) &&
                  ( ( globalEntryTime - pContext->lastPacketRxTime ) >= PACKET_RX_TIMEOUT_MS ) ) )
            {
                MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
                /* Replace pointer parameter being passed to the method. */
                MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
                MQTT_SerializePingreq_ExpectAnyArgsAndReturn( serializeStatus );
            }
        }

        expectMoreCalls = false;
    }

    /* Deserialize based on the packet type (PUB or ACK) being received. */
    if( expectMoreCalls )
    {
        if( incomingPublish )
        {
            MQTT_DeserializePublish_ExpectAnyArgsAndReturn( deserializeStatus );

            if( pPubInfo != NULL )
            {
                MQTT_DeserializePublish_ReturnThruPtr_pPublishInfo( pPubInfo );
            }
        }
        else
        {
            MQTT_DeserializeAck_ExpectAnyArgsAndReturn( deserializeStatus );
        }

        if( ( deserializeStatus != MQTTSuccess ) ||
            ( currentPacketType == MQTT_PACKET_TYPE_PINGRESP ) ||
            ( currentPacketType == MQTT_PACKET_TYPE_SUBACK ) ||
            ( currentPacketType == MQTT_PACKET_TYPE_UNSUBACK ) )
        {
            expectMoreCalls = false;
        }
    }

    /* Update state based on the packet type (PUB or ACK) being received. */
    if( expectMoreCalls )
    {
        if( incomingPublish )
        {
            MQTT_UpdateStatePublish_ExpectAnyArgsAndReturn( updateStateStatus );
            MQTT_UpdateStatePublish_ReturnThruPtr_pNewState( &stateAfterDeserialize );
        }
        else
        {
            MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( updateStateStatus );
            MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
        }

        if( stateAfterDeserialize == MQTTPublishDone )
        {
            expectMoreCalls = false;
        }
        else
        {
            switch( updateStateStatus )
            {
                case MQTTSuccess:
                    expectMoreCalls = true;
                    break;

                case MQTTStateCollision:
                    /* Execution will continue regardless of the dup flag. */
                    expectMoreCalls = true;
                    MQTT_CalculateStatePublish_ExpectAnyArgsAndReturn( stateAfterDeserialize );
                    break;

                default:
                    expectMoreCalls = false;
            }
        }
    }

    /* Serialize the packet to be sent in response to the received packet. */
    if( expectMoreCalls )
    {
        MQTT_SerializeAck_ExpectAnyArgsAndReturn( serializeStatus );

        if( serializeStatus != MQTTSuccess )
        {
            expectMoreCalls = false;
        }
    }

    /* Update the state based on the sent packet. */
    if( expectMoreCalls )
    {
        MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( ( stateAfterSerialize == MQTTStateNull ) ?
                                                    MQTTIllegalState : MQTTSuccess );
        MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
    }

    /* Expect the above calls when running MQTT_ProcessLoop. */

    mqttStatus = MQTT_ProcessLoop( pContext );
    TEST_ASSERT_EQUAL( processLoopStatus, mqttStatus );

    /* Any final assertions to end the test. */
    if( mqttStatus == MQTTSuccess )
    {
        if( currentPacketType == MQTT_PACKET_TYPE_PUBLISH )
        {
            TEST_ASSERT_TRUE( pContext->controlPacketSent );
        }

        if( currentPacketType == MQTT_PACKET_TYPE_PINGRESP )
        {
            TEST_ASSERT_FALSE( pContext->waitingForPingResp );
        }
    }
}

void test_MQTT_Connect()
{
    MQTTContext_t mqttContext = { 0 };
    MQTTConnectInfo_t connectInfo = { 0 };
    bool sessionPresent = false;
    MQTTStatus_t status;
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTPacketInfo_t incomingPacket = { 0 };

    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );

    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );

/*  No connect Properties */
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
    status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0U, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTT_Connect_happy_path()
{
    MQTTContext_t mqttContext = { 0 };
    MQTTConnectInfo_t connectInfo = { 0 };
    MQTTPublishInfo_t willInfo = { 0 };
    uint32_t timeout = 2;
    bool sessionPresent;
    MQTTStatus_t status;
    TransportInterface_t transport = { 0 };
    MQTTFixedBuffer_t networkBuffer = { 0 };
    MQTTPacketInfo_t incomingPacket = { 0 };
    MQTTConnectProperties_t properties ={0};
    setupTransportInterface( &transport );
    setupNetworkBuffer( &networkBuffer );

    memset( &mqttContext, 0x0, sizeof( mqttContext ) );
    memset( &willInfo, 0x0, sizeof( MQTTPublishInfo_t ) );
    memset( &connectInfo, 0x0, sizeof( MQTTConnectInfo_t ) );
    MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer );

    /* With non-NULL Will. */
    mqttContext.connectStatus = MQTTNotConnected;
    mqttContext.keepAliveIntervalSec = 0;
    mqttContext.connectProperties= &properties;
    properties.sessionExpiry = 13;
    properties.receiveMax = 12;
    properties.isMaxPacketSize = true;
    properties.maxPacketSize =13;
    properties.topicAliasMax = 13;
    properties.reqProbInfo= 0;
    properties.reqResInfo = 1;
    willInfo.pTopicName = "test";
    willInfo.topicNameLength = 4;
    willInfo.pPayload = "Payload";
    willInfo.payloadLength = 7;
    willInfo.willDelay = 22;
    willInfo.correlationData = "fhf";
    willInfo.correlationLength = 3;
    incomingPacket.type = MQTT_PACKET_TYPE_CONNACK;
    mqttContext.transportInterface.send = transportSendSuccess;
    MQTT_SerializeConnect_IgnoreAndReturn( MQTTSuccess );
    MQTTV5_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
    MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
    MQTT_SerializeConnectProperties_Stub( MQTT_SerializeConnectProperties_cb );
    MQTT_SerializePublishProperties_Stub( MQTT_SerializePublishProperties_cb );
    MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
    status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );
}

