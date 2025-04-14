// /*
//  * coreMQTT <DEVELOPMENT BRANCH>
//  * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
//  *
//  * SPDX-License-Identifier: MIT
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy of
//  * this software and associated documentation files (the "Software"), to deal in
//  * the Software without restriction, including without limitation the rights to
//  * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
//  * the Software, and to permit persons to whom the Software is furnished to do so,
//  * subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all
//  * copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
//  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
//  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
//  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  */

// /**
//  * @file core_mqtt_utest.c
//  * @brief Unit tests for functions in core_mqtt.h.
//  */
// #include <string.h>
// #include <stdbool.h>
// #include <stdint.h>

// #include "unity.h"

// /* Include paths for public enums, structures, and macros. */

// // #include "mock_core_mqttv5_serializer.h"
// /* #include "mock_core_mqttv5_state.h" */

// #include "mock_core_mqtt_serializer.h"
// #include "mock_core_mqtt_state.h"
// #include "core_mqtt_config_defaults.h"

// #include "core_mqtt.h"

// /* Set network context to double pointer to buffer (uint8_t**). */
// struct NetworkContext
// {
//     uint8_t ** buffer;
// };

// static MQTTStatus_t validateSubscribeReturn = MQTTSuccess;

// /**
//  * @brief MQTT client identifier.
//  */
// #define MQTT_CLIENT_IDENTIFIER                 "testclient"

// /**
//  * @brief A valid starting packet ID per MQTT spec. Start from 1.
//  */
// #define MQTT_FIRST_VALID_PACKET_ID             ( 1 )

// /**
//  * @brief A PINGREQ packet is always 2 bytes in size, defined by MQTT 3.1.1 spec.
//  */
// #define MQTT_PACKET_PINGREQ_SIZE               ( 2U )

// /**
//  * @brief A packet type not handled by MQTT_ProcessLoop.
//  */
// #define MQTT_PACKET_TYPE_INVALID               ( 0U )

// /**
//  * @brief Number of milliseconds in a second.
//  */
// #define MQTT_ONE_SECOND_TO_MS                  ( 1000U )

// /**
//  * @brief Length of the MQTT network buffer.
//  */
// #define MQTT_TEST_BUFFER_LENGTH                ( 128 )

// /**
//  * @brief Sample keep-alive interval that should be greater than 0.
//  */
// #define MQTT_SAMPLE_KEEPALIVE_INTERVAL_S       ( 1U )

// /**
//  * @brief Length of time spent for single test case with
//  * multiple iterations spent in the process loop for coverage.
//  */
// #define MQTT_SAMPLE_PROCESS_LOOP_TIMEOUT_MS    ( 1U )

// /**
//  * @brief Zero timeout in the process loop implies one iteration.
//  */
// #define MQTT_NO_TIMEOUT_MS                     ( 0U )

// /**
//  * @brief Sample length of remaining serialized data.
//  */
// #define MQTT_SAMPLE_REMAINING_LENGTH           ( 64 )

// /**
//  * @brief Subtract this value from max value of global entry time
//  * for the timer overflow test.
//  */
// #define MQTT_OVERFLOW_OFFSET                   ( 3 )

// /**
//  * @brief The number of times the "getTime()" function is called
//  * within a single iteration of the #MQTT_ProcessLoop.
//  *
//  * This constant is used for the timer overflow test which checks
//  * that the API can support normal behavior even if the timer
//  * overflows.
//  *
//  * @note Currently, there are 6 calls within a single iteration.
//  * This can change when the implementation changes which would be
//  * caught through unit test failure.
//  */
// #define MQTT_TIMER_CALLS_PER_ITERATION         ( 6 )

// /**
//  * @brief Timeout for the timer overflow test.
//  */
// #define MQTT_TIMER_OVERFLOW_TIMEOUT_MS         ( 10 )

// /**
//  * @brief A sample network context that we set to NULL.
//  */
// #define MQTT_SAMPLE_NETWORK_CONTEXT            ( NULL )

// /**
//  * @brief Sample topic filter to subscribe to.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER               "iot"

// /**
//  * @brief Length of sample topic filter.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER_LENGTH        ( sizeof( MQTT_SAMPLE_TOPIC_FILTER ) - 1 )

// /**
//  * @brief Sample topic filter to subscribe to.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER1              "TopicFilter2"

// /**
//  * @brief Length of sample topic filter.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER_LENGTH1       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER1 ) - 1 )

// /**
//  * @brief Sample topic filter to subscribe to.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER2              "SomeTopic"

// /**
//  * @brief Length of sample topic filter.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER_LENGTH2       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER2 ) - 1 )

// /**
//  * @brief Sample topic filter to subscribe to.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER3              "iotTopicFilter"

// /**
//  * @brief Length of sample topic filter.
//  */
// #define MQTT_SAMPLE_TOPIC_FILTER_LENGTH3       ( sizeof( MQTT_SAMPLE_TOPIC_FILTER3 ) - 1 )


// #define   TEST_TOPIC_ALIAS                     ( 2U )
// #define   TEST_MSG_EXPIRY                      ( 100U )
// #define   TEST_CONTENT_TYPE_LENGTH             ( 2 )
// #define   TEST_CONTENT_TYPE                    ( "ab" )
// #define   TEST_RESPONSE_TOPIC_LENGTH           ( 10 )
// #define   TEST_RESPONSE_TOPIC                  ( "aaaaaaaaaa" )
// #define   TEST_CORRELATION_DATA_LENGTH         ( 5 )
// #define   TEST_CORRELATION_DATA                ( "abcde" )

// /**
//  * @brief Return values of mocked calls in MQTT_ProcessLoop(). Used by
//  * `expectProcessLoopCalls`
//  */
// typedef struct ProcessLoopReturns
// {
//     MQTTStatus_t deserializeStatus;           /**< @brief Status after deserializing incoming packet. */
//     MQTTPublishState_t stateAfterDeserialize; /**< @brief Publish state after deserializing incoming packet. */
//     MQTTStatus_t updateStateStatus;           /**< @brief Status after updating publish state. */
//     MQTTStatus_t serializeStatus;             /**< @brief Status after serializing a publish ack to send. */
//     MQTTPublishState_t stateAfterSerialize;   /**< @brief Publish state after serializing an ack to send. */
//     MQTTStatus_t processLoopStatus;           /**< @brief Return value of the process loop. */
//     bool incomingPublish;                     /**< @brief Whether the incoming packet is a publish. */
//     MQTTPublishInfo_t * pPubInfo;             /**< @brief Publish information to be returned by the deserializer. */
//     uint32_t timeoutMs;                       /**< @brief The timeout value to call MQTT_ProcessLoop API with. */
// } ProcessLoopReturns_t;


// /**
//  * @brief Time at the beginning of each test. Note that this is not updated with
//  * a real clock. Instead, we simply increment this variable.
//  */
// static uint32_t globalEntryTime = 0;

// /**
//  * @brief A static buffer used by the MQTT library for storing packet data.
//  */
// static uint8_t mqttBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

// /**
//  * @brief A flag to indicate whether event callback is called from
//  * MQTT_ProcessLoop.
//  */
// static bool isEventCallbackInvoked = false;
// static bool receiveOnce = false;
// static MQTTStatus_t modifyIncomingPacketStatus = MQTTSuccess;


// /* ============================   UNITY FIXTURES ============================ */

// /* Called before each test method. */
// void setUp()
// {
//     memset( mqttBuffer, 0x0, sizeof( mqttBuffer ) );
//     MQTT_State_strerror_IgnoreAndReturn( "DUMMY_MQTT_STATE" );

//     globalEntryTime = 0;
// }

// /* Called after each test method. */
// void tearDown()
// {
// }

// /* Called at the beginning of the whole suite. */
// void suiteSetUp()
// {
//     receiveOnce = 0;
// }

// /* Called at the end of the whole suite. */
// int suiteTearDown( int numFailures )
// {
//     return numFailures;
// }


// /* ========================================================================== */

// /**
//  * @brief A mocked timer query function that increments on every call. This
//  * guarantees that only a single iteration runs in the ProcessLoop for ease
//  * of testing.
//  */
// static uint32_t getTime( void )
// {
//     return globalEntryTime++;
// }

// /**
//  * @brief Mocked failed transport read.
//  */
// static int32_t transportRecvFailure( NetworkContext_t * pNetworkContext,
//                                      void * pBuffer,
//                                      size_t bytesToRead )
// {
//     ( void ) pNetworkContext;
//     ( void ) pBuffer;
//     ( void ) bytesToRead;
//     return -1;
// }
// /**
//  * @brief Mocked transport reading one byte at a time.
//  */
// static int32_t transportRecvOneByte( NetworkContext_t * pNetworkContext,
//     void * pBuffer,
//     size_t bytesToRead )
// {
// ( void ) pNetworkContext;
// ( void ) pBuffer;
// ( void ) bytesToRead;
// return 1;
// }

// /**
//  * @brief Mocked transport send that succeeds then fails.
//  */
// static int32_t transportSendSucceedThenFail( NetworkContext_t * pNetworkContext,
//     const void * pMessage,
//     size_t bytesToSend )
// {
// int32_t retVal = bytesToSend;
// static int counter = 0;

// ( void ) pNetworkContext;
// ( void ) pMessage;

// if( counter++ )
// {
// retVal = -1;
// counter = 0;
// }

// return retVal;
// }
// /**
//  * @brief Mocked MQTT event callback.
//  *
//  * @param[in] pContext MQTT context pointer.
//  * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
//  * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
//  */
// static void eventCallback( MQTTContext_t * pContext,
//                            MQTTPacketInfo_t * pPacketInfo,
//                            MQTTDeserializedInfo_t * pDeserializedInfo, 
//                            MQTTSuccessFailReasonCode_t * pReasonCode, 
//                            MqttPropBuilder_t *sendPropsBuffer,
//                            MqttPropBuilder_t *getPropsBuffer )
// {
//     ( void ) pContext;
//     ( void ) pPacketInfo;
//     ( void ) pDeserializedInfo;
//     ( void ) pReasonCode;
//     ( void ) sendPropsBuffer ; 
//     ( void ) getPropsBuffer ; 

//     /* Update the global state to indicate that event callback is invoked. */
//     isEventCallbackInvoked = true;
//     sendPropsBuffer->pBuffer = NULL; 
// }

// /**
//  * @brief Mocked MQTT event callback.
//  *
//  * @param[in] pContext MQTT context pointer.
//  * @param[in] pPacketInfo Packet Info pointer for the incoming packet.
//  * @param[in] pDeserializedInfo Deserialized information from the incoming packet.
//  */

// MQTTReasonCodeInfo_t GlobalAckInfo;


// static void eventCallback2( MQTTContext_t * pContext,
//                             MQTTPacketInfo_t * pPacketInfo,
//                             MQTTDeserializedInfo_t * pDeserializedInfo,
//                             MQTTSuccessFailReasonCode_t * pReasonCode, 
//                             MqttPropBuilder_t *sendPropsBuffer,
//                             MqttPropBuilder_t *getPropsBuffer )
//     {

//         ( void ) pContext;
//         ( void ) pPacketInfo;
//         ( void ) pDeserializedInfo;
//         ( void ) pReasonCode;
//         ( void ) sendPropsBuffer ; 
//         ( void ) getPropsBuffer ; 


