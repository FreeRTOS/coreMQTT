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
 * @brief MQTT protocol version 3.1.1.
 */
#define MQTT_VERSION_3_1_1                          ( ( uint8_t ) 4U )

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

#define   TEST_TOPIC_ALIAS  (2U)
#define   TEST_MSG_EXPIRY   (100U)


#define MQTT_TEST_UTF8_STRING ("test")
#define MQTT_TEST_UTF8_STRING_LENGTH (sizeof(MQTT_TEST_UTF8_STRING) -1)
#define MQTT_TEST_UINT8 (1U)
#define MQTT_TEST_UINT16 (32U)
#define MQTT_TEST_UINT32 (300U)

/* MQTT CONNECT flags. */
#define MQTT_CONNECT_FLAG_CLEAN                     ( 1 ) /**< @brief Clean session. */
#define MQTT_CONNECT_FLAG_WILL                      ( 2 ) /**< @brief Will present. */
#define MQTT_CONNECT_FLAG_WILL_QOS1                 ( 3 ) /**< @brief Will QoS 1. */
#define MQTT_CONNECT_FLAG_WILL_QOS2                 ( 4 ) /**< @brief Will QoS 2. */
#define MQTT_CONNECT_FLAG_WILL_RETAIN               ( 5 ) /**< @brief Will retain. */
#define MQTT_CONNECT_FLAG_PASSWORD                  ( 6 ) /**< @brief Password present. */
#define MQTT_CONNECT_FLAG_USERNAME                  ( 7 ) /**< @brief User name present. */

/*Default connect properties. */
#define DEFAULT_RECEIVE_MAX  (65535U)
#define DEFAULT_REQUEST_PROBLEM (1)

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
 * to MQTT 3.1.1 spec.
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
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) |  \
                   ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
                   ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) | \
                   ( ( uint32_t ) ptr[ 3 ] ) )

#define UINT32_BYTE3( x )    ( ( uint8_t ) ( ( x ) >> 24 ) )

#define UINT32_BYTE2( x )    ( ( uint8_t ) ( ( x ) >> 16 ) )

#define UINT32_BYTE1( x )    ( ( uint8_t ) ( ( x ) >> 8 ) )

#define UINT32_BYTE0( x )    ( ( uint8_t ) ( ( x ) & 0x000000FFU ) )

#define MQTT_VERSION_5                   ( 5U )
#define MQTT_SESSION_EXPIRY_SIZE         ( 5U )
#define MQTT_RECEIVE_MAX_SIZE            ( 3U )
#define MQTT_MAX_PACKET_SIZE             ( 5U )
#define MQTT_TOPIC_ALIAS_SIZE            ( 3U )
#define MQTT_REQUEST_RESPONSE_SIZE       ( 2U )
#define MQTT_REQUEST_PROBLEM_SIZE        ( 2U )

#define MQTT_SESSION_EXPIRY_ID           ( 0x11 )
#define MQTT_RECEIVE_MAX_ID              ( 0x21 )
#define MQTT_MAX_PACKET_SIZE_ID          ( 0x27 )
#define MQTT_TOPIC_ALIAS_MAX_ID          ( 0x22 )
#define MQTT_REQUEST_RESPONSE_ID         ( 0x19 )
#define MQTT_REQUEST_PROBLEM_ID          ( 0x17 )
#define MQTT_USER_PROPERTY_ID            ( 0x26 )
#define MQTT_AUTH_METHOD_ID              ( 0x15 )
#define MQTT_AUTH_DATA_ID                ( 0x16 )

#define MQTT_WILL_DELAY_ID               ( 0x18 )
#define MQTT_PAYLOAD_FORMAT_ID           ( 0x01 )
#define MQTT_MSG_EXPIRY_ID               ( 0x02 )
#define MQTT_CONTENT_TYPE_ID             ( 0x03 )
#define MQTT_RESPONSE_TOPIC_ID           ( 0x08 )
#define MQTT_CORRELATION_DATA_ID         ( 0x09 )

#define MQTT_MAX_QOS_ID                  ( 0x24 )
#define MQTT_RETAIN_AVAILABLE_ID         ( 0x25 )
#define MQTT_ASSIGNED_CLIENT_ID          ( 0x12 )
#define MQTT_REASON_STRING_ID            ( 0x1F )
#define MQTT_WILDCARD_ID                 ( 0x28 )
#define MQTT_SUB_AVAILABLE_ID            ( 0x29 )
#define MQTT_SHARED_SUB_ID               ( 0x2A )
#define MQTT_SERVER_KEEP_ALIVE_ID        ( 0x13 )
#define MQTT_RESPONSE_INFO_ID            ( 0x1A )
#define MQTT_SERVER_REF_ID               ( 0x1C )

#define CORE_MQTT_ID_SIZE                ( 1U )
#define MQTT_REMAINING_LENGTH_INVALID    ( ( size_t ) 268435456 )

static uint8_t remainingLengthBuffer[ MQTT_REMAINING_BUFFER_MAX_LENGTH ] = { 0 };

static uint8_t encodedStringBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

static uint8_t mqttBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };


/* Variables common to testcases */
MQTTConnectProperties_t properties;
MQTTUserProperties_t userProperties;
MQTTPublishInfo_t publishInfo;
MQTTConnectInfo_t connectInfo;
MQTTPacketInfo_t packetInfo;
MQTTStatus_t status;

/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp( void )
{
    memset(&properties, 0x0, sizeof(properties));
    memset(&userProperties, 0x0, sizeof(userProperties));
    memset(&publishInfo, 0x0, sizeof(publishInfo));
    memset(&connectInfo, 0x0, sizeof(connectInfo));
    memset(&packetInfo, 0x0, sizeof(packetInfo));
   
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
 * @brief Initialize pNetworkBuffer using static buffer.
 *
 * @param[in] pNetworkBuffer Network buffer provided for the context.
 */
static void setupNetworkBuffer( MQTTFixedBuffer_t * const pNetworkBuffer )
{
    pNetworkBuffer->pBuffer = mqttBuffer;
    pNetworkBuffer->size = 2048;
}

/**
 * @brief Initialize pConnectInfo using test-defined macros.
 *
 * @param[in] pConnectInfo MQTT CONNECT packet parameters.
 */
static void setupConnectInfo( MQTTConnectInfo_t * const pConnectInfo )
{
    pConnectInfo->cleanSession = true;
    pConnectInfo->pClientIdentifier = MQTT_CLIENT_IDENTIFIER;
    pConnectInfo->clientIdentifierLength = MQTT_CLIENT_IDENTIFIER_LEN;
    pConnectInfo->keepAliveSeconds = 0;
    pConnectInfo->pUserName = MQTT_TEST_USERNAME;
    pConnectInfo->userNameLength = MQTT_TEST_USERNAME_LEN;
    pConnectInfo->pPassword = MQTT_TEST_PASSWORD;
    pConnectInfo->passwordLength = MQTT_TEST_PASSWORD_LEN;
}

static size_t remainingLengthEncodedSize( size_t length )
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 3.1.1 spec. */

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

    LogDebug( ( "Encoded size for length %lu is %lu bytes.",
                ( unsigned long ) length,
                ( unsigned long ) encodedSize ) );

    return encodedSize;
}

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
 * @brief Initialize pPublishInfo using test-defined macros.
 *
 * @param[in] pPublishInfo Publish information.
 */
static void setupPublishProperties( MQTTPublishInfo_t * pPublishInfo )
{
    pPublishInfo->payloadFormat = 1;
    pPublishInfo->topicAlias = TEST_TOPIC_ALIAS;
    pPublishInfo->msgExpiryInterval = TEST_MSG_EXPIRY;
    pPublishInfo->msgExpiryPresent = 1;
    pPublishInfo->contentTypeLength = MQTT_TEST_UTF8_STRING_LENGTH;
    pPublishInfo->pContentType = MQTT_TEST_UTF8_STRING;
    pPublishInfo->responseTopicLength = MQTT_TEST_UTF8_STRING_LENGTH;
    pPublishInfo->pResponseTopic = MQTT_TEST_UTF8_STRING;
    pPublishInfo->correlationLength = MQTT_TEST_UTF8_STRING_LENGTH;
    pPublishInfo->pCorrelationData = MQTT_TEST_UTF8_STRING;
}

