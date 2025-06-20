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
 * @file core_mqtt_serializer_utest.c
 * @brief Unit tests for functions in core_mqtt_serializer.h.
 */
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "unity.h"

/* Include paths for public enums, structures, and macros. */
#include "core_mqtt_serializer.h"

/* Set network context to double pointer to buffer (uint8_t**). */
struct NetworkContext
{
    uint8_t ** buffer;
};

#define MQTT_MAX_REMAINING_LENGTH                   ( 268435455UL )
#define MQTT_PACKET_CONNACK_REMAINING_LENGTH        ( ( uint8_t ) 2U )    /**< @brief A CONNACK packet always has a "Remaining length" of 2. */
#define MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK    ( ( uint8_t ) 0x01U ) /**< @brief The "Session Present" bit is always the lowest bit. */
#define MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH     ( ( uint8_t ) 2 )     /**< @brief PUBACK, PUBREC, PUBREl, PUBCOMP, UNSUBACK Remaining length. */
#define MQTT_PACKET_PINGRESP_REMAINING_LENGTH       ( 0U )                /**< @brief A PINGRESP packet always has a "Remaining length" of 0. */
#define MQTT_PACKET_PUBACK_REMAINING_LENGTH         ( 2U )
#define MQTT_PACKET_UNSUBACK_REMAINING_LENGTH       ( 2U )

/*
 * MQTT client identifier.
 */
#define MQTT_CLIENT_IDENTIFIER                      "testclient"

/*
 * Client identifier and length to use for the MQTT API tests.
 */
#define CLIENT_IDENTIFIER                           ( "test" )                                           /**< @brief Client identifier. */
#define CLIENT_IDENTIFIER_LENGTH                    ( ( uint16_t ) ( sizeof( CLIENT_IDENTIFIER ) - 1 ) ) /**< @brief Length of client identifier. */

/*
 * Topic name and length to use for the MQTT API tests.
 */
#define TEST_TOPIC_NAME                             ( "/test/topic" )                                  /**< @brief An arbitrary topic name. */
#define TEST_TOPIC_NAME_LENGTH                      ( ( uint16_t ) ( sizeof( TEST_TOPIC_NAME ) - 1 ) ) /**< @brief Length of topic name. */

/**
 * @brief Test-defined macro for MQTT username.
 */
#define MQTT_TEST_USERNAME                          "username"
#define MQTT_TEST_USERNAME_LEN                      ( sizeof( MQTT_TEST_USERNAME ) - 1 )

/**
 * @brief Test-defined macro for MQTT password.
 */
#define MQTT_TEST_PASSWORD                          "password"
#define MQTT_TEST_PASSWORD_LEN                      ( sizeof( MQTT_TEST_PASSWORD ) - 1 )

/**
 * @brief Length of the client identifier.
 */
#define MQTT_CLIENT_IDENTIFIER_LEN                  ( sizeof( MQTT_CLIENT_IDENTIFIER ) - 1 )

/**
 * @brief Sample payload.
 */
#define MQTT_SAMPLE_PAYLOAD                         "Hello World"
#define MQTT_SAMPLE_PAYLOAD_LEN                     ( sizeof( MQTT_SAMPLE_PAYLOAD ) - 1 )

#define   TEST_TOPIC_ALIAS                          ( 2U )
#define   TEST_MSG_EXPIRY                           ( 100U )


#define MQTT_TEST_UTF8_STRING                       ( "test" )
#define MQTT_TEST_UTF8_STRING_LENGTH                ( sizeof( MQTT_TEST_UTF8_STRING ) - 1 )
#define MQTT_TEST_UINT8                             ( 1U )
#define MQTT_TEST_UINT16                            ( 32U )
#define MQTT_TEST_UINT32                            ( 300U )

/* MQTT CONNECT flags. */
#define MQTT_CONNECT_FLAG_CLEAN                     ( 1 ) /**< @brief Clean session. */
#define MQTT_CONNECT_FLAG_WILL                      ( 2 ) /**< @brief Will present. */
#define MQTT_CONNECT_FLAG_WILL_QOS1                 ( 3 ) /**< @brief Will QoS 1. */
#define MQTT_CONNECT_FLAG_WILL_QOS2                 ( 4 ) /**< @brief Will QoS 2. */
#define MQTT_CONNECT_FLAG_WILL_RETAIN               ( 5 ) /**< @brief Will retain. */
#define MQTT_CONNECT_FLAG_PASSWORD                  ( 6 ) /**< @brief Password present. */
#define MQTT_CONNECT_FLAG_USERNAME                  ( 7 ) /**< @brief User name present. */

/*Default connect properties. */
#define DEFAULT_RECEIVE_MAX                         ( 65535U )
#define DEFAULT_REQUEST_PROBLEM                     ( 1 )

/**
 * @brief The Remaining Length field of MQTT disconnect packets, per MQTT spec.
 */
#define MQTT_DISCONNECT_REMAINING_LENGTH            ( ( uint8_t ) 0 )

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
 * @brief Get the high byte of a 16-bit unsigned integer.
 */
#define UINT16_HIGH_BYTE( x )             ( ( uint8_t ) ( ( x ) >> 8 ) )

/**
 * @brief Get the low byte of a 16-bit unsigned integer.
 */
#define UINT16_LOW_BYTE( x )              ( ( uint8_t ) ( ( x ) & 0x00ffU ) )

/**
 * @brief Maximum number of bytes in the Remaining Length field is four according
 * to MQTT 5.0 spec.
 */
#define MQTT_REMAINING_BUFFER_MAX_LENGTH    ( 4 )

/**
 * @brief Length of buffer padding to use in under/overflow checks.
 */
#define BUFFER_PADDING_LENGTH               ( 4 )

/**
 * @brief Byte to use for buffer padding in under/overflow checks.
 */
#define BUFFER_PADDING_BYTE                 ( 0xA5 )

/**
 * @brief Length of the MQTT network buffer.
 */
#define MQTT_TEST_BUFFER_LENGTH             ( 1024 )

#define UINT16_DECODE( ptr )                            \
    ( uint16_t ) ( ( ( ( uint16_t ) ptr[ 0 ] ) << 8 ) | \
                   ( ( uint16_t ) ptr[ 1 ] ) )

#define UINT32_DECODE( ptr )                             \
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) | \
                   ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
                   ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) |  \
                   ( ( uint32_t ) ptr[ 3 ] ) )

#define UINT32_BYTE3( x )    ( ( uint8_t ) ( ( x ) >> 24 ) )

#define UINT32_BYTE2( x )    ( ( uint8_t ) ( ( x ) >> 16 ) )

#define UINT32_BYTE1( x )    ( ( uint8_t ) ( ( x ) >> 8 ) )

#define UINT32_BYTE0( x )    ( ( uint8_t ) ( ( x ) & 0x000000FFU ) )


#define MQTT_MAX_PACKET_SIZE    ( 268435460UL )

/* Variables common to testcases */
MQTTConnectionProperties_t properties;
MQTTUserProperty_t userProperty;
MQTTPublishInfo_t publishInfo;
MQTTConnectInfo_t connectInfo;
MQTTPacketInfo_t packetInfo;
MQTTStatus_t status;

/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp( void )
{
    memset( &properties, 0x0, sizeof( properties ) );
    memset( &userProperty, 0x0, sizeof( userProperty ) );
    memset( &publishInfo, 0x0, sizeof( publishInfo ) );
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    memset( &packetInfo, 0x0, sizeof( packetInfo ) );
}

/* Called after each test method. */
void tearDown( void )
{
}

/* Called at the beginning of the whole suite. */
void suiteSetUp()
{
}

/* Called at the end of the whole suite. */
int suiteTearDown( int numFailures )
{
    return numFailures;
}

/* ========================================================================== */

/**
 * @brief Mock successful transport receive by reading data from a buffer.
 */
int32_t mockReceive( NetworkContext_t * pNetworkContext,
                     void * pBuffer,
                     size_t bytesToRecv )
{
    uint8_t * returnBuffer = ( uint8_t * ) pBuffer;
    uint8_t * mockNetwork;
    size_t bytesRead = 0;

    /* Treat network context as pointer to buffer for mocking  */
    mockNetwork = *( pNetworkContext->buffer );

    while( bytesRead++ < bytesToRecv )
    {
        /* Read single byte and advance buffer. */
        *returnBuffer++ = *mockNetwork++;
    }

    /* Move stream by bytes read. */
    *( pNetworkContext->buffer ) = mockNetwork;

    return bytesToRecv;
}

/**
 * @brief Mock transport receive with no data available.
 */
static int32_t mockReceiveNoData( NetworkContext_t * pNetworkContext,
                                  void * pBuffer,
                                  size_t bytesToRecv )
{
/* Suppress unused parameter warning. */
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToRecv;

    return 0;
}

/**
 * @brief Mock transport receive failure.
 */
static int32_t mockReceiveFailure( NetworkContext_t * pNetworkContext,
                                   void * pBuffer,
                                   size_t bytesToRecv )
{
/* Suppress unused parameter warning. */
    ( void ) pNetworkContext;
    ( void ) pBuffer;
    ( void ) bytesToRecv;

    return -1;
}

/**
 * @brief Mock transport receive that succeeds once, then fails.
 */
static int32_t mockReceiveSucceedThenFail( NetworkContext_t * pNetworkContext,
                                           void * pBuffer,
                                           size_t bytesToRecv )
{
    int32_t retVal = 0;
    static int counter = 0;

    if( counter++ )
    {
        retVal = mockReceiveFailure( pNetworkContext, pBuffer, bytesToRecv );
        counter = 0;
    }
    else
    {
        retVal = mockReceive( pNetworkContext, pBuffer, bytesToRecv );
    }

    return retVal;
}

/* ========================================================================== */


/**
 * @brief Initialize pPublishInfo using test-defined macros.
 *
 * @param[in] pPublishInfo Publish information.
 */
static void setupPublishInfo( MQTTPublishInfo_t * pPublishInfo )
{
    pPublishInfo->pTopicName = TEST_TOPIC_NAME;
    pPublishInfo->topicNameLength = TEST_TOPIC_NAME_LENGTH;
    pPublishInfo->pPayload = MQTT_SAMPLE_PAYLOAD;
    pPublishInfo->payloadLength = MQTT_SAMPLE_PAYLOAD_LEN;
    pPublishInfo->qos = MQTTQoS0;
    pPublishInfo->dup = false;
    pPublishInfo->retain = false;
}

/**
 * @brief Encode remaining length into pDestination for packet serialization
 * using MQTT 5.0 spec.
 *
 * @param[in] pDestination Buffer to write encoded remaining length.
 * @param[in] length Actual remaining length.
 */
static size_t encodeRemainingLength( uint8_t * pDestination,
                                     size_t length )
{
    uint8_t lengthByte;
    uint8_t * pLengthEnd = NULL;
    size_t remainingLength = length;

    TEST_ASSERT_NOT_NULL( pDestination );

    pLengthEnd = pDestination;

    /* This algorithm is copied from the MQTT 5.0 spec. */
    do
    {
        lengthByte = ( uint8_t ) ( remainingLength % 128U );
        remainingLength = remainingLength / 128U;

        /* Set the high bit of this byte, indicating that there's more data. */
        if( remainingLength > 0U )
        {
            UINT8_SET_BIT( lengthByte, 7 );
        }

        /* Output a single encoded byte. */
        *pLengthEnd = lengthByte;
        pLengthEnd++;
    } while( remainingLength > 0U );

    return ( size_t ) ( pLengthEnd - pDestination );
}

/**
 * @brief Encode UTF-8 string and its length into pDestination for
 * packet serialization.
 *
 * @param[in] pDestination Buffer to write encoded string.
 * @param[in] source String to encode.
 * @param[in] sourceLength Length of the string to encode.
 */
static size_t encodeString( uint8_t * pDestination,
                            const char * source,
                            uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    /* Typecast const char * typed source buffer to const uint8_t *.
     * This is to use same type buffers in memcpy. */
    const uint8_t * pSourceBuffer = ( const uint8_t * ) source;

    TEST_ASSERT_NOT_NULL( pSourceBuffer );
    TEST_ASSERT_NOT_NULL( pDestination );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */
    ( void ) memcpy( pBuffer, pSourceBuffer, sourceLength );

    /* Return the pointer to the end of the encoded string. */
    pBuffer += sourceLength;

    return ( size_t ) ( pBuffer - pDestination );
}

/**
 * @brief Encode UTF-8 string and its length into pDestination for
 * packet serialization.
 *
 * @param[in] pDestination Buffer to write encoded string.
 * @param[in] source String to encode.
 * @param[in] sourceLength Length of the string to encode.
 */
static size_t encodeStringSize( uint8_t * pDestination,
                                const char * source,
                                uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    /* Typecast const char * typed source buffer to const uint8_t *.
     * This is to use same type buffers in memcpy. */
    const uint8_t * pSourceBuffer = ( const uint8_t * ) source;

    TEST_ASSERT_NOT_NULL( pSourceBuffer );
    TEST_ASSERT_NOT_NULL( pDestination );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */
    ( void ) memcpy( pBuffer, pSourceBuffer, sourceLength );

    /* Return the pointer to the end of the encoded string. */
    pBuffer += sourceLength;

    return ( size_t ) ( pBuffer - pDestination );
}


/**
 * @brief Pad beginning and end of buffer with non-zero bytes to be used in
 * checking for under/overflow after serialization.
 *
 * @param[in] pBuffer Buffer to pad.
 * @param[in] bufferLength Total length of buffer.
 */
static void padAndResetBuffer( uint8_t * pBuffer,
                               size_t bufferLength )
{
    int i = 0;

    for( i = 0; i < BUFFER_PADDING_LENGTH; i++ )
    {
        pBuffer[ i ] = BUFFER_PADDING_BYTE;
        pBuffer[ bufferLength - 1 - i ] = BUFFER_PADDING_BYTE;
    }

    /* Zero out rest of buffer. */
    memset( &pBuffer[ BUFFER_PADDING_LENGTH ], 0x0, bufferLength - 2 * BUFFER_PADDING_LENGTH );
}

/**
 * @brief Test buffer for under/overflow.
 *
 * @param[in] pBuffer Buffer to check.
 * @param[in] bufferLength Total length of buffer.
 */
static void checkBufferOverflow( uint8_t * pBuffer,
                                 size_t bufferLength )
{
    /* Check beginning of buffer. */
    TEST_ASSERT_EACH_EQUAL_UINT8( BUFFER_PADDING_BYTE,
                                  pBuffer,
                                  BUFFER_PADDING_LENGTH );
    /* Check end. */
    TEST_ASSERT_EACH_EQUAL_UINT8( BUFFER_PADDING_BYTE,
                                  pBuffer + bufferLength - BUFFER_PADDING_LENGTH,
                                  BUFFER_PADDING_LENGTH );
}


static uint8_t * initializeDeserialize( MQTTPacketInfo_t * packetInfo,
                                        uint8_t * pIndex )
{
    uint8_t * pIndexLocal = pIndex;

    packetInfo->pRemainingData = pIndexLocal;
    packetInfo->type = MQTT_PACKET_TYPE_CONNACK;
    *pIndexLocal = 0x01;
    pIndexLocal++;
    *pIndexLocal = 0x00;
    pIndexLocal++;
    return pIndexLocal;
}