//         uint8_t buf[13] ; 
//         buf[0] = 0x26 ; 
//         buf[1] = 0x00 ; 
//         buf[2] = 0x02 ; 
//         buf[3] = 'a' ; 
//         buf[4] = 'b' ; 
//         buf[5] = 0x00 ;
//         buf[6] = 0x02 ;
//         buf[7] = 'c' ;
//         buf[8] = 'd' ;
//         buf[9] = 0x1F ; 
//         buf[10] = 0x00 ; 
//         buf[11] = 0x01 ; 
//         buf[12] = 'e' ; 

//         sendPropsBuffer->pBuffer = buf ; 
//         sendPropsBuffer->currentIndex = 13 ; 
//         isEventCallbackInvoked = true;
//     }
// static void eventCallback3( MQTTContext_t * pContext,
//                             MQTTPacketInfo_t * pPacketInfo,
//                             MQTTDeserializedInfo_t * pDeserializedInfo,
//                             MQTTSuccessFailReasonCode_t * pReasonCode, 
//                             MqttPropBuilder_t *sendPropsBuffer,
//                             MqttPropBuilder_t *getPropsBuffer )
//     {

//         ( void ) pContext;
//         ( void ) pPacketInfo;
//         ( void ) pDeserializedInfo;

//         isEventCallbackInvoked = true;

//         *pReasonCode = MQTTReasonSuccess ; 
//         pContext->ackPropsBuffer.pBuffer = NULL ;

//         pDeserializedInfo->packetIdentifier = 0;
//     }


// /**
//  * @brief Mocked transport send that always returns 0 bytes sent.
//  */
// static int32_t transportSendNoBytes( NetworkContext_t * pNetworkContext,
//                                      const void * pBuffer,
//                                      size_t bytesToWrite )
// {
//     ( void ) pNetworkContext;
//     ( void ) pBuffer;
//     ( void ) bytesToWrite;
//     return 0;
// }

// static uint8_t * MQTTV5_SerializeAckFixed_cb( uint8_t * pIndex,
//                                               uint8_t packetType,
//                                               uint16_t packetId,
//                                               size_t remainingLength,
//                                               size_t propertyLength,
//                                               int numcallbacks )
// {
//     ( void ) packetType;
//     ( void ) packetId;
//     ( void ) remainingLength;
//     ( void ) propertyLength;
//     ( void ) numcallbacks;

//     return pIndex;
// }

// static uint8_t * MQTTV5_SerializeDisconnectFixed_cb( uint8_t * pIndex,
//                                                      const MQTTReasonCodeInfo_t * pAckInfo,
//                                                      size_t remainingLength,
//                                                      uint32_t sessionExpiry,
//                                                      int numcallbacks )
// {
//     ( void ) pIndex;
//     ( void ) pAckInfo;
//     ( void ) remainingLength;
//     ( void ) sessionExpiry;
//     ( void ) numcallbacks;

//     return pIndex;
// }



// static uint8_t * MQTT_SerializeConnectFixedHeader_cb( uint8_t * pIndex,
//                                                       const MQTTConnectInfo_t * pConnectInfo,
//                                                       const MQTTPublishInfo_t * pWillInfo,
//                                                       size_t remainingLength,
//                                                       int numcallbacks )
// {
//     ( void ) pConnectInfo;
//     ( void ) pWillInfo;
//     ( void ) remainingLength;
//     ( void ) numcallbacks;

//     return pIndex;
// }

// static uint8_t * MQTTV5_SerializeConnectProperties_cb( uint8_t * pIndex,
//                                                        const MQTTConnectProperties_t * pConnectProperties
//                                                        ,
//                                                        int numcallbacks )
// {
//     ( void ) pConnectProperties;
//     ( void ) numcallbacks;

//     return pIndex;
// }

// static uint8_t * MQTT_SerializePublishProperties_cb( const MQTTPublishInfo_t * pPublishInfo,
//                                                      uint8_t * pIndex
//                                                      ,
//                                                      int numcallbacks )
// {
//     ( void ) pPublishInfo;
//     ( void ) numcallbacks;

//     return pIndex;
// }

// /**
//  * @brief Mocked successful transport writev.
//  *
//  * @param[in] tcpSocket TCP socket.
//  * @param[in] pMessage vectors to send
//  * @param[in] bytesToWrite number of vectors
//  *
//  * @return Number of bytes sent; negative value on error;
//  * 0 for timeout or 0 bytes sent.
//  */
// static int32_t transportWritevSuccess( NetworkContext_t * pNetworkContext,
//                                        TransportOutVector_t * pIoVectorIterator,
//                                        size_t vectorsToBeSent )
// {
//     TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
//     int32_t bytesToWrite = 0;
//     size_t i;

//     for( i = 0; i < vectorsToBeSent; ++i )
//     {
//         bytesToWrite += pIoVectorIterator->iov_len;
//         pIoVectorIterator++;
//     }

//     return bytesToWrite;
// }
// static int32_t transportWritevFail( NetworkContext_t * pNetworkContext,
//     TransportOutVector_t * pIoVectorIterator,
//     size_t vectorsToBeSent )
// {
//     TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
//     int32_t bytesToWrite = 0;
//     size_t i;

//     for( i = 0; i < vectorsToBeSent; ++i )
//     {
//         bytesToWrite += pIoVectorIterator->iov_len;
//         pIoVectorIterator++;
//     }

//     return bytesToWrite + 3;
// }


// /**
//  * @brief Mocked successful transport send.
//  *
//  * @param[in] tcpSocket TCP socket.
//  * @param[in] pMessage Data to send.
//  * @param[in] bytesToWrite Length of data to send.
//  *
//  * @return Number of bytes sent; negative value on error;
//  * 0 for timeout or 0 bytes sent.
//  */
// static int32_t transportSendSuccess( NetworkContext_t * pNetworkContext,
//                                      const void * pBuffer,
//                                      size_t bytesToWrite )
// {
//     TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
//     ( void ) pBuffer;
//     return bytesToWrite;
// }

// /**
//  * @brief Mocked failed transport send.
//  */
// static int32_t transportSendFailure( NetworkContext_t * pNetworkContext,
//                                      const void * pBuffer,
//                                      size_t bytesToWrite )
// {
//     ( void ) pNetworkContext;
//     ( void ) pBuffer;
//     ( void ) bytesToWrite;
//     return -1;
// }

// /**
//  * @brief Mocked successful transport read.
//  *
//  * @param[in] tcpSocket TCP socket.
//  * @param[out] pBuffer Buffer for receiving data.
//  * @param[in] bytesToRead Size of pBuffer.
//  *
//  * @return Number of bytes received; negative value on error.
//  */
// static int32_t transportRecvSuccess( NetworkContext_t * pNetworkContext,
//                                      void * pBuffer,
//                                      size_t bytesToRead )
// {
//     TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
//     ( void ) pBuffer;
//     return bytesToRead;
// }


// /**
//  * @brief Initialize the transport interface with the mocked functions for
//  * send and receive.
//  *
//  * @brief param[in] pTransport The transport interface to use with the context.
//  */
// static void setupTransportInterface( TransportInterface_t * pTransport )
// {
//     pTransport->pNetworkContext = MQTT_SAMPLE_NETWORK_CONTEXT;
//     pTransport->send = transportSendSuccess;
//     pTransport->recv = transportRecvSuccess;
//     pTransport->writev = transportWritevSuccess;
// }

// /**
//  * @brief Initialize pNetworkBuffer using static buffer.
//  *
//  * @param[in] pNetworkBuffer Network buffer provided for the context.
//  */
// static void setupNetworkBuffer( MQTTFixedBuffer_t * const pNetworkBuffer )
// {
//     pNetworkBuffer->pBuffer = mqttBuffer;
//     pNetworkBuffer->size = MQTT_TEST_BUFFER_LENGTH;
// }

// static int32_t transportRecvNoData( NetworkContext_t * pNetworkContext,
//     void * pBuffer,
//     size_t bytesToRead )
// {
// ( void ) pNetworkContext;
// ( void ) pBuffer;
// ( void ) bytesToRead;
// return 0;
// }

// /**
//  * @brief A mocked timer function that could be used on a device with no system
//  * time.
//  */
// static uint32_t getTimeDummy( void )
// {
//     return 0;
// }
// /**
//  * @brief create default context
//  *
//  * @param[out] context to initialize
//  */
// static void setUPContext( MQTTContext_t * mqttContext )
// {
//     MQTTStatus_t mqttStatus;
//     static TransportInterface_t transport = { 0 };
//     static MQTTFixedBuffer_t networkBuffer = { 0 };
//     static MQTTPubAckInfo_t incomingRecords[ 10 ] = { 0 };
//     static MQTTPubAckInfo_t outgoingRecords[ 10 ] = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     memset( mqttContext, 0x0, sizeof( MQTTContext_t ) );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( mqttContext,
//                             &transport,
//                             getTime,
//                             eventCallback,
//                             &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( mqttContext,
//                                        outgoingRecords,
//                                        10,
//                                        incomingRecords,
//                                        10 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTT_Connect()
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTConnectInfo_t connectInfo = { 0 };
//     bool sessionPresent = false;
//     MQTTStatus_t status;
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties = {0} ;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength); 

//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
// /*  No connect Properties */
//     MQTT_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter ) ;
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0U, &sessionPresent, NULL , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTT_Connect_error_path()
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTConnectInfo_t connectInfo = { 0 };
//     MQTTPublishInfo_t willInfo = { 0 };
//     uint32_t timeout = 2;
//     bool sessionPresent;
//     MQTTStatus_t status;
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     memset( &willInfo, 0x0, sizeof( MQTTPublishInfo_t ) );
//     memset( &connectInfo, 0x0, sizeof( MQTTConnectInfo_t ) );

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);

//     /* With non-NULL Will. */
//     mqttContext.connectStatus = MQTTNotConnected;
//     mqttContext.keepAliveIntervalSec = 0;

//     MqttPropBuilder_t propBuilder;
//     uint8_t buf[500];
//     size_t bufLength = sizeof(buf);
//     status = MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength);


//     MqttPropBuilder_t willPropsBuilder;
//     uint8_t wbuf[500];
//     size_t wbufLength = sizeof(wbuf);
//     status = MqttPropertyBuilder_Init(&(willPropsBuilder), wbuf, wbufLength);
//     willInfo.pTopicName = "test";
//     willInfo.topicNameLength = 4;
//     willInfo.pPayload = "Payload";
//     willInfo.payloadLength = 7;
//     willInfo.payloadFormat = 1;

//     connectInfo.pUserName = "abcd";
//     connectInfo.userNameLength = 3;
//     connectInfo.passwordLength = 4;
//     connectInfo.pPassword = "1234";
//     mqttContext.transportInterface.send = transportSendSuccess;
//     MQTT_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
//     updateContextWithConnectProps_ExpectAnyArgsAndReturn(MQTTSuccess) ; 
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, &propBuilder , &willPropsBuilder);
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );


//     MQTT_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );

//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
//     status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent , NULL , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     MQTT_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );

//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTRecvFailed );
//     status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     status = MQTT_Connect( NULL, &connectInfo, &willInfo, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     willInfo.pTopicName = NULL;
//     mqttContext.transportInterface.send = transportSendSuccess;
//     MQTT_GetConnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );

//     status = MQTT_Connect( &mqttContext, &connectInfo, &willInfo, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTT_Connect_receiveConnack( void )
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTConnectInfo_t connectInfo = { 0 };
//     uint32_t timeout = 0;
//     bool sessionPresent;
//     MQTTStatus_t status;
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties = { 0 };
//     MQTTPacketInfo_t incomingPacket = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     transport.recv = transportRecvFailure;

//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     memset( &properties, 0x0, sizeof( properties ) );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     /* Everything before receiving the CONNACK should succeed. */
//     MQTT_GetConnectPacketSize_IgnoreAndReturn( MQTTSuccess );