/**
 * @brief Encode remaining length into pDestination for packet serialization
 * using MQTT v3.1.1 spec.
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

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
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

static void setupProperties( MQTTConnectProperties_t * pProperties )
{
    pProperties->receiveMax = DEFAULT_RECEIVE_MAX;
    pProperties->requestProblemInfo = DEFAULT_REQUEST_PROBLEM;
}

static uint8_t * initializeDeserialize(MQTTPacketInfo_t *packetInfo,uint8_t *pIndex)
{
    uint8_t * pIndexLocal = pIndex;
    packetInfo->pRemainingData = pIndexLocal;
    packetInfo->type = MQTT_PACKET_TYPE_CONNACK; 
    *pIndexLocal = 0x01;
    pIndexLocal++;
    *pIndexLocal  = 0x00;
    pIndexLocal++;
    return pIndexLocal;
}

static uint8_t * serializeuint_32(uint8_t *pIndex, uint8_t propertyId)
{
    uint8_t *pIndexLocal = pIndex;
    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT32_BYTE3( MQTT_TEST_UINT32 );
    pIndexLocal[ 1 ] = UINT32_BYTE2( MQTT_TEST_UINT32 );
    pIndexLocal[ 2 ] = UINT32_BYTE1( MQTT_TEST_UINT32 );
    pIndexLocal[ 3 ] = UINT32_BYTE0( MQTT_TEST_UINT32 );
    pIndexLocal = &pIndexLocal[ 4 ]; 
    return pIndexLocal;
}


static uint8_t * serializeuint_16(uint8_t *pIndex,uint8_t propertyId)
{
    uint8_t *pIndexLocal = pIndex;
    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( MQTT_TEST_UINT16 );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( MQTT_TEST_UINT16 );
    pIndexLocal = &pIndexLocal[ 2 ];
    return pIndexLocal;
}

static uint8_t * serializeuint_8(uint8_t *pIndex,uint8_t propertyId)
{   
    uint8_t *pIndexLocal = pIndex;
    *pIndexLocal = propertyId;
    pIndexLocal++;
    pIndexLocal[ 0 ] = MQTT_TEST_UINT8;
    pIndexLocal++;
    return pIndexLocal;
}
static uint8_t * serializeutf_8(uint8_t *pIndex, uint8_t propertyId)
{
    uint8_t *pIndexLocal = pIndex;
    *pIndexLocal = propertyId;
    pIndexLocal++;
    size_t dummy = encodeString( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}

static uint8_t * serializeutf_8pair(uint8_t *pIndex){
    uint8_t* pIndexLocal = pIndex;
    *pIndexLocal = MQTT_USER_PROPERTY_ID;
    pIndexLocal++;
    size_t dummy = encodeString( pIndexLocal,MQTT_TEST_UTF8_STRING,MQTT_TEST_UTF8_STRING_LENGTH);
    pIndexLocal = &pIndexLocal[ dummy ];
    dummy = encodeString( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}
static MQTTStatus_t MQTT_GetUserPropertySize( const MQTTUserProperty_t * pUserProperty,
                                              uint32_t number,
                                              size_t * pSize )
{
    MQTTStatus_t status = MQTTSuccess;
    uint32_t i = 0;

    /*Number of user properties can't be more than the max user properties specified*/
    if( number > ( uint32_t ) MAX_USER_PROPERTY )
    {
        status = MQTTBadParameter;
    }
    else
    {
        for( ; ( i < number ); i++ )
        {
            /*Validate the key and value*/
            if( ( pUserProperty[ i ].keyLength == 0U ) || ( pUserProperty[ i ].valueLength == 0U ) || ( pUserProperty[ i ].pKey == NULL ) || ( pUserProperty[ i ].pValue == NULL ) )
            {
                status = MQTTBadParameter;
                break;
            }
            else
            {
                *pSize += ( pUserProperty[ i ].keyLength );
                *pSize += 3U;
                *pSize += ( pUserProperty[ i ].valueLength );
                *pSize += 2U;
            }
        }
    }

    return status;
}

static MQTTStatus_t MQTT_GetPublishPropertiesSize( MQTTPublishInfo_t * pPublishProperties )
{
    size_t propertyLength = 0U;
    MQTTStatus_t status = MQTTSuccess;

    /*Add the length of all the parameters which are applicable*/
    if( pPublishProperties->willDelay != 0U )
    {
        propertyLength += 5U;
    }

    if( pPublishProperties->payloadFormat != 0U )
    {
        propertyLength += 2U;
    }

    if( pPublishProperties->msgExpiryPresent == true )
    {
        propertyLength += 5U;
    }

    if( pPublishProperties->contentTypeLength != 0U )
    {
        if( pPublishProperties->pContentType == NULL )
        {
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength += pPublishProperties->contentTypeLength + 3U;
        }
    }

    /*Validate if length and pointers are valid*/
    if( ( status == MQTTSuccess ) && ( pPublishProperties->responseTopicLength != 0U ) )
    {
        if( pPublishProperties->pResponseTopic == NULL )
        {
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength += pPublishProperties->responseTopicLength + 3U;
        }
    }

    if( ( status == MQTTSuccess ) && ( pPublishProperties->correlationLength != 0U ) )
    {
        if( pPublishProperties->pCorrelationData == NULL )
        {
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength += pPublishProperties->correlationLength + 3U;
        }
    }

    /*Get the length of user properties*/
    if( ( status == MQTTSuccess ) && ( pPublishProperties->pUserProperty != NULL ) )
    {
        status = MQTT_GetUserPropertySize( pPublishProperties->pUserProperty->userProperty, pPublishProperties->pUserProperty->count, &propertyLength );
    }

    /*Variable encoded can't have a value more than 268435455UL*/
    if( propertyLength > MQTT_MAX_REMAINING_LENGTH )
    {
        status = MQTTBadParameter;
    }

    if( status == MQTTSuccess )
    {
        pPublishProperties->propertyLength = propertyLength;
    }

    return status;
}

static uint8_t * serializeUserProperties( uint8_t * pIndex,
                                          const MQTTUserProperty_t * pUserProperty,
                                          uint16_t size )
{
    uint16_t i = 0;

    assert( pIndex != NULL );
    uint8_t * pIndexLocal = pIndex;
    uint8_t dummy;

    for( ; i < size; i++ )
    {
        *pIndexLocal = MQTT_USER_PROPERTY_ID;
        pIndexLocal++;
        dummy = encodeString( pIndexLocal, ( pUserProperty + i )->pKey, ( pUserProperty + i )->keyLength );
        dummy = encodeString( pIndexLocal, ( pUserProperty + i )->pValue, ( pUserProperty + i )->valueLength );
    }

    i = dummy;
    return pIndexLocal;
}

MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                   size_t * length )
{
    size_t remainingLength = 0;
    size_t multiplier = 1;
    size_t bytesDecoded = 0;
    size_t expectedSize = 0;
    uint8_t encodedByte = 0;
    MQTTStatus_t status = MQTTSuccess;

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
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
            /* Get the next byte. It is at the next position after the bytes
             * decoded till now since the header of one byte was read before. */
            encodedByte = pBuffer[ bytesDecoded ];
            remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
            multiplier *= 128U;
            bytesDecoded++;
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
        expectedSize = remainingLengthEncodedSize( remainingLength );

        if( bytesDecoded != expectedSize )
        {
            LogError( ( "Expected and actual length of decoded bytes do not match.\n" ) );
            status = MQTTBadResponse;
        }
        else
        {
            *length = remainingLength;
        }
    }

    return status;
}

