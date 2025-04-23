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
//  * @file core_mqtt_serializer_utest.c
//  * @brief Unit tests for functions in core_mqtt_serializer.h.
//  */
// #include <string.h>
// #include <stdint.h>
// #include <assert.h>
// #include "unity.h"

// /* Include paths for public enums, structures, and macros. */
// #include "core_mqtt_serializer.h"

// /* Set network context to double pointer to buffer (uint8_t**). */
// struct NetworkContext
// {
//     uint8_t ** buffer;
// };

// #define MQTT_MAX_REMAINING_LENGTH                   ( 268435455UL )
// #define MQTT_PACKET_CONNACK_REMAINING_LENGTH        ( ( uint8_t ) 2U )    /**< @brief A CONNACK packet always has a "Remaining length" of 2. */
// #define MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK    ( ( uint8_t ) 0x01U ) /**< @brief The "Session Present" bit is always the lowest bit. */
// #define MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH     ( ( uint8_t ) 2 )     /**< @brief PUBACK, PUBREC, PUBREl, PUBCOMP, UNSUBACK Remaining length. */
// #define MQTT_PACKET_PINGRESP_REMAINING_LENGTH       ( 0U )                /**< @brief A PINGRESP packet always has a "Remaining length" of 0. */
// #define MQTT_PACKET_PUBACK_REMAINING_LENGTH         ( 2U )
// #define MQTT_PACKET_UNSUBACK_REMAINING_LENGTH       ( 2U )

// /*
//  * MQTT client identifier.
//  */
// #define MQTT_CLIENT_IDENTIFIER                      "testclient"

// /*
//  * Client identifier and length to use for the MQTT API tests.
//  */
// #define CLIENT_IDENTIFIER                           ( "test" )                                           /**< @brief Client identifier. */
// #define CLIENT_IDENTIFIER_LENGTH                    ( ( uint16_t ) ( sizeof( CLIENT_IDENTIFIER ) - 1 ) ) /**< @brief Length of client identifier. */

// /*
//  * Topic name and length to use for the MQTT API tests.
//  */
// #define TEST_TOPIC_NAME                             ( "/test/topic" )                                  /**< @brief An arbitrary topic name. */
// #define TEST_TOPIC_NAME_LENGTH                      ( ( uint16_t ) ( sizeof( TEST_TOPIC_NAME ) - 1 ) ) /**< @brief Length of topic name. */

// /**
//  * @brief MQTT protocol version 3.1.1.
//  */
// #define MQTT_VERSION_3_1_1                          ( ( uint8_t ) 4U )

// /**
//  * @brief Test-defined macro for MQTT username.
//  */
// #define MQTT_TEST_USERNAME                          "username"
// #define MQTT_TEST_USERNAME_LEN                      ( sizeof( MQTT_TEST_USERNAME ) - 1 )

// /**
//  * @brief Test-defined macro for MQTT password.
//  */
// #define MQTT_TEST_PASSWORD                          "password"
// #define MQTT_TEST_PASSWORD_LEN                      ( sizeof( MQTT_TEST_PASSWORD ) - 1 )

// /**
//  * @brief Length of the client identifier.
//  */
// #define MQTT_CLIENT_IDENTIFIER_LEN                  ( sizeof( MQTT_CLIENT_IDENTIFIER ) - 1 )

// /**
//  * @brief Sample payload.
//  */
// #define MQTT_SAMPLE_PAYLOAD                         "Hello World"
// #define MQTT_SAMPLE_PAYLOAD_LEN                     ( sizeof( MQTT_SAMPLE_PAYLOAD ) - 1 )

// #define   TEST_TOPIC_ALIAS                          ( 2U )
// #define   TEST_MSG_EXPIRY                           ( 100U )


// #define MQTT_TEST_UTF8_STRING                       ( "test" )
// #define MQTT_TEST_UTF8_STRING_LENGTH                ( sizeof( MQTT_TEST_UTF8_STRING ) - 1 )
// #define MQTT_TEST_UINT8                             ( 1U )
// #define MQTT_TEST_UINT16                            ( 32U )
// #define MQTT_TEST_UINT32                            ( 300U )

// /* MQTT CONNECT flags. */
// #define MQTT_CONNECT_FLAG_CLEAN                     ( 1 ) /**< @brief Clean session. */
// #define MQTT_CONNECT_FLAG_WILL                      ( 2 ) /**< @brief Will present. */
// #define MQTT_CONNECT_FLAG_WILL_QOS1                 ( 3 ) /**< @brief Will QoS 1. */
// #define MQTT_CONNECT_FLAG_WILL_QOS2                 ( 4 ) /**< @brief Will QoS 2. */
// #define MQTT_CONNECT_FLAG_WILL_RETAIN               ( 5 ) /**< @brief Will retain. */
// #define MQTT_CONNECT_FLAG_PASSWORD                  ( 6 ) /**< @brief Password present. */
// #define MQTT_CONNECT_FLAG_USERNAME                  ( 7 ) /**< @brief User name present. */

// /*Default connect properties. */
// #define DEFAULT_RECEIVE_MAX                         ( 65535U )
// #define DEFAULT_REQUEST_PROBLEM                     ( 1 )

// /**
//  * @brief The Remaining Length field of MQTT disconnect packets, per MQTT spec.
//  */
// #define MQTT_DISCONNECT_REMAINING_LENGTH            ( ( uint8_t ) 0 )

// /**
//  * @brief Set a bit in an 8-bit unsigned integer.
//  */
// #define UINT8_SET_BIT( x, position )      ( ( x ) = ( uint8_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )

// /**
//  * @brief Macro for checking if a bit is set in a 1-byte unsigned int.
//  *
//  * @param[in] x The unsigned int to check.
//  * @param[in] position Which bit to check.
//  */
// #define UINT8_CHECK_BIT( x, position )    ( ( ( x ) & ( 0x01U << ( position ) ) ) == ( 0x01U << ( position ) ) )

// /**
//  * @brief Get the high byte of a 16-bit unsigned integer.
//  */
// #define UINT16_HIGH_BYTE( x )             ( ( uint8_t ) ( ( x ) >> 8 ) )

// /**
//  * @brief Get the low byte of a 16-bit unsigned integer.
//  */
// #define UINT16_LOW_BYTE( x )              ( ( uint8_t ) ( ( x ) & 0x00ffU ) )

// /**
//  * @brief Maximum number of bytes in the Remaining Length field is four according
//  * to MQTT 3.1.1 spec.
//  */
// #define MQTT_REMAINING_BUFFER_MAX_LENGTH    ( 4 )

// /**
//  * @brief Length of buffer padding to use in under/overflow checks.
//  */
// #define BUFFER_PADDING_LENGTH               ( 4 )

// /**
//  * @brief Byte to use for buffer padding in under/overflow checks.
//  */
// #define BUFFER_PADDING_BYTE                 ( 0xA5 )

// /**
//  * @brief Length of the MQTT network buffer.
//  */
// #define MQTT_TEST_BUFFER_LENGTH             ( 1024 )

// #define UINT16_DECODE( ptr )                            \
//     ( uint16_t ) ( ( ( ( uint16_t ) ptr[ 0 ] ) << 8 ) | \
//                    ( ( uint16_t ) ptr[ 1 ] ) )

// #define UINT32_DECODE( ptr )                             \
//     ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) | \
//                    ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
//                    ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) |  \
//                    ( ( uint32_t ) ptr[ 3 ] ) )

// #define UINT32_BYTE3( x )    ( ( uint8_t ) ( ( x ) >> 24 ) )

// #define UINT32_BYTE2( x )    ( ( uint8_t ) ( ( x ) >> 16 ) )

// #define UINT32_BYTE1( x )    ( ( uint8_t ) ( ( x ) >> 8 ) )

// #define UINT32_BYTE0( x )    ( ( uint8_t ) ( ( x ) & 0x000000FFU ) )


// #define MQTT_MAX_PACKET_SIZE                   ( 268435460UL )
// #define MQTT_VERSION_5                         ( 5U )
// #define MQTT_SESSION_EXPIRY_SIZE               ( 5U )
// #define MQTT_RECEIVE_MAX_SIZE                  ( 3U )
// #define MQTT_MAX_PACKET_PROPERTY_SIZE          ( 5U )
// #define MQTT_TOPIC_ALIAS_SIZE                  ( 3U )
// #define MQTT_REQUEST_RESPONSE_SIZE             ( 2U )
// #define MQTT_REQUEST_PROBLEM_SIZE              ( 2U )

// #define MQTT_SESSION_EXPIRY_ID                 ( 0x11 )
// #define MQTT_RECEIVE_MAX_ID                    ( 0x21 )
// #define MQTT_MAX_PACKET_SIZE_ID                ( 0x27 )
// #define MQTT_TOPIC_ALIAS_MAX_ID                ( 0x22 )
// #define MQTT_REQUEST_RESPONSE_ID               ( 0x19 )
// #define MQTT_REQUEST_PROBLEM_ID                ( 0x17 )
// #define MQTT_USER_PROPERTY_ID                  ( 0x26 )
// #define MQTT_AUTH_METHOD_ID                    ( 0x15 )
// #define MQTT_AUTH_DATA_ID                      ( 0x16 )

// #define MQTT_WILL_DELAY_ID                     ( 0x18 )
// #define MQTT_PAYLOAD_FORMAT_ID                 ( 0x01 )
// #define MQTT_MSG_EXPIRY_ID                     ( 0x02 )
// #define MQTT_CONTENT_TYPE_ID                   ( 0x03 )
// #define MQTT_RESPONSE_TOPIC_ID                 ( 0x08 )
// #define MQTT_CORRELATION_DATA_ID               ( 0x09 )

// #define MQTT_MAX_QOS_ID                        ( 0x24 )
// #define MQTT_RETAIN_AVAILABLE_ID               ( 0x25 )
// #define MQTT_ASSIGNED_CLIENT_ID                ( 0x12 )
// #define MQTT_REASON_STRING_ID                  ( 0x1F )
// #define MQTT_WILDCARD_ID                       ( 0x28 )
// #define MQTT_SUB_AVAILABLE_ID                  ( 0x29 )
// #define MQTT_SHARED_SUB_ID                     ( 0x2A )
// #define MQTT_SERVER_KEEP_ALIVE_ID              ( 0x13 )
// #define MQTT_RESPONSE_INFO_ID                  ( 0x1A )
// #define MQTT_SERVER_REF_ID                     ( 0x1C )

// #define MQTT_REASON_SUCCESS                    ( 0x00 )
// #define MQTT_REASON_SEND_WILL                  ( 0x04 )
// #define MQTT_REASON_NO_MATCHING_SUBSCRIBERS    ( 0x10 )
// #define MQTT_REASON_UNSPECIFIED_ERR            ( 0x80 )
// #define MQTT_REASON_MALFORMED_PACKET           ( 0x81 )
// #define MQTT_REASON_PROTOCOL_ERR               ( 0x82 )
// #define MQTT_REASON_IMPL_SPECIFIC_ERR          ( 0x83 )
// #define MQTT_REASON_UNSUPPORTED_PROTO_VER      ( 0x84 )
// #define MQTT_REASON_CLIENT_ID_NOT_VALID        ( 0x85 )
// #define MQTT_REASON_BAD_USER_OR_PASS           ( 0x86 )
// #define MQTT_REASON_NOT_AUTHORIZED             ( 0x87 )
// #define MQTT_REASON_SERVER_UNAVAILABLE         ( 0x88 )
// #define MQTT_REASON_SERVER_BUSY                ( 0x89 )
// #define MQTT_REASON_BANNED                     ( 0x8A )
// #define MQTT_REASON_SERVER_SHUTTING_DOWN       ( 0x8B )
// #define MQTT_REASON_BAD_AUTH_METHOD            ( 0x8C )
// #define MQTT_REASON_KEEP_ALIVE_TIMEOUT         ( 0x8D )
// #define MQTT_REASON_SESSION_TAKEN_OVER         ( 0x8E )
// #define MQTT_REASON_TOPIC_FILTER_INVALID       ( 0x8F )
// #define MQTT_REASON_TOPIC_NAME_INVALID         ( 0x90 )
// #define MQTT_REASON_PACKET_ID_IN_USE           ( 0x91 )
// #define MQTT_REASON_PACKET_ID_NOT_FOUND        ( 0x92 )
// #define MQTT_REASON_RX_MAX_EXCEEDED            ( 0x93 )
// #define MQTT_REASON_TOPIC_ALIAS_INVALID        ( 0x94 )
// #define MQTT_REASON_PACKET_TOO_LARGE           ( 0x95 )
// #define MQTT_REASON_MSG_RATE_TOO_HIGH          ( 0x96 )
// #define MQTT_REASON_QUOTA_EXCEEDED             ( 0x97 )
// #define MQTT_REASON_ADMIN_ACTION               ( 0x98 )
// #define MQTT_REASON_PAYLOAD_FORMAT_INVALID     ( 0x99 )
// #define MQTT_REASON_RETAIN_NOT_SUPPORTED       ( 0x9A )
// #define MQTT_REASON_QOS_NOT_SUPPORTED          ( 0x9B )
// #define MQTT_REASON_USE_ANOTHER_SERVER         ( 0x9C )
// #define MQTT_REASON_SERVER_MOVED               ( 0x9D )
// #define MQTT_REASON_SS_NOT_SUPPORTED           ( 0x9E )
// #define MQTT_REASON_CON_RATE_EXCEED            ( 0x9F )
// #define MQTT_REASON_MAX_CON_TIME               ( 0xA0 )
// #define MQTT_REASON_SUB_ID_NOT_SUP             ( 0xA1 )
// #define MQTT_REASON_WILDCARD_SUB_NOT_SUP       ( 0xA2 )