//     /* Nothing received from transport interface. Set timeout to 2 for branch coverage. */
//     timeout = 2;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTNoDataAvailable );
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTNoDataAvailable );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL, NULL  );
//     TEST_ASSERT_EQUAL_INT( MQTTNoDataAvailable, status );


//     /* Did not receive a CONNACK. */
//     incomingPacket.type = MQTT_PACKET_TYPE_PINGRESP;
//     incomingPacket.remainingLength = 0;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* Transport receive failure when receiving rest of packet. */
//     incomingPacket.type = MQTT_PACKET_TYPE_CONNACK;
//     incomingPacket.remainingLength = 2;
//     timeout = 2;
//     mqttContext.transportInterface.recv = transportRecvFailure;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     mqttContext.transportInterface.recv = transportRecvSuccess;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTT_DeserializeConnack_ExpectAnyArgsAndReturn( MQTTBadResponse );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     mqttContext.transportInterface.recv = transportRecvSuccess;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTT_DeserializeConnack_ExpectAnyArgsAndReturn( MQTTSuccess );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL , NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }

// void test_MQTT_Publish( void )
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTPublishInfo_t publishInfo = { 0 };
//     MQTTConnectProperties_t properties = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t status;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     const uint16_t PACKET_ID = 1;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     transport.send = transportSendFailure;

//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     memset( &publishInfo, 0x0, sizeof( publishInfo ) );
//     memset( &properties, 0x0, sizeof( properties ) );


//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     status = MQTT_InitStatefulQoS( &mqttContext,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
    
//     MqttPropBuilder_t propBuilder;
//     uint8_t buf[500];
//     size_t bufLength = sizeof(buf);
//     status = MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength);

//     MQTT_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
//     status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     MQTT_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


//     publishInfo.qos = 0 ; 
//     publishInfo.pTopicName = "ab" ; 
//     publishInfo.topicNameLength = 2 ; 
//     publishInfo.retain = 1 ; 
//     publishInfo.dup = 1 ;
//     publishInfo.pPayload = "Payload" ;
//     publishInfo.payloadLength = 7 ;

//     validatePublishProperties_ExpectAnyArgsAndReturn(MQTTSuccess); 
//     MQTT_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
//     status = MQTT_Publish( &mqttContext, &publishInfo, PACKET_ID, &propBuilder );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }

// /**
//  * @brief Test that MQTT_Publish works as intended.
//  */
// void test_MQTT_Publish_DuplicatePublish_UpdateFailed( void )
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTPublishInfo_t publishInfo = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t status;
//     MQTTPubAckInfo_t outgoingPublishRecord[ 10 ];

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     transport.send = transportSendFailure;
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);


//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     memset( &publishInfo, 0x0, sizeof( publishInfo ) );
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     /* Always return success from now on. */
//     MQTT_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ReserveState_ExpectAnyArgsAndReturn( MQTTStateCollision );
//     MQTT_UpdateStatePublish_ExpectAnyArgsAndReturn( MQTTSendFailed );

//     mqttContext.outgoingPublishRecordMaxCount = 10;
//     mqttContext.outgoingPublishRecords = outgoingPublishRecord;

//     publishInfo.qos = MQTTQoS1;
//     publishInfo.dup = true;
//     publishInfo.pPayload = "TestPublish";
//     publishInfo.payloadLength = strlen( publishInfo.pPayload );
//     publishInfo.pTopicName = "TestTopic";
//     publishInfo.topicNameLength = strlen( publishInfo.pTopicName );

//     status = MQTT_Publish( &mqttContext, &publishInfo, 10, NULL );

//     TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
// }

// /**
//  * @brief Test that MQTT_Publish works as intended.
//  */
// void test_MQTT_Publish8( void )
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTPublishInfo_t publishInfo = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t status;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     transport.writev = transportWritevFail;

//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     memset( &publishInfo, 0x0, sizeof( publishInfo ) );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     /* We want to test the first call to sendPacket within sendPublish succeeding,
//      * and the second one failing. */
//     mqttContext.transportInterface.send = transportSendSucceedThenFail;

//     MQTT_ValidatePublishParams_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPublishPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializePublishHeaderWithoutTopic_ExpectAnyArgsAndReturn( MQTTSuccess );
//     publishInfo.pPayload = "Test";
//     publishInfo.payloadLength = 4;
//     status = MQTT_Publish( &mqttContext, &publishInfo, 0, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
// }


// void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths1( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTPublishState_t stateAfterSerialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Update state returns bad response.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelSend;
//     stateAfterSerialize = MQTTPubCompPending;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTBadResponse );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths2( void )
// {
//     MQTTStatus_t status;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Update state returns bad response.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     modifyIncomingPacketStatus = MQTTSuccess;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths3( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     uint16_t packetId = 2;
//     MQTTPacketInfo_t incomingPacket = { 0 };

//     /*Invalid packet parameters.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelSend;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingAck_Happy_Paths2( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTPublishState_t stateAfterSerialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     uint16_t packetId = 1;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Using event call back to set reason string and user properties,*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelSend;
//     stateAfterSerialize = MQTTPubCompPending;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths4( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Invalid packet parameters.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelPending;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingAck_Error_Paths5( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     uint16_t packetId = 1;
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Unable to send the packet using transport interface.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBREC;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     context.transportInterface.send = transportSendNoBytes;
//     context.transportInterface.writev = NULL;
//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelSend;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_DeserializeAck_ReturnThruPtr_pPacketId( &packetId );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
// }

// void test_MQTT_Connect_partial_receive( void )
// {
//     MQTTContext_t mqttContext = { 0 };
//     MQTTConnectInfo_t connectInfo = { 0 };
//     uint32_t timeout = 0;
//     bool sessionPresent;
//     MQTTStatus_t status;
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTPacketInfo_t incomingPacket = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     transport.recv = transportRecvNoData;

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     memset( &mqttContext, 0x0, sizeof( mqttContext ) );
//     MQTT_Init( &mqttContext, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     /* Everything before receiving the CONNACK should succeed. */
//     MQTT_GetConnectPacketSize_IgnoreAndReturn( MQTTSuccess );
//     incomingPacket.type = MQTT_PACKET_TYPE_CONNACK;
//     incomingPacket.remainingLength = 2;

//     /* Timeout in receiving entire packet, for branch coverage. This is due to the fact that the mocked
//      * receive function always returns 0 bytes read. */
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTT_SerializeConnectFixedHeader_Stub( MQTT_SerializeConnectFixedHeader_cb );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     /* Update to use mock receive function that receives one byte at a time for the
//      * rest of the test. */
//     mqttContext.transportInterface.recv = transportRecvOneByte;
//     timeout = 10;

//     /* Not enough space for packet, discard it. */
//     mqttContext.networkBuffer.size = 2;
//     incomingPacket.remainingLength = 3;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTNoDataAvailable, status );

//     /* Discard packet, no timeout provided. This should fail since multiple
//      * iterations of the discard loop are required to discard the packet, but only
//      * one will run. */
//     mqttContext.transportInterface.recv = transportRecvSuccess;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, 0, &sessionPresent, NULL , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     /* Timeout while discarding packet. */
//     /* (Mocked) read only one byte at a time to ensure timeout will occur. */
//     mqttContext.transportInterface.recv = transportRecvOneByte;
//     incomingPacket.remainingLength = 20;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, timeout, &sessionPresent, NULL, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );

//     /* Receive failure while discarding packet. */
//     mqttContext.transportInterface.recv = transportRecvFailure;
//     /* Test with dummy get time function to make sure there are no infinite loops. */
//     mqttContext.getTime = getTimeDummy;
//     MQTT_GetIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     status = MQTT_Connect( &mqttContext, &connectInfo, NULL, MQTT_NO_TIMEOUT_MS, &sessionPresent, NULL, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTRecvFailed, status );
// }

// void test_MQTTV5_Disconnect()
// {
//     uint32_t sessionExpiry = 0U;
//     MQTTStatus_t status;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );


//     /*Invalid parameters*/
//     status = MQTT_Disconnect( NULL, NULL , 0);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     MQTT_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     status = MQTT_Disconnect( &context, NULL , 0);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     // /*Bad Parameters*/
//     MQTT_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     status = MQTT_Disconnect( &context, NULL, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     MqttPropBuilder_t propBuilder;
//     uint8_t buf[500];
//     size_t bufLength = sizeof(buf);
//     MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength);


//     MQTT_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeDisconnectFixed_Stub( MQTTV5_SerializeDisconnectFixed_cb );
//     status = MQTT_Disconnect( &context, &propBuilder, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     // /*Send failed*/
//     context.transportInterface.send = transportSendFailure;
//     context.transportInterface.writev = NULL;
//     MQTT_GetDisconnectPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_SerializeDisconnectFixed_Stub( MQTTV5_SerializeDisconnectFixed_cb );
//     status = MQTT_Disconnect( &context,NULL, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTSendFailed, status );
// }

// void test_MQTT_ProcessLoop_handleIncomingDisconnect( void )
// {
//     MQTTStatus_t status;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTReasonCodeInfo_t disconnectInfo;
//     MQTTPacketInfo_t incomingPacket = { 0 };

//     memset( &disconnectInfo, 0x0, sizeof( disconnectInfo ) );
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);


//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     context.pDisconnectInfo = &disconnectInfo;
//     incomingPacket.type = MQTT_PACKET_TYPE_DISCONNECT;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeDisconnect_IgnoreAndReturn( MQTTSuccess );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Invalid packet parameters.*/
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeDisconnect_IgnoreAndReturn( MQTTProtocolError );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );
// }

// static void setupSubscriptionInfo( MQTTSubscribeInfo_t * pSubscribeInfo )
// {
//     pSubscribeInfo->qos = MQTTQoS1;
//     pSubscribeInfo->pTopicFilter = MQTT_SAMPLE_TOPIC_FILTER;
//     pSubscribeInfo->topicFilterLength = MQTT_SAMPLE_TOPIC_FILTER_LENGTH;
//     pSubscribeInfo->noLocalOption = 0 ; 
//     pSubscribeInfo->retainAsPublishedOption = 0;
//     pSubscribeInfo->retainHandlingOption = retainSendOnSubIfNotPresent ; 
// }
// static uint8_t * MQTTV5_SerializeSubscribedHeader_cb( size_t remainingLength,
//                                                     uint8_t * pIndex,
//                                                     uint16_t packetId,
//                                                     int numcallbacks )
// {
//     ( void ) remainingLength;
//     ( void ) pIndex;
//     ( void ) packetId;
//     ( void ) numcallbacks;

//     return pIndex;
// }


// void test_MQTTV5_Subscribe_invalid_params( void ) 
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };

//     /* Call subscribe with a NULL context. */
//     mqttStatus = MQTT_Subscribe( NULL, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );


//     /* Call  with a NULL subscription list. */
//     mqttStatus = MQTT_Subscribe( &context, NULL, 1, MQTT_FIRST_VALID_PACKET_ID , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Call subscribe with 0 subscriptions. */
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 0, MQTT_FIRST_VALID_PACKET_ID , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Packet ID cannot be 0 per MQTT 5.0 spec. */
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, 0, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Incoming publish records NULL but QoS > 0. */
//     subscribeInfo.qos = MQTTQoS1;
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, 10, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Test topic filter length is zero */
//     subscribeInfo.pTopicFilter = "test/topic";
//     subscribeInfo.topicFilterLength = 0;
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1,MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Test NULL topic filter */
//     subscribeInfo.pTopicFilter = NULL;
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1,MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Test invalid QoS level */
//     subscribeInfo.qos = 3; /* QoS must be 0, 1, or 2 */
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID , NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     /* Test invalid shared subscription */
//     subscribeInfo.pTopicFilter = "$share/invalid#";
//     subscribeInfo.noLocalOption = 0 ; 
//     subscribeInfo.topicFilterLength = strlen( subscribeInfo.pTopicFilter );
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

