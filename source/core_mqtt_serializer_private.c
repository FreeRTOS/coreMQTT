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
 * @file core_mqtt_serializer_private.c
 * @brief Implements private functions used by serializer and deserializer.
 */
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>

#include "core_mqtt_serializer.h"
#include "private/core_mqtt_serializer_private.h"

/* Include config defaults header to get default values of configs. */
#include "core_mqtt_config_defaults.h"

/**
 * @brief Version 5 has the value 5.
 */
#define MQTT_VERSION_5    ( 5U )

/*-----------------------------------------------------------*/

size_t variableLengthEncodedSize( uint32_t length )
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 5.0 spec. */

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

/*-----------------------------------------------------------*/

uint8_t * encodeString( uint8_t * pDestination,
                        const char * pSource,
                        uint16_t sourceLength )
{
    uint8_t * pBuffer = NULL;

    assert( pDestination != NULL );
    assert( pSource != NULL );

    pBuffer = pDestination;

    /* The first byte of a UTF-8 string is the high byte of the string length. */
    *pBuffer = UINT16_HIGH_BYTE( sourceLength );
    pBuffer++;

    /* The second byte of a UTF-8 string is the low byte of the string length. */
    *pBuffer = UINT16_LOW_BYTE( sourceLength );
    pBuffer++;

    /* Copy the string into pBuffer. */
    if( pSource != NULL )
    {
        ( void ) memcpy( ( void * ) pBuffer, ( const void * ) pSource, sourceLength );
    }

    /* Return the pointer to the end of the encoded string. */
    pBuffer = &pBuffer[ sourceLength ];

    return pBuffer;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUserProp( const char ** pPropertyKey,
                             uint16_t * pPropertyKeyLen,
                             const char ** pPropertyValue,
                             uint16_t * pPropertyValueLen,
                             uint32_t * pPropertyLength,
                             uint8_t ** pIndex )
{
    MQTTStatus_t status = MQTTSuccess;
    const char * pKey = NULL;
    const char * pValue = NULL;
    uint16_t keyLength = 0U;
    uint16_t valueLength = 0U;
    bool used = false;

    /* Decode the user property key using decodeUtf8. */
    status = decodeUtf8( &pKey, &keyLength, pPropertyLength, &used, pIndex );

    if( status == MQTTSuccess )
    {
        used = false;
        /* Decode the user property value using decodeUtf8. */
        status = decodeUtf8( &pValue, &valueLength, pPropertyLength, &used, pIndex );
    }

    if( status == MQTTSuccess )
    {
        /* Store the decoded key and value. */
        *pPropertyKey = pKey;
        *pPropertyKeyLen = keyLength;
        *pPropertyValue = pValue;
        *pPropertyValueLen = valueLength;
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint32t( uint32_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* Protocol error to include the same property twice. */
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /* Validate the length and decode. */
    else if( *pPropertyLength < sizeof( uint32_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        if( pProperty != NULL )
        {
            *pProperty = UINT32_DECODE( pLocalIndex );
        }

        pLocalIndex = &pLocalIndex[ sizeof( uint32_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint32_t );
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint16t( uint16_t * pProperty,
                            uint32_t * pPropertyLength,
                            bool * pUsed,
                            uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* Protocol error to include the same property twice. */
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /* Validate the length and decode. */
    else if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        if( pProperty != NULL )
        {
            *pProperty = UINT16_DECODE( pLocalIndex );
        }

        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint16_t );
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUint8t( uint8_t * pProperty,
                           uint32_t * pPropertyLength,
                           bool * pUsed,
                           uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* Protocol error to include the same property twice. */
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /* Validate the length and decode. */
    else if( *pPropertyLength < sizeof( uint8_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pProperty = *pLocalIndex;
        pLocalIndex = &pLocalIndex[ sizeof( uint8_t ) ];
        *pUsed = true;
        *pPropertyLength -= sizeof( uint8_t );

        if( *pProperty > 1U )
        {
            status = MQTTBadResponse;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeUtf8( const char ** pProperty,
                         uint16_t * pLength,
                         uint32_t * pPropertyLength,
                         bool * pUsed,
                         uint8_t ** pIndex )
{
    uint8_t * pLocalIndex = *pIndex;
    MQTTStatus_t status = MQTTSuccess;

    /* Protocol error to include the same property twice. */
    if( *pUsed == true )
    {
        status = MQTTBadResponse;
    }
    /* Validate the length and decode. */
    else if( *pPropertyLength < sizeof( uint16_t ) )
    {
        status = MQTTBadResponse;
    }
    else
    {
        *pLength = UINT16_DECODE( pLocalIndex );
        pLocalIndex = &pLocalIndex[ sizeof( uint16_t ) ];
        *pPropertyLength -= sizeof( uint16_t );

        if( *pPropertyLength < *pLength )
        {
            status = MQTTBadResponse;
        }
        else
        {
            *pProperty = ( const char * ) pLocalIndex;
            pLocalIndex = &pLocalIndex[ *pLength ];
            *pPropertyLength -= *pLength;
            *pUsed = true;
        }
    }

    *pIndex = pLocalIndex;
    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeVariableLength( const uint8_t * pBuffer,
                                   size_t bufferLength,
                                   uint32_t * pLength )
{
    uint32_t remainingLength = 0;
    uint32_t multiplier = 1;
    size_t bytesDecoded = 0;
    uint8_t encodedByte = 0;
    size_t localBufferLength = bufferLength;
    MQTTStatus_t status = MQTTSuccess;

    /* This algorithm is copied from the MQTT 5.0 spec. */
    do
    {
        if( multiplier > 2097152U ) /* 128 ^ 3 */
        {
            LogError( ( "Invalid remaining length in the packet.\n" ) );
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;
            status = MQTTBadResponse;
        }
        else
        {
            if( localBufferLength > 0U )
            {
                encodedByte = pBuffer[ bytesDecoded ];
                remainingLength += ( ( size_t ) encodedByte & 0x7FU ) * multiplier;
                multiplier *= 128U;
                bytesDecoded++;
                localBufferLength--;
            }
            else
            {
                status = MQTTBadResponse;
            }
        }

        /* If the response is incorrect then break out
         * of the loop. */
        if( ( remainingLength >= MQTT_REMAINING_LENGTH_INVALID ) ||
            ( status != MQTTSuccess ) )
        {
            status = MQTTBadResponse;
            break;
        }
    } while( ( encodedByte & 0x80U ) != 0U );

    if( status == MQTTSuccess )
    {
        /* Check that the decoded remaining length conforms to the MQTT specification. */
        size_t expectedSize = variableLengthEncodedSize( remainingLength );

        if( bytesDecoded != expectedSize )
        {
            LogError( ( "Expected and actual length of decoded bytes do not match.\n" ) );
            status = MQTTBadResponse;
        }
        else
        {
            *pLength = remainingLength;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

uint8_t * encodeRemainingLength( uint8_t * pDestination,
                                 size_t length )
{
    uint8_t lengthByte;
    uint8_t * pLengthEnd = NULL;
    size_t remainingLength = length;

    assert( pDestination != NULL );

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

    return pLengthEnd;
}

/*-----------------------------------------------------------*/

uint8_t * serializeAckFixed( uint8_t * pIndex,
                             uint8_t packetType,
                             uint16_t packetId,
                             size_t remainingLength,
                             MQTTSuccessFailReasonCode_t reasonCode )
{
    uint8_t * pIndexLocal = pIndex;

    /* The first byte in the publish ack packet is the control packet type. */
    *pIndexLocal = packetType;
    pIndexLocal++;
    /* After the packet type fixed header has remaining length. */
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );
    /* Encode the packet id. */
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( packetId );
    pIndexLocal = &pIndexLocal[ 2 ];
    /* We are now sending the ack. */
    *pIndexLocal = ( uint8_t ) reasonCode;
    pIndexLocal++;
    return pIndexLocal;
}

/*-----------------------------------------------------------*/

uint8_t * serializeConnectFixedHeader( uint8_t * pIndex,
                                       const MQTTConnectInfo_t * pConnectInfo,
                                       const MQTTPublishInfo_t * pWillInfo,
                                       size_t remainingLength )
{
    uint8_t * pIndexLocal = pIndex;
    uint8_t connectFlags = 0U;

    /* The first byte in the CONNECT packet is the control packet type. */
    *pIndexLocal = MQTT_PACKET_TYPE_CONNECT;
    pIndexLocal++;

    /* The remaining length of the CONNECT packet is encoded starting from the
     * second byte. The remaining length does not include the length of the fixed
     * header or the encoding of the remaining length. */
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );

    /* The string "MQTT" is placed at the beginning of the CONNECT packet's variable
     * header. This string is 4 bytes long. */
    pIndexLocal = encodeString( pIndexLocal, "MQTT", 4 );

    /* The MQTT protocol version is the second field of the variable header. */

    *pIndexLocal = MQTT_VERSION_5;

    pIndexLocal++;

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

    *pIndexLocal = connectFlags;
    pIndexLocal++;

    /* Write the 2 bytes of the keep alive interval into the CONNECT packet. */
    pIndexLocal[ 0 ] = UINT16_HIGH_BYTE( pConnectInfo->keepAliveSeconds );
    pIndexLocal[ 1 ] = UINT16_LOW_BYTE( pConnectInfo->keepAliveSeconds );
    pIndexLocal = &pIndexLocal[ 2 ];

    return pIndexLocal;
}

/*-----------------------------------------------------------*/

uint8_t * serializeSubscribeHeader( size_t remainingLength,
                                    uint8_t * pIndex,
                                    uint16_t packetId )
{
    uint8_t * pIterator = pIndex;

    /* The first byte in SUBSCRIBE is the packet type. */
    *pIterator = MQTT_PACKET_TYPE_SUBSCRIBE;
    pIterator++;

    /* Encode the "Remaining length" starting from the second byte. */
    pIterator = encodeRemainingLength( pIterator, remainingLength );

    /* Place the packet identifier into the SUBSCRIBE packet. */
    pIterator[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIterator[ 1 ] = UINT16_LOW_BYTE( packetId );
    /* Advance the pointer. */
    pIterator = &pIterator[ 2 ];

    return pIterator;
}

/*-----------------------------------------------------------*/

uint8_t * serializeUnsubscribeHeader( size_t remainingLength,
                                      uint8_t * pIndex,
                                      uint16_t packetId )
{
    uint8_t * pIterator = pIndex;

    /* The first byte in UNSUBSCRIBE is the packet type. */
    *pIterator = MQTT_PACKET_TYPE_UNSUBSCRIBE;
    pIterator++;

    /* Encode the "Remaining length" starting from the second byte. */
    pIterator = encodeRemainingLength( pIterator, remainingLength );

    /* Place the packet identifier into the SUBSCRIBE packet. */
    pIterator[ 0 ] = UINT16_HIGH_BYTE( packetId );
    pIterator[ 1 ] = UINT16_LOW_BYTE( packetId );
    /* Increment the pointer. */
    pIterator = &pIterator[ 2 ];

    return pIterator;
}

/*-----------------------------------------------------------*/

uint8_t * serializeDisconnectFixed( uint8_t * pIndex,
                                    MQTTSuccessFailReasonCode_t * pReasonCode,
                                    size_t remainingLength )
{
    uint8_t * pIndexLocal = pIndex;

    assert( pIndex != NULL );
    /* The first byte in the publish ack packet is the control packet type. */
    *pIndexLocal = MQTT_PACKET_TYPE_DISCONNECT;
    pIndexLocal++;

    /* After the packet type fixed header has remaining length. */
    pIndexLocal = encodeVariableLength( pIndexLocal, remainingLength );

    if( pReasonCode != NULL )
    {
        /* Encode the reason code. */
        *pIndexLocal = ( uint8_t ) *pReasonCode;
        pIndexLocal++;
    }

    return pIndexLocal;
}

/*-----------------------------------------------------------*/

MQTTStatus_t decodeSubackPropertyLength( const uint8_t * pIndex,
                                         uint32_t remainingLength,
                                         uint32_t * subackPropertyLength )
{
    MQTTStatus_t status;
    const uint8_t * pLocalIndex = pIndex;
    uint32_t propertyLength = 0U;

    status = decodeVariableLength( pLocalIndex, remainingLength - sizeof( uint16_t ), &propertyLength );

    if( status == MQTTSuccess )
    {
        *subackPropertyLength = ( propertyLength + variableLengthEncodedSize( propertyLength ) );

        if( *subackPropertyLength > ( remainingLength - sizeof( uint16_t ) ) )
        {
            status = MQTTBadResponse;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/