static void verifySerializedConnectPacket( const MQTTConnectInfo_t * const pConnectInfo,
                                           const MQTTPublishInfo_t * const pWillInfo,
                                           const MQTTConnectProperties_t * pConnectProperties,
                                           size_t remainingLength,
                                           const MQTTFixedBuffer_t * const pBuffer )
{
    uint8_t connectFlags = 0U;
    uint8_t encodedRemainingLength = 0U;
    uint8_t encodedStringLength = 0U;
    uint8_t * pIndex = NULL;

    pIndex = pBuffer->pBuffer;
    /* The first byte in the CONNECT packet is the control packet type. */
    TEST_ASSERT_EQUAL_MESSAGE( MQTT_PACKET_TYPE_CONNECT, *pIndex, "MQTT_PACKET_TYPE_CONNECT is not equal to *pIndex" );
    pIndex++;

    /* The remaining length of the CONNECT packet is encoded starting from the
     * second byte. The remaining length does not include the length of the fixed
     * header or the encoding of the remaining length. */
    encodedRemainingLength = encodeRemainingLength( remainingLengthBuffer, remainingLength );
    TEST_ASSERT_EQUAL_MEMORY( remainingLengthBuffer, pIndex, encodedRemainingLength );
    pIndex += encodedRemainingLength;

    /* The string "MQTT" is placed at the beginning of the CONNECT packet's variable
     * header. This string is 4 bytes long. */
    encodedStringLength = encodeString( encodedStringBuffer, "MQTT", 4 );
    TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    pIndex += encodedStringLength;

    /* The MQTT protocol version is the second field of the variable header. */
    TEST_ASSERT_EQUAL( 5, *pIndex );
    pIndex++;

    /* Set the clean session flag if needed. */
    if( pConnectInfo->cleanSession == true )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_CLEAN );
    }

    /* Set the flags for username and password if provided. */
    if( pConnectInfo->pUserName != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_USERNAME );
    }

    if( pConnectInfo->pPassword != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_PASSWORD );
    }

    /* Set will flag if a Last Will and Testament is provided. */
    if( pWillInfo != NULL )
    {
        UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL );

        /* Flags only need to be changed for Will QoS 1 or 2. */
        if( pWillInfo->qos == MQTTQoS1 )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_QOS1 );
        }
        else if( pWillInfo->qos == MQTTQoS2 )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_QOS2 );
        }
        else
        {
            /* Empty else MISRA 15.7 */
        }

        if( pWillInfo->retain == true )
        {
            UINT8_SET_BIT( connectFlags, MQTT_CONNECT_FLAG_WILL_RETAIN );
        }
    }

    TEST_ASSERT_EQUAL( connectFlags, *pIndex );
    pIndex++;

    /* Verify the 2 bytes of the keep alive interval into the CONNECT packet. */
    TEST_ASSERT_EQUAL( UINT16_HIGH_BYTE( pConnectInfo->keepAliveSeconds ),
                       *pIndex );
    pIndex++;
    TEST_ASSERT_EQUAL( UINT16_LOW_BYTE( pConnectInfo->keepAliveSeconds ),
                       *pIndex );
    pIndex++;
    /* Verify the connect properties into the CONNECT packet. */
    pIndex = MQTT_SerializeConnectProperties( pIndex, pConnectProperties );

    if( pConnectProperties->pOutgoingUserProperty != NULL )
    {
        pIndex = serializeUserProperties( pIndex, pConnectProperties->pOutgoingUserProperty->userProperty, pConnectProperties->pOutgoingUserProperty->count );
    }

    if( pConnectProperties->pOutgoingAuth != NULL )
    {
        if( pConnectProperties->pOutgoingAuth->authMethodLength != 0 )
        {
            encodedStringLength = encodeString( encodedStringBuffer,
                                                pConnectProperties->pOutgoingAuth->pAuthMethod,
                                                pConnectProperties->pOutgoingAuth->authMethodLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;

            if( pConnectProperties->pOutgoingAuth->authDataLength != 0 )
            {
                encodedStringLength = encodeString( encodedStringBuffer,
                                                    pConnectProperties->pOutgoingAuth->pAuthData,
                                                    pConnectProperties->pOutgoingAuth->authDataLength );
                TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
                pIndex += encodedStringLength;
            }
        }
    }

    /* Verify the client identifier into the CONNECT packet. */
    encodedStringLength = encodeString( encodedStringBuffer,
                                        pConnectInfo->pClientIdentifier,
                                        pConnectInfo->clientIdentifierLength );
    TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    pIndex += encodedStringLength;

    /* Verify the will topic name and message into the CONNECT packet if provided. */
    if( pWillInfo != NULL )
    {
        /*Will Properties*/
        pIndex = MQTT_SerializePublishProperties( pWillInfo, pIndex );

        if( pWillInfo->contentTypeLength != 0U )
        {
            TEST_ASSERT_EQUAL_INT( MQTT_CONTENT_TYPE_ID, *pIndex );
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                                pWillInfo->pContentType,
                                                pWillInfo->contentTypeLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }

        if( pWillInfo->responseTopicLength != 0U )
        {
            TEST_ASSERT_EQUAL_INT( MQTT_RESPONSE_TOPIC_ID, *pIndex );
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                                pWillInfo->pResponseTopic,
                                                pWillInfo->responseTopicLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }

        if( pWillInfo->correlationLength != 0U )
        {
            TEST_ASSERT_EQUAL_INT( MQTT_CORRELATION_DATA_ID, *pIndex );
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                                pWillInfo->pCorrelationData,
                                                pWillInfo->correlationLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }

        if( pWillInfo->pUserProperty != NULL )
        {
            pIndex = serializeUserProperties( pIndex, pWillInfo->pUserProperty->userProperty, pWillInfo->pUserProperty->count );
        }

        encodedStringLength = encodeString( encodedStringBuffer,
                                            pWillInfo->pTopicName,
                                            pWillInfo->topicNameLength );
        TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
        pIndex += encodedStringLength;
        encodedStringLength = encodeString( encodedStringBuffer,
                                            pWillInfo->pPayload,
                                            ( uint16_t ) pWillInfo->payloadLength );
        TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
        pIndex += encodedStringLength;
    }

    /* Verify the user name if provided. */
    if( pConnectInfo->pUserName != NULL )
    {
        encodedStringLength = encodeString( encodedStringBuffer,
                                            pConnectInfo->pUserName,
                                            pConnectInfo->userNameLength );
        TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
        pIndex += encodedStringLength;
    }

    /* Verify the password if provided. */
    if( pConnectInfo->pPassword != NULL )
    {
        encodedStringLength = encodeString( encodedStringBuffer,
                                            pConnectInfo->pPassword,
                                            pConnectInfo->passwordLength );
        TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
        pIndex += encodedStringLength;
    }
}

void test_MQTT_GetPublishPropertiesSize( void )
{
    MQTTStatus_t status = MQTTSuccess;

    /* Call MQTT_GetPublishPropertiesSize() with various combinations of
     * incorrect paramters */

    /*
     * Max Packet Size cannot be null
     */

    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 0, publishInfo.propertyLength );

    publishInfo.willDelay = 10;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 5, publishInfo.propertyLength );

    publishInfo.payloadFormat = 1;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 7, publishInfo.propertyLength );

    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 12, publishInfo.propertyLength );

    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 12, publishInfo.propertyLength );

    publishInfo.contentTypeLength = MQTT_TEST_UTF8_STRING_LENGTH;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 19, publishInfo.propertyLength );

    publishInfo.responseTopicLength = MQTT_TEST_UTF8_STRING_LENGTH;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 26, publishInfo.propertyLength );

    publishInfo.correlationLength = MQTT_TEST_UTF8_STRING_LENGTH;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 33, publishInfo.propertyLength );

    MQTTUserProperties_t userProperties;
    userProperties.userProperty[ 0 ].pKey = MQTT_TEST_UTF8_STRING;
    userProperties.userProperty[ 0 ].keyLength = MQTT_TEST_UTF8_STRING_LENGTH;
    userProperties.userProperty[ 0 ].valueLength = MQTT_TEST_UTF8_STRING_LENGTH;
    userProperties.userProperty[ 0 ].pValue =  MQTT_TEST_UTF8_STRING;
    userProperties.userProperty[ 1 ].pKey = MQTT_TEST_UTF8_STRING;
    userProperties.userProperty[ 1 ].keyLength = MQTT_TEST_UTF8_STRING_LENGTH;
    userProperties.userProperty[ 1 ].valueLength = MQTT_TEST_UTF8_STRING_LENGTH;
    userProperties.userProperty[ 1 ].pValue = MQTT_TEST_UTF8_STRING;
    publishInfo.pUserProperty = &userProperties;
    userProperties.count = 2;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 59, publishInfo.propertyLength );
}