//     subscribeInfo.pTopicFilter = "$share/abc/#" ;
//     subscribeInfo.topicFilterLength = strlen( subscribeInfo.pTopicFilter );
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus); 

//     subscribeInfo.pTopicFilter = "abc" ; 
//     subscribeInfo.topicFilterLength = strlen( subscribeInfo.pTopicFilter );
//     subscribeInfo.retainHandlingOption= 3 ; 
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus);

//     subscribeInfo.retainHandlingOption  = 1 ; 
//     subscribeInfo.qos = 3 ; 
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus);

// }

// void test_MQTTV5_Subscribe_ValidateFailure( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTSubscribeInfo_t subscribeInfo;
//     MQTTContext_t testContext = { 0 }; 
    
//     memset( &subscribeInfo, 0, sizeof(subscribeInfo) );
    
//     /* Force the validation function to return an error */
//     validateSubscribeReturn = MQTTBadParameter;
    
//     mqttStatus = MQTT_Subscribe( &testContext, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID , NULL);
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
    
//     /* Restore for later tests */
//     validateSubscribeReturn = MQTTSuccess;
// }

// void test_MQTTV5_Subscribe_happy_path(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };

//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     setupSubscriptionInfo( &subscribeInfo );

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /* Initialize context. */
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     MqttPropBuilder_t propBuilder;
//     uint8_t buf[500];
//     size_t bufLength = sizeof(buf);
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength); 
    
//     propBuilder.pBuffer[0] = 0x0B ; 
//     propBuilder.pBuffer[1] = 2 ; 
//     propBuilder.currentIndex = 2 ;


//     /* Verify MQTTSuccess is returned with the following mocks. */
//     validateSubscribeProperties_ExpectAnyArgsAndReturn(MQTTSuccess) ; 
//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );

//     /* Expect the above calls when running MQTT_Subscribe. */
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, &propBuilder);

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }


// void test_MQTTV5_Subscribe_happy_path1(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo[2];
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    

//     subscribeInfo[0].qos = MQTTQoS2 ; 
//     subscribeInfo[0].pTopicFilter = "abc" ; 
//     subscribeInfo[0].topicFilterLength = 3 ;
//     subscribeInfo[0].noLocalOption = 1 ; 
//     subscribeInfo[0].retainAsPublishedOption = 1 ; 
//     subscribeInfo[0].retainHandlingOption = 0 ; 

//     subscribeInfo[1].qos = MQTTQoS0 ; 
//     subscribeInfo[1].pTopicFilter = "def" ;
//     subscribeInfo[1].retainHandlingOption = 2 ;
//     subscribeInfo[1].topicFilterLength = 3 ;
//     subscribeInfo[1].noLocalOption = 0 ;
//     subscribeInfo[1].retainAsPublishedOption = 0 ;

//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );
    
//     /* Expect the above calls when running MQTT_Subscribe. */
//     mqttStatus = MQTT_Subscribe( &context, subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );

//     subscribeInfo[0].qos = MQTTQoS0 ; 
//     subscribeInfo[0].pTopicFilter = "def" ;
//     subscribeInfo[0].retainHandlingOption = 2 ;
//     subscribeInfo[0].topicFilterLength = 3 ;
//     subscribeInfo[0].noLocalOption = 0 ;
//     subscribeInfo[0].retainAsPublishedOption = 0 ;

//     // validateSubscribeProperties_ExpectAnyArgsAndReturn(MQTTSuccess); 
//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );
    
//     mqttStatus = MQTT_Subscribe( &context, subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );

//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

//     subscribeInfo[0].qos = 3 ; 
//     subscribeInfo[0].pTopicFilter = "def" ;
//     subscribeInfo[0].retainHandlingOption = 2 ;
//     subscribeInfo[0].topicFilterLength = 3 ;
//     subscribeInfo[0].noLocalOption = 0 ;
//     subscribeInfo[0].retainAsPublishedOption = 0 ;

//     mqttStatus = MQTT_Subscribe( &context, subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );

//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
// }

// void test_MQTTV5_Subscribe_happy_path2(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = {0};
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     /* Verify MQTTSuccess is returned with the following mocks. */
//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );


//     subscribeInfo.qos = MQTTQoS1 ; 
//     subscribeInfo.pTopicFilter = "$share/abc/bcd" ;
//     subscribeInfo.topicFilterLength = 14 ;
//     subscribeInfo.noLocalOption = 0 ;
//     subscribeInfo.retainAsPublishedOption = 1 ;
//     subscribeInfo.retainHandlingOption = 0 ;

//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

// }

// void test_MQTTV5_Subscribe_happy_path3(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = {0};
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     mqttStatus = MQTT_Init(&context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL(MQTTSuccess, mqttStatus);

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL(MQTTSuccess, mqttStatus);

//     /* Verify MQTTSuccess is returned with the following mocks. */
//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );

//     subscribeInfo.qos = MQTTQoS1 ;
//     subscribeInfo.pTopicFilter = "$share/abc/" ;
//     subscribeInfo.topicFilterLength = 11 ;
//     subscribeInfo.noLocalOption = 0 ;
//     subscribeInfo.retainAsPublishedOption = 1 ;
//     subscribeInfo.retainHandlingOption = 0 ;

//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus);

// }

// void test_MQTTV5_shared_subscriptions(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = {0};
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     mqttStatus = MQTT_Init(&context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL(MQTTSuccess, mqttStatus);

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL(MQTTSuccess, mqttStatus);

//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );

//     subscribeInfo.pTopicFilter = "$share/abc/bcd" ;
//     subscribeInfo.topicFilterLength = 10 ;
//     subscribeInfo.noLocalOption = 0 ;
//     subscribeInfo.retainAsPublishedOption = 1 ;
//     subscribeInfo.retainHandlingOption = 0 ;
//     subscribeInfo.qos = MQTTQoS2 ;
    
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus);

//     subscribeInfo.qos = MQTTQoS1 ;
//     subscribeInfo.retainHandlingOption = 3 ;
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus);
//     subscribeInfo.retainHandlingOption = 0 ;



//     /** Invalid Sharename */
//     subscribeInfo.pTopicFilter = "$share/abc" ; 
//     subscribeInfo.topicFilterLength = 10 ; 
    
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus);

//     subscribeInfo.pTopicFilter = "$share/abc/bcd" ;
//     subscribeInfo.topicFilterLength = 14 ;
//     subscribeInfo.noLocalOption = 1 ; 
//     mqttStatus = MQTT_Subscribe(&context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus);
// }

// /* Suback- Unsuback Happy path */
// void test_MQTT_ProcessLoop_handleIncomingAck_Happy_Paths_suback( void )
// {
//     MQTTStatus_t status;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties = {0} ; 
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     /* Modify incoming packet depending on type to be tested. */
//     incomingPacket.type = MQTT_PACKET_TYPE_SUBACK;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     status = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     modifyIncomingPacketStatus = MQTTSuccess;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTTV5_DeserializeSuback_ExpectAnyArgsAndReturn( MQTTSuccess );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }


// static uint8_t * MQTTV5_SerializeUnsubscribeHeader_cb( size_t remainingLength,
//     uint8_t * pIndex,
//     uint16_t packetId,
//     int numcallbacks )
// {
// ( void ) remainingLength;
// ( void ) pIndex;
// ( void ) packetId;
// ( void ) numcallbacks;

// return pIndex;
// }

// /**
// * @brief This test case verifies that MQTT_Unsubscribe returns successfully
// * when valid parameters are passed and all bytes are sent.
// */
// void test_MQTT_Unsubscribe_happy_path( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };
//     MQTTConnectProperties_t properties = {0} ; 
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     setupSubscriptionInfo( &subscribeInfo );
//     subscribeInfo.qos = MQTTQoS0;

//     /* Initialize context. */
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );


//     /* Verify MQTTSuccess is returned with the following mocks. */
//     MQTT_SerializeUnsubscribeHeader_Stub(MQTTV5_SerializeUnsubscribeHeader_cb);
//     MQTT_GetUnsubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );

//     /* Expect the above calls when running MQTT_Unsubscribe. */

//     MqttPropBuilder_t propBuilder;
//     uint8_t buf[500];
//     size_t bufLength = sizeof(buf);
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength);
//     mqttStatus = MQTT_Unsubscribe( &context, &subscribeInfo,  1, MQTT_FIRST_VALID_PACKET_ID, &propBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     MQTT_GetUnsubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     mqttStatus = MQTT_Unsubscribe( &context, &subscribeInfo,  1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );


// }

// void test_MQTT_Unsubscribe_happy_path_withUP( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     setupSubscriptionInfo( &subscribeInfo );
//     subscribeInfo.qos = MQTTQoS0;
//     /* Initialize context. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     /* Verify MQTTSuccess is returned with the following mocks. */
//     MQTT_SerializeUnsubscribeHeader_Stub(MQTTV5_SerializeUnsubscribeHeader_cb);
//     MQTT_GetUnsubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetUnsubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );

//     mqttStatus = MQTT_Unsubscribe( &context, &subscribeInfo,  1, MQTT_FIRST_VALID_PACKET_ID, NULL);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTTV5_Unsubscribe_happy_path(void){
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };

//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     setupSubscriptionInfo( &subscribeInfo );