// #define CORE_MQTT_ID_SIZE                      ( 1U )
// #define MQTT_REMAINING_LENGTH_INVALID          ( ( size_t ) 268435456 )
// #define MQTT_SUBSCRIPTION_ID_ID                     ( 0x0B )
// #define MQTT_TOPIC_ALIAS_ID         ( 0x23 )

// static uint8_t remainingLengthBuffer[ MQTT_REMAINING_BUFFER_MAX_LENGTH ] = { 0 };

// static uint8_t encodedStringBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

// static uint8_t mqttBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };


// /* Variables common to testcases */
// MQTTConnectProperties_t properties;
// MQTTUserProperty_t userProperty;
// MQTTPublishInfo_t publishInfo;
// MQTTConnectInfo_t connectInfo;
// MQTTPacketInfo_t packetInfo;
// MQTTStatus_t status;

// /* ============================   UNITY FIXTURES ============================ */

// /* Called before each test method. */
// void setUp( void )
// {
//     memset( &properties, 0x0, sizeof( properties ) );
//     memset( &userProperty, 0x0, sizeof( userProperty ) );
//     memset( &publishInfo, 0x0, sizeof( publishInfo ) );
//     memset( &connectInfo, 0x0, sizeof( connectInfo ) );
//     memset( &packetInfo, 0x0, sizeof( packetInfo ) );
// }

// /* Called after each test method. */
// void tearDown( void )
// {
// }

// /* Called at the beginning of the whole suite. */
// void suiteSetUp()
// {
// }

// /* Called at the end of the whole suite. */
// int suiteTearDown( int numFailures )
// {
//     return numFailures;
// }

// /* ========================================================================== */

// /**
//  * @brief Initialize pNetworkBuffer using static buffer.
//  *
//  * @param[in] pNetworkBuffer Network buffer provided for the context.
//  */
// static void setupNetworkBuffer( MQTTFixedBuffer_t * const pNetworkBuffer )
// {
//     pNetworkBuffer->pBuffer = mqttBuffer;
//     pNetworkBuffer->size = 2048;
// }

// /**
//  * @brief Initialize pConnectInfo using test-defined macros.
//  *
//  * @param[in] pConnectInfo MQTT CONNECT packet parameters.
//  */
// static void setupConnectInfo( MQTTConnectInfo_t * const pConnectInfo )
// {
//     pConnectInfo->cleanSession = true;
//     pConnectInfo->pClientIdentifier = MQTT_CLIENT_IDENTIFIER;
//     pConnectInfo->clientIdentifierLength = MQTT_CLIENT_IDENTIFIER_LEN;
//     pConnectInfo->keepAliveSeconds = 0;
//     pConnectInfo->pUserName = MQTT_TEST_USERNAME;
//     pConnectInfo->userNameLength = MQTT_TEST_USERNAME_LEN;
//     pConnectInfo->pPassword = MQTT_TEST_PASSWORD;
//     pConnectInfo->passwordLength = MQTT_TEST_PASSWORD_LEN;
// }

// static size_t remainingLengthEncodedSize( size_t length )
// {
//     size_t encodedSize;

//     /* Determine how many bytes are needed to encode length.
//      * The values below are taken from the MQTT 3.1.1 spec. */

//     /* 1 byte is needed to encode lengths between 0 and 127. */
//     if( length < 128U )
//     {
//         encodedSize = 1U;
//     }
//     /* 2 bytes are needed to encode lengths between 128 and 16,383. */
//     else if( length < 16384U )
//     {
//         encodedSize = 2U;
//     }
//     /* 3 bytes are needed to encode lengths between 16,384 and 2,097,151. */
//     else if( length < 2097152U )
//     {
//         encodedSize = 3U;
//     }
//     /* 4 bytes are needed to encode lengths between 2,097,152 and 268,435,455. */
//     else
//     {
//         encodedSize = 4U;
//     }

//     LogDebug( ( "Encoded size for length %lu is %lu bytes.",
//                 ( unsigned long ) length,
//                 ( unsigned long ) encodedSize ) );

//     return encodedSize;
// }

// /**
//  * @brief Initialize pPublishInfo using test-defined macros.
//  *
//  * @param[in] pPublishInfo Publish information.
//  */
// static void setupPublishInfo( MQTTPublishInfo_t * pPublishInfo )
// {
//     pPublishInfo->pTopicName = TEST_TOPIC_NAME;
//     pPublishInfo->topicNameLength = TEST_TOPIC_NAME_LENGTH;
//     pPublishInfo->pPayload = MQTT_SAMPLE_PAYLOAD;
//     pPublishInfo->payloadLength = MQTT_SAMPLE_PAYLOAD_LEN;
//     pPublishInfo->qos = MQTTQoS0;
//     pPublishInfo->dup = false;
//     pPublishInfo->retain = false;
// }

// /**
//  * @brief Initialize pPublishInfo using test-defined macros.
//  *
//  * @param[in] pPublishInfo Publish information.
//  */
// static void setupPublishProperties( MQTTPublishInfo_t * pPublishInfo )
// {
//     pPublishInfo->payloadFormat = 1;
//     pPublishInfo->topicAlias = TEST_TOPIC_ALIAS;

// }

// /**
//  * @brief Encode remaining length into pDestination for packet serialization
//  * using MQTT v3.1.1 spec.
//  *
//  * @param[in] pDestination Buffer to write encoded remaining length.
//  * @param[in] length Actual remaining length.
//  */
// static size_t encodeRemainingLength( uint8_t * pDestination,
//                                      size_t length )
// {
//     uint8_t lengthByte;
//     uint8_t * pLengthEnd = NULL;
//     size_t remainingLength = length;

//     TEST_ASSERT_NOT_NULL( pDestination );

//     pLengthEnd = pDestination;

//     /* This algorithm is copied from the MQTT v3.1.1 spec. */
//     do
//     {
//         lengthByte = ( uint8_t ) ( remainingLength % 128U );
//         remainingLength = remainingLength / 128U;

//         /* Set the high bit of this byte, indicating that there's more data. */
//         if( remainingLength > 0U )
//         {
//             UINT8_SET_BIT( lengthByte, 7 );
//         }

//         /* Output a single encoded byte. */
//         *pLengthEnd = lengthByte;
//         pLengthEnd++;
//     } while( remainingLength > 0U );

//     return ( size_t ) ( pLengthEnd - pDestination );
// }

// /**
//  * @brief Encode UTF-8 string and its length into pDestination for
//  * packet serialization.
//  *
//  * @param[in] pDestination Buffer to write encoded string.
//  * @param[in] source String to encode.
//  * @param[in] sourceLength Length of the string to encode.
//  */
// static size_t encodeStringSize( uint8_t * pDestination,
//                             const char * source,
//                             uint16_t sourceLength )
// {
//     uint8_t * pBuffer = NULL;

//     /* Typecast const char * typed source buffer to const uint8_t *.
//      * This is to use same type buffers in memcpy. */
//     const uint8_t * pSourceBuffer = ( const uint8_t * ) source;

//     TEST_ASSERT_NOT_NULL( pSourceBuffer );
//     TEST_ASSERT_NOT_NULL( pDestination );

//     pBuffer = pDestination;

//     /* The first byte of a UTF-8 string is the high byte of the string length. */
//     *pBuffer = UINT16_HIGH_BYTE( sourceLength );
//     pBuffer++;

//     /* The second byte of a UTF-8 string is the low byte of the string length. */
//     *pBuffer = UINT16_LOW_BYTE( sourceLength );
//     pBuffer++;

//     /* Copy the string into pBuffer. */
//     ( void ) memcpy( pBuffer, pSourceBuffer, sourceLength );

//     /* Return the pointer to the end of the encoded string. */
//     pBuffer += sourceLength;

//     return ( size_t ) ( pBuffer - pDestination );
// }


// /**
// * @brief Mock transport receive with no data available.
// */
// static int32_t mockReceiveNoData( NetworkContext_t * pNetworkContext,
//           void * pBuffer,
//           size_t bytesToRecv )
// {
// /* Suppress unused parameter warning. */
// ( void ) pNetworkContext;
// ( void ) pBuffer;
// ( void ) bytesToRecv;

// return 0;
// }

// /**
// * @brief Mock transport receive failure.
// */
// static int32_t mockReceiveFailure( NetworkContext_t * pNetworkContext,
//            void * pBuffer,
//            size_t bytesToRecv )
// {
// /* Suppress unused parameter warning. */
// ( void ) pNetworkContext;
// ( void ) pBuffer;
// ( void ) bytesToRecv;

// return -1;
// }

// /**
// * @brief Mock transport receive that succeeds once, then fails.
// */
// static int32_t mockReceiveSucceedThenFail( NetworkContext_t * pNetworkContext,
//                    void * pBuffer,
//                    size_t bytesToRecv )
// {
// int32_t retVal = 0;
// static int counter = 0;

// if( counter++ )
// {
// retVal = mockReceiveFailure( pNetworkContext, pBuffer, bytesToRecv );
// counter = 0;
// }
// else
// {
// retVal = mockReceive( pNetworkContext, pBuffer, bytesToRecv );
// }

// return retVal;
// }

// /**
//  * @brief Pad beginning and end of buffer with non-zero bytes to be used in
//  * checking for under/overflow after serialization.
//  *
//  * @param[in] pBuffer Buffer to pad.
//  * @param[in] bufferLength Total length of buffer.
//  */
// static void padAndResetBuffer( uint8_t * pBuffer,
//                                size_t bufferLength )
// {
//     int i = 0;

//     for( i = 0; i < BUFFER_PADDING_LENGTH; i++ )
//     {
//         pBuffer[ i ] = BUFFER_PADDING_BYTE;
//         pBuffer[ bufferLength - 1 - i ] = BUFFER_PADDING_BYTE;
//     }

//     /* Zero out rest of buffer. */
//     memset( &pBuffer[ BUFFER_PADDING_LENGTH ], 0x0, bufferLength - 2 * BUFFER_PADDING_LENGTH );
// }

// /**
//  * @brief Test buffer for under/overflow.
//  *
//  * @param[in] pBuffer Buffer to check.
//  * @param[in] bufferLength Total length of buffer.
//  */
// static void checkBufferOverflow( uint8_t * pBuffer,
//                                  size_t bufferLength )
// {
//     /* Check beginning of buffer. */
//     TEST_ASSERT_EACH_EQUAL_UINT8( BUFFER_PADDING_BYTE,
//                                   pBuffer,
//                                   BUFFER_PADDING_LENGTH );
//     /* Check end. */
//     TEST_ASSERT_EACH_EQUAL_UINT8( BUFFER_PADDING_BYTE,
//                                   pBuffer + bufferLength - BUFFER_PADDING_LENGTH,
//                                   BUFFER_PADDING_LENGTH );
// }

// /**
//  * @brief Mock successful transport receive by reading data from a buffer.
//  */
// int32_t mockReceive( NetworkContext_t * pNetworkContext,
//                             void * pBuffer,
//                             size_t bytesToRecv )
// {
//     uint8_t * returnBuffer = ( uint8_t * ) pBuffer;
//     uint8_t * mockNetwork;
//     size_t bytesRead = 0;

//     /* Treat network context as pointer to buffer for mocking  */
//     mockNetwork = *( pNetworkContext->buffer );

//     while( bytesRead++ < bytesToRecv )
//     {
//         /* Read single byte and advance buffer. */
//         *returnBuffer++ = *mockNetwork++;
//     }

//     /* Move stream by bytes read. */
//     *( pNetworkContext->buffer ) = mockNetwork;

//     return bytesToRecv;
// }


// static void setupProperties( MQTTConnectProperties_t * pProperties )
// {
//     pProperties->receiveMax = DEFAULT_RECEIVE_MAX;
//     pProperties->requestProblemInfo = DEFAULT_REQUEST_PROBLEM;
//     pProperties->maxPacketSize = MQTT_MAX_PACKET_SIZE;
// }

// static uint8_t * initializeDeserialize( MQTTPacketInfo_t * packetInfo,
//                                         uint8_t * pIndex )
// {
//     uint8_t * pIndexLocal = pIndex;

//     packetInfo->pRemainingData = pIndexLocal;
//     packetInfo->type = MQTT_PACKET_TYPE_CONNACK;
//     *pIndexLocal = 0x01;
//     pIndexLocal++;
//     *pIndexLocal = 0x00;
//     pIndexLocal++;
//     return pIndexLocal;
// }

// static uint8_t * serializeuint_32( uint8_t * pIndex,
//                                    uint8_t propertyId )
// {
//     uint8_t * pIndexLocal = pIndex;