void test_MQTT_SerializeConnectProperties( void )
{
    uint8_t properties[ 24U ];
    uint8_t * pIndex = properties;
    uint8_t * index = properties;
    MQTTConnectProperties_t connect;
    
    size_t propertyLength;

    memset( &connect, 0x0, sizeof( connect ) );
    connect.sessionExpiry = 12;
    connect.receiveMax = 32;
    connect.isMaxPacketSize = true;
    connect.maxPacketSize = 56;
    connect.topicAliasMax = 11;
    connect.requestResponseInfo = 1;
    connect.requestProblemInfo = 0;
    connect.propertyLength = 20;
    pIndex = MQTT_SerializeConnectProperties( pIndex, &connect );
    TEST_ASSERT_EQUAL_INT( 21, ( pIndex - properties ) );
    status = decodeVariableLength( properties, &propertyLength );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( connect.propertyLength, propertyLength );
    index++;

    TEST_ASSERT_EQUAL_INT( MQTT_SESSION_EXPIRY_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_UINT32( connect.sessionExpiry, UINT32_DECODE( index ) );
    index += 4;

    TEST_ASSERT_EQUAL_INT( MQTT_RECEIVE_MAX_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_UINT32( connect.receiveMax, UINT16_DECODE( index ) );
    index += 2;

    TEST_ASSERT_EQUAL_INT( MQTT_MAX_PACKET_SIZE_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_INT( connect.maxPacketSize, UINT32_DECODE( index ) );
    index += 4;

    TEST_ASSERT_EQUAL_INT( MQTT_TOPIC_ALIAS_MAX_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_INT( connect.topicAliasMax, UINT16_DECODE( index ) );
    index += 2;

    TEST_ASSERT_EQUAL_INT( MQTT_REQUEST_RESPONSE_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_INT( connect.requestResponseInfo, *index );
    index++;

    TEST_ASSERT_EQUAL_INT( MQTT_REQUEST_PROBLEM_ID, *index );
    index++;
    TEST_ASSERT_EQUAL_INT( connect.requestProblemInfo, *index );
    index++;
}


void test_MQTTV5_DeserializeConnackOnlyStatus( void )
{
    
    uint8_t buffer[ 50 ];
    uint8_t * pIndex = buffer;
    MQTTUserProperties_t incomingProperty;
    properties.pIncomingUserProperty = &incomingProperty;
    status = MQTTV5_DeserializeConnack( NULL, NULL, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    bool sessionPresent;
    status = MQTTV5_DeserializeConnack( NULL, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_DeserializeConnack( &properties, NULL, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    packetInfo.pRemainingData = pIndex;
    packetInfo.type = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    /*Reserved bit incorrect*/
    buffer[ 0 ] = 0x11;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /*
     * Session Present Bit is set but reason code is not equal to 0;
     */
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x01;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadResponse, status );

    /* 5 + 1 + 2 = 8 */
    size_t propertyLength = encodeRemainingLength( pIndex, 5 );
    packetInfo.remainingLength = propertyLength + 7;
    /*Not a valid reason code*/
    buffer[ 0 ] = 0x00;
    buffer[ 1 ] = 0x03;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTProtocolError, status );
    /*All the valid response code*/
    buffer[ 1 ] = 0x80;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x81;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x82;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x83;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x84;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x80;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x85;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x86;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x87;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x88;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x89;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x8A;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x8C;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x88;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x90;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x95;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x97;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x99;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9A;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9A;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9B;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9C;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9D;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    buffer[ 1 ] = 0x9F;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTServerRefused, status );

    /*Exceeds the max packet size set by the client*/
    properties.maxPacketSize = 2;
    properties.isMaxPacketSize = 1;
    buffer[ 1 ] = 0x00;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTProtocolError, status );

    /*Validate the remaining length*/
    properties.isMaxPacketSize = false;
    packetInfo.remainingLength = 7;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTMalformedPacket, status );

    /*Invalid property length*/
    properties.isMaxPacketSize = 200;
    packetInfo.remainingLength = 20;
    pIndex = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndex, 20971556356235 );
    LogDebug( ( "Encoded size for length is %lu bytes.",
                ( unsigned long ) propertyLength ) );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid property length*/
    pIndex = &buffer[ 2 ];
    *pIndex = 0x81;
     pIndex++;
    *pIndex = 0x00;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Incoming user property not initialized*/
    properties.pIncomingUserProperty = NULL;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &sessionPresent );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    
}

void test_MQTTV5_DeserializeConnackOnlyuint_32( void )
{
    uint8_t buffer[200] = {0};
    bool session = false;
    uint8_t * pIndexLocal = initializeDeserialize(&packetInfo,buffer);
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );
    packetInfo.remainingLength = propertyLength + 12;
    properties.isMaxPacketSize = true;
    properties.maxPacketSize = 150;
    properties.pIncomingUserProperty = &userProperties;
    pIndexLocal++;
    pIndexLocal = serializeuint_32(pIndexLocal, MQTT_SESSION_EXPIRY_ID);
    pIndexLocal = serializeuint_32(pIndexLocal, MQTT_MAX_PACKET_SIZE_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.sessionExpiry );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT32, properties.serverMaxPacketSize );

    /*Protocol error to include the same property twice*/
    packetInfo.remainingLength = 13;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 10 );
    pIndexLocal++;
    pIndexLocal = serializeuint_32(pIndexLocal, MQTT_SESSION_EXPIRY_ID);
    pIndexLocal = serializeuint_32(pIndexLocal, MQTT_SESSION_EXPIRY_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 7;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    pIndexLocal++;
    *pIndexLocal = MQTT_SESSION_EXPIRY_ID;
    pIndexLocal++;
    pIndexLocal = serializeuint_32(pIndexLocal, MQTT_SESSION_EXPIRY_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid id*/
    packetInfo.remainingLength = 8;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 5 );
    pIndexLocal++;
    pIndexLocal = serializeuint_32(pIndexLocal, 0x00);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

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
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );



}