//     /* Initialize context. */
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( &context,
//                                        &outgoingRecords, 4,
//                                        &incomingRecords, 4 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     /* Verify MQTTSuccess is returned with the following mocks. */
//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTTV5_SerializeSubscribedHeader_cb );

//     /* Expect the above calls when running MQTT_Subscribe. */
//     mqttStatus = MQTT_Subscribe( &context, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// static void resetProcessLoopParams( ProcessLoopReturns_t * pExpectParams )
// {
//     pExpectParams->deserializeStatus = MQTTSuccess;
//     pExpectParams->stateAfterDeserialize = MQTTStateNull;
//     pExpectParams->updateStateStatus = MQTTSuccess;
//     pExpectParams->serializeStatus = MQTTSuccess;
//     pExpectParams->stateAfterSerialize = MQTTStateNull;
//     pExpectParams->processLoopStatus = MQTTSuccess;
//     pExpectParams->incomingPublish = false;
//     pExpectParams->pPubInfo = NULL;
//     pExpectParams->timeoutMs = MQTT_NO_TIMEOUT_MS;
// }
// uint8_t currentPacketType ; 
// static void expectProcessLoopCalls( MQTTContext_t * const pContext, ProcessLoopReturns_t * pExpectParams )
// {
//     MQTTStatus_t mqttStatus = MQTTSuccess;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     size_t pingreqSize = MQTT_PACKET_PINGREQ_SIZE;
//     bool expectMoreCalls = true;
//     /* Copy values passed in the parameter struct. */
//     MQTTStatus_t deserializeStatus = pExpectParams->deserializeStatus;
//     MQTTPublishState_t stateAfterDeserialize = pExpectParams->stateAfterDeserialize;
//     MQTTStatus_t updateStateStatus = pExpectParams->updateStateStatus;
//     MQTTStatus_t serializeStatus = pExpectParams->serializeStatus;
//     MQTTPublishState_t stateAfterSerialize = pExpectParams->stateAfterSerialize;
//     MQTTStatus_t processLoopStatus = pExpectParams->processLoopStatus;
//     bool incomingPublish = pExpectParams->incomingPublish;
//     MQTTPublishInfo_t * pPubInfo = pExpectParams->pPubInfo;
//     uint32_t packetTxTimeoutMs = 0U;

//     /* Modify incoming packet depending on type to be tested. */
//     incomingPacket.type = currentPacketType;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     /* More calls are expected only with the following packet types. */
//     if( ( currentPacketType != MQTT_PACKET_TYPE_PUBLISH ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_PUBACK ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_PUBREC ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_PUBREL ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_PUBCOMP ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_PINGRESP ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_SUBACK ) &&
//     ( currentPacketType != MQTT_PACKET_TYPE_UNSUBACK ) )
//     {
//         expectMoreCalls = false;
//     }

//     /* When no data is available, the process loop tries to send a PINGREQ. */
//     if( modifyIncomingPacketStatus == MQTTNoDataAvailable )
//     {
//         packetTxTimeoutMs = 1000U * ( uint32_t ) pContext->keepAliveIntervalSec;

//         if( PACKET_TX_TIMEOUT_MS < packetTxTimeoutMs )
//         {
//         packetTxTimeoutMs = PACKET_TX_TIMEOUT_MS;
//         }

//         if( pContext->waitingForPingResp == false )
//         {
//             if( ( ( packetTxTimeoutMs != 0U ) &&
//             ( ( globalEntryTime - pContext->lastPacketTxTime ) >= packetTxTimeoutMs ) ) ||
//             ( ( PACKET_RX_TIMEOUT_MS != 0U ) &&
//             ( ( globalEntryTime - pContext->lastPacketRxTime ) >= PACKET_RX_TIMEOUT_MS ) ) )
//             {
//                 MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//                 /* Replace pointer parameter being passed to the method. */
//                 MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//                 MQTT_SerializePingreq_ExpectAnyArgsAndReturn( serializeStatus );
//             }
//         }

//         expectMoreCalls = false;
//     }

//     /* Deserialize based on the packet type (PUB or ACK) being received. */
//     if( expectMoreCalls )
//     {
//         if( incomingPublish )
//         {
//             MQTT_DeserializePublish_ExpectAnyArgsAndReturn( deserializeStatus );

//             if( pPubInfo != NULL )
//             {
//                 MQTT_DeserializePublish_ReturnThruPtr_pPublishInfo( pPubInfo );
//             }
//         }
//         else
//         {
//             MQTT_DeserializePing_ExpectAnyArgsAndReturn( deserializeStatus );
//         }

//         if( ( deserializeStatus != MQTTSuccess ) ||
//         ( currentPacketType == MQTT_PACKET_TYPE_PINGRESP ) ||
//         ( currentPacketType == MQTT_PACKET_TYPE_SUBACK ) ||
//         ( currentPacketType == MQTT_PACKET_TYPE_UNSUBACK ) )
//         {
//         expectMoreCalls = false;
//         }
//     }

//     /* Update state based on the packet type (PUB or ACK) being received. */
//     if( expectMoreCalls )
//     {
//         if( incomingPublish )
//         {
//             MQTT_UpdateStatePublish_ExpectAnyArgsAndReturn( updateStateStatus );
//             MQTT_UpdateStatePublish_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//         }
//         else
//         {
//             MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( updateStateStatus );
//             MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//         }

//         if( stateAfterDeserialize == MQTTPublishDone )
//         {
//             expectMoreCalls = false;
//         }
//         else
//         {
//             switch( updateStateStatus )
//             {
//                 case MQTTSuccess:
//                 expectMoreCalls = true;
//                 break;

//                 case MQTTStateCollision:
//                 /* Execution will continue regardless of the dup flag. */
//                 expectMoreCalls = true;
//                 MQTT_CalculateStatePublish_ExpectAnyArgsAndReturn( stateAfterDeserialize );
//                 break;

//                 default:
//                 expectMoreCalls = false;
//             }
//         }
//     }

//     /* Serialize the packet to be sent in response to the received packet. */
//     if( expectMoreCalls )
//     {
//         if( currentPacketType == MQTT_PACKET_TYPE_PUBLISH )
//         {
//             MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn(MQTTSuccess) ; 
//             MQTTV5_SerializeAckFixed_ExpectAnyArgsAndReturn(MQTTSuccess) ; 
//         }
//         else
//         {
        
//         MQTT_SerializeAck_ExpectAnyArgsAndReturn( serializeStatus );
//         }

//         if( serializeStatus != MQTTSuccess )
//         {
//             expectMoreCalls = false;
//         }
//     }

//     /* Update the state based on the sent packet. */
//     if( expectMoreCalls )
//     {
//         MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( ( stateAfterSerialize == MQTTStateNull ) ?
//                         MQTTIllegalState : MQTTSuccess );
//         MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
//     }

//     /* Expect the above calls when running MQTT_ProcessLoop. */

//     mqttStatus = MQTT_ProcessLoop( pContext );
//     TEST_ASSERT_EQUAL( processLoopStatus, mqttStatus );

//     /* Any final assertions to end the test. */
//     if( mqttStatus == MQTTSuccess )
//     {
//         if( currentPacketType == MQTT_PACKET_TYPE_PUBLISH )
//         {
//             TEST_ASSERT_TRUE( pContext->controlPacketSent );
//         }

//         if( currentPacketType == MQTT_PACKET_TYPE_PINGRESP )
//         {
//             TEST_ASSERT_FALSE( pContext->waitingForPingResp );
//         }
//     }
// }
// void test_IncomingPublishV5(void)
// {
//     MQTTStatus_t status;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     /* Modify incoming packet depending on type to be tested. */
//     uint8_t buffer[19] = { 0 };

//     incomingPacket.type = MQTT_PACKET_TYPE_PUBLISH;
//     incomingPacket.pRemainingData = buffer;
//     incomingPacket.remainingLength = 19;

//     buffer[0] = 0x00 ; 
//     buffer[1] = 0x03 ; 
//     buffer[2] = 0x61 ; 
//     buffer[3] = 0x62 ; 
//     buffer[4] = 0x63 ; 
//     buffer[5] = 0xD; 
//     buffer[6] = 0x01 ;
//     buffer[7] = 0x01 ; 
//     buffer[8] = 0x26 ; 
//     buffer[9] = 0x00 ; 
//     buffer[10] = 0x03 ; 
//     buffer[11] = 0x61 ; 
//     buffer[12] = 0x62 ;
//     buffer[13] = 0x63 ; 
//     buffer[14] = 0x00 ; 
//     buffer[15] = 0x03 ; 
//     buffer[16] = 0x61 ; 
//     buffer[17] = 0x62 ;
//     buffer[18] = 0x63 ; 

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);



//     status = MQTT_Init( &context, &transport, getTime, eventCallback3, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     properties.requestProblemInfo = 1;
//     modifyIncomingPacketStatus = MQTTSuccess;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTT_DeserializePublish_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStatePublish_ExpectAnyArgsAndReturn(MQTTSuccess) ; 
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }
// void test_IncomingPublishV51(void) {

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     ProcessLoopReturns_t expectParams = { 0 };
//     MQTTPubAckInfo_t pIncomingCallback[ 10 ];

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MQTT_InitStatefulQoS( &context, NULL, 0, pIncomingCallback, 10 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     modifyIncomingPacketStatus = MQTTSuccess;

//     /* Assume QoS = 1 so that a PUBACK will be sent after receiving PUBLISH.
//      * That is, expectProcessLoopCalls will take on the following parameters:
//      * incomingPublish=true and stateAfterDeserialize=MQTTPubAckSend. */
//     currentPacketType = MQTT_PACKET_TYPE_PUBLISH;
//     /* Set expected return values during the loop. */
//     resetProcessLoopParams( &expectParams );
//     expectParams.stateAfterDeserialize = MQTTPubAckSend;
//     expectParams.stateAfterSerialize = MQTTPublishDone;
//     expectParams.incomingPublish = true;
//     expectProcessLoopCalls( &context, &expectParams );


// }
// static uint8_t * MQTT_SerializeSubscribedHeader_cb( size_t remainingLength,
//     uint8_t * pIndex,
//     uint16_t packetId,
//     int numcallbacks )
// {
//     ( void ) remainingLength;
//     ( void ) pIndex;
//     ( void ) packetId;
//     ( void ) numcallbacks;

//     return pIndex;
// }


// void test_MQTT_SubscribeV5_sendFailed( void )
// {
//     MQTTStatus_t mqttStatus = { 0 };
//     MQTTContext_t con = { 0 };
//     TransportInterface_t trans = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTSubscribeInfo_t subscribeInfo = { 0 };
//     size_t remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     size_t packetSize = MQTT_SAMPLE_REMAINING_LENGTH;

//     /* Verify that an error is propagated when transport interface returns an error. */
//     setupNetworkBuffer( &networkBuffer );
//     setupSubscriptionInfo( &subscribeInfo );
//     subscribeInfo.qos = MQTTQoS0;
//     setupTransportInterface( &trans );
//     trans.send = transportSendFailure;
//     trans.writev = transportWritevFail;

//     /* Initialize context. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &con, &trans, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     MQTT_GetSubscribePacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pPacketSize( &packetSize );
//     MQTT_GetSubscribePacketSize_ReturnThruPtr_pRemainingLength( &remainingLength );
//     MQTT_SerializeSubscribeHeader_Stub( MQTT_SerializeSubscribedHeader_cb );
//     /* Expect the above calls when running MQTT_Subscribe. */
//     mqttStatus = MQTT_Subscribe( &con, &subscribeInfo, 1, MQTT_FIRST_VALID_PACKET_ID, NULL );
//     TEST_ASSERT_EQUAL( MQTTSendFailed, mqttStatus );
// }
// static int32_t transportRecvOneSuccessOneFail( NetworkContext_t * pNetworkContext,
//                                                void * pBuffer,
//                                                size_t bytesToRead )
// {
//     TEST_ASSERT_EQUAL( MQTT_SAMPLE_NETWORK_CONTEXT, pNetworkContext );
//     ( void ) pBuffer;

//     if( receiveOnce == false )
//     {
//         receiveOnce = true;
//         return bytesToRead;
//     }
//     else
//     {
//         return -1;
//     }
// }
// void test_MQTT_ProcessLoop_discardPacket_second_recv_fail( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupTransportInterface( &transport );
//     transport.recv = transportRecvOneSuccessOneFail;
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     context.networkBuffer.size = 20;

//     incomingPacket.type = currentPacketType;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTRecvFailed, mqttStatus );
// }
// /**
//  * @brief Test that NULL pContext causes MQTT_ProcessLoop to return MQTTBadParameter.
//  */
// void test_MQTT_ProcessLoop_Invalid_Params( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus = MQTT_ProcessLoop( NULL );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     setupTransportInterface( &transport );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     /* Get time function cannot be NULL. */
//     context.getTime = NULL;
//     mqttStatus = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
//     /* Restore the time function for the next test. */
//     context.getTime = getTime;

//     /* The fixed network buffer cannot be NULL. */
//     context.networkBuffer.pBuffer = NULL;

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }

// void test_MQTT_ProcessLoop_discardPacket( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupNetworkBuffer( &networkBuffer );
//     setupTransportInterface( &transport );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     context.networkBuffer.size = 20;

//     incomingPacket.type = currentPacketType;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTT_ProcessLoop_IncomingBufferNotInit( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MQTTPublishInfo_t publishInfo = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     /* First byte shows that it is a publish type packet. */
//     mqttBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     /* Length of the packet. */
//     mqttBuffer[ 1 ] = 20;