static uint8_t * serializeuint_32( uint8_t * pIndex,
                                   uint8_t propertyId )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT32_BYTE3( MQTT_TEST_UINT32 );
    pIndexLocal[ 1 ] = UINT32_BYTE2( MQTT_TEST_UINT32 );
    pIndexLocal[ 2 ] = UINT32_BYTE1( MQTT_TEST_UINT32 );
    pIndexLocal[ 3 ] = UINT32_BYTE0( MQTT_TEST_UINT32 );
    pIndexLocal = &pIndexLocal[ 4 ];
    return pIndexLocal;
}


static uint8_t * serializeuint_16( uint8_t * pIndex,
                                   uint8_t propertyId )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( MQTT_TEST_UINT16 );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( MQTT_TEST_UINT16 );
    pIndexLocal = &pIndexLocal[ 2 ];
    return pIndexLocal;
}

static uint8_t * serializeuint_8( uint8_t * pIndex,
                                  uint8_t propertyId )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = MQTT_TEST_UINT8;
    pIndexLocal++;
    return pIndexLocal;
}
static uint8_t * serializeutf_8( uint8_t * pIndex,
                                 uint8_t propertyId )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = propertyId;
    pIndexLocal++;
    size_t dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}

static uint8_t * serializeutf_8pair( uint8_t * pIndex )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = MQTT_USER_PROPERTY_ID;
    pIndexLocal++;
    size_t dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    dummy = encodeStringSize( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}


void test_MQTTV5_DeserializeConnackOnlyStatus( void )
{
    uint8_t buffer[ 50 ];
    uint8_t * pIndex = buffer;
    MQTTPropBuilder_t propBuffer = { 0 };

    memset( &properties, 0x00, sizeof( properties ) );

    status = MQTT_DeserializeAck( NULL, NULL, NULL, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    bool sessionPresent;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( NULL, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( &packetInfo, NULL, NULL, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    packetInfo.pRemainingData = pIndex;
    packetInfo.type = MQTT_PACKET_TYPE_CONNECT;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Incomplete connack received. */
    properties.maxPacketSize = 100;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 2;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /*Reserved bit incorrect*/
    packetInfo.remainingLength = 3;
    buffer[ 0 ] = 0x11;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /*
     * Session Present Bit is set but reason code is not equal to 0;
     */
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x01;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* 5 + 1 + 2 = 8 */
    size_t propertyLength = encodeRemainingLength( pIndex, 5 );
    packetInfo.remainingLength = propertyLength + 7;
    /*Not a valid reason code*/
    buffer[ 0 ] = 0x00;
    buffer[ 1 ] = 0x03;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
    /*All the valid response code*/
    buffer[ 1 ] = 0x80;
    buffer[ 2 ] = 0;
    properties.maxPacketSize = 100;
    packetInfo.remainingLength = 3;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x81;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x82;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x83;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x84;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x85;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x86;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x87;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x88;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x89;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x8A;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x8C;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x88;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x90;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x95;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x97;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x99;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9A;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9A;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9B;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9C;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9D;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9F;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    /*Exceeds the max packet size set by the client*/
    properties.maxPacketSize = 2;
    buffer[ 1 ] = 0x00;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /*Validate the remaining length*/
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    packetInfo.remainingLength = 7;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 20;
    pIndex = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    pIndex = &buffer[ 2 ];
    *pIndex = 0x81;
    pIndex++;
    *pIndex = 0x00;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTT_DeserializeAck( &packetInfo, NULL, NULL, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( &packetInfo, NULL, &sessionPresent, NULL, &propBuffer, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTTV5_DeserializeConnackOnlyuint_32( void )
{
    uint8_t buffer[ 200 ] = { 0 };
    bool session = false;
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t * pIndexLocal = initializeDeserialize( &packetInfo, buffer );
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );

    packetInfo.remainingLength = propertyLength + 12;
    properties.maxPacketSize = 150;
    pIndexLocal++;
    pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
    pIndexLocal = serializeuint_32( pIndexLocal, MQTT_MAX_PACKET_SIZE_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.serverMaxPacketSize );

    /*Test with NULL propBuffer. */
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Protocol error to include the same property twice*/
    packetInfo.remainingLength = 13;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 10 );
    pIndexLocal++;
    pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
    pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 7;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    pIndexLocal++;
    *pIndexLocal = MQTT_SESSION_EXPIRY_ID;
    pIndexLocal++;
    pIndexLocal = serializeuint_32( pIndexLocal, MQTT_SESSION_EXPIRY_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid id*/
    packetInfo.remainingLength = 8;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 5 );
    pIndexLocal++;
    pIndexLocal = serializeuint_32( pIndexLocal, 0x00 );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* Max packet size cannot have  a value 0*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 5 );
    packetInfo.remainingLength = propertyLength + 7;
    pIndexLocal++;
    *pIndexLocal = MQTT_MAX_PACKET_SIZE_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT32_BYTE3( 0 );
    pIndexLocal[ 1 ] = UINT32_BYTE2( 0 );
    pIndexLocal[ 2 ] = UINT32_BYTE1( 0 );
    pIndexLocal[ 3 ] = UINT32_BYTE0( 0 );
    pIndexLocal = &pIndexLocal[ 4 ];
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTTV5_DeserializeConnackOnlyuint_16( void )
{
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    MQTTPropBuilder_t propBuffer = { 0 };

    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;
    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    pIndexLocal = &buffer[ 2 ];
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 9 );
    packetInfo.remainingLength = propertyLength + 11;
    pIndexLocal++;
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_TOPIC_ALIAS_MAX_ID );
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_SERVER_KEEP_ALIVE_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverReceiveMax );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverTopicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverKeepAlive );

    /*Receive Max cannot have a value 0*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 3 );
    packetInfo.remainingLength = propertyLength + 5;
    pIndexLocal++;
    *pIndexLocal = MQTT_RECEIVE_MAX_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( 0 );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( 0 );
    pIndexLocal = &pIndexLocal[ 2 ];
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Protocol error to include the same property twice*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 6 );
    packetInfo.remainingLength = propertyLength + 8;
    pIndexLocal++;
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    pIndexLocal = serializeuint_16( pIndexLocal, MQTT_RECEIVE_MAX_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTTV5_DeserializeConnackOnlyuint_8( void )
{
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    MQTTPropBuilder_t propBuffer = { 0 };

    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;
    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 13;
    pIndexLocal = &buffer[ 2 ];
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );
    pIndexLocal++;
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_RETAIN_AVAILABLE_ID );
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_WILDCARD_ID );
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_SHARED_SUB_ID );
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_SUB_AVAILABLE_ID );

    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.serverMaxQos );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.retainAvailable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isWildcardAvailable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isSharedAvailable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isSubscriptionIdAvailable );

    /*Protocol error to have a value other than 0 or 1*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    *pIndexLocal = MQTT_MAX_QOS_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = 3;
    pIndexLocal++;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Protocol error to include the same property twice*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    packetInfo.remainingLength = propertyLength + 6;
    pIndexLocal++;
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
    pIndexLocal = serializeuint_8( pIndexLocal, MQTT_MAX_QOS_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 4;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 1 );
    pIndexLocal++;
    *pIndexLocal = MQTT_MAX_QOS_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = 0;
    pIndexLocal++;
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}


void test_MQTTV5_DeserializeConnackOnlyutf_8( void )
{
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    MQTTPropBuilder_t propBuffer;

    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;

    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    pIndexLocal = &buffer[ 2 ];
    properties.requestResponseInfo = 1;
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 28 );
    packetInfo.remainingLength = propertyLength + 28 + 2;
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_REASON_STRING_ID );
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_RESPONSE_INFO_ID );
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_SERVER_REF_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Protocol error to include the same property twice*/
    packetInfo.remainingLength = 17;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 14 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 7;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    packetInfo.remainingLength = propertyLength + 4;
    pIndexLocal++;
    serializeutf_8( pIndexLocal, MQTT_ASSIGNED_CLIENT_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Protocol error to include response information if is is set to false by client*/
    properties.requestResponseInfo = 0;
    packetInfo.remainingLength = 10;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 7 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_RESPONSE_INFO_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    packetInfo.remainingLength = 10;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 7 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_AUTH_METHOD_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 7 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8( pIndexLocal, MQTT_AUTH_DATA_ID );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}


void test_MQTTV5_DeserializeConnackOnlyUserProperty( void )
{
    uint8_t buffer[ 70000 ] = { 0 };
    uint8_t * pIndexLocal = buffer;

    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;
    MQTTPropBuilder_t propBuffer = { 0 };

    bool session = false;
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 16;
    pIndexLocal = &buffer[ 2 ];
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 13 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 6;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 3 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 8 );
    packetInfo.remainingLength = propertyLength + 10;
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 15;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 12 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Discard user property*/
    packetInfo.remainingLength = 65018;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 65013 );
    pIndexLocal += 3;
    uint32_t i = 0U;

    for( ; i < 5001; i++ )
    {
        pIndexLocal = serializeutf_8pair( pIndexLocal );
    }

    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    packetInfo.remainingLength = 65017;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 65012 );
    pIndexLocal += 3;

    for( ; i < 5001; i++ )
    {
        pIndexLocal = serializeutf_8pair( pIndexLocal );
    }

    status = MQTT_DeserializeAck( &packetInfo, NULL, &session, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}