void test_MQTTV5_DeserializeConnackOnlyuint_16( void )
{
    
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;
    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    pIndexLocal = &buffer[ 2 ];
    properties.pIncomingUserProperty = &userProperties;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 9 );
    packetInfo.remainingLength = propertyLength + 11;
    pIndexLocal++;
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_RECEIVE_MAX_ID);
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_TOPIC_ALIAS_MAX_ID);
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_SERVER_KEEP_ALIVE_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverReceiveMax );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverTopicAliasMax );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT16, properties.serverKeepAlive);

    /*Receive Max cannot have a value 0*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 3 );
    packetInfo.remainingLength = propertyLength + 5;
    pIndexLocal++;
    *pIndexLocal = MQTT_RECEIVE_MAX_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( 0 );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( 0);
    pIndexLocal = &pIndexLocal[ 2 ];
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Protocol error to include the same property twice*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 6 );
    packetInfo.remainingLength = propertyLength + 8;
    pIndexLocal++;
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_RECEIVE_MAX_ID);
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_RECEIVE_MAX_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    pIndexLocal = serializeuint_16(pIndexLocal,MQTT_RECEIVE_MAX_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );
}

void test_MQTTV5_DeserializeConnackOnlyuint_8( void )
{
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;
    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 13;
    pIndexLocal = &buffer[ 2 ];
    properties.pIncomingUserProperty = &userProperties;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 10 );
    pIndexLocal++;
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_MAX_QOS_ID);
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_RETAIN_AVAILABLE_ID);
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_WILDCARD_ID);
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_SHARED_SUB_ID);
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_SUB_AVAILABLE_ID);

    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.serverMaxQos );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.retainAvailable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isWildcardAvaiable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.isSharedAvailable );
    TEST_ASSERT_EQUAL_INT( MQTT_TEST_UINT8, properties.subscriptionId );

    /*Protocol error to have a value other than 0 or 1*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    *pIndexLocal = MQTT_MAX_QOS_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = 3;
    pIndexLocal++;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Protocol error to include the same property twice*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    packetInfo.remainingLength = propertyLength + 6;
    pIndexLocal++;
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_MAX_QOS_ID);
    pIndexLocal = serializeuint_8(pIndexLocal,MQTT_MAX_QOS_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 4;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 1 );
    pIndexLocal++;
    *pIndexLocal = MQTT_MAX_QOS_ID;
    pIndexLocal++;
    pIndexLocal[ 0 ] = 0;
    pIndexLocal++;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );


}


void test_MQTTV5_DeserializeConnackOnlyutf_8( void )
{
    
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;

    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    pIndexLocal = &buffer[ 2 ];
    properties.pIncomingUserProperty = &userProperties;
    properties.requestResponseInfo = 1;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 28 );
    packetInfo.remainingLength = propertyLength + 28 + 2;
    pIndexLocal++;
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_ASSIGNED_CLIENT_ID);
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_REASON_STRING_ID);
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_RESPONSE_INFO_ID);
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_SERVER_REF_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Protocol error to include the same property twice*/
    packetInfo.remainingLength = 17;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 14 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_ASSIGNED_CLIENT_ID);
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_ASSIGNED_CLIENT_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 7;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 4 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_ASSIGNED_CLIENT_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    packetInfo.remainingLength = propertyLength + 4;
    pIndexLocal++;
    serializeutf_8(pIndexLocal,MQTT_ASSIGNED_CLIENT_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Protocol error to include response information if is is set to false by client*/
    properties.requestResponseInfo = 0;
    packetInfo.remainingLength = 10;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 7);
    pIndexLocal++;
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_RESPONSE_INFO_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );
}


void test_MQTTV5_DeserializeConnackOnlyUserProperty( void )
{
    properties.pIncomingUserProperty = &userProperties;
    uint8_t buffer[ 70000 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;

    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 16;
    pIndexLocal = &buffer[ 2 ];
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 13 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( 1, properties.pIncomingUserProperty->count );
    TEST_ASSERT_EQUAL( MQTT_TEST_UTF8_STRING_LENGTH, ( properties.pIncomingUserProperty->userProperty[ 0 ].valueLength ) );
    TEST_ASSERT_EQUAL( MQTT_TEST_UTF8_STRING_LENGTH, ( properties.pIncomingUserProperty->userProperty[ 0 ].keyLength ) );

    /*Invalid property length*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 6;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 3 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 8 );
    packetInfo.remainingLength = propertyLength + 10;
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 15;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 12 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Discard user property*/
    packetInfo.remainingLength = 65018;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 65013 );
    pIndexLocal += 3;
    uint32_t i = 0U;

    for( ; i < 5001; i++ )
    {
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    }

    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    packetInfo.remainingLength = 65017;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 65012 );
    pIndexLocal += 3;

    for( ; i < 5001; i++ )
    {
    pIndexLocal = serializeutf_8pair(pIndexLocal);
    }

    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );
}



void test_MQTTV5_DeserializeConnackOnlyAuthInfo( void )
{
    MQTTAuthInfo_t auth;
    MQTTAuthInfo_t auth1;
    properties.pIncomingAuth = &auth;
    properties.pOutgoingAuth = &auth1;
    uint8_t buffer[ 200 ] = { 0 };
    uint8_t * pIndexLocal = buffer;
    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;

    bool session = false;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 17;
    pIndexLocal = &buffer[ 2 ];
    properties.pIncomingUserProperty = &userProperties;
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 14 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_AUTH_METHOD_ID);
    pIndexLocal = serializeutf_8(pIndexLocal,MQTT_AUTH_DATA_ID);
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TEST_UTF8_STRING_LENGTH, properties.pIncomingAuth->authMethodLength );
    TEST_ASSERT_EQUAL( MQTT_TEST_UTF8_STRING_LENGTH, properties.pIncomingAuth->authDataLength );
    

    /*Outgoing auth is null*/
    properties.pOutgoingAuth = NULL;
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );


}