//     incomingPacket.type = MQTT_PACKET_TYPE_PUBLISH;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     /* Assume QoS = 1. */
//     publishInfo.qos = MQTTQoS1;

//     MQTT_DeserializePublish_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_DeserializePublish_ReturnThruPtr_pPublishInfo( &publishInfo );

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTRecvFailed, mqttStatus );
// }

// void test_MQTT_ProcessLoop_handleIncomingPublish_Happy_Path3( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTPublishInfo_t pubInfo = { 0 };
//     ProcessLoopReturns_t expectParams = { 0 };
//     MQTTPubAckInfo_t incomingRecords = { 0 };
//     MQTTPubAckInfo_t outgoingRecords = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     mqttStatus = MQTT_Init( &context,
//                             &transport,
//                             getTime,
//                             eventCallback,
//                             &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     MQTT_InitStatefulQoS( &context,
//                           &outgoingRecords, 4,
//                           &incomingRecords, 4 );

//     modifyIncomingPacketStatus = MQTTSuccess;

//     /* Duplicate QoS1 publish received.
//      * expectProcessLoopCalls will take on the following parameters:
//      * incomingPublish=true, stateAfterDeserialize=MQTTPubAckSend,
//      * updateStateStatus=MQTTStateCollision and pPubInfo is passed with
//      * dup flag set. The event callback should not be invoked. */
//     currentPacketType = MQTT_PACKET_TYPE_PUBLISH;
//     pubInfo.dup = true;
//     pubInfo.qos = MQTTQoS1;
//     isEventCallbackInvoked = false;
//     /* Set expected return values during the loop. */
//     resetProcessLoopParams( &expectParams );
//     expectParams.stateAfterDeserialize = MQTTPubAckSend;
//     expectParams.updateStateStatus = MQTTStateCollision;
//     expectParams.stateAfterSerialize = MQTTPublishDone;
//     expectParams.deserializeStatus = MQTTSuccess;
//     expectParams.incomingPublish = true;
//     expectParams.pPubInfo = &pubInfo;
//     expectProcessLoopCalls( &context, &expectParams );
//     TEST_ASSERT_TRUE( isEventCallbackInvoked );
// }

// /**
//  * @brief Test that MQTT_ReceiveLoop() works as intended. Since the only difference
//  * between this and the process loop is keep alive, we only need to test the
//  * differences for coverage.
//  */
// void test_MQTT_ReceiveLoop( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer,&ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     /* Verify that a NULL Context returns an error. */
//     mqttStatus = MQTT_ReceiveLoop( NULL );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     /* Verify that a NULL time function returns an error. */
//     context.getTime = NULL;
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
//     context.getTime = getTime;

//     /* Verify that a null fixed network buffer returns an error. */
//     context.networkBuffer.pBuffer = NULL;
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
//     setupNetworkBuffer( &( context.networkBuffer ) );

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTBadResponse );
//     /* Error case, for branch coverage. */
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, mqttStatus );

//     /* This will cover the case when there is data available in the buffer to be processed but
//      * no additional bytes have been received by the transport interface. */
//     context.transportInterface.recv = transportRecvNoData;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTBadResponse );
//     /* Error case, for branch coverage. */
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, mqttStatus );

//     /* Reset the index to clear the buffer of any remaining data. */
//     context.index = 0;
//     /* Keep Alive should not trigger.*/
//     context.keepAliveIntervalSec = 1;
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     TEST_ASSERT_FALSE( context.controlPacketSent );

//     /* Test with a dummy getTime to ensure there's no infinite loops. */
//     context.getTime = getTimeDummy;
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     context.getTime = getTime;

//     /* Receive a PINGRESP. */
//     mqttStatus = MQTT_ReceiveLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }


// void test_MQTT_Ping_invalid_params( void )
// {
//     MQTTStatus_t mqttStatus;

//     /* Call ping with a NULL context. */
//     mqttStatus = MQTT_Ping( NULL );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }

// /**
//  * @brief This test case verifies that MQTT_Ping returns successfully
//  * when valid parameters are passed and all bytes are sent.
//  */
// void test_MQTT_Ping_happy_path( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     size_t pingreqSize = MQTT_PACKET_PINGREQ_SIZE;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     /* Initialize context. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     /* Verify MQTTSuccess is returned. */
//     MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//     MQTT_SerializePingreq_ExpectAnyArgsAndReturn( MQTTSuccess );
//     /* Expect the above calls when running MQTT_Ping. */
//     mqttStatus = MQTT_Ping( &context );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     TEST_ASSERT_EQUAL( context.lastPacketTxTime, context.pingReqSendTimeMs );
//     TEST_ASSERT_TRUE( context.waitingForPingResp );
// }

// /**
//  * @brief This test case verifies that MQTT_Ping returns MQTTSendFailed
//  * if transport interface send returns an error.
//  */
// void test_MQTT_Ping_error_path( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     size_t pingreqSize = MQTT_PACKET_PINGREQ_SIZE;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     /* Test a network error is returned from sending the PING packet over the
//      * transport send . */
//     transport.send = transportSendFailure;
//     transport.recv = transportRecvFailure;

//     /* Initialize context. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     /* Verify MQTTSendFailed is propagated when transport interface returns an error. */
//     MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//     MQTT_SerializePingreq_ExpectAnyArgsAndReturn( MQTTSuccess );
//     /* Expect the above calls when running MQTT_Ping. */
//     mqttStatus = MQTT_Ping( &context );
//     TEST_ASSERT_EQUAL( MQTTSendFailed, mqttStatus );

//     /* Case when there is timeout in sending data through transport send. */
//     transport.recv = transportRecvSuccess;
//     transport.send = transportSendNoBytes; /* Use the mock function that returns zero bytes sent. */
//     /* Initialize context. */
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//     MQTT_SerializePingreq_ExpectAnyArgsAndReturn( MQTTSuccess );
//     /* Verify that the API returns failure. */
//     mqttStatus = MQTT_Ping( &context );
//     TEST_ASSERT_EQUAL( MQTTSendFailed, mqttStatus );


//     /* Initialize context. */
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     /* Verify MQTTBadParameter is propagated when getting PINGREQ packet size fails. */
//     MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTBadParameter );
//     MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//     /* Expect the above calls when running MQTT_Ping. */
//     mqttStatus = MQTT_Ping( &context );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }

// /**
//  * @brief Test MQTT_MatchTopic for invalid input parameters.
//  */
// void test_MQTT_MatchTopic_InvalidInput( void )
// {
//     bool matchResult = false;

//     /* NULL topic name. */
//     TEST_ASSERT_EQUAL( MQTTBadParameter, MQTT_MatchTopic( NULL,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic name length. */
//     TEST_ASSERT_EQUAL( MQTTBadParameter, MQTT_MatchTopic( MQTT_SAMPLE_TOPIC_FILTER,
//                                                           0u,
//                                                           MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* NULL topic filter. */
//     TEST_ASSERT_EQUAL( MQTTBadParameter, MQTT_MatchTopic( MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           NULL,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic filter length. */
//     TEST_ASSERT_EQUAL( MQTTBadParameter, MQTT_MatchTopic( MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           MQTT_SAMPLE_TOPIC_FILTER,
//                                                           0u,
//                                                           &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid output parameter. */
//     TEST_ASSERT_EQUAL( MQTTBadParameter, MQTT_MatchTopic( MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           MQTT_SAMPLE_TOPIC_FILTER,
//                                                           MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                           NULL ) );
// }

// /**
//  * @brief Verifies that MQTT_MatchTopic is able to determine an exact match between the
//  * topic name and topic filter.
//  */
// void test_MQTT_MatchTopic_ExactMatch( void )
// {
//     const char * pTopicFilter = NULL;
//     const char * pTopicName = NULL;
//     bool matchResult = false;

//     /* Test for topic filter and topic name having exact match. */
//     pTopicName = "/test/match";
//     pTopicFilter = "/test/match";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test for exact match when topic name and filter start with '$' .*/
//     pTopicName = "$///";
//     pTopicFilter = "$///";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test for no match (with no wildcard in the topic filter). */
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      MQTT_SAMPLE_TOPIC_FILTER,
//                                                      MQTT_SAMPLE_TOPIC_FILTER_LENGTH,
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Edge case tests (for branch coverage) to match at end with no wildcards. */
//     pTopicName = "/test/match/";
//     pTopicFilter = "/test/match/a";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     pTopicName = "a";
//     pTopicFilter = "a/";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Edge case test (for branch coverage) when topic name has more levels
//      * than topic filter. */
//     pTopicName = "test/match";
//     pTopicFilter = "test";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );
// }

// /**
//  * @brief Verifies that MQTT_MatchTopic meets the MQTT 3.1.1 specification of all
//  * cases of matching topic filters that contain the single-level '+' wildcard.
//  */
// void test_MQTT_MatchTopic_Wildcard_SingleLevel_Match_Cases( void )
// {
//     const char * pTopicName = NULL;
//     const char * pTopicFilter = NULL;
//     bool matchResult = false;

//     /* Nominal case of topic filter ending with '+' .*/
//     pTopicName = "/test/match/level1";
//     pTopicFilter = "/test/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test for match with a topic name starting with '$' .*/
//     pTopicName = "$test/match/level1";
//     pTopicFilter = "$test/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test with '+' as the topic filter. */
//     pTopicName = "test";
//     pTopicFilter = "+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test that '+' in topic filter matches topic name containing consecutive
//      * level separators, "//" in the corresponding level. */
//     pTopicName = "/test//level1";
//     pTopicFilter = "/test/+/level1";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test with multiple placements of wildcard in topic filter. */
//     pTopicName = "/test/match/level1";
//     pTopicFilter = "/+/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     pTopicName = "/test/match/level1";
//     pTopicFilter = "+/+/+/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     pTopicName = "/test///level1";
//     pTopicFilter = "/test/+/+/level1";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test that match fails when topic name has more levels than topic filter. */
//     pTopicName = "/test/match/level1/level2";
//     pTopicFilter = "/test/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     pTopicName = "/";
//     pTopicFilter = "+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Edge case where filter ending with '/+' matches topic ending with '/'. */
//     pTopicName = "/test/match/";
//     pTopicFilter = "/test/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );
// }

// /**
//  * @brief Verifies that MQTT_MatchTopic meets the MQTT 3.1.1 specification for
//  * cases of where topic filter containing '+' wildcard do not match topic name.
//  */
// void test_MQTT_MatchTopic_Wildcard_SingleLevel_No_Match_Cases( void )
// {
//     const char * pTopicName = NULL;
//     const char * pTopicFilter = NULL;
//     bool matchResult = false;

//     /* Edge case where filter ending with '/+' should not match a topic ending with
//      * at parent level. */
//     pTopicName = "/test/match";
//     pTopicFilter = "/test/match/+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Edge case where topic filter starts with '+' and topic name starts with '$'. */
//     pTopicName = "$/test/match";
//     pTopicFilter = "+/test/match";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Topic name matches all characters with topic filter, but topic filter is invalid. */
//     pTopicName = "test/match/level";
//     pTopicFilter = "test/match/level+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic filter where non-starting '+' is not placed after '/'.*/
//     pTopicName = "test/match/level1";
//     pTopicFilter = "test/match/level+";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic filter where intermediate '+' is not followed by '/'.*/
//     pTopicName = "test/match/level";
//     pTopicFilter = "test/+?level";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );
// }