//     *pIndexLocal = propertyId;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = UINT32_BYTE3( MQTT_TEST_UINT32 );
//     pIndexLocal[ 1 ] = UINT32_BYTE2( MQTT_TEST_UINT32 );
//     pIndexLocal[ 2 ] = UINT32_BYTE1( MQTT_TEST_UINT32 );
//     pIndexLocal[ 3 ] = UINT32_BYTE0( MQTT_TEST_UINT32 );
//     pIndexLocal = &pIndexLocal[ 4 ];
//     return pIndexLocal;
// }


// static uint8_t * serializeuint_16( uint8_t * pIndex,
//                                    uint8_t propertyId )
// {
//     uint8_t * pIndexLocal = pIndex;

//     *pIndexLocal = propertyId;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( MQTT_TEST_UINT16 );
//     pIndexLocal[ 1 ] = UINT16_LOW_BYTE( MQTT_TEST_UINT16 );
//     pIndexLocal = &pIndexLocal[ 2 ];
//     return pIndexLocal;
// }

// static uint8_t * serializeuint_8( uint8_t * pIndex,
//                                   uint8_t propertyId )
// {
//     uint8_t * pIndexLocal = pIndex;

//     *pIndexLocal = propertyId;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = MQTT_TEST_UINT8;
//     pIndexLocal++;
//     return pIndexLocal;
// }
// static uint8_t * serializeutf_8( uint8_t * pIndex,
//                                  uint8_t propertyId )
// {
//     uint8_t * pIndexLocal = pIndex;

//     *pIndexLocal = propertyId;
//     pIndexLocal++;
//     size_t dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
//     pIndexLocal = &pIndexLocal[ dummy ];
//     return pIndexLocal;
// }

// static uint8_t * serializeutf_8pair( uint8_t * pIndex )
// {
//     uint8_t * pIndexLocal = pIndex;

//     *pIndexLocal = MQTT_USER_PROPERTY_ID;
//     pIndexLocal++;
//     size_t dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
//     pIndexLocal = &pIndexLocal[ dummy ];
//     dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
//     pIndexLocal = &pIndexLocal[ dummy ];
//     return pIndexLocal;
// }


// void test_MQTTV5_DeserializeConnackOnlyStatus( void )
// {
//     uint8_t buffer[ 50 ];
//     uint8_t * pIndex = buffer;
//     MqttPropBuilder_t propBuffer = {0} ; 


//     status = MQTT_DeserializeConnack( NULL, NULL, NULL, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     bool sessionPresent;
//     status = MQTT_DeserializeConnack( NULL, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_DeserializeConnack( &properties, NULL, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_DeserializeConnack( &properties, &packetInfo, NULL, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     packetInfo.pRemainingData = pIndex;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNECT;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );
//     /*Reserved bit incorrect*/
//     buffer[ 0 ] = 0x11;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /*
//      * Session Present Bit is set but reason code is not equal to 0;
//      */
//     buffer[ 0 ] = 0x01;
//     buffer[ 1 ] = 0x01;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /* 5 + 1 + 2 = 8 */
//     size_t propertyLength = encodeRemainingLength( pIndex, 5 );
//     packetInfo.remainingLength = propertyLength + 7;
//     /*Not a valid reason code*/
//     buffer[ 0 ] = 0x00;
//     buffer[ 1 ] = 0x03;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );
//     /*All the valid response code*/
//     buffer[ 1 ] = 0x80;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x80;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x81;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x82;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x83;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x80;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x84;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x80;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x85;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x86;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x87;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x88;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent,&propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x89;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent,&propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x8A;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent,&propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x8C;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x88;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x90;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x95;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x97;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x99;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9A;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9A;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9B;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9C;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9D;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     buffer[ 1 ] = 0x9F;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTServerRefused, status );

//     /*Exceeds the max packet size set by the client*/
//     properties.maxPacketSize = 2;
//     buffer[ 1 ] = 0x00;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /*Validate the remaining length*/
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     packetInfo.remainingLength = 7;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 20;
//     pIndex = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndex, 20971556356235 );
//     LogDebug( ( "Encoded size for length is %lu bytes.",
//                 ( unsigned long ) propertyLength ) );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     pIndex = &buffer[ 2 ];
//     *pIndex = 0x81;
//     pIndex++;
//     *pIndex = 0x00;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &sessionPresent, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTTV5_DeserializeConnackOnlyuint_32( void )
// {
//     uint8_t buffer[ 200 ] = { 0 };
//     bool session = false;
//     MqttPropBuilder_t propBuffer = {0} ; 
//     uint8_t * pIndexLocal = initializeDeserialize( &packetInfo, buffer );
//     size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );

//     packetInfo.remainingLength = propertyLength + 12;
//     properties.maxPacketSize = 150;
//     pIndexLocal++;
//     pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
//     pIndexLocal = serializeuint_32( pIndexLocal, MQTT_MAX_PACKET_SIZE_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.sessionExpiry );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.serverMaxPacketSize );

//     /*Protocol error to include the same property twice*/
//     packetInfo.remainingLength = 13;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 10 );
//     pIndexLocal++;
//     pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
//     pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,&propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 7;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 4 );
//     pIndexLocal++;
//     *pIndexLocal = MQTT_SESSION_EXPIRY_ID;
//     pIndexLocal++;
//     pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,&propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid id*/
//     packetInfo.remainingLength = 8;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 5 );
//     pIndexLocal++;
//     pIndexLocal = serializeuint_32( pIndexLocal, 0x00 );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* Max packet size cannot have  a value 0*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 5 );
//     packetInfo.remainingLength = propertyLength + 7;
//     pIndexLocal++;
//     *pIndexLocal = MQTT_MAX_PACKET_SIZE_ID;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = UINT32_BYTE3( 0 );
//     pIndexLocal[ 1 ] = UINT32_BYTE2( 0 );
//     pIndexLocal[ 2 ] = UINT32_BYTE1( 0 );
//     pIndexLocal[ 3 ] = UINT32_BYTE0( 0 );
//     pIndexLocal = &pIndexLocal[ 4 ];
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTTV5_DeserializeConnackOnlyuint_16( void )
// {
//     uint8_t buffer[ 200 ] = { 0 };
//     uint8_t * pIndexLocal = buffer;
//     MqttPropBuilder_t propBuffer = {0} ; 

//     buffer[ 0 ] = 0x01;
//     buffer[ 1 ] = 0x00;
//     bool session = false;
//     packetInfo.pRemainingData = buffer;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     pIndexLocal = &buffer[ 2 ];
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     size_t propertyLength = encodeRemainingLength( pIndexLocal, 9 );
//     packetInfo.remainingLength = propertyLength + 11;
//     pIndexLocal++;
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_TOPIC_ALIAS_MAX_ID );
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_SERVER_KEEP_ALIVE_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverReceiveMax );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverTopicAliasMax );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverKeepAlive );

//     /*Receive Max cannot have a value 0*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 3 );
//     packetInfo.remainingLength = propertyLength + 5;
//     pIndexLocal++;
//     *pIndexLocal = MQTT_RECEIVE_MAX_ID;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( 0 );
//     pIndexLocal[ 1 ] = UINT16_LOW_BYTE( 0 );
//     pIndexLocal = &pIndexLocal[ 2 ];
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Protocol error to include the same property twice*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 6 );
//     packetInfo.remainingLength = propertyLength + 8;
//     pIndexLocal++;
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 5;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 2 );
//     pIndexLocal++;
//     pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTTV5_DeserializeConnackOnlyuint_8( void )
// {
//     uint8_t buffer[ 200 ] = { 0 };
//     uint8_t * pIndexLocal = buffer;
//     MqttPropBuilder_t propBuffer = {0} ; 

//     buffer[ 0 ] = 0x01;
//     buffer[ 1 ] = 0x00;
//     bool session = false;
//     packetInfo.pRemainingData = buffer;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     packetInfo.remainingLength = 13;
//     pIndexLocal = &buffer[ 2 ];
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );
//     pIndexLocal++;
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_RETAIN_AVAILABLE_ID );
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_WILDCARD_ID );
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_SHARED_SUB_ID );
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_SUB_AVAILABLE_ID );

//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.serverMaxQos );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.retainAvailable );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isWildcardAvaiable );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isSharedAvailable );
//     TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isSubscriptionIdAvailable );

//     /*Protocol error to have a value other than 0 or 1*/
//     packetInfo.remainingLength = 5;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 2 );
//     pIndexLocal++;
//     *pIndexLocal = MQTT_MAX_QOS_ID;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = 3;
//     pIndexLocal++;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,  &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Protocol error to include the same property twice*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 4 );
//     packetInfo.remainingLength = propertyLength + 6;
//     pIndexLocal++;
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
//     pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 4;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 1 );
//     pIndexLocal++;
//     *pIndexLocal = MQTT_MAX_QOS_ID;
//     pIndexLocal++;
//     pIndexLocal[ 0 ] = 0;
//     pIndexLocal++;
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }


// void test_MQTTV5_DeserializeConnackOnlyutf_8( void )
// {
//     uint8_t buffer[ 200 ] = { 0 };
//     uint8_t * pIndexLocal = buffer;
//     MqttPropBuilder_t propBuffer ; 

//     buffer[ 0 ] = 0x01;
//     buffer[ 1 ] = 0x00;

//     bool session = false;
//     packetInfo.pRemainingData = buffer;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     pIndexLocal = &buffer[ 2 ];
//     properties.requestResponseInfo = 1;
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     size_t propertyLength = encodeRemainingLength( pIndexLocal, 28 );
//     packetInfo.remainingLength = propertyLength + 28 + 2;
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_REASON_STRING_ID );
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_RESPONSE_INFO_ID );
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_SERVER_REF_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Protocol error to include the same property twice*/
//     packetInfo.remainingLength = 17;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 14 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,  &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 7;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 4 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,  &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 2 );
//     packetInfo.remainingLength = propertyLength + 4;
//     pIndexLocal++;
//     serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,  &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Protocol error to include response information if is is set to false by client*/
//     properties.requestResponseInfo = 0;
//     packetInfo.remainingLength = 10;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 7 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8( pIndexLocal, MQTT_RESPONSE_INFO_ID );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session,  &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }


// void test_MQTTV5_DeserializeConnackOnlyUserProperty( void )
// {
//     // change this, it is not put in the struct at all. 

//     uint8_t buffer[ 70000 ] = { 0 };
//     uint8_t * pIndexLocal = buffer;
//     buffer[ 0 ] = 0x01;
//     buffer[ 1 ] = 0x00;
//     MqttPropBuilder_t propBuffer = {0} ; 

//     bool session = false;
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     packetInfo.pRemainingData = buffer;
//     packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     packetInfo.remainingLength = 16;
//     pIndexLocal = &buffer[ 2 ];
//     size_t propertyLength = encodeRemainingLength( pIndexLocal, 13 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8pair( pIndexLocal );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 5;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 2 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8pair( pIndexLocal );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 6;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 3 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8pair( pIndexLocal );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 8 );
//     packetInfo.remainingLength = propertyLength + 10;
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8pair( pIndexLocal );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid property length*/
//     packetInfo.remainingLength = 15;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 12 );
//     pIndexLocal++;
//     pIndexLocal = serializeutf_8pair( pIndexLocal );
//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Discard user property*/
//     packetInfo.remainingLength = 65018;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 65013 );
//     pIndexLocal += 3;
//     uint32_t i = 0U;

//     for( ; i < 5001; i++ )
//     {
//         pIndexLocal = serializeutf_8pair( pIndexLocal );
//     }

//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     packetInfo.remainingLength = 65017;
//     pIndexLocal = &buffer[ 2 ];
//     propertyLength = encodeRemainingLength( pIndexLocal, 65012 );
//     pIndexLocal += 3;

//     for( ; i < 5001; i++ )
//     {
//         pIndexLocal = serializeutf_8pair( pIndexLocal );
//     }

//     status = MQTT_DeserializeConnack( &properties, &packetInfo, &session, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }




// void test_MQTTV5_GetConnectPacketSize( void )
// {
//     size_t remainingLength = 0;
//     size_t packetSize = 0;


//     /* Call MQTT_GetConnectPacketSize() with various combinations of
//      * incorrect paramters */
//     properties.receiveMax = 65535U;
//     properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
//     properties.requestProblemInfo = 1;
//     status = MQTT_GetConnectPacketSize( NULL, NULL, 0, 0 ,  &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL, 0 , 0 , NULL, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL,0 , 0 , &remainingLength, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Verify empty connect info fails. */
//     memset( &connectInfo, 0x0, sizeof( connectInfo ) );
//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL,0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Verify empty client identifier fails. */
//     connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
//     connectInfo.clientIdentifierLength = 0;
//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL, 0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     connectInfo.pClientIdentifier = NULL;
//     connectInfo.clientIdentifierLength = CLIENT_IDENTIFIER_LENGTH;
//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL, 0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL, 0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Test a will message payload length that is too large. */
//     connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
//     connectInfo.clientIdentifierLength = UINT16_MAX;
//     connectInfo.pPassword = "";
//     connectInfo.passwordLength = UINT16_MAX;
//     connectInfo.pUserName = "";
//     connectInfo.userNameLength = UINT16_MAX;
//     publishInfo.pTopicName = TEST_TOPIC_NAME;
//     publishInfo.topicNameLength = UINT16_MAX;
//     /* A valid will message payload is less than the maximum 16 bit integer. */
//     publishInfo.payloadLength = UINT16_MAX + 2;
//     status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, 0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /* Verify good case */
//     memset( &connectInfo, 0x0, sizeof( connectInfo ) );
//     connectInfo.cleanSession = true;
//     connectInfo.pClientIdentifier = "TEST";
//     connectInfo.clientIdentifierLength = 4;
//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL,0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     // TEST_ASSERT_EQUAL_INT( 0, properties.propertyLength );
//     /* Make sure remaining size returned is 17. */
//     TEST_ASSERT_EQUAL_INT( 17, remainingLength );
//     /* Make sure packet size is 19. */
//     TEST_ASSERT_EQUAL_INT( 19, packetSize );