void test_MQTTV5_GetConnectPacketSize( void )
{
    size_t remainingLength = 0;
    size_t packetSize = 0;
    MQTTUserProperties_t incomingProperty;


    /* Call MQTTV5_GetConnectPacketSize() with various combinations of
     * incorrect paramters */
    properties.receiveMax = 65535U;
    properties.maxPacketSize = UINT32_MAX;
    properties.requestProblemInfo = 1;
    properties.pIncomingUserProperty = &incomingProperty;
    status = MQTTV5_GetConnectPacketSize( NULL, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, NULL, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify empty connect info fails. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify empty client identifier fails. */
    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    connectInfo.pClientIdentifier = NULL;
    connectInfo.clientIdentifierLength = CLIENT_IDENTIFIER_LENGTH;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, NULL, &remainingLength, &packetSize );
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
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Verify good case */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.cleanSession = true;
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 0, properties.propertyLength );
    /* Make sure remaining size returned is 17. */
    TEST_ASSERT_EQUAL_INT( 17, remainingLength );
    /* Make sure packet size is 19. */
    TEST_ASSERT_EQUAL_INT( 19, packetSize );

    /* With will. These parameters will cause the packet to be
     * 4 + 2 + 8 + 2 = 16 bytes larger. */
    publishInfo.pTopicName = "test";
    publishInfo.topicNameLength = 4;
    publishInfo.pPayload = "testload";
    publishInfo.payloadLength = 8;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
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
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 28 = 16 + 12. */
    TEST_ASSERT_EQUAL_INT( 29, remainingLength );
    /* Make sure packet size is 30 = 18 + 12. */
    TEST_ASSERT_EQUAL_INT( 31, packetSize );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, NULL, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Validating the function Get Connect properties*/

    /* Call MQTT_GetConnectPropertiesSize() with various combinations of
     * incorrect paramters */

    /*
     * Max Packet Size cannot be null
     */
    properties.receiveMax = 24;
    properties.isMaxPacketSize = true;
    properties.maxPacketSize = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*
     * Receive Maximum cannot be 0
     */
    properties.isMaxPacketSize = true;
    properties.maxPacketSize = 40;
    properties.receiveMax = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    properties.receiveMax = 24;
    properties.requestProblemInfo = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 8, properties.propertyLength );

    properties.sessionExpiry = 24;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 13, properties.propertyLength );

    properties.topicAliasMax = 24;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 16, properties.propertyLength );

    properties.requestResponseInfo = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 18, properties.propertyLength );

    properties.requestProblemInfo = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );

    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 20, properties.propertyLength );

    MQTTUserProperties_t userProperties;
    memset( &userProperties, 0x0, sizeof( userProperties ) );

    userProperties.count = 1;
    properties.pOutgoingUserProperty = &userProperties;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].keyLength = 3;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].valueLength = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].pValue = "1";
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].pKey = "2";
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );

    userProperties.userProperty[ 0 ].pValue = NULL;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].keyLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].valueLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].keyLength = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.userProperty[ 0 ].valueLength = 1;
    userProperties.userProperty[ 0 ].keyLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    properties.pOutgoingUserProperty->count = 6000;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    properties.pOutgoingUserProperty->count = 1;

    /*
     * Incoming AuthInfo not intialized.
     */
    properties.pOutgoingUserProperty = &userProperties;
    MQTTAuthInfo_t auth;
    memset( &auth, 0x0, sizeof( auth ) );
    properties.pOutgoingAuth = &auth;
    auth.authDataLength = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    MQTTAuthInfo_t auth2;
    memset( &auth2, 0x0, sizeof( auth2 ) );
    properties.pIncomingAuth = &auth2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    auth.authDataLength = 0;
    auth.authMethodLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*
     *  Protocol Error to include Authentication Data if there is no Authentication Method
     */
    auth.pAuthData = "1";
    auth.authDataLength = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    auth.authMethodLength = 3;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    auth.pAuthMethod = "234";
    auth.authDataLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    auth.pAuthData = NULL;
    auth.authDataLength = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Connect Properties*/
    memset( &properties, 0x0, sizeof( properties ) );
    properties.sessionExpiry = 22;
    properties.receiveMax = 34;
    properties.isMaxPacketSize = true;
    properties.maxPacketSize = 32;
    properties.topicAliasMax = 12;
    properties.requestResponseInfo = 1;
    properties.requestProblemInfo = 0;
    userProperties.userProperty[ 0 ].keyLength = 3;
    userProperties.userProperty[ 0 ].valueLength = 1;
    userProperties.userProperty[ 0 ].pValue = "1";
    userProperties.userProperty[ 0 ].pKey = "211";
    userProperties.count = 1;
    properties.pOutgoingUserProperty = &userProperties;
    properties.pIncomingUserProperty = &incomingProperty;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 58. */
    TEST_ASSERT_EQUAL_INT( 58, remainingLength );
    /* Make sure packet size is 60. */
    TEST_ASSERT_EQUAL_INT( 60, packetSize );

    /*Validating the will properties*/
    memset( &publishInfo, 0x0, sizeof( publishInfo ) );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 0, publishInfo.propertyLength );

    publishInfo.willDelay = 10;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 5, publishInfo.propertyLength );

    publishInfo.payloadFormat = 1;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 7, publishInfo.propertyLength );

    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 12, publishInfo.propertyLength );

    publishInfo.contentTypeLength = 2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 17, publishInfo.propertyLength );

    publishInfo.responseTopicLength = 2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 22, publishInfo.propertyLength );

    publishInfo.correlationLength = 2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 27, publishInfo.propertyLength );

    MQTTUserProperties_t userProperties1;
    userProperties1.userProperty[ 0 ].pKey = "2";
    userProperties1.userProperty[ 0 ].keyLength = 1;
    userProperties1.userProperty[ 0 ].valueLength = 3;
    userProperties1.userProperty[ 0 ].pValue = "abc";
    userProperties1.userProperty[ 1 ].pKey = "2";
    userProperties1.userProperty[ 1 ].keyLength = 1;
    userProperties1.userProperty[ 1 ].valueLength = 2;
    userProperties1.userProperty[ 1 ].pValue = MQTT_TEST_UTF8_STRING;
    userProperties1.count = 2;
    publishInfo.pUserProperty = &userProperties1;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_size_t( 44, publishInfo.propertyLength );

    publishInfo.payloadFormat = 1;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.contentTypeLength = 2;
    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    publishInfo.responseTopicLength = 2;
    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    publishInfo.correlationLength = 2;
    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    publishInfo.willDelay = 3;
    /* 34 + 12 + 29 */
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 107. */
    TEST_ASSERT_EQUAL_INT( 107, remainingLength );
    /* Make sure packet size is 109. */
    TEST_ASSERT_EQUAL_INT( 109, packetSize );
    /*Limit of property length*/
    memset( &properties, 0x0, sizeof( properties ) );
    memset( &publishInfo, 0x0, sizeof( publishInfo ) );
    /*5*/
    properties.receiveMax = UINT16_MAX;
    properties.requestProblemInfo = 1;
    userProperties.count = 2078;
    properties.pOutgoingUserProperty = &userProperties;
    properties.pIncomingUserProperty = &incomingProperty;
    uint16_t i = 0;
    char str[ 65535 ];
    memset( str, '.', 65535 * sizeof( char ) );

    for( ; i < 3500; i++ )
    {
        userProperties.userProperty[ i ].keyLength = UINT16_MAX;
        userProperties.userProperty[ i ].pKey = str;
        userProperties.userProperty[ i ].pValue = str;
        userProperties.userProperty[ i ].valueLength = UINT16_MAX;
    }

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    properties.pOutgoingUserProperty = NULL;
    userProperties.count = 2048;
    publishInfo.pUserProperty = &userProperties;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperties.count = 2051;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Incoming user property not initialized*/
    properties.pIncomingUserProperty = NULL;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