// /**
//  * @brief Verifies that MQTT_MatchTopic meets the MQTT 3.1.1 specification of all
//  * cases of matching topic filters that contain the multi-level '#' wildcard.
//  */
// void test_MQTT_MatchTopic_Wildcard_MultiLevel_Match_Cases( void )
// {
//     const char * pTopicName = NULL;
//     const char * pTopicFilter = NULL;
//     bool matchResult = false;

//     /* Match topic filter ending with '#' with a single level .*/
//     pTopicName = "/test/match/level1";
//     pTopicFilter = "/test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Match topic filter ending with '#' with multiple levels in topic name.*/
//     pTopicName = "/test/match/level1/level2/level3";
//     pTopicFilter = "/test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     pTopicName = "/test/match/level1/level2/level3";
//     pTopicFilter = "/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Match when topic filter is "#" */
//     pTopicName = "test/match/level";
//     pTopicFilter = "#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test for match with a topic name starting with '$' .*/
//     pTopicName = "$test/match/level1";
//     pTopicFilter = "$test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Match '#' with topic name ending at the parent level. */
//     pTopicName = "/test/match";
//     pTopicFilter = "/test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Edge case where filter ending with '/#' matches topic ending with '/'. */
//     pTopicName = "/test/match/";
//     pTopicFilter = "/test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     /* Test for topic filters containing both '+' and '#' wildcard characters. */
//     pTopicName = "/test/match";
//     pTopicFilter = "+/test/match/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );

//     pTopicName = "/test/match/level";
//     pTopicFilter = "+/+/+/#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( true, matchResult );
// }

// /**
//  * @brief Verifies that MQTT_MatchTopic meets the MQTT 3.1.1 specification for
//  * cases of where topic filter containing '#' wildcard do not match topic name.
//  */
// void test_MQTT_MatchTopic_Wildcard_MultiLevel_No_Match_Cases( void )
// {
//     const char * pTopicName = NULL;
//     const char * pTopicFilter = NULL;
//     bool matchResult = false;

//     /* Edge case where topic filter starts with '#' and topic name starts with '$'. */
//     pTopicName = "$/test/match";
//     pTopicFilter = "#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic filter where non-starting '#' is not placed after '/'.*/
//     pTopicName = "test/match/level1";
//     pTopicFilter = "test/match/level#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Topic name matches all characters with topic filter, but topic filter is invalid. */
//     pTopicName = "test/match/level";
//     pTopicFilter = "test/match/level?#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     pTopicName = "test/match/level";
//     pTopicFilter = "test/match/level#";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     /* Invalid topic filters that contain '#' at a non-ending position .*/
//     pTopicName = "test/match/level1/level2";
//     pTopicFilter = "test/match/#/level2";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );

//     pTopicName = "test/match/level2";
//     pTopicFilter = "#/match/level2";
//     TEST_ASSERT_EQUAL( MQTTSuccess, MQTT_MatchTopic( pTopicName,
//                                                      strlen( pTopicName ),
//                                                      pTopicFilter,
//                                                      strlen( pTopicFilter ),
//                                                      &matchResult ) );
//     TEST_ASSERT_EQUAL( false, matchResult );
// }

// /**
//  * @brief Tests that MQTT_GetSubAckStatusCodes works as expected in parsing the
//  * payload information of a SUBACK packet.
//  */
// void test_MQTT_GetSubAckStatusCodes( void )
// {
//     MQTTPacketInfo_t mqttPacketInfo = { 0 };
//     size_t payloadSize;
//     uint8_t * pPayloadStart;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[ 10 ] = { 0 };

//     buffer[ 0 ] = 0;
//     buffer[ 1 ] = 1;
//     buffer[ 2 ] = 0x00;
//     buffer[ 3 ] = 0x01;
//     buffer[ 4 ] = 0x02;
//     buffer[ 5 ] = 0x80;

//     /* Process a valid SUBACK packet containing whole range of server response codes. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_SUBACK;
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.remainingLength = 6;
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, &pPayloadStart, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_PTR( &buffer[ 2 ], pPayloadStart );
//     TEST_ASSERT_EQUAL_INT( MQTTSubAckSuccessQos0, pPayloadStart[ 0 ] );
//     TEST_ASSERT_EQUAL_INT( MQTTSubAckSuccessQos1, pPayloadStart[ 1 ] );
//     TEST_ASSERT_EQUAL_INT( MQTTSubAckSuccessQos2, pPayloadStart[ 2 ] );
//     TEST_ASSERT_EQUAL_INT( MQTTSubAckFailure, pPayloadStart[ 3 ] );
//     TEST_ASSERT_EQUAL_INT( 4, payloadSize );

//     /* Packet is NULL. */
//     status = MQTT_GetSubAckStatusCodes( NULL, &pPayloadStart, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Output parameter, pPayloadStart, is NULL. */
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, NULL, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Output parameter, pPayloadSize, is NULL. */
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, &pPayloadStart, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Remaining Data is NULL. */
//     mqttPacketInfo.pRemainingData = NULL;
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, &pPayloadStart, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* non-SUBACK packet type. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     mqttPacketInfo.pRemainingData = buffer;
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, &pPayloadStart, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Invalid remaining length value in packet. */
//     mqttPacketInfo.remainingLength = 0;
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_SUBACK;
//     status = MQTT_GetSubAckStatusCodes( &mqttPacketInfo, &pPayloadStart, &payloadSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// /**
//  * @brief Test MQTT_Status_strerror returns correct strings.
//  */
// void test_MQTT_Status_strerror( void )
// {
//     MQTTStatus_t status;
//     const char * str = NULL;

//     status = MQTTSuccess;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTSuccess", str );

//     status = MQTTBadParameter;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTBadParameter", str );

//     status = MQTTNoMemory;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTNoMemory", str );

//     status = MQTTSendFailed;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTSendFailed", str );

//     status = MQTTRecvFailed;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTRecvFailed", str );

//     status = MQTTBadResponse;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTBadResponse", str );

//     status = MQTTServerRefused;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTServerRefused", str );

//     status = MQTTNoDataAvailable;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTNoDataAvailable", str );

//     status = MQTTIllegalState;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTIllegalState", str );

//     status = MQTTStateCollision;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTStateCollision", str );

//     status = MQTTKeepAliveTimeout;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTKeepAliveTimeout", str );

//     status = MQTTNeedMoreBytes;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "MQTTNeedMoreBytes", str );

//     status = MQTTNeedMoreBytes + 1;
//     str = MQTT_Status_strerror( status );
//     TEST_ASSERT_EQUAL_STRING( "Invalid MQTT Status code", str );
// }

// /* ========================================================================== */
// void test_MQTT_GetPacketId_NULL_Context( void )
// {
//     uint16_t packetId = 0U;

//     packetId = MQTT_GetPacketId( NULL );

//     TEST_ASSERT_EQUAL( 0, packetId );
// }
// /* ========================================================================== */

// void test_MQTT_GetPacketId_happy_path( void )
// {
//     uint16_t packetId = 0U;
//     MQTTContext_t mqttContext = { 0 };

//     mqttContext.nextPacketId = 5;

//     packetId = MQTT_GetPacketId( &mqttContext );

//     TEST_ASSERT_EQUAL( 5, packetId );
//     TEST_ASSERT_EQUAL( 6, mqttContext.nextPacketId );
// }

// /* ========================================================================== */
// void test_MQTT_GetPacketId_happy_path_uint16_max( void )
// {
//     uint16_t packetId = 0U;
//     MQTTContext_t mqttContext = { 0 };

//     mqttContext.nextPacketId = UINT16_MAX;

//     packetId = MQTT_GetPacketId( &mqttContext );

//     TEST_ASSERT_EQUAL( UINT16_MAX, packetId );
//     TEST_ASSERT_EQUAL( 1, mqttContext.nextPacketId );
// }
// /* ========================================================================== */

// void test_MQTT_CancelCallback_null_context( void )
// {
//     uint16_t packetId = 0U;
//     MQTTStatus_t mqttStatus;

//     mqttStatus = MQTT_CancelCallback( NULL, packetId );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_CancelCallback_null_outgoingPublishRecords( void )
// {
//     uint16_t packetId = 0U;
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t mqttContext = { 0 };

//     mqttContext.outgoingPublishRecords = NULL;

//     mqttStatus = MQTT_CancelCallback( &mqttContext, packetId );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_CancelCallback_happy_path( void )
// {
//     uint16_t packetId = 0U;
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t mqttContext = { 0 };

//     mqttContext.outgoingPublishRecords = NULL;
//     setUPContext( &mqttContext );

//     MQTT_RemoveStateRecord_ExpectAndReturn( &mqttContext, packetId, MQTTSuccess );

//     mqttStatus = MQTT_CancelCallback( &mqttContext, packetId );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_InitStatefulQoS_fail_null_context( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTPubAckInfo_t pOutgoingPublishRecords[ 10 ] = { 0 };
//     MQTTPubAckInfo_t pIncomingPublishRecords[ 10 ] = { 0 };

//     mqttStatus = MQTT_InitStatefulQoS( NULL,
//                                        pOutgoingPublishRecords,
//                                        10,
//                                        pIncomingPublishRecords,
//                                        10 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_InitStatefulQoS_zero_outgoing_size( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTPubAckInfo_t pOutgoingPublishRecords[ 10 ] = { 0 };
//     MQTTPubAckInfo_t pIncomingPublishRecords[ 10 ] = { 0 };

//     MQTTContext_t mqttContext = { 0 };

//     mqttStatus = MQTT_InitStatefulQoS( &mqttContext,
//                                        pOutgoingPublishRecords,
//                                        0,
//                                        pIncomingPublishRecords,
//                                        10 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */
// void test_MQTT_InitStatefulQoS_zero_incoming_size( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTPubAckInfo_t pOutgoingPublishRecords[ 10 ] = { 0 };
//     MQTTPubAckInfo_t pIncomingPublishRecords[ 10 ] = { 0 };

//     MQTTContext_t mqttContext = { 0 };

//     mqttStatus = MQTT_InitStatefulQoS( &mqttContext,
//                                        pOutgoingPublishRecords,
//                                        10,
//                                        pIncomingPublishRecords,
//                                        0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_InitStatefulQoS_callback_is_null( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTPubAckInfo_t pOutgoingPublishRecords[ 10 ] = { 0 };
//     MQTTPubAckInfo_t pIncomingPublishRecords[ 10 ] = { 0 };

//     MQTTContext_t mqttContext = { 0 };

//     mqttContext.appCallback = NULL;

//     mqttStatus = MQTT_InitStatefulQoS( &mqttContext,
//                                        pOutgoingPublishRecords,
//                                        10,
//                                        pIncomingPublishRecords,
//                                        10 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }
// /* ========================================================================== */

// void test_MQTT_ProcessLoop_HandleKeepAlive( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupTransportInterface( &transport );
//     transport.recv = transportRecvNoData;
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer , &ackPropsBuilder);

//     /* Set this value such that the library will be forced to send keep alive. */
//     context.waitingForPingResp = true;
//     /* Update the time so that there is timeout. */
//     globalEntryTime = MQTT_PINGRESP_TIMEOUT_MS;
//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTT_ProcessLoop_HandleKeepAlive1( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus;
//     size_t pingreqSize = MQTT_PACKET_PINGREQ_SIZE;

//     setupTransportInterface( &transport );
//     transport.recv = transportRecvNoData;
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer , &ackPropsBuilder);

//     /* Verify MQTTSuccess is returned. */
//     MQTT_GetPingreqPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_GetPingreqPacketSize_ReturnThruPtr_pPacketSize( &pingreqSize );
//     MQTT_SerializePingreq_ExpectAnyArgsAndReturn( MQTTSuccess );