//     status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, UINT16_MAX , UINT16_MAX , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /* With will. These parameters will cause the packet to be
//      * 4 + 2 + 8 + 2 = 16 bytes larger. */
//     publishInfo.pTopicName = "test";
//     publishInfo.topicNameLength = 4;
//     publishInfo.pPayload = "testload";
//     publishInfo.payloadLength = 8;
//     status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo,0, 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     /* Make sure remaining size returned is 32 = 16 + 16 + 2. */
//     TEST_ASSERT_EQUAL_INT( 34, remainingLength );
//     /* Make sure packet size is 34 = 18 + 16 + 2. */
//     TEST_ASSERT_EQUAL_INT( 36, packetSize );

//     /* With username and password. This will add 4 + 2 + 4 + 2 = 12 bytes. */
//     connectInfo.cleanSession = true;
//     connectInfo.pUserName = "USER";
//     connectInfo.userNameLength = 4;
//     connectInfo.pPassword = "PASS";
//     connectInfo.passwordLength = 4;
//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL,0 , 0 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     /* Make sure remaining size returned is 28 = 16 + 12. */
//     TEST_ASSERT_EQUAL_INT( 29, remainingLength );
//     /* Make sure packet size is 30 = 18 + 12. */
//     TEST_ASSERT_EQUAL_INT( 31, packetSize );


//     status = MQTT_GetConnectPacketSize( &connectInfo, NULL, 4, 1 , &remainingLength, &packetSize );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT(33, remainingLength) ; 
//     TEST_ASSERT_EQUAL_INT(35, packetSize);


// }

// void test_RemaininglengthLimit( void )
// {
//     /* Test will property length more than the max value allowed. */
//     size_t remainingLength = 0;
//     size_t packetSize = 0;
//     uint32_t maxPacketSize = 100;
//     MQTTStatus_t status = MQTTSuccess;

//     publishInfo.topicNameLength = 0U;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, MQTT_MAX_REMAINING_LENGTH, MQTT_MAX_REMAINING_LENGTH);
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_GetDisconnectPacketSize(&remainingLength, &packetSize, maxPacketSize, MQTT_MAX_REMAINING_LENGTH, 0x00 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTTV5_ValidatePublishParams()
// {
//     uint16_t topicAliasMax = 10U;
//     uint8_t maxQos = 0U;
//     uint8_t retain = 0U;
//     uint32_t maxPacketSize = 0U;

//     /*Publish info cannot be null*/
//     status = MQTT_ValidatePublishParams( NULL, retain, maxQos, topicAliasMax,maxPacketSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /*Retain is not allowed. */
//     publishInfo.topicAlias = 2U;
//     publishInfo.retain = true;
//     status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos,topicAliasMax,maxPacketSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /*Qos invalid*/
//     publishInfo.retain = false;
//     publishInfo.qos = 1;
//     status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos,topicAliasMax,maxPacketSize);
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /*Valid parameters should return success*/
//     publishInfo.qos = 1;
//     maxQos = 1;
//     publishInfo.retain = true;
//     retain = 1;
//     publishInfo.pTopicName = "abc" ; 
//     publishInfo.topicNameLength = 3;
//     maxPacketSize = 10; 
//     status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos,topicAliasMax,maxPacketSize );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     /*Invalid topic name and topic name length*/
//     publishInfo.pTopicName = NULL ; 
//     publishInfo.topicNameLength = 2 ; 
//     status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos,topicAliasMax,maxPacketSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /*Invalid maxPacket size*/
//     publishInfo.pTopicName = "abc" ; 
//     publishInfo.topicNameLength = 3;
//     maxPacketSize = 0 ; 
//     status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos,topicAliasMax,maxPacketSize );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );


// }

// void test_MQTTV5_GetPublishPacketSize()
// {
//     size_t remainingLength = 0U;
//     size_t packetSize = 0U;
//     uint32_t maxPacketSize = 0U;

//     setupPublishInfo( &publishInfo );
//     /*Test with invalid paramters*/
//     status = MQTT_GetPublishPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, NULL, maxPacketSize , 0);
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /*Topic name invalid*/
//     publishInfo.pTopicName = NULL;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     publishInfo.pTopicName = TEST_TOPIC_NAME;

//     /*Topic alias is not allowed and topic name is not provided.*/
//     publishInfo.topicNameLength = 0;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     maxPacketSize = 100;
//     publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
//     /*Packet size too large*/
//     publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH - TEST_TOPIC_NAME_LENGTH - 4;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );

//     /* Good case succeeds. */
//     publishInfo.pPayload = "";
//     publishInfo.payloadLength = 0;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     /* Again with QoS 2. */
//     publishInfo.qos = MQTTQoS2;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     setupPublishProperties( &publishInfo );
//     publishInfo.retain = true;

//     /*Valid properties*/
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 10 );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     /* No topic name*/
//     publishInfo.topicNameLength = 0U;
//     publishInfo.pTopicName = NULL;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0);
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     /*Packet size is more than the server allowed max packet size*/
//     maxPacketSize = 4;
//     status = MQTT_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );
// }

// void test_MQTTV5_DeserializeAck_puback( void )
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     MQTTReasonCodeInfo_t ackInfo;
//     MqttPropBuilder_t propBuffer = {0} ; 
//     uint16_t packetIdentifier;
//     uint32_t maxPacketSize = 0U;
//     bool requestProblem = false;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[ 100 ] = { 0 };
//     uint8_t * pIndex = buffer;
//     size_t dummy;

//     /* Verify parameters */
//     memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
//     memset( &ackInfo, 0x00, sizeof( ackInfo ) );


//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
//     status = MQTT_DeserializePublishAck( NULL, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, NULL, &ackInfo, requestProblem, maxPacketSize, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Remaining data cannot be NULL.*/
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Max packet size cannot be 0*/
//     mqttPacketInfo.pRemainingData = buffer;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     maxPacketSize = 200U;
//     /* Packet identifier 0 is not valid (per spec). */
//     buffer[ 0 ] = 0;
//     buffer[ 1 ] = 0;
//     mqttPacketInfo.remainingLength = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


//     /*Remaining length connot be less than 2*/
//     mqttPacketInfo.remainingLength = 1;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Packet size greater than allowed.*/
//     mqttPacketInfo.remainingLength = 1000U;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* Process a valid PUBACK. */
//     mqttPacketInfo.remainingLength = 2;
//     buffer[ 1 ] = 1;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );

//     mqttPacketInfo.remainingLength = 3;
//     buffer[ 2 ] = 0x00; 
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );

//     /*Property length should be zero when request problem is set to false*/
//     mqttPacketInfo.remainingLength = 24;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     requestProblem = true;
//     /*User properties not initialized.*/
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status );

//     /*Valid parameters.*/
//     pIndex = &buffer[ 3 ];
//     dummy = encodeRemainingLength( pIndex, 20 );
//     pIndex++;
//     pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
//     pIndex = serializeutf_8pair( pIndex );
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Invalid property id*/
//     pIndex = &buffer[ 3 ];
//     dummy = encodeRemainingLength( pIndex, 7 );
//     mqttPacketInfo.remainingLength = dummy + 7 + 3;
//     pIndex++;
//     pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid remaining length*/
//     pIndex = &buffer[ 3 ];
//     dummy = encodeRemainingLength( pIndex, 12 );
//     pIndex++;
//     pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


//     /*Invalid property length*/
//     pIndex = &buffer[ 3 ];
//     dummy = encodeRemainingLength( pIndex, 20971556356235 );
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTTV5_DeserializeAck_LogPuback()
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     MQTTReasonCodeInfo_t ackInfo;
//     uint16_t packetIdentifier;
//     uint32_t maxPacketSize = 10U;
//     bool requestProblem = false;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[ 4 ] = { 0 };

//     memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
//     memset( &ackInfo, 0x00, sizeof( ackInfo ) );

//     // MQTTSuccessFailReasonCode_t rc ; 
//     // ackInfo.reasonCode = &rc ; 

//     MqttPropBuilder_t propBuffer = {0}; 
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
//     mqttPacketInfo.remainingLength = 4;
//     /*Validate all the correct reason codes.*/
//     buffer[ 1 ] = 1;
//     buffer[ 2 ] = MQTT_REASON_SUCCESS;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     buffer[ 2 ] = MQTT_REASON_NO_MATCHING_SUBSCRIBERS;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     buffer[ 2 ] = MQTT_REASON_UNSPECIFIED_ERR;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_IMPL_SPECIFIC_ERR;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_NOT_AUTHORIZED;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_TOPIC_NAME_INVALID;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize , &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_PACKET_ID_IN_USE;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize , &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_QUOTA_EXCEEDED;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     buffer[ 2 ] = MQTT_REASON_PAYLOAD_FORMAT_INVALID;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     /*Invlaid reason code.*/
//     buffer[ 2 ] = MQTT_REASON_BANNED;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize , &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTTV5_DeserializeAck_Pubrel()
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     MQTTReasonCodeInfo_t ackInfo;
//     uint32_t maxPacketSize = 10U;
//     uint16_t packetIdentifier;
//     bool requestProblem = false;
//     MQTTStatus_t status = MQTTSuccess;
//     MqttPropBuilder_t propBuffer ; 
//     uint8_t buffer[ 4 ] = { 0 };

//     memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
//     memset( &ackInfo, 0x00, sizeof( ackInfo ) );

//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBREL;
//     mqttPacketInfo.remainingLength = 4;
//     /*Validate all the correct reason codes.*/
//     buffer[ 1 ] = 1;
//     buffer[ 2 ] = MQTT_REASON_SUCCESS;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     buffer[ 2 ] = MQTT_REASON_PACKET_ID_NOT_FOUND;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

//     /*Invalid reason code.*/
//     buffer[ 2 ] = MQTT_REASON_BANNED;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid reason code.*/
//     buffer[ 2 ] = MQTT_REASON_SEND_WILL;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid packet id*/
//     buffer[ 1 ] = 0;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Invalid packet type. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_CONNACK;
//     status = MQTT_DeserializePublishAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }


// void test_MQTTV5_GetAckPacketSize()
// {
//     MQTTStatus_t status;
//     size_t remainingLength;
//     size_t packetSize;
//     uint32_t maxPacketSize = 0U;

//     /*Invalid parameters*/
//     status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetAckPacketSize( NULL, &packetSize, maxPacketSize, 0);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetAckPacketSize(&remainingLength, NULL, maxPacketSize , 0);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Max packet size cannot be 0*/
//     status = MQTT_GetAckPacketSize( &remainingLength, NULL, maxPacketSize , 0);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Valid parameters*/
//     maxPacketSize = UINT32_MAX;
//     status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize , 0);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*With properties*/
//     status = MQTT_GetAckPacketSize(&remainingLength, &packetSize, maxPacketSize, 10 );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Packet size greater than max allowed.*/
//     maxPacketSize = 2;
//     status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Max packet size cannot be 0*/
//     maxPacketSize = 0;
//     status = MQTT_GetAckPacketSize(&remainingLength, &packetSize, maxPacketSize, 0 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

// }


// void test_MQTTV5_GetDisconnectPacketSize()
// {
//     size_t remainingLength;
//     size_t packetSize;
//     uint32_t maxPacketSize = 0U;
//     uint32_t sessionExpiry = 0U;
//     uint32_t prevSessionExpiry = 0U;
//     MQTTStatus_t status;

//     /*Invalid arguments*/
//     status = MQTT_GetDisconnectPacketSize( &remainingLength, &packetSize, maxPacketSize, 0, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


//     status = MQTT_GetDisconnectPacketSize( NULL, &packetSize, maxPacketSize, 0, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     status = MQTT_GetDisconnectPacketSize(&remainingLength, NULL, maxPacketSize, 0, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Max packet size cannot be 0.*/
//     status = MQTT_GetDisconnectPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 , MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status ); 


//     status = MQTT_GetDisconnectPacketSize(&remainingLength, &packetSize, maxPacketSize, 10, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     // /*Invalid Reason code*/
//     maxPacketSize = 60U;
//     status = MQTT_GetDisconnectPacketSize(&remainingLength, &packetSize, maxPacketSize,0 , MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Valid parameters*/
//     status = MQTT_GetDisconnectPacketSize( &remainingLength, &packetSize, maxPacketSize, 0, MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Valid parameters*/
//     status = MQTT_GetDisconnectPacketSize(&remainingLength, &packetSize, maxPacketSize, 0, MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Max packet size lesser than packet size */
//     status = MQTT_GetDisconnectPacketSize(&remainingLength, &packetSize, 5, 6, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