void test_MQTTV5_GetConnectPacketSize( void )
{
    size_t remainingLength = 0;
    size_t packetSize = 0;


    /* Call MQTT_GetConnectPacketSize() with various combinations of
     * incorrect paramters */
    status = MQTT_GetConnectPacketSize( NULL, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, NULL, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify empty connect info fails. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify empty client identifier fails. */
    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = 0;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    connectInfo.pClientIdentifier = NULL;
    connectInfo.clientIdentifierLength = CLIENT_IDENTIFIER_LENGTH;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Test a will message payload length that is too large. */
    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = UINT16_MAX;
    connectInfo.pPassword = "";
    connectInfo.passwordLength = UINT16_MAX;
    connectInfo.pUserName = "";
    connectInfo.userNameLength = UINT16_MAX;
    publishInfo.pTopicName = TEST_TOPIC_NAME;
    publishInfo.topicNameLength = UINT16_MAX;
    /* A valid will message payload is less than the maximum 16 bit integer. */
    publishInfo.payloadLength = UINT16_MAX + 2;
    status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Verify good case */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.cleanSession = true;
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure remaining size returned is 17. */
    TEST_ASSERT_EQUAL_INT( 17, remainingLength );
    /* Make sure packet size is 19. */
    TEST_ASSERT_EQUAL_INT( 19, packetSize );

    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.currentIndex = MQTT_MAX_REMAINING_LENGTH;

    status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, &propBuffer, &propBuffer, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* With will. These parameters will cause the packet to be
     * 4 + 2 + 8 + 2 = 16 bytes larger. */
    publishInfo.pTopicName = "test";
    publishInfo.topicNameLength = 4;
    publishInfo.pPayload = "testload";
    publishInfo.payloadLength = 8;
    status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure remaining size returned is 32 = 16 + 16 + 2. */
    TEST_ASSERT_EQUAL_INT( 34, remainingLength );
    /* Make sure packet size is 34 = 18 + 16 + 2. */
    TEST_ASSERT_EQUAL_INT( 36, packetSize );

    /* With username and password. This will add 4 + 2 + 4 + 2 = 12 bytes. */
    connectInfo.cleanSession = true;
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 4;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 28 = 16 + 12. */
    TEST_ASSERT_EQUAL_INT( 29, remainingLength );
    /* Make sure packet size is 30 = 18 + 12. */
    TEST_ASSERT_EQUAL_INT( 31, packetSize );

    propBuffer.pBuffer = NULL;
    status = MQTT_GetConnectPacketSize( &connectInfo, &publishInfo, &propBuffer, &propBuffer, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    propBuffer.pBuffer = buf;
    propBuffer.currentIndex = MQTT_MAX_REMAINING_LENGTH + 1;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, &propBuffer, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

/**
 * @brief Tests that MQTT_SerializeConnect works as intended.
 */
void test_MQTT_SerializeConnect( void )
{
    MQTTConnectInfo_t connectInfo;
    MQTTPublishInfo_t willInfo = { 0 };
    size_t remainingLength = 0;
    uint8_t buffer[ 70 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };

    /* Verify bad parameter errors. */
    status = MQTT_SerializeConnect( NULL, &willInfo, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, NULL, remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, NULL, NULL, 120, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /* Create a good connection info. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;

    /* Inject a invalid fixed buffer test with a good connectInfo. */
    memset( &fixedBuffer, 0x0, sizeof( fixedBuffer ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Good case succeeds. */
    /* Set the fixedBuffer properly for the rest of the succeeding test. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];
    fixedBuffer.size = bufferSize;

    /* Calculate a good packet size. */
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Make sure test succeeds. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Encode user name. Also try clean session. */
    connectInfo.cleanSession = true;
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 4;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Serialize connect with LWT. */
    /* Test for NULL topic name. */
    ( void ) memset( &willInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    willInfo.retain = true;
    willInfo.qos = MQTTQoS1;
    willInfo.pPayload = "test";
    willInfo.payloadLength = ( uint16_t ) strlen( willInfo.pPayload );
    status = MQTT_GetConnectPacketSize( &connectInfo, &willInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Success. */
    ( void ) memset( &willInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    willInfo.retain = true;
    willInfo.qos = MQTTQoS1;
    willInfo.pTopicName = "test";
    willInfo.topicNameLength = ( uint16_t ) strlen( willInfo.pTopicName );
    willInfo.pPayload = "test";
    willInfo.payloadLength = ( uint16_t ) strlen( willInfo.pPayload );
    status = MQTT_GetConnectPacketSize( &connectInfo, &willInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Again with QoS 2 and 0. */

    willInfo.qos = MQTTQoS2;
    status = MQTT_GetConnectPacketSize( &connectInfo, &willInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    willInfo.qos = MQTTQoS0;
    willInfo.retain = false;
    /* NULL payload is acceptable. */
    willInfo.pPayload = NULL;
    willInfo.payloadLength = 0;
    status = MQTT_GetConnectPacketSize( &connectInfo, &willInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Success right on the buffer boundary. */
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    /* Throwing in a possible valid zero length password. */
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 0;
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, NULL, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );

    MQTTPropAdd_SessionExpiry( &propBuffer, 100 );
    status = MQTT_GetConnectPacketSize( &connectInfo, NULL, &propBuffer, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, NULL, &propBuffer, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Testing with will property */
    propBuffer.currentIndex = 0;
    MQTTPropAdd_PubMessageExpiry( &propBuffer, 100 );

    status = MQTT_GetConnectPacketSize( &connectInfo, &willInfo, NULL, &propBuffer, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, &propBuffer, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*Test with null buffer*/
    propBuffer.pBuffer = NULL;
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, NULL, &propBuffer, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    status = MQTT_SerializeConnect( &connectInfo, &willInfo, &propBuffer, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
}

void test_RemaininglengthLimit( void )
{
    /* Test will property length more than the max value allowed. */
    size_t remainingLength = 0;
    size_t packetSize = 0;
    uint32_t maxPacketSize = 100;
    MQTTStatus_t status = MQTTSuccess;

    publishInfo.topicNameLength = 0U;
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.currentIndex = MQTT_MAX_REMAINING_LENGTH; /* property length == max_remaining_length */

    status = MQTT_GetPublishPacketSize( &publishInfo, &propBuffer, &remainingLength, &packetSize, MQTT_MAX_REMAINING_LENGTH );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_GetDisconnectPacketSize( &propBuffer, &remainingLength, &packetSize, maxPacketSize, 0x00 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTTV5_ValidatePublishParams()
{
    uint16_t topicAlias = 10U;
    uint8_t maxQos = 0U;
    uint8_t retain = 0U;
    uint32_t maxPacketSize = 0U;

    /*Publish info cannot be null*/
    status = MQTT_ValidatePublishParams( NULL, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Retain is not allowed. */
    publishInfo.retain = true;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Qos invalid*/
    publishInfo.retain = false;
    publishInfo.qos = 1;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Valid parameters should return success*/
    publishInfo.qos = 1;
    maxQos = 1;
    publishInfo.retain = true;
    retain = 1;
    publishInfo.pTopicName = "abc";
    publishInfo.topicNameLength = 3;
    maxPacketSize = 10;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    publishInfo.pTopicName = NULL;
    publishInfo.topicNameLength = 0;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /*Invalid topic name and topic name length*/
    publishInfo.pTopicName = NULL;
    publishInfo.topicNameLength = 2;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Invalid maxPacket size*/
    publishInfo.pTopicName = "abc";
    publishInfo.topicNameLength = 3;
    maxPacketSize = 0;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    maxPacketSize = 100;
    topicAlias = 0;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    publishInfo.topicNameLength = 0;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, maxQos, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pTopicName = "abc";
    publishInfo.topicNameLength = 3;
    publishInfo.qos = 0;
    status = MQTT_ValidatePublishParams( &publishInfo, retain, 0, topicAlias, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
}

void test_MQTTV5_GetPublishPacketSize()
{
    size_t remainingLength = 0U;
    size_t packetSize = 0U;
    uint32_t maxPacketSize = 0U;

    setupPublishInfo( &publishInfo );
    /*Test with invalid paramters*/
    status = MQTT_GetPublishPacketSize( NULL, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, NULL, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, NULL, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Topic name invalid*/
    publishInfo.pTopicName = NULL;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pTopicName = TEST_TOPIC_NAME;

    /*Topic alias is not allowed and topic name is not provided.*/
    publishInfo.topicNameLength = 0;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    maxPacketSize = 100;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    /*Packet size too large*/
    publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH - TEST_TOPIC_NAME_LENGTH - 4;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Good case succeeds. */
    publishInfo.pPayload = "";
    publishInfo.payloadLength = 0;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /* Again with QoS 2. */
    publishInfo.qos = MQTTQoS2;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    publishInfo.retain = true;

    /*Valid properties*/
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 100 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );
    MQTTPropAdd_PubTopicAlias( &propBuffer, 1 );

    status = MQTT_GetPublishPacketSize( &publishInfo, &propBuffer, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /* No topic name*/
    publishInfo.topicNameLength = 0U;
    publishInfo.pTopicName = NULL;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    propBuffer.pBuffer = NULL;
    status = MQTT_GetPublishPacketSize( &publishInfo, &propBuffer, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    propBuffer.pBuffer = buf;

    /*Packet size is more than the server allowed max packet size*/
    maxPacketSize = 4;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, maxPacketSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

/**
 * @brief Tests that MQTT_SerializePublish works as intended.
 */
void test_MQTT_SerializePublish( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 98;
    uint8_t buffer[ 200 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    uint8_t expectedPacket[ 200 ];
    uint8_t * pIterator;

    const uint16_t PACKET_ID = 1;
    const char * longTopic = "/test/topic/name/longer/than/one/hundred/twenty/eight/characters" \
                             "/test/topic/name/longer/than/one/hundred/twenty/eight/characters";

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );
    publishInfo.pTopicName = "/test/topic";
    publishInfo.topicNameLength = sizeof( "/test/topic" );

    status = MQTT_SerializePublish( NULL,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify a NULL buffer in the fixed buffer struct fails */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the fixed buffer. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

    /* Verify that a non-zero payload length and a NULL payload fails. */
    publishInfo.payloadLength = 1;
    publishInfo.pPayload = NULL;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the payload length to valid for tests. */
    publishInfo.payloadLength = 0;

    /* Verify that 0 packet ID for QoS > 0 fails. */
    publishInfo.qos = MQTTQoS1;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    0,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify that a duplicate flag for Qos 0 fails. */
    publishInfo.qos = MQTTQoS0;
    publishInfo.dup = true;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the previous flags for other tests. */
    publishInfo.qos = MQTTQoS1;
    publishInfo.dup = false;

    /* Empty topic fails. */
    publishInfo.pTopicName = NULL;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    publishInfo.pTopicName = TEST_TOPIC_NAME;
    publishInfo.topicNameLength = 0;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Remaining length larger than buffer size. */
    publishInfo.pTopicName = TEST_TOPIC_NAME;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    fixedBuffer.size = 5;
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    10,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /* Good case succeeds */
    publishInfo.qos = MQTTQoS0;
    publishInfo.pTopicName = "/test/topic";
    publishInfo.topicNameLength = sizeof( "/test/topic" );
    fixedBuffer.size = bufferSize;
    /* Calculate exact packet size and remaining length. */
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* MQTT PUBLISH packet format:
     * 0x30 | publish flags (dup, qos, retain) (1 byte)
     * Remaining length (1-4 bytes)
     * Topic name length (2 bytes)
     * Topic name (variable)
     * Packet ID (if QoS > 0) (1 byte)
     * Properties length (1-4 bytes) - 1 in this case
     * Payload (>= 0 bytes) */
    expectedPacket[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    expectedPacket[ 1 ] = remainingLength;
    ( void ) encodeString( &expectedPacket[ 2 ], publishInfo.pTopicName, publishInfo.topicNameLength );
    expectedPacket[ 16 ] = 0;
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    /* Again with QoS2, dup, and retain. Also encode remaining length > 2 bytes. */
    publishInfo.qos = MQTTQoS2;
    publishInfo.retain = true;
    publishInfo.dup = true;
    publishInfo.pTopicName = longTopic;
    publishInfo.topicNameLength = strlen( longTopic );
    publishInfo.pPayload = MQTT_SAMPLE_PAYLOAD;
    publishInfo.payloadLength = MQTT_SAMPLE_PAYLOAD_LEN;
    memset( buffer, 0x00, bufferSize );
    /* Calculate exact packet size and remaining length. */
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublish( &publishInfo,
                                    NULL,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    memset( expectedPacket, 0x00, sizeof( expectedPacket ) );
    pIterator = expectedPacket;
    /* Set the flags as follows: Dup = 0x8, QoS2 = 0x4, Retain = 0x1. 8 | 4 | 1 = 0xD. */
    *pIterator++ = MQTT_PACKET_TYPE_PUBLISH | 0xD;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    pIterator += encodeString( pIterator, publishInfo.pTopicName, publishInfo.topicNameLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0;
    ( void ) memcpy( pIterator, publishInfo.pPayload, publishInfo.payloadLength );
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );
}

/* ========================================================================== */

void test_MQTTV5_DeserializeAck_puback( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTReasonCodeInfo_t ackInfo;
    MQTTPropBuilder_t propBuffer = { 0 };
    uint16_t packetIdentifier;
    uint32_t maxPacketSize = 0U;
    bool requestProblem = false;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndex = buffer;
    size_t dummy;

    /* Verify parameters */
    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    memset( &properties, 0x00, sizeof( properties ) );

    properties.requestProblemInfo = requestProblem;
    properties.maxPacketSize = maxPacketSize;

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
    status = MQTT_DeserializeAck( NULL, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( &mqttPacketInfo, NULL, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Remaining data cannot be NULL.*/
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size cannot be 0*/
    mqttPacketInfo.pRemainingData = buffer;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    maxPacketSize = 200U;
    properties.maxPacketSize = maxPacketSize;
    /* Packet identifier 0 is not valid (per spec). */
    buffer[ 0 ] = 0;
    buffer[ 1 ] = 0;
    mqttPacketInfo.remainingLength = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


    /*Remaining length connot be less than 2*/
    mqttPacketInfo.remainingLength = 1;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Packet size greater than allowed.*/
    mqttPacketInfo.remainingLength = 1000U;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* Process a valid PUBACK. */
    mqttPacketInfo.remainingLength = 2;
    buffer[ 1 ] = 1;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );

    mqttPacketInfo.remainingLength = 3;
    buffer[ 2 ] = 0x00;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );

    /*Property length should be zero when request problem is set to false*/
    mqttPacketInfo.remainingLength = 24;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    requestProblem = true;
    properties.requestProblemInfo = requestProblem;
    /*User properties not initialized.*/
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Valid parameters.*/
    pIndex = &buffer[ 3 ];
    dummy = encodeRemainingLength( pIndex, 20 );
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8pair( pIndex );
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*With NULL prop builder. */
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid property id*/
    pIndex = &buffer[ 3 ];
    dummy = encodeRemainingLength( pIndex, 7 );
    mqttPacketInfo.remainingLength = dummy + 7 + 3;
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid remaining length*/
    pIndex = &buffer[ 3 ];
    dummy = encodeRemainingLength( pIndex, 12 );
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


    /*Invalid property length*/
    pIndex = &buffer[ 3 ];
    dummy = encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTTV5_DeserializeAck_LogPuback()
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTReasonCodeInfo_t ackInfo;
    uint16_t packetIdentifier;
    uint32_t maxPacketSize = 10U;
    bool requestProblem = false;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 4 ] = { 0 };

    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    memset( &properties, 0x00, sizeof( properties ) );
    properties.requestProblemInfo = requestProblem;
    properties.maxPacketSize = maxPacketSize;

    MQTTPropBuilder_t propBuffer = { 0 };
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
    mqttPacketInfo.remainingLength = 3;
    /*Validate all the correct reason codes.*/
    buffer[ 1 ] = 1;
    buffer[ 2 ] = MQTT_REASON_PUBACK_SUCCESS;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_UNSPECIFIED_ERROR;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_NOT_AUTHORIZED;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_TOPIC_NAME_INVALID;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_QUOTA_EXCEEDED;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[ 2 ] = MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    /*Invalid reason code.*/
    buffer[ 2 ] = MQTT_REASON_CONNACK_BANNED;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_MQTTV5_DeserializeAck_Pubrel()
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTReasonCodeInfo_t ackInfo;
    uint32_t maxPacketSize = 10U;
    uint16_t packetIdentifier;
    bool requestProblem = false;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuffer;
    uint8_t buffer[ 4 ] = { 0 };

    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    memset( &properties, 0x00, sizeof( properties ) );
    properties.requestProblemInfo = requestProblem;
    properties.maxPacketSize = maxPacketSize;

    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBREL;
    mqttPacketInfo.remainingLength = 3;
    /*Validate all the correct reason codes.*/
    buffer[ 1 ] = 1;
    buffer[ 2 ] = MQTT_REASON_PUBREL_SUCCESS;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[ 2 ] = MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    /*Invalid reason code.*/
    buffer[ 2 ] = MQTT_REASON_CONNACK_BANNED;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid reason code.*/
    buffer[ 2 ] = MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid packet id*/
    buffer[ 1 ] = 0;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid packet type. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_DISCONNECT;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBREL;
    buffer[ 1 ] = 1; /* Reset packet identifier to a valid value. */
    mqttPacketInfo.remainingLength = 2;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &ackInfo, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}


void test_MQTTV5_GetAckPacketSize()
{
    MQTTStatus_t status;
    size_t remainingLength;
    size_t packetSize;
    uint32_t maxPacketSize = 0U;

    /*Invalid parameters*/
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetAckPacketSize( NULL, &packetSize, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetAckPacketSize( &remainingLength, NULL, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size cannot be 0*/
    status = MQTT_GetAckPacketSize( &remainingLength, NULL, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Valid parameters*/
    maxPacketSize = UINT32_MAX;
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*With properties*/
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Packet size greater than max allowed.*/
    maxPacketSize = 2;
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size cannot be 0*/
    maxPacketSize = 0;
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    maxPacketSize = 500;
    status = MQTT_GetAckPacketSize( &remainingLength, &packetSize, maxPacketSize, MQTT_MAX_REMAINING_LENGTH );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}


void test_MQTTV5_GetDisconnectPacketSize()
{
    size_t remainingLength;
    size_t packetSize;
    uint32_t maxPacketSize = 0U;
    MQTTStatus_t status;

    /*Invalid arguments*/
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


    status = MQTT_GetDisconnectPacketSize( NULL, NULL, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, NULL, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size cannot be 0.*/
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Invalid Reason code. */
    maxPacketSize = 60U;
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_SUBACK_GRANTED_QOS1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Valid parameters*/
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Valid parameters*/
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid reason code. */
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_SERVER_BUSY );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size lesser than packet size */
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );
    propBuffer.currentIndex = 10;
    status = MQTT_GetDisconnectPacketSize( &propBuffer, &remainingLength, &packetSize, 6, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


    propBuffer.currentIndex = MQTT_MAX_REMAINING_LENGTH; /* Other fields do not have to be set as we only testing if the length of properties == max_remaining_length */
    status = MQTT_GetDisconnectPacketSize( &propBuffer, &remainingLength, &packetSize, maxPacketSize, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    propBuffer.pBuffer = NULL;
    status = MQTT_GetDisconnectPacketSize( &propBuffer, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}



void test_MQTTV5_DeserializeDisconnect()
{
    MQTTReasonCodeInfo_t disconnectInfo;
    size_t dummy;
    int32_t maxPacketSize = 0U;
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndex = buffer;
    MQTTPropBuilder_t propBuffer = { 0 };

    memset( &disconnectInfo, 0x0, sizeof( disconnectInfo ) );
    /*Invalid parameters*/
    status = MQTT_DeserializeDisconnect( NULL, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Remaining data not initialized.*/
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    packetInfo.pRemainingData = buffer;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, NULL, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Max packet size cannot be 0.*/
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    maxPacketSize = 100;

    /*Remaining length can be 0*/
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Remaining Length invalid. */
    packetInfo.remainingLength = 200;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );


    /*Invalid reason code.*/
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    packetInfo.remainingLength = 1;
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_NOT_AUTHORIZED;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Property length is 0.*/
    packetInfo.remainingLength = 2;
    pIndex = &buffer[ 1 ];
    dummy = encodeRemainingLength( pIndex, 0 );
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*With properties*/
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;
    pIndex = &buffer[ 1 ];
    packetInfo.remainingLength = 29;
    dummy = encodeRemainingLength( pIndex, 27 );
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeutf_8( pIndex, MQTT_SERVER_REF_ID );
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid reason code for incoming DISCONNECT packet. */
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;

    /*Invalid property id.*/
    pIndex = &buffer[ 1 ];
    packetInfo.remainingLength = 9;
    dummy = encodeRemainingLength( pIndex, 7 );
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_SESSION_EXPIRY_ID );
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
    TEST_ASSERT_EQUAL_INT( 1, dummy );

    /*Invalid property length.*/
    pIndex = &buffer[ 1 ];
    packetInfo.remainingLength = 9;
    dummy = encodeRemainingLength( pIndex, 4 );
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length.*/
    buffer[ 1 ] = 0x81;
    buffer[ 2 ] = 0x00;
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, &propBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Property length is 0. */
    buffer[ 0 ] = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;
    pIndex = &buffer[ 1 ];
    packetInfo.remainingLength = 2;
    dummy = encodeRemainingLength( pIndex, 0 );
    status = MQTT_DeserializeDisconnect( &packetInfo, maxPacketSize, &disconnectInfo, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_MQTT_GetIncomingPacketTypeAndLength( void )
{
    MQTTPacketInfo_t mqttPacket;
    NetworkContext_t networkContext;
    uint8_t buffer[ 10 ];
    uint8_t * bufPtr = buffer;

    /* Dummy network context - pointer to pointer to a buffer. */
    networkContext.buffer = &bufPtr;
    /* Check when network receive fails. */
    memset( buffer, 0x00, 10 );
    /* Branch coverage for Disconnect. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_DISCONNECT;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
}

void test_MQTTV5_GetSubscribePacketSize( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTSubscribeInfo_t subscribeInfo;
    size_t remainingLength = 0;
    size_t packetSize = 0;

    /** Verify Parameters */

    /** NULL parameters */
    status = MQTT_GetSubscribePacketSize( NULL, 1, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetSubscribePacketSize( &subscribeInfo, 0, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    subscribeInfo.topicFilterLength = 13;
    subscribeInfo.pTopicFilter = "example/topic";

    /*Invalid max packet size*/
    status = MQTT_GetSubscribePacketSize( &subscribeInfo, 1, NULL, &remainingLength, &packetSize, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

void test_MQTTV5_GetSubscribePacketSize_HappyPath( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTSubscribeInfo_t subscribeInfo;
    size_t remainingLength = 0;
    size_t packetSize = 0;

    subscribeInfo.pTopicFilter = TEST_TOPIC_NAME;
    subscribeInfo.topicFilterLength = TEST_TOPIC_NAME_LENGTH;
    subscribeInfo.qos = MQTTQoS0;
    subscribeInfo.noLocalOption = 0;
    subscribeInfo.retainAsPublishedOption = 0;
    subscribeInfo.retainHandlingOption = 1;

    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = 50;
    propBuffer.currentIndex = 2;

    status = MQTT_GetSubscribePacketSize( &subscribeInfo, 1, &propBuffer, &remainingLength, &packetSize, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_UINT32( 19U, remainingLength );
    TEST_ASSERT_EQUAL_UINT32( 21U, packetSize );

    /* Test function with null buffer. */
    propBuffer.pBuffer = NULL;
    status = MQTT_GetSubscribePacketSize( &subscribeInfo, 1, &propBuffer, &remainingLength, &packetSize, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

/** Subscribe Packet size with multiple subscriptions and User Properties */
void test_MQTTV5_GetSubscribePacketSize_MultipleSubscriptions( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTSubscribeInfo_t subscribeInfo[ 2 ];
    size_t remainingLength = 0;
    size_t packetSize = 0;

    subscribeInfo[ 0 ].pTopicFilter = TEST_TOPIC_NAME;
    subscribeInfo[ 0 ].topicFilterLength = TEST_TOPIC_NAME_LENGTH;
    subscribeInfo[ 0 ].qos = MQTTQoS0;
    subscribeInfo[ 0 ].noLocalOption = 0;
    subscribeInfo[ 0 ].retainAsPublishedOption = 0;
    subscribeInfo[ 0 ].retainHandlingOption = 1;

    subscribeInfo[ 1 ].pTopicFilter = TEST_TOPIC_NAME;
    subscribeInfo[ 1 ].topicFilterLength = TEST_TOPIC_NAME_LENGTH;
    subscribeInfo[ 1 ].qos = MQTTQoS0;
    subscribeInfo[ 1 ].noLocalOption = 0;
    subscribeInfo[ 1 ].retainAsPublishedOption = 0;
    subscribeInfo[ 1 ].retainHandlingOption = 1;

    status = MQTT_GetSubscribePacketSize( subscribeInfo, 2, NULL, &remainingLength, &packetSize, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_calculateSubscriptionPacketSizeV5( void )
{
    size_t subscriptionCount = 1;
    size_t remainingLength = 0;
    size_t packetSize = 0;
    MQTTStatus_t status = MQTTSuccess;
    MQTTSubscribeInfo_t fourThousandSubscriptions[ 4096 ] = { 0 };
    int i;

    for( i = 0; i < 4096; i++ )
    {
        fourThousandSubscriptions[ i ].topicFilterLength = UINT16_MAX;

        /* We need to set this to avoid an early bad parameter, however we do
         * not need a 65535 byte buffer as the packet will not be serialized. */
        fourThousandSubscriptions[ i ].pTopicFilter = "";
    }

    subscriptionCount = sizeof( fourThousandSubscriptions ) / sizeof( fourThousandSubscriptions[ 0 ] );
    status = MQTT_GetSubscribePacketSize( fourThousandSubscriptions,
                                          subscriptionCount,
                                          NULL,
                                          &remainingLength,
                                          &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

/**
 * @brief Tests that MQTT_SerializeSubscribe works as intended.
 */
void test_MQTT_SerializeSubscribe( void )
{
    MQTTSubscribeInfo_t subscriptionList;
    size_t subscriptionCount = 1;
    size_t remainingLength = 0;
    uint8_t buffer[ 25 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    uint8_t expectedPacket[ 100 ];
    uint8_t * pIterator = expectedPacket;

    const uint16_t PACKET_ID = 1;

    /* Verify bad parameters fail. */
    status = MQTT_SerializeSubscribe( NULL,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      0,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify a NULL buffer in the fixed buffer struct fails */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the fixed buffer. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

    /* Get correct values of packet size and remaining length. */
    memset( &subscriptionList, 0x0, sizeof( subscriptionList ) );
    subscriptionList.qos = MQTTQoS0;
    subscriptionList.pTopicFilter = "/example/topic";
    subscriptionList.topicFilterLength = sizeof( "/example/topic" );
    subscriptionList.noLocalOption = 1;
    subscriptionList.retainAsPublishedOption = 1;
    subscriptionList.retainHandlingOption = 1;
    uint32_t maxPacketSize = MQTT_MAX_PACKET_SIZE;
    subscriptionCount = sizeof( subscriptionList ) / sizeof( MQTTSubscribeInfo_t );
    status = MQTT_GetSubscribePacketSize( &subscriptionList,
                                          subscriptionCount,
                                          NULL,
                                          &remainingLength,
                                          &packetSize,
                                          maxPacketSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    /* Make sure subscription count of zero fails. */
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      0,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Test if buffer is too small. */
    fixedBuffer.size = 1;
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );
    fixedBuffer.size = bufferSize;

    /* Make sure success is returned for good case. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* MQTT SUBSCRIBE packet format:
     * 0x82 (1 byte)
     * Remaining length (1-4 bytes)
     * Packet ID (2 bytes)
     * Topic filters (series of 2 byte lengths followed by filter, then QoS) (variable) */
    /* uint8_t subscriptionOptions = 0 ; */
    /* subscriptionOptions = subscriptionOptions & */
    *pIterator++ = MQTT_PACKET_TYPE_SUBSCRIBE;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0; /* Length of properties = 0 */
    pIterator += encodeString( pIterator, subscriptionList.pTopicFilter, subscriptionList.topicFilterLength );
    *pIterator++ = 0x1C;
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    padAndResetBuffer( buffer, sizeof( buffer ) );
    subscriptionList.noLocalOption = 0;
    subscriptionList.retainAsPublishedOption = 0;
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      NULL,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    pIterator = expectedPacket;
    *pIterator++ = MQTT_PACKET_TYPE_SUBSCRIBE;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0; /* Length of properties = 0 */
    pIterator += encodeString( pIterator, subscriptionList.pTopicFilter, subscriptionList.topicFilterLength );
    *pIterator++ = 0x10;
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    /* Serialize subscribe with properties */
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );

    MQTTPropAdd_SubscribeId( &propBuffer, 10 );
    status = MQTT_GetSubscribePacketSize( &subscriptionList,
                                          subscriptionCount,
                                          &propBuffer,
                                          &remainingLength,
                                          &packetSize,
                                          maxPacketSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    fixedBuffer.size = bufferSize;

    /* Make sure success is returned for good case. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      &propBuffer,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*test SerializeSubscribe with NULL property buffer. */
    propBuffer.pBuffer = NULL;
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      &propBuffer,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );

    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*test MQTT_SerializeSubscribe subscription options. */
    subscriptionList.qos = MQTTQoS1;
    subscriptionList.pTopicFilter = "/example/topic";
    subscriptionList.topicFilterLength = sizeof( "/example/topic" );
    subscriptionList.noLocalOption = 1;
    subscriptionList.retainAsPublishedOption = 1;
    subscriptionList.retainHandlingOption = retainSendOnSub;

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      &propBuffer,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    subscriptionList.qos = MQTTQoS2;
    subscriptionList.retainHandlingOption = retainDoNotSendonSub;

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeSubscribe( &subscriptionList,
                                      subscriptionCount,
                                      &propBuffer,
                                      PACKET_ID,
                                      remainingLength,
                                      &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

/**
 * @brief Tests that MQTT_SerializeUnsubscribe works as intended.
 */
void test_MQTT_SerializeUnsubscribe( void )
{
    MQTTSubscribeInfo_t subscriptionList;
    size_t subscriptionCount = 1;
    size_t remainingLength = 0;
    uint8_t buffer[ 33 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    uint8_t expectedPacket[ 100 ];
    uint8_t * pIterator = expectedPacket;

    const uint16_t PACKET_ID = 1;

    status = MQTT_SerializeUnsubscribe( NULL,
                                        subscriptionCount,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        NULL,
                                        0,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify a NULL buffer in the fixed buffer struct fails */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the fixed buffer. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

    /* Get correct values of packetsize and remaining length. */
    memset( &subscriptionList, 0x0, sizeof( subscriptionList ) );
    subscriptionList.qos = MQTTQoS0;
    subscriptionList.pTopicFilter = "/example/topic";
    subscriptionList.topicFilterLength = sizeof( "/example/topic" );
    subscriptionCount = sizeof( subscriptionList ) / sizeof( MQTTSubscribeInfo_t );
    status = MQTT_GetUnsubscribePacketSize( &subscriptionList,
                                            subscriptionCount,
                                            NULL,
                                            &remainingLength,
                                            &packetSize,
                                            MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    /* Make sure subscription count of zero fails. */
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        0,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Test if buffer is too small. */
    fixedBuffer.size = 1;
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );
    fixedBuffer.size = bufferSize;

    /* Make sure success it returned for good case. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        NULL,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* MQTT UNSUBSCRIBE packet format:
     * 0xA2 (1 byte)
     * Remaining length (1-4 bytes)
     * Packet ID (2 bytes)
     * Topic filters (series of 2 byte lengths followed by filter) (variable) */
    *pIterator++ = MQTT_PACKET_TYPE_UNSUBSCRIBE;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0;
    pIterator += encodeString( pIterator, subscriptionList.pTopicFilter, subscriptionList.topicFilterLength );
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );

    MQTTUserProperty_t userProp;
    userProp.pKey = "abc";
    userProp.pValue = "def";
    userProp.keyLength = 3;
    userProp.valueLength = 3;
    MQTTPropAdd_UserProp( &propBuffer, &userProp );

    status = MQTT_GetUnsubscribePacketSize( &subscriptionList,
                                            subscriptionCount,
                                            &propBuffer,
                                            &remainingLength,
                                            &packetSize,
                                            MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        &propBuffer,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*Test with null property buffer. */
    propBuffer.pBuffer = NULL;
    status = MQTT_SerializeUnsubscribe( &subscriptionList,
                                        subscriptionCount,
                                        &propBuffer,
                                        PACKET_ID,
                                        remainingLength,
                                        &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
}
/* ========================================================================== */

void test_MQTTV5_suback( void )
{
    MQTTStatus_t status;
    uint8_t * pIndex;

    memset( &properties, 0x00, sizeof( properties ) );
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;
    uint8_t packetBuffer[ 23 ] =
    {
        0x90,             /* Fixed header: SUBACK type (0x90) */
        0x14,             /* Remaining Length = 20 bytes */
        0x00, 0x01,       /* Packet Identifier = 1 */
        0x11,             /* Property Length = 17 bytes */
        0x1F,             /* Property ID = 0x1F (Reason String) */
        0x00, 0x03,       /* UTF-8 string length = 3 */
        0x61, 0x62, 0x63, /* The string "abc" */
        0x26, 0x00, 0x03, 0x61, 0x62, 0x63, 0x00, 0x03, 0x61, 0x62, 0x63,
        0x00              /* Payload: Reason code = 0x00 (Success) */
    };

    MQTTPacketInfo_t subackPacket;

    memset( &subackPacket, 0, sizeof( subackPacket ) );
    subackPacket.type = MQTT_PACKET_TYPE_SUBACK; /* Should be defined as 0x90 */
    subackPacket.remainingLength = 21;           /* From the fixed header (0x0A) */
    subackPacket.headerLength = 2;               /* Fixed header size in this example */
    subackPacket.pRemainingData = &packetBuffer[ 2 ];
    uint16_t packetIdentifier = 0;
    MQTTReasonCodeInfo_t subackReasonCodes;
    MQTTPropBuilder_t propBuffer = { 0 };
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    packetBuffer[ 11 ] = 0x00;
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, NULL, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( NULL, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_DeserializeAck( &subackPacket, NULL, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    uint8_t packetBufferNoProperties[ 7 ] =
    {
        0x90,       /* Fixed header: SUBACK type (0x90) */
        4,          /* Remaining Length = 4 bytes */
        0x00, 0x01, /* Packet Identifier = 1 */
        0x00,       /* Property Length = 1 byte */
        0x00,       /* Payload: Reason code = 0x00 (Success) */
        0x00
    };

    memset( &subackPacket, 0, sizeof( subackPacket ) );
    subackPacket.type = MQTT_PACKET_TYPE_SUBACK; /* Should be defined as 0x90 */
    subackPacket.remainingLength = 4;
    subackPacket.headerLength = 2;
    subackPacket.pRemainingData = &packetBufferNoProperties[ 2 ];
    packetIdentifier = 0;
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    packetBufferNoProperties[ 5 ] = 0x80; /* Change reason code to 0x01 (Unspecified error) */
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    /*Invalid Property Length. */
    subackPacket.remainingLength = 20;
    pIndex = &packetBufferNoProperties[ 4 ];
    encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    subackPacket.remainingLength = 4;
    packetBufferNoProperties[ 4 ] = 0;    /* Set property length to 0 bytes */
    packetBufferNoProperties[ 5 ] = 0x00; /* Set reason code to 0x00 (Success) */
    status = MQTT_DeserializeAck( &subackPacket, &packetIdentifier, NULL, &subackReasonCodes, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_MQTTV5_GetUnsubscribePacketSize( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTSubscribeInfo_t subscribeInfo = { 0 };
    size_t remainingLength = 0;
    size_t packetSize = 0;

    status = MQTT_GetUnsubscribePacketSize( NULL, 1, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetUnsubscribePacketSize( &subscribeInfo, 1, NULL, NULL, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_GetUnsubscribePacketSize( &subscribeInfo, 1, NULL, &remainingLength, NULL, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    subscribeInfo.pTopicFilter = TEST_TOPIC_NAME;
    subscribeInfo.topicFilterLength = TEST_TOPIC_NAME_LENGTH;

    status = MQTT_GetUnsubscribePacketSize( &subscribeInfo, 1, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_UINT32( 16U, remainingLength );
    TEST_ASSERT_EQUAL_UINT32( 18U, packetSize );

    status = MQTT_GetUnsubscribePacketSize( &subscribeInfo, 0, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    MQTTPropBuilder_t propBuffer;
    propBuffer.pBuffer = NULL;
    status = MQTT_GetUnsubscribePacketSize( &subscribeInfo, 1, &propBuffer, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}
void test_MQTTV5_DeserializeSuback( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    uint16_t packetIdentifier;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buffer[ 50 ] = { 0 };

    memset( &properties, 0x00, sizeof( properties ) );
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;

    /* Bad remaining length. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_SUBACK;
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.remainingLength = 14;
    /* Set packet identifier. */
    buffer[ 0 ] = 0;
    buffer[ 1 ] = 1;
    buffer[ 2 ] = 0;
    buffer[ 3 ] = 0x01;
    buffer[ 4 ] = 0x02;
    buffer[ 5 ] = 0x80;
    buffer[ 6 ] = 0x83;
    buffer[ 7 ] = 0x87;
    buffer[ 8 ] = 0x8F;
    buffer[ 9 ] = 0x91;
    buffer[ 10 ] = 0x97;
    buffer[ 11 ] = 0x9E;
    buffer[ 12 ] = 0xA1;
    buffer[ 13 ] = 0xA2;

    MQTTReasonCodeInfo_t subackReasonCodes;

    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[ 13 ] = 0xA4;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Max Packet Size lesser than suback packet size*/
    properties.maxPacketSize = 1U;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;

    /*Invalid Remaining Length*/
    mqttPacketInfo.remainingLength = 2;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
    mqttPacketInfo.remainingLength = 14;

    /*Invalid packet type*/
    buffer[ 1 ] = 0;
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    buffer[ 1 ] = 1;
    mqttPacketInfo.remainingLength = 17;
    buffer[ 2 ] = 14;
    uint8_t * pIndex = &buffer[ 3 ];
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    status = MQTT_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, &subackReasonCodes, &propBuffer, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}


void test_incoming_publish2( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    uint16_t packetIdentifier = 1;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndex = NULL;

    buffer[ 0 ] = 0x00;
    buffer[ 1 ] = 0x04;
    buffer[ 2 ] = 't', buffer[ 3 ] = 'e', buffer[ 4 ] = 's', buffer[ 5 ] = 't';
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    mqttPacketInfo.pRemainingData = buffer;
    pIndex = &buffer[ 6 ];

    size_t propertyLength = encodeRemainingLength( pIndex, 46 );
    mqttPacketInfo.remainingLength = 52 + propertyLength;
    pIndex++;
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_TOPIC_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CONTENT_TYPE_ID );
    pIndex = serializeutf_8pair( pIndex );
    *pIndex++ = MQTT_SUBSCRIPTION_ID_ID;

    MQTTPublishInfo_t publishIn;
    ( void ) memset( &publishIn, 0x0, sizeof( publishIn ) );

    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Test with NULL Property Builder. */
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, NULL, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid property length*/
    buffer[ 6 ] = 100;
    mqttPacketInfo.remainingLength = 46;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Only packet ID present*/
    mqttPacketInfo.type = ( MQTT_PACKET_TYPE_PUBLISH | 0x04 );
    mqttPacketInfo.remainingLength = 9;
    buffer[ 6 ] = 0x00, buffer[ 7 ] = 0x01, buffer[ 8 ] = 0x00;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*A property is received twice. */
    buffer[ 6 ] = 12;
    pIndex = &buffer[ 7 ];
    mqttPacketInfo.remainingLength = 21;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_TOPIC_ID );
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_TOPIC_ID );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*A property is received twice. */
    pIndex = &buffer[ 7 ];
    pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*A property is received twice. */
    pIndex = &buffer[ 7 ];
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*A property is received twice. */
    pIndex = &buffer[ 7 ];
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*A property is received twice. */
    pIndex = &buffer[ 7 ];
    buffer[ 6 ] = 10;
    mqttPacketInfo.remainingLength = 17;
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid topic alias. */
    buffer[ 6 ] = 3;
    pIndex = &buffer[ 7 ];
    mqttPacketInfo.remainingLength = 10;
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    uint16_t topicAliasMax = 1;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, topicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property type. */
    buffer[ 6 ] = 5;
    pIndex = &buffer[ 7 ];
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );
    mqttPacketInfo.remainingLength = 12;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, topicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Test Incoming Publish with Payload. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    buffer[ 6 ] = 0x00;
    buffer[ 7 ] = 0x01;
    mqttPacketInfo.remainingLength = 8;

    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, NULL, 100, topicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid Property Length. */
    pIndex = &buffer[ 6 ];
    mqttPacketInfo.remainingLength = 20;
    propertyLength = encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, NULL, 100, topicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    buffer[ 6 ] = 10;
    buffer[ 7 ] = MQTT_SUBSCRIPTION_ID_ID;
    pIndex = &buffer[ 8 ];
    encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, topicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_incoming_publish_withPacketId( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    uint16_t packetIdentifier = 1;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 8 ] = { 0 };
    MQTTPropBuilder_t propBuffer = { 0 };

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x2;
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.remainingLength = 8;

    buffer[ 0 ] = 0x00;
    buffer[ 1 ] = 0x03;
    buffer[ 2 ] = 0x61;
    buffer[ 3 ] = 0x62;
    buffer[ 4 ] = 0x63;
    buffer[ 5 ] = 0x00;
    buffer[ 6 ] = 0x01;
    buffer[ 7 ] = 0x00;

    MQTTPublishInfo_t publishIn;
    ( void ) memset( &publishIn, 0x0, sizeof( publishIn ) );

    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[ 6 ] = 0x00;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_Invalid_IncomingPublish( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    uint16_t packetIdentifier = 1;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 3 ] = { 0 };
    MQTTPropBuilder_t propBuffer = { 0 };

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.remainingLength = 3;

    buffer[ 0 ] = 0x00;
    buffer[ 1 ] = 0x01;
    buffer[ 2 ] = 0x61;


    MQTTPublishInfo_t publishIn;
    ( void ) memset( &publishIn, 0x0, sizeof( publishIn ) );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x02;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishIn, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

/**
 * @brief Tests that MQTT_SerializeDisconnect works as intended.
 */
void test_MQTT_SerializeDisconnect( void )
{
    uint8_t buffer[ 25 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    uint8_t expectedPacket[ 10 ] = { 0 };
    uint8_t * pIterator = expectedPacket;
    MQTTStatus_t status = MQTTSuccess;
    size_t remainingLength = 0;

    /* Buffer size less than disconnect request fails. */
    fixedBuffer.size = 1;
    status = MQTT_SerializeDisconnect( NULL, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /* NULL buffer fails. */
    status = MQTT_SerializeDisconnect( NULL, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the fixed buffer. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];
    fixedBuffer.size = bufferSize;
    status = MQTT_GetDisconnectPacketSize( NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Good case succeeds. */
    status = MQTT_SerializeDisconnect( NULL, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    *pIterator++ = MQTT_PACKET_TYPE_DISCONNECT;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    *pIterator++ = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;
    *pIterator = 0; /*Property length is 0 */
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    /*Test with properties. */

    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );

    MQTTPropAdd_SessionExpiry( &propBuffer, 10 );
    status = MQTT_GetDisconnectPacketSize( &propBuffer, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );

    status = MQTT_SerializeDisconnect( &propBuffer, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    pIterator = expectedPacket;
    *pIterator++ = MQTT_PACKET_TYPE_DISCONNECT;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    *pIterator++ = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;
    *pIterator++ = 5;    /*Property length is 2 */
    *pIterator++ = 0x11; /*Session Expiry ID*/
    pIterator += 3;
    *pIterator = 10;     /*Session Expiry value */
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );

    /* test with null property buffer. */
    propBuffer.pBuffer = NULL;
    status = MQTT_SerializeDisconnect( &propBuffer, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /* pFixedBuffer->pBuffer is NULL */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializeDisconnect( &propBuffer, MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

/**
 * @brief Tests that MQTT_GetPingreqPacketSize works as intended.
 */
void test_MQTT_GetPingreqPacketSize( void )
{
    MQTTStatus_t status;
    size_t packetSize;

    /* Verify parameters. */
    status = MQTT_GetPingreqPacketSize( NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Good case succeeds. A PINGREQ is 2 bytes. */
    status = MQTT_GetPingreqPacketSize( &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 2, packetSize );
}

/**
 * @brief Tests that MQTT_SerializePingreq works as intended.
 */
void test_MQTT_SerializePingreq( void )
{
    uint8_t buffer[ 10 + 2 * BUFFER_PADDING_LENGTH ];
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ] };
    uint8_t expectedPacket[ 2 ] = { MQTT_PACKET_TYPE_PINGREQ, 0 };
    MQTTStatus_t status = MQTTSuccess;

    /* Buffer size less than ping request fails. */
    fixedBuffer.size = 1;
    status = MQTT_SerializePingreq( &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /* NULL buffer fails. */
    status = MQTT_SerializePingreq( NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify a NULL buffer in the fixed buffer struct fails */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializePingreq( &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the fixed buffer. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

    /* Good case succeeds. */
    fixedBuffer.size = 2;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePingreq( &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], 2 );
}

/**
 * @brief Tests that MQTT_DeserializeAck works as intended with a PINGRESP.
 */
void test_MQTT_DeserializeAck_pingresp( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTStatus_t status = MQTTSuccess;

    properties.maxPacketSize = MQTT_MAX_PACKET_SIZE;

    /* Bad remaining length. */
    ( void ) memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PINGRESP;
    mqttPacketInfo.remainingLength = MQTT_PACKET_PINGRESP_REMAINING_LENGTH + 1;
    status = MQTT_DeserializeAck( &mqttPacketInfo, NULL, NULL, NULL, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* Process a valid PINGRESP. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PINGRESP;
    mqttPacketInfo.remainingLength = MQTT_PACKET_PINGRESP_REMAINING_LENGTH;
    mqttPacketInfo.pRemainingData = NULL;
    status = MQTT_DeserializeAck( &mqttPacketInfo, NULL, NULL, NULL, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    status = MQTT_DeserializeAck( NULL, NULL, NULL, NULL, NULL, &properties );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
}

/**
 * @brief Tests that MQTT_GetIncomingPacketTypeAndLength works as intended.
 */
void test_MQTT_GetIncomingPacketTypeAndLength1( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTPacketInfo_t mqttPacket;
    NetworkContext_t networkContext;
    uint8_t buffer[ 10 ];
    uint8_t * bufPtr = buffer;

    /* Dummy network context - pointer to pointer to a buffer. */
    networkContext.buffer = &bufPtr;

    /* Test a NULL pIncomingPacket parameter. */
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Test a typical happy path case for a CONN ACK packet. */
    buffer[ 0 ] = 0x20; /* CONN ACK */
    buffer[ 1 ] = 0x02; /* Remaining length. */

    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 0x20, mqttPacket.type );
    TEST_ASSERT_EQUAL_INT( 0x02, mqttPacket.remainingLength );

    /* Remaining length of 128. MQTT uses 7 bits for data and 1 continuation
     * bit in each byte. Since 128 is 8 bits, it needs 2 bytes. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    buffer[ 1 ] = 0x80; /* LSB: CB=1, value=0x00 */
    buffer[ 2 ] = 0x01; /* MSB: CB=0, value=0x01 */
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_PACKET_TYPE_PUBLISH, mqttPacket.type );
    TEST_ASSERT_EQUAL_INT( 128, mqttPacket.remainingLength );

    /* Remaining length of 16384. MQTT uses 7 bits for data and 1 continuation
     * bit in each byte. Since 16384 is 15 bits, it needs 3 bytes. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    buffer[ 1 ] = 0x80; /* LSB   : CB=1, value=0x00 */
    buffer[ 2 ] = 0x80; /* Byte 1: CB=1, value=0x00 */
    buffer[ 3 ] = 0x01; /* MSB   : CB=0, value=0x01 */
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_PACKET_TYPE_PUBLISH, mqttPacket.type );
    TEST_ASSERT_EQUAL_INT( 16384, mqttPacket.remainingLength );

    /* Test with incorrect packet type. */
    bufPtr = buffer;
    buffer[ 0 ] = 0x10; /* INVALID */
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* Test with disconnect packet type. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_DISCONNECT;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /* Test with invalid remaining length. */
    bufPtr = buffer;
    buffer[ 0 ] = 0x20; /* CONN ACK */

    /* To generate invalid remaining length response,
     * four bytes need to have MSB (or continuation bit, 0x80) set */
    buffer[ 1 ] = 0xFF;
    buffer[ 2 ] = 0xFF;
    buffer[ 3 ] = 0xFF;
    buffer[ 4 ] = 0xFF;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* Check with an encoding that does not conform to the MQTT spec. */
    bufPtr = buffer;
    buffer[ 1 ] = 0x80;
    buffer[ 2 ] = 0x80;
    buffer[ 3 ] = 0x80;
    buffer[ 4 ] = 0x00;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* Check when network receive fails. */
    memset( buffer, 0x00, 10 );
    bufPtr = buffer;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveFailure, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTRecvFailed, status );

    /* Test if no data is available. */
    bufPtr = buffer;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveNoData, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTNoDataAvailable, status );

    /* Branch coverage for PUBREL. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_PUBREL & 0xF0U;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceive, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* Receive type then fail. */
    bufPtr = buffer;
    buffer[ 0 ] = MQTT_PACKET_TYPE_PUBREL;
    status = MQTT_GetIncomingPacketTypeAndLength( mockReceiveSucceedThenFail, &networkContext, &mqttPacket );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_AllNULL( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set. Except publish flag. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
}

/* ========================================================================== */

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_QoS1( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    publishInfo.qos = MQTTQoS1;

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set except QoS1 and publish flag. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 1 ) | 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
}

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_QoS2( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    publishInfo.qos = MQTTQoS2;

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set except QoS2. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 2 ) | 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
}

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_retain( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    publishInfo.retain = true;

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set except retain flag. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 0 ) | 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
}

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_Duplicate( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    publishInfo.dup = true;

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set except dup. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 3 ) | 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 0U );
}

/**
 * @brief Tests that MQTT_SerializePublishHeaderWithoutTopic works as intended.
 */
void test_MQTT_SerializePublishHeaderWithoutTopic_VariousFlagsSetTopicLength( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 7 ];
    MQTTStatus_t status = MQTTSuccess;
    size_t headerSize = 0;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );

    publishInfo.qos = MQTTQoS2;
    publishInfo.dup = true;
    publishInfo.retain = true;
    publishInfo.topicNameLength = 20;

    status = MQTT_SerializePublishHeaderWithoutTopic( &publishInfo,
                                                      remainingLength,
                                                      buffer,
                                                      &headerSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( headerSize, 4U );
    /* No flag should be set except QoS2/dup/retain. */
    TEST_ASSERT_EQUAL( buffer[ 0 ], ( 1U << 2 ) | ( 1U << 3 ) | ( 1U << 0 ) | 0x30 );
    /* The encoded length must be 0. */
    TEST_ASSERT_EQUAL( buffer[ 1 ], 0U );
    /* The topic name length should be 0 too. */
    TEST_ASSERT_EQUAL( buffer[ 2 ], 0U );
    TEST_ASSERT_EQUAL( buffer[ 3 ], 20U );
}

/**
 * @brief Tests that MQTT_SerializePublishHeader works as intended.
 */
void test_MQTT_SerializePublishHeader( void )
{
    MQTTPublishInfo_t publishInfo;
    size_t remainingLength = 0;
    uint8_t buffer[ 200 + 2 * BUFFER_PADDING_LENGTH ];
    uint8_t expectedPacket[ 200 ];
    uint8_t * pIterator;
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    size_t headerSize = 0;

    const uint16_t PACKET_ID = 1;

    /* Verify bad parameters fail. */
    memset( &publishInfo, 0x00, sizeof( publishInfo ) );
    publishInfo.pTopicName = TEST_TOPIC_NAME;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    status = MQTT_SerializePublishHeader( NULL,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          NULL,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify a NULL buffer in the fixed buffer struct fails */
    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    /* Restore the fixed buffer. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];

    /* Empty topic fails. */
    publishInfo.pTopicName = NULL;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    publishInfo.pTopicName = TEST_TOPIC_NAME;
    publishInfo.topicNameLength = 0;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;

    /* 0 packet ID for QoS > 0. */
    publishInfo.qos = MQTTQoS1;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          0,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify that a duplicate flag for Qos 0 fails. */
    publishInfo.qos = MQTTQoS0;
    publishInfo.dup = true;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Restore the previous flags for other tests. */
    publishInfo.qos = MQTTQoS1;
    publishInfo.dup = false;

    /* Buffer too small. */
    fixedBuffer.size = 1;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );
    fixedBuffer.size = bufferSize;

    /* Success case. */
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /* MQTT PUBLISH packet format:
     * 0x30 | publish flags (dup, qos, retain) (1 byte)
     * Remaining length (1-4 bytes)
     * Topic name length (2 bytes)
     * Topic name (variable)
     * Packet ID (if QoS > 0) (1 byte)
     * Payload (>= 0 bytes) */
    memset( expectedPacket, 0x00, sizeof( expectedPacket ) );
    pIterator = expectedPacket;
    *pIterator++ = MQTT_PACKET_TYPE_PUBLISH | ( publishInfo.qos << 1 );
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    pIterator += encodeString( pIterator, publishInfo.pTopicName, publishInfo.topicNameLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0;
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    publishInfo.qos = MQTTQoS0;
    publishInfo.pPayload = "test";
    publishInfo.payloadLength = 4;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          0,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    memset( expectedPacket, 0x00, sizeof( expectedPacket ) );
    pIterator = expectedPacket;
    *pIterator++ = MQTT_PACKET_TYPE_PUBLISH;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    pIterator += encodeString( pIterator, publishInfo.pTopicName, publishInfo.topicNameLength );
    *pIterator++ = 0;
    /* Payload should not be serialized. */
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );
    checkBufferOverflow( buffer, sizeof( buffer ) );


    /* Again with QoS2 and dup. */
    publishInfo.qos = MQTTQoS2;
    publishInfo.dup = true;
    status = MQTT_GetPublishPacketSize( &publishInfo, NULL, &remainingLength, &packetSize, MQTT_MAX_PACKET_SIZE );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          NULL,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    memset( expectedPacket, 0x00, sizeof( expectedPacket ) );
    pIterator = expectedPacket;
    /* Set the flags as follows: Dup = 0x8, QoS2 = 0x4, 8 | 4 = 0xC. */
    *pIterator++ = MQTT_PACKET_TYPE_PUBLISH | 0xC;
    pIterator += encodeRemainingLength( pIterator, remainingLength );
    pIterator += encodeString( pIterator, publishInfo.pTopicName, publishInfo.topicNameLength );
    *pIterator++ = UINT16_HIGH_BYTE( PACKET_ID );
    *pIterator++ = UINT16_LOW_BYTE( PACKET_ID );
    *pIterator++ = 0;
    TEST_ASSERT_EQUAL_MEMORY( expectedPacket, &buffer[ BUFFER_PADDING_LENGTH ], packetSize );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* test with publish properties */
    MQTTPropBuilder_t propBuffer = { 0 };
    uint8_t buf[ 10 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = sizeof( buf );
    MQTTPropAdd_PubMessageExpiry( &propBuffer, 100 );
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          &propBuffer,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*test with null buffer. */
    propBuffer.pBuffer = NULL;
    status = MQTT_SerializePublishHeader( &publishInfo,
                                          &propBuffer,
                                          PACKET_ID,
                                          remainingLength,
                                          &fixedBuffer,
                                          &headerSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

/* ========================================================================== */


void test_MQTT_ProcessIncomingPacketTypeAndLength_PacketNULL( void )
{
    uint8_t pBuffer[ 100 ] = { 0 };
    size_t index = 0;
    MQTTStatus_t status;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, NULL );

    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_BufferNULL( void )
{
    MQTTPacketInfo_t packetInfo;
    size_t index = 0;
    MQTTStatus_t status;

    status = MQTT_ProcessIncomingPacketTypeAndLength( NULL, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_IndexNULL( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ] = { 0 };
    MQTTStatus_t status;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, NULL, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_NoData( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ] = { 0 };
    size_t index = 0;
    MQTTStatus_t status;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTNoDataAvailable, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_InvalidData( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ];
    size_t index = 2;
    MQTTStatus_t status;

    memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
    memset( pBuffer, 0, 100 );

    pBuffer[ 0 ] = 0xF0;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_ValidDataOneByte( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ];
    size_t index = 1;
    MQTTStatus_t status;

    memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
    memset( pBuffer, 0, 100 );

    pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTNeedMoreBytes, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_ValidDataTwoBytes( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ];
    size_t index = 2;
    MQTTStatus_t status;

    memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
    memset( pBuffer, 0, 100 );

    pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    /* 2nd byte is the length. */
    pBuffer[ 1 ] = 10;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( packetInfo.remainingLength, 10U );
    TEST_ASSERT_EQUAL( packetInfo.headerLength, 2U );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_InvalidLength( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ];
    size_t index = 6;
    MQTTStatus_t status;

    memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
    memset( pBuffer, 0, 100 );

    pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    /* 2nd byte onward is the length. */
    pBuffer[ 1 ] = 0xFF;
    pBuffer[ 2 ] = 0xFF;
    pBuffer[ 3 ] = 0xFF;
    /* This byte doesn't terminate the length. */
    pBuffer[ 4 ] = 0xFF;
    pBuffer[ 5 ] = 0xFF;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

void test_MQTT_ProcessIncomingPacketTypeAndLength_NonConformingLength( void )
{
    MQTTPacketInfo_t packetInfo;
    uint8_t pBuffer[ 100 ];
    size_t index = 6;
    MQTTStatus_t status;

    memset( &packetInfo, 0, sizeof( MQTTPacketInfo_t ) );
    memset( pBuffer, 0, 100 );

    pBuffer[ 0 ] = MQTT_PACKET_TYPE_PUBLISH;
    /* 2nd byte onward is the length. */
    pBuffer[ 1 ] = 0x80;
    pBuffer[ 2 ] = 0x80;
    pBuffer[ 3 ] = 0x80;
    /* This byte doesn't terminate the length. */
    pBuffer[ 4 ] = 0x00;

    status = MQTT_ProcessIncomingPacketTypeAndLength( pBuffer, &index, &packetInfo );

    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

/**
 * @brief Tests that MQTT_DeserializePublish works as intended.
 */
void test_MQTT_DeserializePublish( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTPublishInfo_t publishInfo;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 100 ];
    MQTTPropBuilder_t propBuffer = { 0 };

    uint16_t packetIdentifier;


    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );

    /* Verify parameters. */
    status = MQTT_DeserializePublish( NULL, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    status = MQTT_DeserializePublish( &mqttPacketInfo, NULL, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, NULL, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Bad Packet Type. */
    mqttPacketInfo.type = 0x01;
    mqttPacketInfo.pRemainingData = buffer;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Incorrect flags. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0xf;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* QoS 0 bad remaining length. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH;
    mqttPacketInfo.remainingLength = 0;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* QoS 1 bad remaining length. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBLISH | 0x2;
    mqttPacketInfo.remainingLength = 0;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /* QoS 1 invalid packet identifier. */
    mqttPacketInfo.remainingLength = 5;
    buffer[ 0 ] = 0;
    buffer[ 1 ] = 1;
    buffer[ 2 ] = ( uint8_t ) 'a';
    buffer[ 3 ] = 0;
    buffer[ 4 ] = 0;
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 100, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid max packet size*/
    status = MQTT_DeserializePublish( &mqttPacketInfo, &packetIdentifier, &publishInfo, &propBuffer, 1, 100 );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}
void test_serializeHeaders( void )
{
    uint8_t buffer[ 4 ];

    MQTT_SerializeSubscribeHeader( 3, buffer, 1 );
    TEST_ASSERT_EQUAL( buffer[ 0 ], MQTT_PACKET_TYPE_SUBSCRIBE );
    MQTT_SerializeUnsubscribeHeader( 3, buffer, 1 );
    TEST_ASSERT_EQUAL( buffer[ 0 ], MQTT_PACKET_TYPE_UNSUBSCRIBE );
    uint8_t buf[ 22 ];
    MQTTConnectInfo_t pConnectInfo;
    pConnectInfo.cleanSession = true;
    pConnectInfo.pUserName = "abc";
    pConnectInfo.pPassword = "def";
    pConnectInfo.keepAliveSeconds = 100;
    MQTTPublishInfo_t pWillInfo;
    pWillInfo.qos = MQTTQoS1;
    pWillInfo.retain = true;
    MQTT_SerializeConnectFixedHeader( buf, &pConnectInfo, &pWillInfo, 20 );
    pWillInfo.qos = MQTTQoS2;
    MQTT_SerializeConnectFixedHeader( buf, &pConnectInfo, &pWillInfo, 20 );
}

void test_OptionalProperties( void )
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTPropBuilder_t propBuilder;
    MQTTPropBuilder_t prop1;

    prop1.pBuffer = NULL;
    uint8_t buf[ 100 ];
    size_t bufLength = sizeof( buf );
    propBuilder.pBuffer = buf;
    propBuilder.bufferLength = bufLength;
    propBuilder.currentIndex = 0;
    propBuilder.fieldSet = 0;

    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_SubscribeId( &propBuilder, 2 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_SubscribeId( &propBuilder, 2 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_SubscribeId( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_SubscribeId( NULL, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_SubscribeId( &prop1, 2 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    MQTTUserProperty_t userProperty;
    memset( &userProperty, 0x0, sizeof( userProperty ) );
    userProperty.pKey = "abc";
    userProperty.pValue = "def";
    userProperty.keyLength = 3;
    userProperty.valueLength = 3;
    mqttStatus = MQTTPropAdd_UserProp( &( propBuilder ), &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    mqttStatus = MQTTPropAdd_UserProp( NULL, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_UserProp( &( propBuilder ), NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_UserProp( &prop1, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    userProperty.pKey = NULL;
    mqttStatus = MQTTPropAdd_UserProp( &propBuilder, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    userProperty.pKey = "abc";
    userProperty.pValue = NULL;
    mqttStatus = MQTTPropAdd_UserProp( &propBuilder, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    userProperty.pValue = "def";
    userProperty.keyLength = 0;
    mqttStatus = MQTTPropAdd_UserProp( &propBuilder, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    userProperty.keyLength = 3;
    userProperty.valueLength = 0;
    mqttStatus = MQTTPropAdd_UserProp( &propBuilder, &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_SessionExpiry( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_SessionExpiry( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_SessionExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_SessionExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( &( propBuilder ), 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnTopicAliasMax( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnTopicAliasMax( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnTopicAliasMax( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnTopicAliasMax( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( NULL, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( &prop1, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    propBuilder.fieldSet = 0;
    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( NULL, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( &prop1, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    propBuilder.fieldSet = 0;
    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    /*Test Auth Data before Auth Method.*/
    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnAuthMethod( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthMethod( &propBuilder, NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthMethod( &propBuilder, "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthMethod( &prop1, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthMethod( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthMethod( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnAuthData( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthData( &prop1, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_PubPayloadFormat( NULL, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubPayloadFormat( &prop1, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubPayloadFormat( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    propBuilder.fieldSet = 0;
    mqttStatus = MQTTPropAdd_PubPayloadFormat( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_PubPayloadFormat( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_PubMessageExpiry( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubMessageExpiry( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubMessageExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_PubMessageExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_PubTopicAlias( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubTopicAlias( &prop1, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubTopicAlias( &propBuilder, 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubTopicAlias( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_PubTopicAlias( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_PubResponseTopic( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &prop1, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc/#", 5 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc/+/def", 9 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    MQTTPropBuilder_t propBuilder2;
    uint8_t buf2[ 500 ];
    propBuilder2.pBuffer = buf2;
    propBuilder2.bufferLength = sizeof( buf2 );
    propBuilder2.currentIndex = 0;
    propBuilder2.fieldSet = 0;
    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder2, "abc/#/def", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_PubCorrelationData( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubCorrelationData( &( prop1 ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );


    mqttStatus = MQTTPropAdd_PubContentType( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubContentType( &( prop1 ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubContentType( &( propBuilder ), NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubContentType( &( propBuilder ), "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_PubContentType( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_PubContentType( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_ReasonString( NULL, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ReasonString( &( prop1 ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ReasonString( &( propBuilder ), NULL, 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ReasonString( &( propBuilder ), "abc", 0 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_ReasonString( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_ReasonString( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropAdd_WillDelayInterval( NULL, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_WillDelayInterval( &( prop1 ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
    mqttStatus = MQTTPropAdd_WillDelayInterval( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );
    mqttStatus = MQTTPropAdd_WillDelayInterval( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
}

void test_MQTTPropAdd_NoMemory( void )
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTPropBuilder_t propBuilder;
    uint8_t buf[ 1 ];
    size_t bufLength = sizeof( buf );

    propBuilder.pBuffer = buf;
    propBuilder.bufferLength = bufLength;
    propBuilder.currentIndex = 0;
    propBuilder.fieldSet = 0;

    mqttStatus = MQTTPropAdd_ReasonString( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubContentType( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubCorrelationData( &( propBuilder ), "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubResponseTopic( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubTopicAlias( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubMessageExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_PubPayloadFormat( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnAuthMethod( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnAuthData( &propBuilder, "abc", 3 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnRequestProbInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnRequestRespInfo( &propBuilder, 1 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnTopicAliasMax( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnMaxPacketSize( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_ConnReceiveMax( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_SessionExpiry( &propBuilder, 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_WillDelayInterval( &( propBuilder ), 10 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    MQTTUserProperty_t userProperty;
    memset( &userProperty, 0x0, sizeof( userProperty ) );
    userProperty.pKey = "abc";
    userProperty.pValue = "def";
    userProperty.keyLength = 3;
    userProperty.valueLength = 3;
    mqttStatus = MQTTPropAdd_UserProp( &( propBuilder ), &userProperty );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );

    mqttStatus = MQTTPropAdd_SubscribeId( &propBuilder, 2 );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, mqttStatus );
}
void test_updateContextWithConnectProps( void )
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    MQTTPropBuilder_t propBuilder;
    uint8_t buffer[ 50 ];
    MQTTConnectionProperties_t connectProps;
    size_t bufLength = sizeof( buffer );

    propBuilder.pBuffer = buffer;
    propBuilder.bufferLength = bufLength;
    propBuilder.currentIndex = 0;
    propBuilder.fieldSet = 0;

    mqttStatus = updateContextWithConnectProps( &propBuilder, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    uint8_t * pIndex = buffer;
    pIndex = serializeuint_16( pIndex, MQTT_RECEIVE_MAX_ID );
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_MAX_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MAX_PACKET_SIZE_ID );
    pIndex = serializeutf_8( pIndex, MQTT_AUTH_METHOD_ID );
    pIndex = serializeuint_8( pIndex, MQTT_REQUEST_PROBLEM_ID );
    pIndex = serializeuint_8( pIndex, MQTT_REQUEST_RESPONSE_ID );
    pIndex = serializeutf_8pair( pIndex );

    propBuilder.currentIndex = 27;
    propBuilder.currentIndex += 13;
    mqttStatus = updateContextWithConnectProps( &propBuilder, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, mqttStatus );

    mqttStatus = updateContextWithConnectProps( NULL, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    mqttStatus = updateContextWithConnectProps( &propBuilder, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    propBuilder.pBuffer = NULL;
    mqttStatus = updateContextWithConnectProps( &propBuilder, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );

    propBuilder.pBuffer = buffer;
    propBuilder.currentIndex = 5;
    mqttStatus = updateContextWithConnectProps( &propBuilder, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, mqttStatus );

    propBuilder.pBuffer = pIndex;
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    propBuilder.currentIndex = 10;
    mqttStatus = updateContextWithConnectProps( &propBuilder, &connectProps );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, mqttStatus );
}

void test_MQTT_SerializeAck( void )
{
    MQTTFixedBuffer_t fixedBuffer;
    uint8_t buf[ 50 ];

    fixedBuffer.pBuffer = buf;
    fixedBuffer.size = sizeof( buf );

    MQTTStatus_t status = MQTTSuccess;
    status = MQTT_SerializeAck( &fixedBuffer, MQTT_PACKET_TYPE_PUBACK, 100 );
    TEST_ASSERT_EQUAL_INT( status, MQTTSuccess );

    status = MQTT_SerializeAck( &fixedBuffer, MQTT_PACKET_TYPE_PUBACK, 0 );
    TEST_ASSERT_EQUAL_INT( status, MQTTBadParameter );


    status = MQTT_SerializeAck( &fixedBuffer, MQTT_PACKET_TYPE_SUBACK, 100 );
    TEST_ASSERT_EQUAL_INT( status, MQTTBadParameter );

    status = MQTT_SerializeAck( NULL, MQTT_PACKET_TYPE_PUBACK, 100 );
    TEST_ASSERT_EQUAL_INT( status, MQTTBadParameter );

    fixedBuffer.pBuffer = NULL;
    status = MQTT_SerializeAck( &fixedBuffer, MQTT_PACKET_TYPE_PUBACK, 100 );
    TEST_ASSERT_EQUAL_INT( status, MQTTBadParameter );

    fixedBuffer.pBuffer = buf;
    fixedBuffer.size = 1;
    status = MQTT_SerializeAck( &fixedBuffer, MQTT_PACKET_TYPE_PUBACK, 100 );
    TEST_ASSERT_EQUAL_INT( status, MQTTNoMemory );
}

void test_MQTTV5_SerializeDisconnect( void )
{
    uint8_t buf[ 10 ];

    MQTT_SerializeDisconnectFixed( buf, 0x00, 10 );
    TEST_ASSERT_EQUAL( buf[ 0 ], MQTT_PACKET_TYPE_DISCONNECT );

    MQTT_SerializeAckFixed( buf, MQTT_PACKET_TYPE_SUBACK, 10, 10, 0x00 );
    TEST_ASSERT_EQUAL( buf[ 0 ], MQTT_PACKET_TYPE_SUBACK );
}

void test_validatePublishProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;
    uint16_t serverTopicAliasMax;
    uint16_t topicAlias;
    MQTTPropBuilder_t propBuilder;
    uint8_t buffer[ 50 ];
    size_t bufLength = sizeof( buffer );

    propBuilder.pBuffer = buffer;
    propBuilder.bufferLength = bufLength;
    propBuilder.currentIndex = 0;
    propBuilder.fieldSet = 0;

    status = MQTT_ValidatePublishProperties( 1, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    uint8_t * pIndex = buffer;
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    propBuilder.currentIndex = 3;

    /*Invalid Topic Alias*/
    serverTopicAliasMax = 2;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    serverTopicAliasMax = 300;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Property length field less than the actual length of the property.*/
    propBuilder.currentIndex = 2;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
    propBuilder.currentIndex = 3;

    pIndex = buffer;
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_TOPIC_ID );
    propBuilder.currentIndex = 7;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}

void test_validateSubscribeProperties( void )
{
    uint8_t isSubIdAvailable = 0;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuilder;
    uint8_t buffer[ 50 ];
    size_t bufLength = sizeof( buffer );

    propBuilder.pBuffer = NULL;
    propBuilder.bufferLength = bufLength;
    propBuilder.currentIndex = 0;
    propBuilder.fieldSet = 0;

    status = MQTT_ValidateSubscribeProperties( isSubIdAvailable, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_ValidateSubscribeProperties( isSubIdAvailable, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    propBuilder.pBuffer = buffer;
    status = MQTT_ValidateSubscribeProperties( isSubIdAvailable, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /* Subscription Id not available is set in the connack, but Subscription Id is sent. */
    uint8_t * pIndex = buffer;
    *pIndex = MQTT_SUBSCRIPTION_ID_ID;
    pIndex++;
    *pIndex = 2;
    propBuilder.currentIndex = 2;
    status = MQTT_ValidateSubscribeProperties( isSubIdAvailable, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Subscription Id is available. */
    isSubIdAvailable = 1;
    status = MQTT_ValidateSubscribeProperties( isSubIdAvailable, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Validating user properties. */
    pIndex = buffer;
    pIndex = serializeutf_8pair( pIndex );
    propBuilder.currentIndex += 11;
    status = MQTT_ValidateSubscribeProperties( 1, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid property sent. */
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    propBuilder.currentIndex += 2;
    status = MQTT_ValidateSubscribeProperties( 1, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Invalid Subscription Id, exceeding 4 bytes. */
    pIndex = buffer;
    *pIndex++ = MQTT_SUBSCRIPTION_ID_ID;
    encodeRemainingLength( pIndex, 20971556356235 );
    propBuilder.currentIndex = 10;
    status = MQTT_ValidateSubscribeProperties( 1, &propBuilder );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

void test_getProps( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuffer;
    MQTTPropBuilder_t propBuffer1;

    propBuffer1.pBuffer = NULL;
    propBuffer1.bufferLength = 0;
    propBuffer1.currentIndex = 0;
    propBuffer1.fieldSet = 0;
    size_t currIndex;


    uint8_t buffer[ 500 ] = { 0 };
    size_t bufLength = 500;

    propBuffer.pBuffer = buffer;
    propBuffer.bufferLength = bufLength;
    propBuffer.currentIndex = 0;
    propBuffer.fieldSet = 0;

    uint8_t * pIndex = buffer;
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_TOPIC_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CORRELATION_DATA_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CONTENT_TYPE_ID );
    *pIndex = MQTT_SUBSCRIPTION_ID_ID;
    pIndex++;
    *pIndex = 2;
    pIndex++;
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_MAX_ID );
    pIndex = serializeuint_16( pIndex, MQTT_RECEIVE_MAX_ID );
    pIndex = serializeuint_8( pIndex, MQTT_MAX_QOS_ID );
    pIndex = serializeuint_8( pIndex, MQTT_RETAIN_AVAILABLE_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MAX_PACKET_SIZE_ID );
    pIndex = serializeutf_8( pIndex, MQTT_ASSIGNED_CLIENT_ID );
    pIndex = serializeuint_8( pIndex, MQTT_WILDCARD_ID );
    pIndex = serializeuint_8( pIndex, MQTT_SUB_AVAILABLE_ID );
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8( pIndex, MQTT_SERVER_REF_ID );
    pIndex = serializeuint_8( pIndex, MQTT_SHARED_SUB_ID );
    pIndex = serializeuint_16( pIndex, MQTT_SERVER_KEEP_ALIVE_ID );
    pIndex = serializeutf_8( pIndex, MQTT_RESPONSE_INFO_ID );
    pIndex = serializeutf_8( pIndex, MQTT_AUTH_METHOD_ID );
    pIndex = serializeutf_8( pIndex, MQTT_AUTH_DATA_ID );

    uint16_t topicAlias;
    uint8_t payloadFormat;
    const char * pResponseTopic;
    uint16_t responseTopicLength;
    const void * correlationData;
    uint16_t correlationLength;
    uint32_t messageExpiry;
    const char * pContentType;
    uint16_t contentTypeLength;
    size_t subscriptionId;
    uint32_t sessionExpiry;
    uint16_t aliasMax;
    uint16_t receiveMax;
    uint8_t maxQoS;
    uint8_t retainAvailable;
    uint32_t maxPacketSize;
    const char * pClientId;
    uint16_t clientIdLength;
    uint8_t wildcard;
    uint8_t subAvailable;
    uint8_t propertyId;
    const char * pUserPropKey;
    uint16_t pUserPropKeyLen;
    const char * pUserPropVal;
    uint16_t pUserPropValLen;
    const char * pReasonString;
    uint16_t reasonStringLength;
    uint8_t sharedSubAvailable;
    uint16_t serverKeepAlive;
    const char * pResponseInfo;
    uint16_t responseInfoLength;
    const char * pAuthMethod;
    uint16_t authMethodLength;
    const char * pAuthData;
    uint16_t authDataLength;


    size_t counter = 0U;
    status = MQTT_IncomingGetNextProp( NULL, &propertyId );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_IncomingGetNextProp( &propBuffer, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_IncomingGetNextProp( &propBuffer1, &propertyId );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTT_IncomingGetNextProp( &propBuffer, &propertyId );
    counter++;

    while( status == MQTTSuccess )
    {
        switch( propertyId )
        {
            case MQTT_TOPIC_ALIAS_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubTopicAlias( NULL, &topicAlias ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubTopicAlias( &propBuffer1, &topicAlias ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubTopicAlias( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubTopicAlias( &propBuffer, &topicAlias ) );
                break;

            case MQTT_PAYLOAD_FORMAT_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator( NULL, &payloadFormat ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator( &propBuffer1, &payloadFormat ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubPayloadFormatIndicator( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubPayloadFormatIndicator( &propBuffer, &payloadFormat ) );
                break;

            case MQTT_RESPONSE_TOPIC_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubResponseTopic( NULL, &pResponseTopic, &responseTopicLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubResponseTopic( &propBuffer1, &pResponseTopic, &responseTopicLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubResponseTopic( &propBuffer, NULL, &responseTopicLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubResponseTopic( &propBuffer, &pResponseTopic, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubResponseTopic( &propBuffer, &pResponseTopic, &responseTopicLength ) );
                propBuffer.bufferLength = 7;
                currIndex = propBuffer.currentIndex;
                propBuffer.currentIndex = 6;
                TEST_ASSERT_EQUAL_INT( MQTTBadResponse, MQTTPropGet_PubResponseTopic( &propBuffer, &pResponseTopic, &responseTopicLength ) );
                propBuffer.currentIndex = 4;
                TEST_ASSERT_EQUAL_INT( MQTTBadResponse, MQTTPropGet_PubResponseTopic( &propBuffer, &pResponseTopic, &responseTopicLength ) );
                propBuffer.bufferLength = 500;
                propBuffer.currentIndex = currIndex;
                break;

            case MQTT_CORRELATION_DATA_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubCorrelationData( NULL, &correlationData, &correlationLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubCorrelationData( &propBuffer1, &correlationData, &correlationLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubCorrelationData( &propBuffer, NULL, &correlationLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubCorrelationData( &propBuffer, &correlationData, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubCorrelationData( &propBuffer, &correlationData, &correlationLength ) );
                propBuffer.bufferLength = 7;
                currIndex = propBuffer.currentIndex;
                propBuffer.currentIndex = 6;
                TEST_ASSERT_EQUAL_INT( MQTTBadResponse, MQTTPropGet_PubCorrelationData( &propBuffer, &correlationData, &correlationLength ) );
                propBuffer.currentIndex = 4;
                TEST_ASSERT_EQUAL_INT( MQTTBadResponse, MQTTPropGet_PubCorrelationData( &propBuffer, &correlationData, &correlationLength ) );
                propBuffer.bufferLength = 500;
                propBuffer.currentIndex = currIndex;
                break;

            case MQTT_MSG_EXPIRY_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval( NULL, &messageExpiry ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval( &propBuffer1, &messageExpiry ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubMessageExpiryInterval( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubMessageExpiryInterval( &propBuffer, &messageExpiry ) );
                break;

            case MQTT_CONTENT_TYPE_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubContentType( NULL, &pContentType, &contentTypeLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubContentType( &propBuffer1, &pContentType, &contentTypeLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubContentType( &propBuffer, NULL, &contentTypeLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubContentType( &propBuffer, &pContentType, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubContentType( &propBuffer, &pContentType, &contentTypeLength ) );
                break;

            case MQTT_SUBSCRIPTION_ID_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubSubscriptionId( NULL, &subscriptionId ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubSubscriptionId( &propBuffer1, &subscriptionId ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_PubSubscriptionId( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_PubSubscriptionId( &propBuffer, &subscriptionId ) );
                break;

            case MQTT_SESSION_EXPIRY_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_SessionExpiry( NULL, &sessionExpiry ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_SessionExpiry( &propBuffer1, &sessionExpiry ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_SessionExpiry( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_SessionExpiry( &propBuffer, &sessionExpiry ) );
                break;

            case MQTT_TOPIC_ALIAS_MAX_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax( NULL, &aliasMax ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax( &propBuffer1, &aliasMax ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnTopicAliasMax( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnTopicAliasMax( &propBuffer, &aliasMax ) );
                break;

            case MQTT_RECEIVE_MAX_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnReceiveMax( NULL, &receiveMax ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnReceiveMax( &propBuffer1, &receiveMax ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnReceiveMax( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnReceiveMax( &propBuffer, &receiveMax ) );
                break;

            case MQTT_MAX_QOS_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxQos( NULL, &maxQoS ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxQos( &propBuffer1, &maxQoS ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxQos( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnMaxQos( &propBuffer, &maxQoS ) );
                break;

            case MQTT_RETAIN_AVAILABLE_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnRetainAvailable( NULL, &retainAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnRetainAvailable( &propBuffer1, &retainAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnRetainAvailable( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnRetainAvailable( &propBuffer, &retainAvailable ) );
                break;

            case MQTT_MAX_PACKET_SIZE_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize( NULL, &maxPacketSize ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize( &propBuffer1, &maxPacketSize ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnMaxPacketSize( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnMaxPacketSize( &propBuffer, &maxPacketSize ) );
                break;

            case MQTT_ASSIGNED_CLIENT_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnClientId( NULL, &pClientId, &clientIdLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnClientId( &propBuffer1, &pClientId, &clientIdLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnClientId( &propBuffer, NULL, &clientIdLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnClientId( &propBuffer, &pClientId, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnClientId( &propBuffer, &pClientId, &clientIdLength ) );
                break;

            case MQTT_WILDCARD_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnWildcard( NULL, &wildcard ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnWildcard( &propBuffer1, &wildcard ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnWildcard( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnWildcard( &propBuffer, &wildcard ) );
                break;

            case MQTT_SUB_AVAILABLE_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSubId( NULL, &subAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSubId( &propBuffer1, &subAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSubId( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnSubId( &propBuffer, &subAvailable ) );
                break;

            case MQTT_USER_PROPERTY_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( NULL, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, &pUserPropValLen ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( &propBuffer1, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, &pUserPropValLen ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( &propBuffer, NULL, &pUserPropKeyLen, &pUserPropVal, &pUserPropValLen ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( &propBuffer, &pUserPropKey, NULL, &pUserPropVal, &pUserPropValLen ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( &propBuffer, &pUserPropKey, &pUserPropKeyLen, NULL, &pUserPropValLen ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_UserProp( &propBuffer, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_UserProp( &propBuffer, &pUserPropKey, &pUserPropKeyLen, &pUserPropVal, &pUserPropValLen ) );
                break;

            case MQTT_REASON_STRING_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ReasonString( NULL, &pReasonString, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ReasonString( &propBuffer1, &pReasonString, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ReasonString( &propBuffer, NULL, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ReasonString( &propBuffer, &pReasonString, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ReasonString( &propBuffer, &pReasonString, &reasonStringLength ) );
                break;

            case MQTT_SERVER_REF_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ServerRef( NULL, &pReasonString, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ServerRef( &propBuffer1, &pReasonString, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ServerRef( &propBuffer, NULL, &reasonStringLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ServerRef( &propBuffer, &pReasonString, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ServerRef( &propBuffer, &pReasonString, &reasonStringLength ) );
                break;

            case MQTT_SHARED_SUB_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSharedSubAvailable( NULL, &sharedSubAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSharedSubAvailable( &propBuffer1, &sharedSubAvailable ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnSharedSubAvailable( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnSharedSubAvailable( &propBuffer, &sharedSubAvailable ) );
                break;

            case MQTT_SERVER_KEEP_ALIVE_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnServerKeepAlive( NULL, &serverKeepAlive ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnServerKeepAlive( &propBuffer1, &serverKeepAlive ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnServerKeepAlive( &propBuffer, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnServerKeepAlive( &propBuffer, &serverKeepAlive ) );
                break;

            case MQTT_RESPONSE_INFO_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnResponseInfo( NULL, &pResponseInfo, &responseInfoLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnResponseInfo( &propBuffer1, &pResponseInfo, &responseInfoLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnResponseInfo( &propBuffer, NULL, &responseInfoLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnResponseInfo( &propBuffer, &pResponseInfo, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnResponseInfo( &propBuffer, &pResponseInfo, &responseInfoLength ) );
                break;

            case MQTT_AUTH_METHOD_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthMethod( NULL, &pAuthMethod, &authMethodLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthMethod( &propBuffer1, &pAuthMethod, &authMethodLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthMethod( &propBuffer, NULL, &authMethodLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthMethod( &propBuffer, &pAuthMethod, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnAuthMethod( &propBuffer, &pAuthMethod, &authMethodLength ) );
                break;

            case MQTT_AUTH_DATA_ID:
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthData( NULL, &pAuthData, &authDataLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthData( &propBuffer1, &pAuthData, &authDataLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthData( &propBuffer, NULL, &authDataLength ) );
                TEST_ASSERT_EQUAL_INT( MQTTBadParameter, MQTTPropGet_ConnAuthData( &propBuffer, &pAuthData, NULL ) );
                TEST_ASSERT_EQUAL_INT( MQTTSuccess, MQTTPropGet_ConnAuthData( &propBuffer, &pAuthData, &authDataLength ) );
                break;

            default:
                break;
        }

        status = MQTT_IncomingGetNextProp( &propBuffer, &propertyId );
    }

    propBuffer.bufferLength = 10;
    status = MQTT_IncomingGetNextProp( &propBuffer, &propertyId );
    TEST_ASSERT_EQUAL_INT( MQTTEndOfProperties, status );
}
void test_getProps_decodeFailure( void )
{
    MQTTStatus_t status = MQTTSuccess;
    MQTTPropBuilder_t propBuffer;
    uint16_t twoByteProperty;
    uint8_t oneByteProperty;
    uint8_t propertyId;
    uint32_t messageExpiry;
    uint32_t sessionExpiry;
    uint32_t maxPacketSize;
    const char * string;
    uint16_t stringLength;

    uint8_t buffer[ 500 ] = { 0 };
    size_t bufLength = 500;

    propBuffer.pBuffer = buffer;
    propBuffer.bufferLength = bufLength;
    propBuffer.currentIndex = 0;
    propBuffer.fieldSet = 0;

    uint8_t * pIndex = buffer;
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    propBuffer.bufferLength = 2;

    status = MQTT_IncomingGetNextProp( &propBuffer, &propertyId );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    status = MQTTPropGet_PubTopicAlias( &propBuffer, &twoByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    propBuffer.bufferLength += 1;
    status = MQTT_IncomingGetNextProp( &propBuffer, &propertyId );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    status = MQTTPropGet_PubPayloadFormatIndicator( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_PubMessageExpiryInterval( &propBuffer, &messageExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_PubContentType( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_SessionExpiry( &propBuffer, &sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnTopicAliasMax( &propBuffer, &twoByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnReceiveMax( &propBuffer, &twoByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnMaxQos( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnRetainAvailable( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnMaxPacketSize( &propBuffer, &maxPacketSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnClientId( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnWildcard( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnSubId( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_UserProp( &propBuffer, &string, &stringLength, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ReasonString( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ServerRef( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnSharedSubAvailable( &propBuffer, &oneByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnServerKeepAlive( &propBuffer, &twoByteProperty );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnResponseInfo( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnAuthMethod( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    status = MQTTPropGet_ConnAuthData( &propBuffer, &string, &stringLength );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    size_t subId;
    pIndex = buffer;
    encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTTPropGet_PubSubscriptionId( &propBuffer, &subId );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );
}

/* ==================  Testing MQTT_UpdateDuplicatePublishFlag ===================== */

/**
 * @brief Call MQTT_UpdateDuplicatePublishFlag using a NULL pHeader and a header that does
 * not come from a publish packet, in order to receive MQTTBadParameter errors.
 */
void test_MQTT_UpdateDuplicatePublishFlag_Invalid_Params()
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    uint8_t pHeader = MQTT_PACKET_TYPE_SUBSCRIBE;

    /* Test NULL pHeader. */
    mqttStatus = MQTT_UpdateDuplicatePublishFlag( NULL, true );
    TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

    /* Test a non-publish header. */
    mqttStatus = MQTT_UpdateDuplicatePublishFlag( &pHeader, true );
    TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
}

/**
 * @brief This method calls MQTT_UpdateDuplicatePublishFlag successfully in order to
 * get full coverage on the method.
 */
void test_MQTT_UpdateDuplicatePublishFlag_Happy_Path()
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    uint8_t pHeader = MQTT_PACKET_TYPE_PUBLISH;

    /* Test to set the flag. */
    mqttStatus = MQTT_UpdateDuplicatePublishFlag( &pHeader, true );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    TEST_ASSERT_NOT_EQUAL_INT( ( pHeader ) & ( 0x01U << ( 3 ) ), 0 );

    /* Test to clear the flag. */
    mqttStatus = MQTT_UpdateDuplicatePublishFlag( &pHeader, false );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    TEST_ASSERT_EQUAL_INT( ( pHeader ) & ( 0x01U << ( 3 ) ), 0 );
}

/* ========================================================================== */

void test_ValidatePublishProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;
    uint16_t topicAlias;
    uint16_t serverTopicAliasMax = 10;

    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, NULL, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    MQTTPropBuilder_t propBuilder = { 0 };
    propBuilder.pBuffer = NULL;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    uint8_t buf[ 50 ];
    propBuilder.pBuffer = buf;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    uint8_t * pIndex = buf;
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    pIndex = serializeutf_8pair( pIndex );
    propBuilder.currentIndex = 20;
    propBuilder.bufferLength = 50;

    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /* Invalid property sent. */
    pIndex = serializeuint_8( pIndex, MQTT_REQUEST_PROBLEM_ID );
    propBuilder.currentIndex += 2;
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Invalid property length. */
    pIndex = buf;
    propBuilder.currentIndex = 1;
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    pIndex = buf;
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    status = MQTT_ValidatePublishProperties( serverTopicAliasMax, &propBuilder, &topicAlias );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

void test_ValidateDisconnectProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;

    status = MQTT_ValidateDisconnectProperties( 0, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    MQTTPropBuilder_t propBuffer;
    propBuffer.pBuffer = NULL;
    status = MQTT_ValidateDisconnectProperties( 0, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.currentIndex = 25;

    uint8_t * pIndex = buf;
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeuint_16( pIndex, MQTT_TOPIC_ALIAS_ID );

    status = MQTT_ValidateDisconnectProperties( 10, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /*Disconnect Session Expiry non-zero while Connect Session Expiry was zero.*/
    status = MQTT_ValidateDisconnectProperties( 0, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Invalid property is sent in the disconnect. */
    propBuffer.currentIndex = 28;
    status = MQTT_ValidateDisconnectProperties( 10, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Invalid property length. */
    propBuffer.currentIndex = 2;
    status = MQTT_ValidateDisconnectProperties( 10, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    buf[ 0 ] = MQTT_SESSION_EXPIRY_ID;
    buf[ 1 ] = 0, buf[ 2 ] = 0, buf[ 3 ] = 0, buf[ 4 ] = 0;
    propBuffer.currentIndex = 5;
    status = MQTT_ValidateDisconnectProperties( 0, &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
}

void test_ValidateUnsubscribeProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;

    status = MQTT_ValidateUnsubscribeProperties( NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    MQTTPropBuilder_t propBuffer;
    propBuffer.pBuffer = NULL;
    status = MQTT_ValidateUnsubscribeProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = 50;
    propBuffer.currentIndex = 13;

    uint8_t * pIndex = buf;
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );

    status = MQTT_ValidateUnsubscribeProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    propBuffer.currentIndex = 18;
    /*Invalid property send in the unsubscribe.*/
    status = MQTT_ValidateUnsubscribeProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_ValidateWillProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;

    status = MQTT_ValidateWillProperties( NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    MQTTPropBuilder_t propBuffer;
    propBuffer.pBuffer = NULL;
    status = MQTT_ValidateWillProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = 50;
    propBuffer.currentIndex = 32;

    uint8_t * pIndex = buf;
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );
    pIndex = serializeuint_32( pIndex, MQTT_WILL_DELAY_ID );
    pIndex = serializeuint_8( pIndex, MQTT_PAYLOAD_FORMAT_ID );
    pIndex = serializeutf_8( pIndex, MQTT_CONTENT_TYPE_ID );
    pIndex = serializeuint_32( pIndex, MQTT_SESSION_EXPIRY_ID );

    status = MQTT_ValidateWillProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    propBuffer.currentIndex = 37;
    /*Invalid property sent in LWT. */
    status = MQTT_ValidateWillProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_ValidatePublishAckProperties( void )
{
    MQTTStatus_t status = MQTTSuccess;

    status = MQTT_ValidatePublishAckProperties( NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    MQTTPropBuilder_t propBuffer;
    propBuffer.pBuffer = NULL;
    status = MQTT_ValidatePublishAckProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    uint8_t buf[ 50 ];
    propBuffer.pBuffer = buf;
    propBuffer.bufferLength = 50;
    propBuffer.currentIndex = 20;

    uint8_t * pIndex = buf;
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeuint_32( pIndex, MQTT_MSG_EXPIRY_ID );

    status = MQTT_ValidatePublishAckProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    propBuffer.currentIndex = 25;

    status = MQTT_ValidatePublishAckProperties( &propBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_Mqtt_PropertyBuilder_Init( void )
{
    MQTTPropBuilder_t ackPropsBuilder;
    uint8_t ackPropsBuf[ 500 ];
    size_t ackPropsBufLength = sizeof( ackPropsBuf );
    MQTTStatus_t mqttStatus;

    mqttStatus = MQTTPropertyBuilder_Init( &( ackPropsBuilder ), ackPropsBuf, ackPropsBufLength );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );

    mqttStatus = MQTTPropertyBuilder_Init( NULL, ackPropsBuf, ackPropsBufLength );
    TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropertyBuilder_Init( &( ackPropsBuilder ), NULL, ackPropsBufLength );
    TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );

    mqttStatus = MQTTPropertyBuilder_Init( &( ackPropsBuilder ), ackPropsBuf, 0 );
    TEST_ASSERT_EQUAL( MQTTBadParameter, mqttStatus );
}

void test_decodeSubackPropertyLength( void )
{
    size_t propertyLength = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 10 ] = { 0 };

    buffer[ 0 ] = 0;
    buffer[ 1 ] = 1;
    buffer[ 2 ] = 0; /*Length of the properties is 0. */
    buffer[ 3 ] = 0x00;
    buffer[ 4 ] = 0x01;
    buffer[ 5 ] = 0x02;
    buffer[ 6 ] = 0x80;

    status = decodeSubackPropertyLength( &buffer[ 2 ], 7, &propertyLength );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 1, propertyLength );

    buffer[ 2 ] = 2;
    buffer[ 3 ] = MQTT_PAYLOAD_FORMAT_ID;
    buffer[ 4 ] = 1;

    status = decodeSubackPropertyLength( &buffer[ 2 ], 7, &propertyLength );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 3, propertyLength );

    /*Invalid remaining length. */
    status = decodeSubackPropertyLength( &buffer[ 2 ], 2, &propertyLength );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    status = decodeSubackPropertyLength( &buffer[ 2 ], 3, &propertyLength );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );
}

void test_MQTT_InitConnect( void )
{
    MQTTStatus_t status = MQTTSuccess;

    status = MQTT_InitConnect( NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    MQTTConnectionProperties_t connectionProperties;
    status = MQTT_InitConnect( &connectionProperties );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
}
/* ========================================================================== */