/* / ** */
/*  * @brief Tests that MQTTV5_SerializeConnect works as intended. */
/*  * / */
void test_MQTTV5_SerializeConnect( void )
{
    size_t remainingLength = 0;
    uint8_t buffer[ 140 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTUserProperties_t incomingProperty;
    properties.pIncomingUserProperty = &incomingProperty;
    setupProperties(&properties);
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };

    /* Verify bad parameter errors. */
    status = MQTTV5_SerializeConnect( NULL, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, 112345, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /* Create a good connection info. */
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;

    /* Inject a invalid fixed buffer test with a good connectInfo. */
    memset( &fixedBuffer, 0x0, sizeof( fixedBuffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Good case succeeds. */
    /* Set the fixedBuffer properly for the rest of the succeeding test. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];
    fixedBuffer.size = bufferSize;

    /* Calculate a good packet size. */
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Make sure test succeeds. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Encode user name. Also try clean session. */
    connectInfo.cleanSession = true;
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 4;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Serialize connect with LWT. */
    /* Test for NULL topic name. */
    ( void ) memset( &publishInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    publishInfo.retain = true;
    publishInfo.qos = MQTTQoS1;
    publishInfo.pPayload = "test";
    publishInfo.payloadLength = ( uint16_t ) strlen( publishInfo.pPayload );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Success. */
    ( void ) memset( &publishInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    publishInfo.retain = true;
    publishInfo.qos = MQTTQoS1;
    publishInfo.pTopicName = "test";
    publishInfo.topicNameLength = ( uint16_t ) strlen( publishInfo.pTopicName );
    publishInfo.pPayload = "test";
    publishInfo.payloadLength = ( uint16_t ) strlen( publishInfo.pPayload );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Again with QoS 2 and 0. */

    publishInfo.qos = MQTTQoS2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    publishInfo.qos = MQTTQoS0;
    publishInfo.retain = false;
    /* NULL payload is acceptable. */
    publishInfo.pPayload = NULL;
    publishInfo.payloadLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Success right on the buffer boundary. */
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    /* Throwing in a possible valid zero length password. */
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    /*Connect properties not initialized*/
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, NULL, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Connect Properties*/
    properties.sessionExpiry = 22;
    properties.receiveMax = 34;
    properties.maxPacketSize = 32;
    properties.topicAliasMax = 12;
    properties.requestResponseInfo = 1;
    properties.requestProblemInfo = 0;
    MQTTAuthInfo_t auth;
    memset( &auth, 0x0, sizeof( auth ) );
    auth.pAuthMethod = MQTT_TEST_UTF8_STRING;
    auth.authMethodLength = 2;
    auth.pAuthData = "abc";
    auth.authDataLength = 3;
    userProperties.userProperty[ 0 ].keyLength = 3;
    userProperties.userProperty[ 0 ].valueLength = 1;
    userProperties.userProperty[ 0 ].pValue = "1";
    userProperties.userProperty[ 0 ].pKey = "211";
    userProperties.count = 1;
    properties.pOutgoingUserProperty = &userProperties;
    properties.pOutgoingAuth = &auth;
    properties.pIncomingAuth = &auth;
    /*  29 */
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    auth.authDataLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    properties.pOutgoingAuth = NULL;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*Will Properties*/
    publishInfo.payloadFormat = 1;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.contentTypeLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    publishInfo.responseTopicLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    publishInfo.correlationLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    publishInfo.willDelay = 3;
    publishInfo.pUserProperty = &userProperties;
    /* 27 */
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
}

/**
 * @brief This method calls MQTT_SerializeConnect successfully using different parameters
 * until we have full coverage on the private method, serializeConnectPacket(...).
 */
void test_MQTTV5_SerializeConnect_Happy_Paths()
{
    MQTTStatus_t mqttStatus = MQTTSuccess;
    size_t remainingLength = 0;
    size_t packetSize = 0;
    MQTTFixedBuffer_t networkBuffer;
    MQTTUserProperties_t incomingProperty;

    properties.sessionExpiry = 22;
    properties.receiveMax = 34;
    properties.maxPacketSize = 32;
    properties.topicAliasMax = 12;
    properties.requestResponseInfo = 1;
    properties.requestProblemInfo = 0;
    properties.pIncomingUserProperty = &incomingProperty;
    publishInfo.payloadFormat = 1;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.contentTypeLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    publishInfo.responseTopicLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    publishInfo.correlationLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    publishInfo.willDelay = 3;
    /* Fill structs to pass into methods to be tested. */
    setupNetworkBuffer( &networkBuffer );
    setupConnectInfo( &connectInfo );
    setupPublishInfo( &publishInfo );
    publishInfo.dup = true;
    publishInfo.retain = true;
    /* Get MQTT connect packet size and remaining length. */
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                              &publishInfo,
                                              &properties,
                                              &remainingLength,
                                              &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties,
                                          remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, &publishInfo, &properties,
                                   remainingLength, &networkBuffer );

    /* / * Repeat with MQTTQoS1. * / */
    publishInfo.qos = MQTTQoS1;
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                              &publishInfo,
                                              &properties,
                                              &remainingLength,
                                              &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties,
                                          remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, &publishInfo, &properties,
                                   remainingLength, &networkBuffer );

    /* Re-initialize objects for branch coverage. */
    publishInfo.pPayload = MQTT_SAMPLE_PAYLOAD;
    publishInfo.payloadLength = MQTT_SAMPLE_PAYLOAD_LEN;
    publishInfo.pTopicName = MQTT_CLIENT_IDENTIFIER;
    publishInfo.topicNameLength = MQTT_CLIENT_IDENTIFIER_LEN;
    publishInfo.dup = true;
    publishInfo.qos = MQTTQoS2;
    publishInfo.retain = false;
    connectInfo.cleanSession = false;
    connectInfo.pClientIdentifier = MQTT_CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = MQTT_CLIENT_IDENTIFIER_LEN;
    connectInfo.pUserName = NULL;
    connectInfo.userNameLength = 0;
    connectInfo.pPassword = NULL;
    connectInfo.passwordLength = 0;

    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                              &publishInfo,
                                              &properties,
                                              &remainingLength,
                                              &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, &publishInfo, &properties,
                                          remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, &publishInfo, &properties,
                                   remainingLength, &networkBuffer );

    /* Repeat with NULL ppublishInfo. */
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                              NULL,
                                              &properties,
                                              &remainingLength,
                                              &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, NULL, &properties,
                                          remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, NULL, &properties,
                                   remainingLength, &networkBuffer );
}


void test_WillLimit( void )
{
    /* Test will property length more than the max value allowed. */
    size_t remainingLength = 0;
    size_t packetSize = 0;
    MQTTStatus_t status = MQTTSuccess;
    MQTTUserProperties_t incomingProperty;

    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = UINT16_MAX;
    connectInfo.pPassword = "";
    connectInfo.passwordLength = UINT16_MAX;
    connectInfo.pUserName = "";
    properties.receiveMax = UINT16_MAX;
    properties.requestProblemInfo = 1;
    properties.pIncomingUserProperty = &incomingProperty;
    publishInfo.payloadFormat = 1;
    publishInfo.msgExpiryPresent = 1;
    publishInfo.msgExpiryInterval = 10;
    publishInfo.contentTypeLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pContentType = MQTT_TEST_UTF8_STRING;
    publishInfo.responseTopicLength = MQTT_TEST_UTF8_STRING_LENGTH ;
    publishInfo.pResponseTopic = MQTT_TEST_UTF8_STRING;
    publishInfo.correlationLength = MQTT_TEST_UTF8_STRING_LENGTH;
    publishInfo.pCorrelationData = MQTT_TEST_UTF8_STRING;
    publishInfo.willDelay = 3;
    uint16_t i = 0;
    char str[ 65535 ];
    memset( str, '.', 65535 * sizeof( char ) );

    for( ; i < 3500; i++ )
    {
        userProperties.userProperty[ i ].keyLength = UINT16_MAX;
        userProperties.userProperty[ i ].pKey = str;
        userProperties.userProperty[ i ].pValue = str;
        userProperties.userProperty[ i ].valueLength = UINT16_MAX;
    }

    userProperties.count = 2048;
    publishInfo.pUserProperty = &userProperties;
    status = MQTT_GetPublishPropertiesSize( &publishInfo );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, &publishInfo, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.topicNameLength = 0U;
    publishInfo.topicAlias = 1U;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,MQTT_MAX_REMAINING_LENGTH);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTTV5_ValidatePublishParams()
{
    uint16_t topicAliasMax = 10U;
    uint8_t maxQos = 0U;
    uint8_t retain= 0U;
    /*Publish info cannot be null*/
    status = MQTTV5_ValidatePublishParams(NULL,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Topic alias greater than the allowed value. */
    publishInfo.topicAlias = 12U;
    status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Retain is not allowed. */
    publishInfo.topicAlias = 2U;
    publishInfo.retain = true;
    status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Qos invalid*/
    publishInfo.retain = false;
    publishInfo.qos = 1;
    status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Valid parameters should return success*/
    publishInfo.qos = 1;
    maxQos = 1;
    publishInfo.retain= true;
    retain = 1;
    status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /*Valid parameters should return success*/
    publishInfo.qos = 0;
    status = MQTTV5_ValidatePublishParams(&publishInfo,topicAliasMax,retain,maxQos);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

}

void test_MQTTV5_GetPublishPacketSize(){
    size_t remainingLength =0U;
    size_t packetSize = 0U;
    uint16_t topicAliasMax = 10U;
    uint32_t maxPacketSize = 0U;
    uint8_t retainAvailable = 0U;
    setupPublishInfo(&publishInfo);
    /*Test with invalid paramters*/
    status = MQTTV5_GetPublishPacketSize(NULL,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_GetPublishPacketSize(&publishInfo,NULL,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,NULL,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /*Topic name invalid*/
    publishInfo.pTopicName = NULL;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.pTopicName = TEST_TOPIC_NAME;
    
    /*Topic alias is not allowed and topic name is not provided.*/
    publishInfo.topicNameLength = 0;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    maxPacketSize = 100;
    publishInfo.topicNameLength = TEST_TOPIC_NAME_LENGTH;
    topicAliasMax = 40U;
    /*Packet size too large*/
    publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    publishInfo.payloadLength = MQTT_MAX_REMAINING_LENGTH - TEST_TOPIC_NAME_LENGTH - 4;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Good case succeeds. */
    publishInfo.pPayload = "";
    publishInfo.payloadLength = 0;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /* Again with QoS 2. */
    publishInfo.qos = MQTTQoS2;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    setupPublishProperties(&publishInfo);
    topicAliasMax = 50U;
    retainAvailable = 1U;
    publishInfo.retain= true;
    /*Valid properties*/
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
   
    /* No topic name*/
    publishInfo.topicNameLength = 0U;
    publishInfo.pTopicName = NULL;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTSuccess, status );

    /*Packet size is more than the server allowed max packet size*/
    maxPacketSize = 4;
    status = MQTTV5_GetPublishPacketSize(&publishInfo,&remainingLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    
}

void test_MQTT_SerializePublish(){
    uint16_t topicAliasMax = 5U;
    uint32_t maxPacketSize = 200U;
    uint8_t retainAvailable = 1U;
    size_t remainingLength = 98;
    uint8_t buffer[ 200 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    uint8_t expectedPacket[ 200 ];
    uint8_t * pIterator;
    const uint16_t PACKET_ID = 1;
    setupPublishInfo(&publishInfo);
    fixedBuffer.size = bufferSize;
    /* Calculate exact packet size and remaining length. */
    status = MQTTV5_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublish( &publishInfo,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
    
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];
    setupPublishProperties(&publishInfo);
    /* Calculate exact packet size and remaining length. */
    status = MQTTV5_GetPublishPacketSize( &publishInfo, &remainingLength, &packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );

    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTT_SerializePublish( &publishInfo,
                                    PACKET_ID,
                                    remainingLength,
                                    &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

}

void test_MQTTV5_DeserializeAck_puback( void )
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTAckInfo_t ackInfo;
    uint16_t packetIdentifier;
    bool requestProblem;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t* pIndex = buffer;
    size_t dummy = 0U;
    /* Verify parameters */
    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    ackInfo.pUserProperty =&userProperties;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
    status = MQTTV5_DeserializeAck( NULL, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, NULL, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    /* ackInfo not set. */
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, NULL, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    

    mqttPacketInfo.pRemainingData = buffer;
    /* Packet identifier 0 is not valid (per spec). */
    buffer[ 0 ] = 0;
    buffer[ 1 ] = 0;
    mqttPacketInfo.remainingLength = MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    
    /*Remaining length connot be less than 2*/
    mqttPacketInfo.remainingLength = 1;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );


    /* Process a valid PUBACK. */
    mqttPacketInfo.remainingLength = 2;
    buffer[ 1 ] = 1;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );
    

    mqttPacketInfo.remainingLength = 3;
    buffer[2] = 0x00;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 1, packetIdentifier );
 
    /*Property length should be zero when request problem is set to false*/
    mqttPacketInfo.remainingLength = 24;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );
    
    requestProblem = true;
    pIndex = &buffer[3];
    dummy= encodeRemainingLength(pIndex,20);
    pIndex++;
    pIndex = serializeutf_8(pIndex, MQTT_REASON_STRING_ID);
    pIndex = serializeutf_8pair(pIndex);
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

   /*Invalid property id*/
    mqttPacketInfo.remainingLength = 11;
    pIndex= &buffer[3];
    dummy= encodeRemainingLength(pIndex,7);
    pIndex++;
    pIndex = serializeutf_8(pIndex, MQTT_CORRELATION_DATA_ID);
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

    /*Invalid remaining length*/
    pIndex= &buffer[3];
    dummy= encodeRemainingLength(pIndex,12);
    pIndex++;
    pIndex = serializeutf_8(pIndex, MQTT_REASON_STRING_ID);
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );


    /*Invalid property length*/
    pIndex = &buffer[3];
    dummy = encodeRemainingLength( pIndex, 20971556356235 );
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

}

void test_MQTTV5_DeserializeAck_LogPuback()
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTAckInfo_t ackInfo;
    uint16_t packetIdentifier;
    bool requestProblem;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 4 ] = { 0 };
    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBACK;
    mqttPacketInfo.remainingLength = 4;
    /*Validate all the correct reason codes.*/
    buffer[1] = 1;
    buffer[2] = MQTT_REASON_SUCCESS;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[2] = MQTT_REASON_NO_MATCHING_SUBSCRIBERS;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[2] = MQTT_REASON_UNSPECIFIED_ERR;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_IMPL_SPECIFIC_ERR;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_NOT_AUTHORIZED;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_TOPIC_NAME_INVALID;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_PACKET_ID_IN_USE;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_QUOTA_EXCEEDED;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    buffer[2] = MQTT_REASON_PAYLOAD_FORMAT_INVALID;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    /*Invlaid reason code.*/
    buffer[2]= MQTT_REASON_BANNED;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

}