//     status = MQTT_GetDisconnectPacketSize( &remainingLength, &packetSize, maxPacketSize, MQTT_MAX_REMAINING_LENGTH, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }



// void test_MQTTV5_DeserializeDisconnect()
// {
//     MQTTReasonCodeInfo_t disconnectInfo;
//     const char * pServerRef;
//     uint16_t serverRefLength;
//     size_t dummy;
//     int32_t maxPacketSize = 0U;
//     uint8_t buffer[ 100 ] = { 0 };
//     uint8_t * pIndex = buffer;
//     MqttPropBuilder_t propBuffer = {0} ; 

//     memset( &disconnectInfo, 0x0, sizeof( disconnectInfo ) );
//     /*Invalid parameters*/
//     status = MQTT_DeserializeDisconnect( NULL, maxPacketSize, &disconnectInfo, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Remaining data not initialized.*/
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     packetInfo.pRemainingData = buffer;
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, NULL , &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /*Max packet size cannot be 0.*/
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize , &disconnectInfo, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
//     maxPacketSize = 100;

//     /*Remaining length cannot be 0*/
//     status = MQTT_DeserializeDisconnect( &packetInfo,maxPacketSize, &disconnectInfo,&propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /*Remaining Length invalid. */
//     packetInfo.remainingLength = 200;
//     status = MQTT_DeserializeDisconnect( &packetInfo,maxPacketSize, &disconnectInfo, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


//     /*Invalid reason code.*/
//     buffer[ 0 ] = MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE;
//     status = MQTT_DeserializeDisconnect( &packetInfo,maxPacketSize, &disconnectInfo, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     packetInfo.remainingLength = 1;
//     buffer[ 0 ] = MQTT_REASON_DISCONNECT_NOT_AUTHORIZED;
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer  );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Property length is 0.*/
//     packetInfo.remainingLength = 2;
//     pIndex = &buffer[ 1 ];
//     dummy = encodeRemainingLength( pIndex, 0 );
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*With properties*/
//     pIndex = &buffer[ 1 ];
//     packetInfo.remainingLength = 29;
//     dummy = encodeRemainingLength( pIndex, 27 );
//     pIndex++;
//     pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
//     pIndex = serializeutf_8pair( pIndex );
//     pIndex = serializeutf_8( pIndex, MQTT_SERVER_REF_ID );
//     status = MQTT_DeserializeDisconnect( &packetInfo,maxPacketSize,&disconnectInfo, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     /*Invalid property id.*/
//     pIndex = &buffer[ 1 ];
//     packetInfo.remainingLength = 9;
//     dummy = encodeRemainingLength( pIndex, 7 );
//     pIndex++;
//     pIndex = serializeutf_8( pIndex, MQTT_SESSION_EXPIRY_ID );
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize,&disconnectInfo, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
//     TEST_ASSERT_EQUAL_INT( 1, dummy );

//     /*Invalid property length.*/
//     pIndex = &buffer[ 1 ];
//     packetInfo.remainingLength = 9;
//     dummy = encodeRemainingLength( pIndex, 4 );
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     buffer[ 1 ] = 0x81;
//     buffer[ 2 ] = 0x00;
//     status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }

// void test_MQTT_GetIncomingPacketTypeAndLength( void )
// {
//     MQTTPacketInfo_t mqttPacket;
//     NetworkContext_t networkContext;
//     uint8_t buffer[ 10 ];
//     uint8_t * bufPtr = buffer;

//     /* Dummy network context - pointer to pointer to a buffer. */
//     networkContext.buffer = &bufPtr;
//     /* Check when network receive fails. */
//     memset( buffer, 0x00, 10 );
//     /* Branch coverage for Disconnect. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_DISCONNECT;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
// }

// void test_MQTTV5_GetSubscribePacketSize( void )
// {
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTSubscribeInfo_t subscribeInfo;
//     size_t remainingLength = 0;
//     size_t packetSize = 0;

//     /** Verify Parameters */

//     /** NULL parameters */
//     status = MQTT_GetSubscribePacketSize(NULL, 1, &remainingLength, &packetSize, 0, MQTT_MAX_PACKET_SIZE); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

//     status = MQTT_GetSubscribePacketSize(&subscribeInfo , 0, &remainingLength, &packetSize, 0, MQTT_MAX_PACKET_SIZE); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

//     /** Empty Empty Subscription List Fails */

//     // memset( &subscribeInfo, 0x0, sizeof( subscribeInfo ) );
//     // status = MQTT_GetSubscribePacketSize(&subscribeInfo , 0, &remainingLength, &packetSize, 0, MQTT_MAX_PACKET_SIZE); 
//     // TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

//     // status = MQTT_GetSubscribePacketSize(&subscribeInfo , 1, &remainingLength, &packetSize, 0, MQTT_MAX_PACKET_SIZE); 
//     // TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

//     /** NULL topic filter , non zero length */

// //     subscribeInfo.topicFilterLength = 1;
// //     status = MQTT_GetSubscribePacketSize(&subscribeInfo , 1, &remainingLength, &packetSize, 0, MQTT_MAX_PACKET_SIZE); 
// //     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

// //     /**Topic Filter Length == 0 , Topic Filter not null */

// //     subscribeInfo.topicFilterLength = 0 ; 
// //     subscribeInfo.pTopicFilter = "example/topic" ; 
// //     status = MQTT_GetSubscribePacketSize(&subscribeInfo , 1, &remainingLength, &packetSize, 0, 100); 
// //     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);
// }

// void test_MQTTV5_GetSubscribePacketSize_HappyPath(void){
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTSubscribeInfo_t subscribeInfo;
//     size_t remainingLength = 0;
//     size_t packetSize = 0;
//     subscribeInfo.pTopicFilter = TEST_TOPIC_NAME ; 
//     subscribeInfo.topicFilterLength = TEST_TOPIC_NAME_LENGTH ;
//     subscribeInfo.qos = MQTTQoS0 ;
//     subscribeInfo.noLocalOption = 0 ; 
//     subscribeInfo.retainAsPublishedOption = 0 ; 
//     subscribeInfo.retainHandlingOption = 1 ;

//     status = MQTT_GetSubscribePacketSize(&subscribeInfo, 1, &remainingLength, &packetSize, 2, 100);
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);
//     TEST_ASSERT_EQUAL_UINT32(19U, remainingLength);
//     TEST_ASSERT_EQUAL_UINT32(21U, packetSize);
// }

// /** Subscribe Packet size with multiple subscriptions and User Properties */
// void test_MQTTV5_GetSubscribePacketSize_MultipleSubscriptions(void){
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTSubscribeInfo_t subscribeInfo[2];
//     size_t remainingLength = 0;
//     size_t packetSize = 0;

//     subscribeInfo[0].pTopicFilter = TEST_TOPIC_NAME ;
//     subscribeInfo[0].topicFilterLength = TEST_TOPIC_NAME_LENGTH ;
//     subscribeInfo[0].qos = MQTTQoS0 ;
//     subscribeInfo[0].noLocalOption = 0 ;
//     subscribeInfo[0].retainAsPublishedOption = 0 ;
//     subscribeInfo[0].retainHandlingOption = 1 ;

//     subscribeInfo[1].pTopicFilter = TEST_TOPIC_NAME ;
//     subscribeInfo[1].topicFilterLength = TEST_TOPIC_NAME_LENGTH ;
//     subscribeInfo[1].qos = MQTTQoS0 ;
//     subscribeInfo[1].noLocalOption = 0 ;
//     subscribeInfo[1].retainAsPublishedOption = 0 ;
//     subscribeInfo[1].retainHandlingOption = 1 ;

//     status = MQTT_GetSubscribePacketSize(subscribeInfo, 2, &remainingLength, &packetSize, 0, 100);
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);
// }

// void test_calculateSubscriptionPacketSizeV5(void)
// {
//     size_t subscriptionCount = 1;
//     size_t remainingLength = 0;
//     size_t packetSize = 0;
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTSubscribeInfo_t fourThousandSubscriptions[ 4096 ] = { 0 };
//     int i;
//     for( i = 0; i < 4096; i++ )
//     {
//         fourThousandSubscriptions[ i ].topicFilterLength = UINT16_MAX;

//         /* We need to set this to avoid an early bad parameter, however we do
//          * not need a 65535 byte buffer as the packet will not be serialized. */
//         fourThousandSubscriptions[ i ].pTopicFilter = "";
//     }

//     subscriptionCount = sizeof( fourThousandSubscriptions ) / sizeof( fourThousandSubscriptions[ 0 ] );
//     status = MQTT_GetSubscribePacketSize( fourThousandSubscriptions,
//                                           subscriptionCount,
//                                           &remainingLength,
//                                           &packetSize, 0, MQTT_MAX_PACKET_SIZE);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
// }

// void test_MQTTV5_suback(void)
// {
//     MQTTStatus_t status;
    
//     uint8_t packetBuffer[23] = {
//         0x90,       // Fixed header: SUBACK type (0x90)
//         0x14,       // Remaining Length = 20 bytes
//         0x00, 0x01, // Packet Identifier = 1
//         0x11,       // Property Length = 17 bytes
//         0x1F,       // Property ID = 0x1F (Reason String)
//         0x00, 0x03, // UTF-8 string length = 3
//         0x61, 0x62, 0x63, // The string "abc"
//         0x26, 0x00, 0x03, 0x61, 0x62, 0x63, 0x00 , 0x03 , 0x61, 0x62, 0x63,
//         0x00        // Payload: Reason code = 0x00 (Success)
//     };

//     MQTTPacketInfo_t subackPacket;
//     memset(&subackPacket, 0, sizeof(subackPacket));
//     subackPacket.type = MQTT_PACKET_TYPE_SUBACK; // Should be defined as 0x90
//     subackPacket.remainingLength = 21;           // From the fixed header (0x0A)
//     subackPacket.headerLength = 2;               // Fixed header size in this example
//     subackPacket.pRemainingData = &packetBuffer[2];
//     uint16_t packetIdentifier = 0;
//     MQTTReasonCodeInfo_t subackReasonCodes;
//     MqttPropBuilder_t propBuffer = {0} ; 
//     status = MQTT_DeserializeSuback(&subackReasonCodes ,&subackPacket ,  &packetIdentifier, &propBuffer, MQTT_MAX_PACKET_SIZE );
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);

//     packetBuffer[11] = 0x00 ; 
//     status = MQTT_DeserializeSuback(&subackReasonCodes ,&subackPacket ,  &packetIdentifier, &propBuffer, MQTT_MAX_PACKET_SIZE);
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);
// }

// void test_MQTTV5_GetUnsubscribePacketSize_Path(void){
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTSubscribeInfo_t subscribeInfo = {0};
//     size_t remainingLength = 0;
//     size_t packetSize = 0;

//     status = MQTT_GetUnsubscribePacketSize(NULL, 1, &remainingLength, &packetSize,MQTT_MAX_PACKET_SIZE, 0);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

//     subscribeInfo.pTopicFilter = TEST_TOPIC_NAME ; 
//     subscribeInfo.topicFilterLength = TEST_TOPIC_NAME_LENGTH ;


//     status = MQTT_GetUnsubscribePacketSize(&subscribeInfo, 1, &remainingLength, &packetSize,MQTT_MAX_PACKET_SIZE, 0);
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);
//     TEST_ASSERT_EQUAL_UINT32(16U, remainingLength);
//     TEST_ASSERT_EQUAL_UINT32(18U, packetSize);

//     status = MQTT_GetUnsubscribePacketSize(&subscribeInfo, 0, &remainingLength, &packetSize,MQTT_MAX_PACKET_SIZE, 0);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);

// }
// void test_MQTTV5_DeserializeSuback( void )
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     uint16_t packetIdentifier;
//     MQTTStatus_t status = MQTTSuccess;
//     MqttPropBuilder_t propBuffer = {0} ; 
//     uint8_t buffer[ 14 ] = { 0 };

//     /* Bad remaining length. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_SUBACK;
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.remainingLength = 14;
//     /* Set packet identifier. */
//     buffer[ 0 ] = 0;
//     buffer[ 1 ] = 1;
//     buffer[2] = 0 ; 
//     buffer[3] = 0x01 ; 
//     buffer[4] = 0x02 ;
//     buffer[5] = 0x80 ;
//     buffer[6] = 0x83 ;
//     buffer[7] = 0x87 ;
//     buffer[8] = 0x8F ; 
//     buffer[9] = 0x91 ; 
//     buffer[10] = 0x97 ; 
//     buffer[11] = 0x9E ; 
//     buffer[12] = 0xA1 ; 
//     buffer[13] = 0xA2 ; 

//     MQTTReasonCodeInfo_t subackReasonCodes;

//     status = MQTT_DeserializeSuback(&subackReasonCodes ,&mqttPacketInfo ,  &packetIdentifier, &propBuffer, MQTT_MAX_PACKET_SIZE);
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     buffer[13] = 0xA4 ; 
//     status = MQTT_DeserializeSuback(&subackReasonCodes ,&mqttPacketInfo ,  &packetIdentifier, &propBuffer, MQTT_MAX_PACKET_SIZE);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

// }


