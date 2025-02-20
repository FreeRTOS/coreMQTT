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


#define MQTT_TEST_UTF8_STRING           ( "test" )
#define MQTT_TEST_UTF8_STRING_LENGTH    ( sizeof( MQTT_TEST_UTF8_STRING ) - 1 )

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

#define MQTT_USER_PROPERTY_ID            ( 0x26 )
#define MQTT_REASON_STRING_ID            ( 0x1F )
#define MQTT_SERVER_REF_ID               ( 0x1C )


#define CORE_MQTT_ID_SIZE                ( 1U )
#define MQTT_REMAINING_LENGTH_INVALID    ( ( size_t ) 268435456 )

/* Variables common to testcases */
MQTTConnectProperties_t properties;
MQTTConnectInfo_t connectInfo;
MQTTPacketInfo_t packetInfo;
MQTTStatus_t status;

/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp( void )
{
    memset( &properties, 0x0, sizeof( properties ) );
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

static uint8_t * serializeutf_8( uint8_t * pIndex,
                                 uint8_t propertyId )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = propertyId;
    pIndexLocal++;
    size_t dummy = encodeString( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}

static uint8_t * serializeutf_8pair( uint8_t * pIndex )
{
    uint8_t * pIndexLocal = pIndex;

    *pIndexLocal = MQTT_USER_PROPERTY_ID;
    pIndexLocal++;
    size_t dummy = encodeString( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    dummy = encodeString( pIndexLocal, MQTT_TEST_UTF8_STRING, MQTT_TEST_UTF8_STRING_LENGTH );
    pIndexLocal = &pIndexLocal[ dummy ];
    return pIndexLocal;
}

void test_MQTTV5_DeserializeConnackOnlyUserProperty( void )
{
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndexLocal = buffer;

    buffer[ 0 ] = 0x01;
    buffer[ 1 ] = 0x00;

    bool session = false;
    properties.maxPacketSize = 100U;
    packetInfo.pRemainingData = buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    packetInfo.remainingLength = 16;
    pIndexLocal = &buffer[ 2 ];
    size_t propertyLength = encodeRemainingLength( pIndexLocal, 13 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 5;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 2 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 6;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 3 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 8 );
    packetInfo.remainingLength = propertyLength + 10;
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );

    /*Invalid property length*/
    packetInfo.remainingLength = 15;
    pIndexLocal = &buffer[ 2 ];
    propertyLength = encodeRemainingLength( pIndexLocal, 12 );
    pIndexLocal++;
    pIndexLocal = serializeutf_8pair( pIndexLocal );
    status = MQTTV5_DeserializeConnack( &properties, &packetInfo, &session );
    TEST_ASSERT_EQUAL_INT( MQTTMalformedPacket, status );
}
void test_MQTTV5_DeserializeAck( void )
{
    MQTTAckInfo_t ackInfo;
    uint16_t packetIdentifier;
    uint32_t maxPacketSize = 200U;
    bool requestProblem = true;
    MQTTStatus_t status = MQTTSuccess;
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndex = buffer;
    size_t dummy;

    /* Verify parameters */
    memset( &ackInfo, 0x00, sizeof( ackInfo ) );
    packetInfo.pRemainingData = buffer;
    buffer[ 0 ] = 0;
    buffer[ 1 ] = 1;
    buffer[ 2 ] = 0x00;
    pIndex = &buffer[ 3 ];
    packetInfo.type = MQTT_PACKET_TYPE_PUBACK;
    dummy = encodeRemainingLength( pIndex, 20 );
    packetInfo.remainingLength = dummy + 23;
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8pair( pIndex );
    status = MQTTV5_DeserializeAck( &packetInfo, &packetIdentifier, &ackInfo, requestProblem, maxPacketSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}


void test_MQTTV5_DeserializeDisconnect()
{
    MQTTAckInfo_t disconnectInfo;
    const char * pServerRef;
    uint16_t serverRefLength;
    int32_t maxPacketSize = 100U;
    uint8_t buffer[ 100 ] = { 0 };
    uint8_t * pIndex = buffer;
    size_t dummy;

    memset( &disconnectInfo, 0x0, sizeof( disconnectInfo ) );
    /*With properties*/
    pIndex = &buffer[ 1 ];
    packetInfo.pRemainingData = buffer;
    dummy = encodeRemainingLength( pIndex, 27 );
    packetInfo.remainingLength = 28 + dummy;
    pIndex++;
    pIndex = serializeutf_8( pIndex, MQTT_REASON_STRING_ID );
    pIndex = serializeutf_8pair( pIndex );
    pIndex = serializeutf_8( pIndex, MQTT_SERVER_REF_ID );
    status = MQTTV5_DeserializeDisconnect( &packetInfo, &disconnectInfo, &pServerRef, &serverRefLength, maxPacketSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
}