void test_MQTTV5_DeserializeAck_Pubrel()
{
    MQTTPacketInfo_t mqttPacketInfo;
    MQTTAckInfo_t ackInfo;
    uint16_t packetIdentifier;
    bool requestProblem;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 4 ] = { 0 };
    memset( &mqttPacketInfo, 0x00, sizeof( mqttPacketInfo ) );
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    mqttPacketInfo.pRemainingData = buffer;
    mqttPacketInfo.type = MQTT_PACKET_TYPE_PUBREL;
    mqttPacketInfo.remainingLength = 4;
    /*Validate all the correct reason codes.*/
    buffer[1] = 1;
    buffer[2] = MQTT_REASON_SUCCESS;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    buffer[2] = MQTT_REASON_PACKET_ID_NOT_FOUND;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTServerRefused, status );

    /*Invalid reason code.*/
    buffer[2]= MQTT_REASON_BANNED;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTProtocolError, status );

        /*Invalid packet id*/
    buffer[1] = 0;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );

    /*Invalid packet type. */
    mqttPacketInfo.type = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTV5_DeserializeAck( &mqttPacketInfo, &packetIdentifier, &ackInfo, requestProblem );
    TEST_ASSERT_EQUAL_INT( MQTTBadResponse, status );



}


void test_MQTTV5_GetAckPacketSize()
{
 MQTTStatus_t status ;
 MQTTAckInfo_t ackInfo;
 size_t remainingLength;
 size_t  propertyLength;
 size_t  packetSize;
 uint32_t maxPacketSize = 0U;
 memset(&ackInfo,0x0, sizeof(ackInfo));

 /*Invalid parameters*/
status = MQTTV5_GetAckPacketSize(NULL,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

status = MQTTV5_GetAckPacketSize(&ackInfo,NULL,&propertyLength,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,NULL,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,NULL,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

/*Max packet size cannot be 0*/
status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,NULL,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

/*Valid parameters*/
maxPacketSize = UINT32_MAX;
status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

/*With properties*/
ackInfo.pReasonString = MQTT_TEST_UTF8_STRING;
ackInfo.reasonStringLength = MQTT_TEST_UTF8_STRING_LENGTH;
ackInfo.pUserProperty = &userProperties;
userProperties.count = 0;
status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

/*Packet size greater than max allowed.*/
maxPacketSize = 2;
status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

}

void test_MQTTV5_SerializePubAckWithProperty()
{
    MQTTStatus_t status ;
    MQTTAckInfo_t ackInfo;
    size_t remainingLength;
    size_t  propertyLength;
    size_t  packetSize;
    uint8_t packetType;
    uint16_t packetId;
    uint32_t maxPacketSize = 1000U;
    uint8_t buffer[ 440 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };
    /*Invalid parameters*/
    status = MQTTV5_SerializePubAckWithProperty(NULL,remainingLength,propertyLength,&fixedBuffer,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_SerializePubAckWithProperty(&ackInfo,remainingLength,propertyLength,NULL,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    fixedBuffer.pBuffer = NULL;
    status = MQTTV5_SerializePubAckWithProperty(&ackInfo,remainingLength,propertyLength,&fixedBuffer,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /*Buffer size not sufficient*/
    // status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
    fixedBuffer.size = 5; 
    fixedBuffer.pBuffer= &buffer[BUFFER_PADDING_LENGTH];
    status = MQTTV5_SerializePubAckWithProperty(&ackInfo,remainingLength,propertyLength,&fixedBuffer,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );

    /*With correct parameters*/
    memset(&ackInfo,0x0, sizeof(ackInfo));
    status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Make sure test succeeds. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    fixedBuffer.size = bufferSize;
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    status = MQTTV5_SerializePubAckWithProperty(&ackInfo,remainingLength,propertyLength,&fixedBuffer,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*With properties*/
    ackInfo.reasonStringLength = MQTT_TEST_UTF8_STRING_LENGTH;
    ackInfo.pReasonString = MQTT_TEST_UTF8_STRING;
    ackInfo.pUserProperty = &userProperties;
    userProperties.count = 1;
    userProperties.userProperty->keyLength = 1; 
    userProperties.userProperty->valueLength = 1;
    userProperties.userProperty->pKey = "a";
    userProperties.userProperty->pValue = "a";
    status = MQTTV5_GetAckPacketSize(&ackInfo,&remainingLength,&propertyLength,&packetSize,maxPacketSize);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Make sure test succeeds. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    fixedBuffer.size = bufferSize;
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    status = MQTTV5_SerializePubAckWithProperty(&ackInfo,remainingLength,propertyLength,&fixedBuffer,packetType,packetId);
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );
}