// void test_incoming_publish1V5(void)
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     uint16_t packetIdentifier = 1 ;
//     MQTTStatus_t status = MQTTSuccess;
//     MqttPropBuilder_t propBuffer = {0} ; 
//     uint8_t buffer[47] = { 0 };

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.remainingLength = 47;

//     buffer[0] = 0x00 ; 
//     buffer[1] = 0x03 ; 
//     buffer[2] = 0x61 ; 
//     buffer[3] = 0x62 ; 
//     buffer[4] = 0x63 ; 
//     buffer[5] = 0x29 ; 
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
//     buffer[19] = 0x09 ; 
//     buffer[20] = 0x00 ; 
//     buffer[21] = 0x03; 
//     buffer[22] = 't' ; 
//     buffer[23] = 'e' ;
//     buffer[24] = 's' ;
//     buffer[25] = 0x23 ; 
//     buffer[26] = 0x00 ; 
//     buffer[27] = 0x01 ; 
//     buffer[28] = 0x08 ; 
//     buffer[29] = 0x00 ; 
//     buffer[30] = 0x03 ; 
//     buffer[31] = 't' ;
//     buffer[32] = 'e' ;
//     buffer[33] = 's' ; 
//     buffer[34] = 0x02 ; 
//     buffer[35] = 0x00 ; 
//     buffer[36] = 0x00 ; 
//     buffer[37] = 0x00 ; 
//     buffer[38] = 0xB ; 
//     buffer[39] = 0x03 ; 
//     buffer[40] = 0x00 ; 
//     buffer[41] = 0x03 ; 
//     buffer[42] = 't' ;
//     buffer[43] = 'e' ;
//     buffer[44] = 's' ;
//     buffer[45] = 0x0B ; 
//     buffer[46] = 0x01 ; 


//     MQTTPublishInfo_t publishIn ; 
//     (void)memset(&publishIn, 0x0, sizeof(publishIn));

//     status = MQTT_DeserializePublish(&mqttPacketInfo ,&packetIdentifier, &publishIn, &propBuffer, 100); 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);

//     buffer[8] = 100 ; 
//     status = MQTT_DeserializePublish(&mqttPacketInfo ,&packetIdentifier, &publishIn, &propBuffer, 100); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);

// }
// void test_incoming_publish_withPacketId(void)
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     uint16_t packetIdentifier = 1 ;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[8] = { 0 };
//     MqttPropBuilder_t propBuffer = {0} ; 

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x2;
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.remainingLength = 8;

//     buffer[0] = 0x00 ; 
//     buffer[1] = 0x03 ; 
//     buffer[2] = 0x61 ; 
//     buffer[3] = 0x62 ; 
//     buffer[4] = 0x63 ; 
//     buffer[5] = 0x00 ; 
//     buffer[6] = 0x01 ; 
//     buffer[7] = 0x00 ; 

//     MQTTPublishInfo_t publishIn ; 
//     (void)memset(&publishIn, 0x0, sizeof(publishIn));

//     status = MQTT_DeserializePublish(&mqttPacketInfo ,&packetIdentifier, &publishIn, &propBuffer, 100); 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, status);

//     buffer[6] = 0x00 ; 
//     status = MQTT_DeserializePublish(&mqttPacketInfo ,&packetIdentifier, &publishIn, &propBuffer, 100); 
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);


// }

// void test_Invalid_IncomingPublish(void)
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     uint16_t packetIdentifier = 1 ;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[3] = { 0 };
//     MqttPropBuilder_t propBuffer = {0} ; 

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
//     mqttPacketInfo.pRemainingData = buffer;
//     mqttPacketInfo.remainingLength = 3;

//     buffer[0] = 0x00 ;
//     buffer[1] = 0x01 ;
//     buffer[2] = 0x61 ;


//     MQTTPublishInfo_t publishIn ;
//     (void)memset(&publishIn, 0x0, sizeof(publishIn));
//     status = MQTT_DeserializePublish(&mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100);
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x02 ; 
//     status = MQTT_DeserializePublish(&mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100);
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);

// }

// /**
//  * @brief Tests that MQTT_GetPingreqPacketSize works as intended.
//  */
// void test_MQTT_GetPingreqPacketSize( void )
// {
//     MQTTStatus_t status;
//     size_t packetSize;

//     /* Verify parameters. */
//     status = MQTT_GetPingreqPacketSize( NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Good case succeeds. A PINGREQ is 2 bytes. */
//     status = MQTT_GetPingreqPacketSize( &packetSize );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( 2, packetSize );
// }

// /**
//  * @brief Tests that MQTT_SerializePingreq works as intended.
//  */
// void test_MQTT_SerializePingreq( void )
// {
//     uint8_t buffer[ 10 + 2 * BUFFER_PADDING_LENGTH ];
//     MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ] };
//     uint8_t expectedPacket[ 2 ] = { MQTT_PACKET_TYPE_PINGREQ, 0 };
//     MQTTStatus_t status = MQTTSuccess;

//     /* Buffer size less than ping request fails. */
//     fixedBuffer.size = 1;
//     status = MQTT_SerializePingreq( &fixedBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

//     /* NULL buffer fails. */
//     status = MQTT_SerializePingreq( NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Verify a NULL buffer in the fixed buffer struct fails */
//     fixedBuffer.pBuffer = NULL;
//     status = MQTT_SerializePingreq( &fixedBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Restore the fixed buffer. */
//     fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

//     /* Good case succeeds. */
//     fixedBuffer.size = 2;
//     padAndResetBuffer( buffer, sizeof( buffer ) );
//     status = MQTT_SerializePingreq( &fixedBuffer );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     checkBufferOverflow( buffer, sizeof( buffer ) );
//     TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], 2 );
// }

// /**
//  * @brief Tests that MQTT_DeserializeAck works as intended with a PINGRESP.
//  */
// void test_MQTT_DeserializeAck_pingresp( void )
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     uint16_t packetIdentifier;
//     bool sessionPresent;
//     MQTTStatus_t status = MQTTSuccess;

//     /* Bad remaining length. */
//     ( void ) memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PINGRESP;
//     mqttPacketInfo.remainingLength = MQTT_PACKET_PINGRESP_REMAINING_LENGTH + 1;
//     status = MQTT_DeserializePing( &mqttPacketInfo, &packetIdentifier, &sessionPresent );
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_SUBACK ; 
//     mqttPacketInfo.remainingLength = 5 ;
//     status = MQTT_DeserializePing(&mqttPacketInfo, &packetIdentifier, &sessionPresent);
//     TEST_ASSERT_EQUAL_INT(MQTTBadResponse, status);

//     /* Process a valid PINGRESP. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PINGRESP;
//     mqttPacketInfo.remainingLength = MQTT_PACKET_PINGRESP_REMAINING_LENGTH;
//     mqttPacketInfo.pRemainingData = NULL;
//     status = MQTT_DeserializePing( &mqttPacketInfo, NULL, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

//     status = MQTT_DeserializePing(NULL , NULL , NULL) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status);
// }

// /**
//  * @brief Tests that MQTT_GetIncomingPacketTypeAndLength works as intended.
//  */
// void test_MQTT_GetIncomingPacketTypeAndLength1( void )
// {
//     MQTTStatus_t status = MQTTSuccess;
//     MQTTPacketInfo_t mqttPacket;
//     NetworkContext_t networkContext;
//     uint8_t buffer[ 10 ];
//     uint8_t * bufPtr = buffer;

//     /* Dummy network context - pointer to pointer to a buffer. */
//     networkContext.buffer = &bufPtr;

//     /* Test a NULL pIncomingPacket parameter. */
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, NULL );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Test a typical happy path case for a CONN ACK packet. */
//     buffer[ 0 ] = 0x20; /* CONN ACK */
//     buffer[ 1 ] = 0x02; /* Remaining length. */

//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( 0x20, mqttPacket.type );
//     TEST_ASSERT_EQUAL_INT( 0x02, mqttPacket.remainingLength );

//     /* Remaining length of 128. MQTT uses 7 bits for data and 1 continuation
//      * bit in each byte. Since 128 is 8 bits, it needs 2 bytes. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     buffer[ 1 ] = 0x80; /* LSB: CB=1, value=0x00 */
//     buffer[ 2 ] = 0x01; /* MSB: CB=0, value=0x01 */
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( MQTT_PACKET_TYPE_PUBLISH, mqttPacket.type );
//     TEST_ASSERT_EQUAL_INT( 128, mqttPacket.remainingLength );

//     /* Remaining length of 16384. MQTT uses 7 bits for data and 1 continuation
//      * bit in each byte. Since 16384 is 15 bits, it needs 3 bytes. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     buffer[ 1 ] = 0x80; /* LSB   : CB=1, value=0x00 */
//     buffer[ 2 ] = 0x80; /* Byte 1: CB=1, value=0x00 */
//     buffer[ 3 ] = 0x01; /* MSB   : CB=0, value=0x01 */
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL_INT( MQTT_PACKET_TYPE_PUBLISH, mqttPacket.type );
//     TEST_ASSERT_EQUAL_INT( 16384, mqttPacket.remainingLength );

//     /* Test with incorrect packet type. */
//     bufPtr = buffer;
//     buffer[ 0 ] = 0x10; /* INVALID */
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /* Test with disconnect packet type. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_DISCONNECT;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTSuccess, status );

//     /* Test with invalid remaining length. */
//     bufPtr = buffer;
//     buffer[ 0 ] = 0x20; /* CONN ACK */

//     /* To generate invalid remaining length response,
//      * four bytes need to have MSB (or continuation bit, 0x80) set */
//     buffer[ 1 ] = 0xFF;
//     buffer[ 2 ] = 0xFF;
//     buffer[ 3 ] = 0xFF;
//     buffer[ 4 ] = 0xFF;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /* Check with an encoding that does not conform to the MQTT spec. */
//     bufPtr = buffer;
//     buffer[ 1 ] = 0x80;
//     buffer[ 2 ] = 0x80;
//     buffer[ 3 ] = 0x80;
//     buffer[ 4 ] = 0x00;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /* Check when network receive fails. */
//     memset( buffer, 0x00, 10 );
//     bufPtr = buffer;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveFailure, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTRecvFailed, status );

//     /* Test if no data is available. */
//     bufPtr = buffer;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveNoData, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTNoDataAvailable, status );

//     /* Branch coverage for PUBREL. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_PUBREL & 0xF0U;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );

//     /* Receive type then fail. */
//     bufPtr = buffer;
//     buffer[ 0 ] = MQTT_PACKET_TYPE_PUBREL;
//     status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveSucceedThenFail, &networkContext, &mqttPacket );
//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );
// }

// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_AllNULL( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set. Except publish flag. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
// }

// /* ========================================================================== */

// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_QoS1( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     publishInfo.qos = MQTTQoS1;

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set except QoS1 and publish flag. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 1 ) | 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
// }

// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_QoS2( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     publishInfo.qos = MQTTQoS2;

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set except QoS2. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 2 ) | 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
// }

// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_retain( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     publishInfo.retain = true;

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set except retain flag. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 0 ) | 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
// }

// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_Duplicate( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     publishInfo.dup = true;

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set except dup. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 3 ) | 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
// }
// /**
//  * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
//  */
// void test_MQTT_SerializePublishHeaderWithoutTopic_VariousFlagsSetTopicLength( void )
// {
//     MQTTPublishInfo_t publishInfo;
//     size_t remainingLength = 0;
//     uint8_t buffer[ 7 ];
//     MQTTStatus_t status = MQTTSuccess;
//     size_t headerSize = 0;

//     /* Verify bad parameters fail. */
//     memset( &publishInfo, 0x00, sizeof( publishInfo ) );

//     publishInfo.qos = MQTTQoS2;
//     publishInfo.dup = true;
//     publishInfo.retain = true;
//     publishInfo.topicNameLength = 20;

//     status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
//                                                       remainingLength,
//                                                       buffer,
//                                                       &headerSize );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( headerSize, 4U );
//     /* No flag should be set except QoS2/dup/retain. */
//     TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 2 ) | ( 1U << 3 ) | ( 1U << 0 ) | 0x30 );
//     /* The encoded length must be 0. */
//     TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
//     /* The topic name length should be 0 too. */
//     TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
//     TEST_ASSERT_EQUAL( buffer[ 3 ], 20U );
// }
// void test_MQTT_ProcessIncomingPacketTypeAndLength_PacketNULL( void )
// {
//     uint8_t pBuffer[ 100 ] = { 0 };
//     size_t index = 0;
//     MQTTStatus_t status;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, NULL );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_BufferNULL( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     size_t index = 0;
//     MQTTStatus_t status;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( NULL, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_IndexNULL( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ] = { 0 };
//     MQTTStatus_t status;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, NULL, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTBadParameter, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_NoData( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ] = { 0 };
//     size_t index = 0;
//     MQTTStatus_t status;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTNoDataAvailable, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_InvalidData( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ];
//     size_t index = 2;
//     MQTTStatus_t status;

//     memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
//     memset( pBuffer, 0, 100 );

//     pBuffer[ 0 ] = 0xF0;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_ValidDataOneByte( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ];
//     size_t index = 1;
//     MQTTStatus_t status;

//     memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
//     memset( pBuffer, 0, 100 );