//     /* Set this value such that the library will be forced to send keep alive. */
//     context.waitingForPingResp = false;
//     context.keepAliveIntervalSec = 1;
//     /* Update the time so that there is timeout. */
//     globalEntryTime = MQTT_PINGRESP_TIMEOUT_MS;
//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTT_ProcessLoop_HandleKeepAlive2( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupTransportInterface( &transport );
//     transport.recv = transportRecvNoData;
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     /* Set this value such that the library will be forced to send keep alive. */
//     context.waitingForPingResp = false;
//     context.keepAliveIntervalSec = 1;

//     /* Update the time so that timeout does not occur and the ping is not sent. */
//     globalEntryTime = 0;

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MQTT_ProcessLoop_RecvFailed( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupTransportInterface( &transport );
//     transport.recv = transportRecvFailure;
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );

//     mqttStatus = MQTT_ProcessLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTRecvFailed, mqttStatus );
// }

// void test_MQTT_ReceiveLoop_KeepAliveACK( void )
// {
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MQTTStatus_t mqttStatus;

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     context.networkBuffer.size = MQTT_TEST_BUFFER_LENGTH;

//     incomingPacket.type = MQTT_PACKET_TYPE_PINGRESP;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     MQTT_DeserializePing_ExpectAnyArgsAndReturn( MQTTSuccess );

//     mqttStatus = MQTT_ReceiveLoop( &context );

//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// /**
//  * @brief This test checks that the ProcessLoop API function is able to
//  * support receiving an entire incoming MQTT packet over the network when
//  * the transport recv function only reads less than requested bytes at a
//  * time, and the timeout passed to the API is "0ms".
//  */
// void test_MQTT_ProcessLoop_Zero_Duration_And_Partial_Network_Read( void )
// {
//     MQTTStatus_t mqttStatus;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     ProcessLoopReturns_t expectParams = { 0 };

//     setupNetworkBuffer( &networkBuffer );

//     transport.send = transportSendSuccess;

//     /* Set the transport recv function for the test to the mock function that represents
//      * partial read of data from network (i.e. less than requested number of bytes)
//      * at a time. */
//     transport.recv = transportRecvOneByte;

//     /* Initialize the context. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     /* Set flag required for configuring behavior of expectProcessLoopCalls()
//      * helper function. */
//     modifyIncomingPacketStatus = MQTTNoDataAvailable;

//     /* Test the ProcessLoop() call with zero duration timeout to verify that it
//      * will be able to support reading the packet over network over multiple calls to
//      * the transport receive function. */
//     expectParams.timeoutMs = MQTT_NO_TIMEOUT_MS;

//     /* Test with an incoming PUBLISH packet whose payload is read only one byte
//      * per call to the transport recv function. */
//     currentPacketType = MQTT_PACKET_TYPE_PUBLISH;
//     /* Set expected return values during the loop. */
//     resetProcessLoopParams( &expectParams );
//     expectParams.stateAfterDeserialize = MQTTPubAckSend;
//     expectParams.stateAfterSerialize = MQTTPublishDone;
//     expectParams.incomingPublish = true;
//     expectParams.processLoopStatus = MQTTNeedMoreBytes;
//     expectParams.updateStateStatus = MQTTNeedMoreBytes;
//     expectProcessLoopCalls( &context, &expectParams );
// }

// /**
//  * @brief This test case covers all calls to the private method,
//  * handleKeepAlive(...),
//  * that result in the process loop returning an error.
//  */
// void test_MQTT_ProcessLoop_handleKeepAlive_Error_Paths1( void )
// {
//     MQTTStatus_t mqttStatus = { 0 };
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     ProcessLoopReturns_t expectParams = { 0 };

//     setupNetworkBuffer( &networkBuffer );
//     setupTransportInterface( &transport );
//     transport.writev = transportWritevSuccess;
//     transport.recv = transportRecvNoData;

//     globalEntryTime = MQTT_PINGRESP_TIMEOUT_MS + 1;

//     /* Coverage for the branch path where PINGRESP timeout interval has expired. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     context.keepAliveIntervalSec = MQTT_SAMPLE_KEEPALIVE_INTERVAL_S;
//     context.lastPacketTxTime = 0;
//     context.lastPacketRxTime = 0;
//     context.pingReqSendTimeMs = 0;
//     context.waitingForPingResp = true;

//     /* Set expected return values in the loop. */
//     resetProcessLoopParams( &expectParams );
//     expectParams.processLoopStatus = MQTTKeepAliveTimeout;

//     mqttStatus = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTKeepAliveTimeout, mqttStatus );
// }

// /**
//  * @brief This test case covers all calls to the private method,
//  * handleKeepAlive(...),
//  * that result in the process loop returning an error.
//  */
// void test_MQTT_ProcessLoop_handleKeepAlive_Error_Paths2( void )
// {
//     MQTTStatus_t mqttStatus = { 0 };
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     ProcessLoopReturns_t expectParams = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     modifyIncomingPacketStatus = MQTTNoDataAvailable;
//     globalEntryTime = MQTT_PINGRESP_TIMEOUT_MS + 1;

//     /* Coverage for the branch path where PINGRESP timeout interval has expired. */
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     context.keepAliveIntervalSec = PACKET_TX_TIMEOUT_MS + 1;
//     context.lastPacketTxTime = 0;
//     context.lastPacketRxTime = 0;
//     context.pingReqSendTimeMs = 0;
//     context.waitingForPingResp = false;
//     /* Set expected return values in the loop. */
//     resetProcessLoopParams( &expectParams );

//     expectParams.processLoopStatus = MQTTKeepAliveTimeout;

//     MQTTPacketInfo_t incomingPacket = { 0 };
//     /* Modify incoming packet depending on type to be tested. */
//     currentPacketType = MQTT_PACKET_TYPE_PINGRESP;
//     incomingPacket.type = currentPacketType;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;

//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTNoDataAvailable );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );

//     mqttStatus = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
// }

// void test_MqttPropertyBuilder_Init( void )
// {
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500];
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MQTTStatus_t mqttStatus ; 
//     mqttStatus = MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

//     mqttStatus = MqttPropertyBuilder_Init(NULL, ackPropsBuf, ackPropsBufLength);
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }


// void test_MQTTV5_InitConnect()
// {
//     MQTTStatus_t status;
//     MQTTConnectProperties_t properties;
//     status = MQTTV5_InitConnect( NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
//     status = MQTTV5_InitConnect( &properties );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }

// /* ========================================================================== */

// /**
//  * @brief Test that MQTT_Init is able to update the context object correctly.
//  */
// void test_MQTT_Init_Happy_Path( void )
// {
//     MQTTStatus_t mqttStatus = { 0 };
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
//     TEST_ASSERT_EQUAL( MQTTNotConnected, context.connectStatus );
//     TEST_ASSERT_EQUAL( MQTT_FIRST_VALID_PACKET_ID, context.nextPacketId );
//     TEST_ASSERT_EQUAL_PTR( getTime, context.getTime );
//     TEST_ASSERT_EQUAL_PTR( eventCallback, context.appCallback );
//     /* These Unity assertions take pointers and compare their contents. */
//     TEST_ASSERT_EQUAL_MEMORY( &transport, &context.transportInterface, sizeof( transport ) );
//     TEST_ASSERT_EQUAL_MEMORY( &networkBuffer, &context.networkBuffer, sizeof( networkBuffer ) );
// }

// /**
//  * @brief Test that any NULL parameter causes MQTT_Init to return MQTTBadParameter.
//  */
// void test_MQTT_Init_Invalid_Params( void )
// {
//     MQTTStatus_t mqttStatus = { 0 };
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };

//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );

//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /* Check that MQTTBadParameter is returned if any NULL parameters are passed. */
//     mqttStatus = MQTT_Init( NULL, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     mqttStatus = MQTT_Init( &context, NULL, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, NULL, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     /* Test if NULL is passed for any of the function pointers. */
//     mqttStatus = MQTT_Init( &context, &transport, NULL, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     mqttStatus = MQTT_Init( &context, &transport, getTime, NULL, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     transport.recv = NULL;
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

//     transport.recv = transportRecvSuccess;
//     transport.send = NULL;
//     mqttStatus = MQTT_Init( &context, &transport, getTime, eventCallback, &networkBuffer, &ackPropsBuilder );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
// }



// /* ========================================================================== */
// MQTTStatus_t decode_utf8_Stub(void)
// {
//     return MQTTSuccess; 
// }

// void test_MQTT_ProcessLoop_handleIncomingPub_Error_Paths1( void )
// {
//     MQTTStatus_t status;
//     MQTTPublishState_t stateAfterDeserialize;
//     MQTTPublishState_t stateAfterSerialize;
//     MQTTContext_t context = { 0 };
//     TransportInterface_t transport = { 0 };
//     MQTTFixedBuffer_t networkBuffer = { 0 };
//     MQTTConnectProperties_t properties;
//     MQTTPacketInfo_t incomingPacket = { 0 };
//     MqttPropBuilder_t ackPropsBuilder;
//     uint8_t ackPropsBuf[500]; 
//     size_t ackPropsBufLength = sizeof(ackPropsBuf);
//     MqttPropertyBuilder_Init(&(ackPropsBuilder), ackPropsBuf, ackPropsBufLength);

//     /*Update state returns bad response.*/
//     setupTransportInterface( &transport );
//     setupNetworkBuffer( &networkBuffer );
//     incomingPacket.type = MQTT_PACKET_TYPE_PUBLISH ;
//     incomingPacket.remainingLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     incomingPacket.headerLength = MQTT_SAMPLE_REMAINING_LENGTH;
//     status = MQTT_Init( &context, &transport, getTime, eventCallback2, &networkBuffer, &ackPropsBuilder);
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     modifyIncomingPacketStatus = MQTTSuccess;
//     stateAfterDeserialize = MQTTPubRelSend;
//     stateAfterSerialize = MQTTPubCompPending;
//     MQTT_ProcessIncomingPacketTypeAndLength_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_ProcessIncomingPacketTypeAndLength_ReturnThruPtr_pIncomingPacket( &incomingPacket );
//     MQTT_DeserializePublish_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStatePublish_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStatePublish_ReturnThruPtr_pNewState( &stateAfterDeserialize );
//     MQTTV5_GetAckPacketSize_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTTV5_SerializeAckFixed_Stub( MQTTV5_SerializeAckFixed_cb );
//     MQTT_UpdateStateAck_ExpectAnyArgsAndReturn( MQTTSuccess );
//     MQTT_UpdateStateAck_ReturnThruPtr_pNewState( &stateAfterSerialize );
//     status = MQTT_ProcessLoop( &context );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
// }


// void test_validatePublish(void)
// {
//     MQTTStatus_t status= MQTTSuccess ; 
//     status = MQTT_Publish(NULL , NULL , 2, NULL); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter , status) ; 

//     MQTTPublishInfo_t publishInfo = { 0 };
//     MQTTContext_t context  = {0} ; 
//     publishInfo.qos = 2 ; 
//     status = MQTT_Publish(&context , &publishInfo , 0, NULL); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status) ;

//     publishInfo.qos = 0 ; 
//     publishInfo.payloadLength = 10 ; 
//     publishInfo.pPayload = NULL ; 
//     status = MQTT_Publish(&context , &publishInfo , 10, NULL) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter , status) ; 

//     publishInfo.pPayload = "abc" ; 
//     publishInfo.payloadLength = 3 ; 
//     publishInfo.qos = 2 ; 
//     status = MQTT_Publish(&context, &publishInfo, 10, NULL) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status); 

// }