//     pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTNeedMoreBytes, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_ValidDataTwoBytes( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ];
//     size_t index = 2;
//     MQTTStatus_t status;

//     memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
//     memset( pBuffer, 0, 100 );

//     pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     /* 2nd byte is the length. */
//     pBuffer[ 1 ] = 10;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTSuccess, status );
//     TEST_ASSERT_EQUAL( packetInfo.remainingLength, 10U );
//     TEST_ASSERT_EQUAL( packetInfo.headerLength, 2U );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_InvalidLength( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ];
//     size_t index = 6;
//     MQTTStatus_t status;

//     memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
//     memset( pBuffer, 0, 100 );

//     pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     /* 2nd byte onward is the length. */
//     pBuffer[ 1 ] = 0xFF;
//     pBuffer[ 2 ] = 0xFF;
//     pBuffer[ 3 ] = 0xFF;
//     /* This byte doesn't terminate the length. */
//     pBuffer[ 4 ] = 0xFF;
//     pBuffer[ 5 ] = 0xFF;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );
// }

// void test_MQTT_ProcessIncomingPacketTypeAndLength_NonConformingLength( void )
// {
//     MQTTPacketInfo_t packetInfo;
//     uint8_t pBuffer[ 100 ];
//     size_t index = 6;
//     MQTTStatus_t status;

//     memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
//     memset( pBuffer, 0, 100 );

//     pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
//     /* 2nd byte onward is the length. */
//     pBuffer[ 1 ] = 0x80;
//     pBuffer[ 2 ] = 0x80;
//     pBuffer[ 3 ] = 0x80;
//     /* This byte doesn't terminate the length. */
//     pBuffer[ 4 ] = 0x00;

//     status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

//     TEST_ASSERT_EQUAL( MQTTBadResponse, status );
// }

// /**
//  * @brief Tests that MQTT_DeserializePublish works as intended.
//  */
// void test_MQTT_DeserializePublish( void )
// {
//     MQTTPacketInfo_t mqttPacketInfo;
//     MQTTPublishInfo_t publishInfo;
//     MQTTStatus_t status = MQTTSuccess;
//     uint8_t buffer[ 100 ];
//     size_t bufferSize = sizeof( buffer );
//     MQTTFixedBuffer_t fixedBuffer = { 0 };
//     size_t packetSize = bufferSize;
//     MqttPropBuilder_t propBuffer = {0} ; 

//     size_t remainingLength = 0;
//     uint16_t packetIdentifier;

//     fixedBuffer.pBuffer = buffer;
//     fixedBuffer.size = bufferSize;

//     const uint16_t PACKET_ID = 1;

//     memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );

//     /* Verify parameters. */
//     status = MQTT_DeserializePublish( NULL, &packetIdentifier, &publishInfo, &propBuffer,100 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
//     status = MQTT_DeserializePublish( &mqttPacketInfo, NULL, &publishInfo , &propBuffer,100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, NULL, &propBuffer, 100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer,100 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Bad Packet Type. */
//     mqttPacketInfo.type = 0x01;
//     mqttPacketInfo.pRemainingData = buffer;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer,100 );
//     TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

//     /* Incorrect flags. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0xf;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer ,100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* QoS 0 bad remaining length. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
//     mqttPacketInfo.remainingLength = 0;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer ,100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* QoS 1 bad remaining length. */
//     mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x2;
//     mqttPacketInfo.remainingLength = 0;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

//     /* QoS 1 invalid packet identifier. */
//     mqttPacketInfo.remainingLength = 5;
//     buffer[ 0 ] = 0;
//     buffer[ 1 ] = 1;
//     buffer[ 2 ] = ( uint8_t ) 'a';
//     buffer[ 3 ] = 0;
//     buffer[ 4 ] = 0;
//     status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer , 100);
//     TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
// }
// void test_serializeHeaders(void)
// {
//     uint8_t buffer[4] ; 

//     MQTT_SerializeSubscribeHeader(3, buffer, 1) ; 
//     TEST_ASSERT_EQUAL(buffer[0], MQTT_PACKET_TYPE_SUBSCRIBE);
//     MQTT_SerializeUnsubscribeHeader(3, buffer, 1) ; 
//     TEST_ASSERT_EQUAL(buffer[0], MQTT_PACKET_TYPE_UNSUBSCRIBE);
//     uint8_t buf[22] ; 
//     MQTTConnectInfo_t pConnectInfo ;  
//     pConnectInfo.cleanSession = true ; 
//     pConnectInfo.pUserName = "abc" ; 
//     pConnectInfo.pPassword = "def" ; 
//     pConnectInfo.keepAliveSeconds = 100 ; 
//     MQTTPublishInfo_t pWillInfo ; 
//     pWillInfo.qos = MQTTQoS1 ; 
//     pWillInfo.retain = true ; 
//     MQTT_SerializeConnectFixedHeader(buf,&pConnectInfo , &pWillInfo, 20 ) ; 
//     pWillInfo.qos = MQTTQoS2 ; 
//     MQTT_SerializeConnectFixedHeader(buf,&pConnectInfo , &pWillInfo, 20 ) ; 
// }

// void test_OptionalProperties(void)
// {
//     MQTTStatus_t mqttStatus = MQTTSuccess ; 
//     MqttPropBuilder_t propBuilder;
//     MqttPropBuilder_t prop1 ; 
//     prop1.pBuffer = NULL ; 
//     uint8_t buf[100];
//     size_t bufLength = sizeof(buf);
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder), buf, bufLength); 
//     TEST_ASSERT_EQUAL_INT(mqttStatus , MQTTSuccess) ; 
//     mqttStatus = MQTTPropAdd_SubscribeId(&propBuilder, 2) ; 
//     TEST_ASSERT_EQUAL_INT(mqttStatus , MQTTSuccess) ; 
//     mqttStatus = MQTTPropAdd_SubscribeId(&propBuilder, 2) ; 
//     TEST_ASSERT_EQUAL_INT(mqttStatus , MQTTBadParameter) ; 
//     mqttStatus = MQTTPropAdd_SubscribeId(&propBuilder, 0) ;
//     TEST_ASSERT_EQUAL_INT(mqttStatus, MQTTBadParameter) ;

//     mqttStatus = MQTTPropAdd_SubscribeId(NULL, 1) ;
//     TEST_ASSERT_EQUAL_INT(mqttStatus, MQTTBadParameter) ;

//     mqttStatus = MQTTPropAdd_SubscribeId(&prop1, 2) ; 
//     TEST_ASSERT_EQUAL_INT(mqttStatus , MQTTBadParameter) ; 

//     MqttPropBuilder_t propBuilder1;
//     uint8_t buf1[1] ; 
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder1), buf1, 1);
//     mqttStatus = MQTTPropAdd_SubscribeId(&propBuilder1, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     MQTTUserProperty_t userProperty ; 
//     memset(&userProperty, 0x0, sizeof(userProperty));
//     userProperty.pKey = "abc" ; 
//     userProperty.pValue = "def" ;
//     userProperty.keyLength = 3 ;
//     userProperty.valueLength = 3 ; 
//     mqttStatus = MQTTPropAdd_UserProp(&(propBuilder) , &userProperty ) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_UserProp(NULL , &userProperty ) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_UserProp(&(propBuilder) , NULL ) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_UserProp(&prop1, &userProperty) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus); 

//     userProperty.pKey = NULL , userProperty.pValue = NULL ; 
//     mqttStatus = MQTTPropAdd_UserProp(&propBuilder, &userProperty) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter , mqttStatus); 

//     mqttStatus = MQTTPropAdd_ConnSessionExpiry(NULL, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnSessionExpiry(&prop1, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnSessionExpiry(&propBuilder, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnSessionExpiry(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnReceiveMax(NULL, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnReceiveMax(&prop1, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnReceiveMax(&(propBuilder) , 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnReceiveMax(&(propBuilder) , 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnMaxPacketSize(NULL, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnMaxPacketSize(&prop1, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnMaxPacketSize(&propBuilder , 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnMaxPacketSize(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(mqttStatus , MQTTBadParameter) ;

//     mqttStatus = MQTTPropAdd_ConnTopicAliasMax(NULL, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnTopicAliasMax(&prop1, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnTopicAliasMax(&propBuilder, 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnTopicAliasMax(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnRequestRespInfo(NULL, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestRespInfo(&prop1, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestRespInfo(&propBuilder , 1) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestRespInfo(&propBuilder, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnRequestProbInfo(NULL, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestProbInfo(&prop1, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestProbInfo(&propBuilder , 1 ) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnRequestProbInfo(&propBuilder, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnAuthMethod(NULL, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthMethod(&prop1, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthMethod(&propBuilder, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthMethod(&propBuilder, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ConnAuthData(NULL, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthData(&prop1, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthData(&propBuilder, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ConnAuthData(&propBuilder, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubPayloadFormat(NULL, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubPayloadFormat(&prop1, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubPayloadFormat(&propBuilder , 1) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubPayloadFormat(&propBuilder, 1) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubMessageExpiry(NULL, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubMessageExpiry(&prop1, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubMessageExpiry(&propBuilder , 10) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubMessageExpiry(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubTopicAlias(NULL, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubTopicAlias(&prop1, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubTopicAlias(&propBuilder, 0) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubTopicAlias(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubTopicAlias(&propBuilder, 10) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubResponseTopic(NULL, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&prop1, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&propBuilder, NULL, 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&propBuilder, "abc", 0) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&propBuilder, "abc", 3) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&propBuilder, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     MqttPropBuilder_t propBuilder2;
//     uint8_t buf2[500];
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder2), buf2, 50);
//     mqttStatus = MQTTPropAdd_PubResponseTopic(&propBuilder2, "abc/#/def", 3) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubCorrelationData(NULL, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubCorrelationData(&(prop1), "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubCorrelationData(&(propBuilder), NULL, 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubCorrelationData(&(propBuilder), "abc", 0) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubCorrelationData(&(propBuilder) , "abc" , 3) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubCorrelationData(&(propBuilder), "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_PubContentType(NULL, "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubContentType(&(prop1), "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubContentType(&(propBuilder), NULL, 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubContentType(&(propBuilder), "abc", 0) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubContentType(&(propBuilder), "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_PubContentType(&(propBuilder), "abc", 3) ;
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;

//     mqttStatus = MQTTPropAdd_ReasonString(NULL, "abc", 3);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ReasonString(&(prop1), "abc", 3);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ReasonString(&(propBuilder), NULL, 3);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ReasonString(&(propBuilder), "abc", 0);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ReasonString(&(propBuilder), "abc", 3); 
//     TEST_ASSERT_EQUAL_INT(MQTTSuccess, mqttStatus) ;
//     mqttStatus = MQTTPropAdd_ReasonString(&(propBuilder), "abc", 3);
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, mqttStatus) ;
// }

// void test_updateContextWithConnectProps(void)
// {
//     MQTTStatus_t mqttStatus = MQTTSuccess ; 
//     MqttPropBuilder_t propBuilder;
//     uint8_t buffer[50];
//     size_t bufLength = sizeof(buffer);
//     mqttStatus = MqttPropertyBuilder_Init(&(propBuilder), buffer, bufLength);
//     uint8_t *pIndex = buffer ; 
//     pIndex = serializeuint_16(pIndex, MQTT_RECEIVE_MAX_ID ) ; 
//     pIndex = serializeuint_32(pIndex , MQTT_SESSION_EXPIRY_ID) ; 
//     pIndex = serializeuint_16(pIndex, MQTT_TOPIC_ALIAS_MAX_ID) ; 
//     pIndex = serializeuint_32(pIndex, MQTT_MAX_PACKET_SIZE_ID) ; 
//     propBuilder.currentIndex = 16 ; 
//     MQTTConnectProperties_t connectProps  ; 
//     mqttStatus = updateContextWithConnectProps(&propBuilder , &connectProps) ;
//     TEST_ASSERT_EQUAL_INT(mqttStatus, MQTTSuccess) ;
// }

// void test_MQTT_SerializeAck(void)
// {
//     MQTTFixedBuffer_t fixedBuffer ; 
//     uint8_t buf[50] ; 
//     fixedBuffer.pBuffer = buf ; 
//     fixedBuffer.size = sizeof(buf) ;

//     MQTTStatus_t status = MQTTSuccess ; 
//     status = MQTT_SerializeAck(&fixedBuffer, MQTT_PACKET_TYPE_PUBACK , 100) ; 
//     TEST_ASSERT_EQUAL_INT(status, MQTTSuccess) ;

//     status = MQTT_SerializeAck(&fixedBuffer, MQTT_PACKET_TYPE_PUBACK , 0) ; 
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter) ;


//     status = MQTT_SerializeAck(&fixedBuffer, MQTT_PACKET_TYPE_SUBACK, 100) ;
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter) ;

//     status = MQTT_SerializeAck(NULL, MQTT_PACKET_TYPE_PUBACK , 100) ; 
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter) ;

//     fixedBuffer.pBuffer = NULL ; 
//     status = MQTT_SerializeAck(&fixedBuffer, MQTT_PACKET_TYPE_PUBACK , 100) ; 
//     TEST_ASSERT_EQUAL_INT(status,MQTTBadParameter ) ;

//     fixedBuffer.pBuffer = buf ;
//     fixedBuffer.size = 1 ; 
//     status = MQTT_SerializeAck(&fixedBuffer, MQTT_PACKET_TYPE_PUBACK , 100) ; 
//     TEST_ASSERT_EQUAL_INT(status,MQTTNoMemory ) ;

// }

// void test_MQTTV5_SerializeDisconnect(void)
// {
//     uint8_t buf[10] ; 

//     MQTTStatus_t status = MQTTSuccess ; 
//     MQTT_SerializeDisconnectFixed(buf , 0x00 , 10) ; 
//     TEST_ASSERT_EQUAL(buf[0], MQTT_PACKET_TYPE_DISCONNECT) ;

//     MQTT_SerializeAckFixed(buf , MQTT_PACKET_TYPE_SUBACK, 10 , 10, 0x00) ; 
//     TEST_ASSERT_EQUAL(buf[0], MQTT_PACKET_TYPE_SUBACK) ;

// }

// void test_validatePublishProperties(void)
// {
//     MQTTStatus_t status = MQTTSuccess ; 
//     uint16_t serverTopicAliasMax ; 
//     uint16_t topicAlias ; 
//     MqttPropBuilder_t propBuilder;
//     uint8_t buffer[50];
//     size_t bufLength = sizeof(buffer);
//     status = MqttPropertyBuilder_Init(&(propBuilder), buffer, bufLength);
//     uint8_t *pIndex = buffer ; 
//     pIndex = serializeuint_16(pIndex, 0x23) ;
//     pIndex = serializeuint_16(pIndex, 0x23) ;
//     propBuilder.currentIndex = 3 ; 
    
//     /*Invalid Topic Alias*/
//     serverTopicAliasMax = 2 ; 
//     status = validatePublishProperties(serverTopicAliasMax, &propBuilder, &topicAlias) ; 
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter) ; 

//     /*More than 1 property received*/
//     propBuilder.currentIndex = 6 ; 
//     status = validatePublishProperties(serverTopicAliasMax, &propBuilder, &topicAlias) ; 
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter) ; 

// }

// void test_validateSubscribeProperties(void)
// {
//     uint8_t isSubIdAvailable = 0 ; 
//     MQTTStatus_t status = MQTTSuccess ; 
//     MqttPropBuilder_t propBuilder;
//     uint8_t buffer[50];
//     size_t bufLength = sizeof(buffer);
//     status = MqttPropertyBuilder_Init(&(propBuilder), buffer, bufLength);
//     uint8_t *pIndex = buffer ; 
//     *pIndex = MQTT_SUBSCRIPTION_ID_ID ; 
//     pIndex ++ ; 
//     *pIndex = 2 ; 
//     propBuilder.currentIndex = 2 ; 
//     status = validateSubscribeProperties(isSubIdAvailable, &propBuilder) ; 
//     TEST_ASSERT_EQUAL_INT(MQTTBadParameter, status) ; 

// }

// void test_getProps(void)
// {
//     MQTTStatus_t status = MQTTSuccess;
//     MqttPropBuilder_t propBuffer;
//     MqttPropBuilder_t propBuffer1;
//     propBuffer1.pBuffer = NULL;
//     uint8_t buffer[500];
//     size_t bufLength = 95; 
//     status = MqttPropertyBuilder_Init(&propBuffer, buffer, bufLength);

//     // 77 , 82 

//     uint8_t *pIndex = buffer;
//     pIndex = serializeuint_16(pIndex , MQTT_TOPIC_ALIAS_ID); 
//     pIndex = serializeuint_8(pIndex , MQTT_PAYLOAD_FORMAT_ID) ; 
//     pIndex = serializeutf_8(pIndex , MQTT_RESPONSE_TOPIC_ID) ; 
//     pIndex = serializeutf_8(pIndex , MQTT_CORRELATION_DATA_ID) ; 
//     pIndex = serializeuint_32(pIndex , MQTT_MSG_EXPIRY_ID); 
//     pIndex = serializeutf_8(pIndex, MQTT_CONTENT_TYPE_ID) ;
//     *pIndex = MQTT_SUBSCRIPTION_ID_ID; 
//     pIndex ++ ; 
//     *pIndex = 2 ; 
//     pIndex++ ; 
//     pIndex = serializeuint_32(pIndex , MQTT_SESSION_EXPIRY_ID); 
//     pIndex = serializeuint_16(pIndex , MQTT_TOPIC_ALIAS_MAX_ID); 
//     pIndex = serializeuint_16(pIndex, MQTT_RECEIVE_MAX_ID); 
//     pIndex = serializeuint_8(pIndex , MQTT_MAX_QOS_ID); 
//     pIndex = serializeuint_8(pIndex, MQTT_RETAIN_AVAILABLE_ID ); 
//     pIndex = serializeuint_32(pIndex , MQTT_MAX_PACKET_SIZE_ID); 
//     pIndex = serializeutf_8(pIndex, MQTT_ASSIGNED_CLIENT_ID); 
//     pIndex = serializeuint_8(pIndex, MQTT_WILDCARD_ID); 
//     pIndex = serializeuint_8(pIndex, MQTT_SUB_AVAILABLE_ID);
//     pIndex = serializeutf_8pair(pIndex) ; 
//     pIndex = serializeutf_8(pIndex , MQTT_REASON_STRING_ID); 
//     pIndex = serializeutf_8(pIndex , MQTT_SERVER_REF_ID); 

//     uint16_t topicAlias;
//     uint8_t payloadFormat;
//     const char *pResponseTopic;
//     uint16_t responseTopicLength;
//     const void *correlationData;
//     uint16_t correlationLength;
//     uint32_t messageExpiry;
//     const char *pContentType;
//     uint16_t contentTypeLength;
//     uint32_t subscriptionId;
//     uint32_t sessionExpiry;
//     uint16_t aliasMax;
//     uint16_t receiveMax;
//     uint8_t maxQoS;
//     uint8_t retainAvailable;
//     uint32_t maxPacketSize;
//     const char *pClientId;
//     uint16_t clientIdLength;
//     uint8_t wildcard;
//     uint8_t subAvailable;
//     uint8_t propertyId;
//     const char* pUserPropKey;
//     uint16_t pUserPropKeyLen;
//     const char*  pUserPropVal;
//     uint16_t pUserPropValLen;
//     const char*  pReasonString;
//     uint16_t reasonStringLength;


//     status = MQTT_IncomingGetNextProp(NULL, &propertyId);
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter);

//     status = MQTT_IncomingGetNextProp(&propBuffer1, &propertyId);
//     TEST_ASSERT_EQUAL_INT(status, MQTTBadParameter);

//     status = MQTT_IncomingGetNextProp(&propBuffer, &propertyId);

//     while (status == MQTTSuccess)
//     {
//         switch (propertyId)
//         {
//             case MQTT_TOPIC_ALIAS_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubTopicAlias(NULL, &topicAlias));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubTopicAlias(&propBuffer1, &topicAlias));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubTopicAlias(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubTopicAlias(&propBuffer, &topicAlias));
//                 break;

//             case MQTT_PAYLOAD_FORMAT_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator(NULL, &payloadFormat));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator(&propBuffer1, &payloadFormat));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubPayloadFormatIndicator(&propBuffer, &payloadFormat));
//                 break;

//             case MQTT_RESPONSE_TOPIC_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubResponseTopic(NULL, &pResponseTopic, &responseTopicLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubResponseTopic(&propBuffer1, &pResponseTopic, &responseTopicLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubResponseTopic(&propBuffer, NULL, &responseTopicLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubResponseTopic(&propBuffer, &pResponseTopic, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubResponseTopic(&propBuffer, &pResponseTopic, &responseTopicLength));
//                 break;

//             case MQTT_CORRELATION_DATA_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubCorrelationData(NULL, &correlationData, &correlationLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubCorrelationData(&propBuffer1, &correlationData, &correlationLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubCorrelationData(&propBuffer, NULL, &correlationLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubCorrelationData(&propBuffer, &correlationData, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubCorrelationData(&propBuffer, &correlationData, &correlationLength));
//                 break;

//             case MQTT_MSG_EXPIRY_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval(NULL, &messageExpiry));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval(&propBuffer1, &messageExpiry));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubMessageExpiryInterval(&propBuffer, &messageExpiry));
//                 break;

//             case MQTT_CONTENT_TYPE_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubContentType(NULL, &pContentType, &contentTypeLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubContentType(&propBuffer1, &pContentType, &contentTypeLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubContentType(&propBuffer, NULL, &contentTypeLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubContentType(&propBuffer, &pContentType, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubContentType(&propBuffer, &pContentType, &contentTypeLength));
//                 break;

//             case MQTT_SUBSCRIPTION_ID_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubSubscriptionId(NULL, &subscriptionId));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubSubscriptionId(&propBuffer1, &subscriptionId));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_PubSubscriptionId(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_PubSubscriptionId(&propBuffer, &subscriptionId));
//                 break;

//             case MQTT_SESSION_EXPIRY_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSessionExpiry(NULL, &sessionExpiry));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSessionExpiry(&propBuffer1, &sessionExpiry));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSessionExpiry(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnSessionExpiry(&propBuffer, &sessionExpiry));
//                 break;

//             case MQTT_TOPIC_ALIAS_MAX_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax(NULL, &aliasMax));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax(&propBuffer1, &aliasMax));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnTopicAliasMax(&propBuffer, &aliasMax));
//                 break;

//             case MQTT_RECEIVE_MAX_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnReceiveMax(NULL, &receiveMax));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnReceiveMax(&propBuffer1, &receiveMax));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnReceiveMax(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnReceiveMax(&propBuffer, &receiveMax));
//                 break;

//             case MQTT_MAX_QOS_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxQos(NULL, &maxQoS));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxQos(&propBuffer1, &maxQoS));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxQos(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnMaxQos(&propBuffer, &maxQoS));
//                 break;

//             case MQTT_RETAIN_AVAILABLE_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnRetainAvailable(NULL, &retainAvailable));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnRetainAvailable(&propBuffer1, &retainAvailable));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnRetainAvailable(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnRetainAvailable(&propBuffer, &retainAvailable));
//                 break;

//             case MQTT_MAX_PACKET_SIZE_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize(NULL, &maxPacketSize));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize(&propBuffer1, &maxPacketSize));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnMaxPacketSize(&propBuffer, &maxPacketSize));
//                 break;

//             case MQTT_ASSIGNED_CLIENT_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnClientId(NULL, &pClientId, &clientIdLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnClientId(&propBuffer1, &pClientId, &clientIdLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnClientId(&propBuffer, NULL, &clientIdLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnClientId(&propBuffer, &pClientId, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnClientId(&propBuffer, &pClientId, &clientIdLength));
//                 break;

//             case MQTT_WILDCARD_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnWildcard(NULL, &wildcard));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnWildcard(&propBuffer1, &wildcard));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnWildcard(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnWildcard(&propBuffer, &wildcard));
//                 break;

//             case MQTT_SUB_AVAILABLE_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSubId(NULL, &subAvailable));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSubId(&propBuffer1, &subAvailable));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ConnSubId(&propBuffer, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ConnSubId(&propBuffer, &subAvailable));
//                 break;
//             case MQTT_USER_PROPERTY_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(NULL, &pUserPropKey, &pUserPropKeyLen,&pUserPropVal, &pUserPropValLen));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(&propBuffer1, &pUserPropKey, &pUserPropKeyLen,&pUserPropVal, &pUserPropValLen));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(&propBuffer,NULL, &pUserPropKeyLen,&pUserPropVal, &pUserPropValLen));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(&propBuffer,&pUserPropKey, NULL ,&pUserPropVal, &pUserPropValLen));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(&propBuffer, &pUserPropKey, &pUserPropKeyLen,NULL, &pUserPropValLen));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_UserProp(&propBuffer, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_UserProp(&propBuffer, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, &pUserPropValLen));
//                 break ; 
//             case MQTT_REASON_STRING_ID:
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ReasonString(NULL, &pReasonString, &reasonStringLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ReasonString(&propBuffer1, &pReasonString, &reasonStringLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ReasonString(&propBuffer, NULL, &reasonStringLength));
//                 TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_ReasonString(&propBuffer, &pReasonString, NULL));
//                 TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_ReasonString(&propBuffer, &pReasonString, &reasonStringLength));
//                 break ; 
//             case MQTT_SERVER_REF_ID:
//             TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_DisconnectServerRef(NULL, &pReasonString, &reasonStringLength));
//             TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_DisconnectServerRef(&propBuffer1, &pReasonString, &reasonStringLength));
//             TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_DisconnectServerRef(&propBuffer, NULL, &reasonStringLength));
//             TEST_ASSERT_EQUAL_INT(MQTTBadParameter, MQTTPropGet_DisconnectServerRef(&propBuffer, &pReasonString, NULL));
//             TEST_ASSERT_EQUAL_INT(MQTTSuccess, MQTTPropGet_DisconnectServerRef(&propBuffer, &pReasonString, &reasonStringLength));
//             break ; 
//             default:
//                 break;
//         }

//         status = MQTT_IncomingGetNextProp(&propBuffer, &propertyId);
//     }
// }
