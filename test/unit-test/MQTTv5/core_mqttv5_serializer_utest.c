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
#include<assert.h>
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
#define MQTT_VERSION_3_1_1                  ( ( uint8_t ) 4U )

/**
 * @brief Test-defined macro for MQTT username.
 */
#define MQTT_TEST_USERNAME                  "username"
#define MQTT_TEST_USERNAME_LEN              ( sizeof( MQTT_TEST_USERNAME ) - 1 )


/**
 * @brief Test-defined macro for MQTT password.
 */
#define MQTT_TEST_PASSWORD                  "password"
#define MQTT_TEST_PASSWORD_LEN              ( sizeof( MQTT_TEST_PASSWORD ) - 1 )

/**
 * @brief Length of the client identifier.
 */
#define MQTT_CLIENT_IDENTIFIER_LEN          ( sizeof( MQTT_CLIENT_IDENTIFIER ) - 1 )

/**
 * @brief Sample payload.
 */
#define MQTT_SAMPLE_PAYLOAD                 "Hello World"
#define MQTT_SAMPLE_PAYLOAD_LEN             ( sizeof( MQTT_SAMPLE_PAYLOAD ) - 1 )

/* MQTT CONNECT flags. */
#define MQTT_CONNECT_FLAG_CLEAN             ( 1 )         /**< @brief Clean session. */
#define MQTT_CONNECT_FLAG_WILL              ( 2 )         /**< @brief Will present. */
#define MQTT_CONNECT_FLAG_WILL_QOS1         ( 3 )         /**< @brief Will QoS 1. */
#define MQTT_CONNECT_FLAG_WILL_QOS2         ( 4 )         /**< @brief Will QoS 2. */
#define MQTT_CONNECT_FLAG_WILL_RETAIN       ( 5 )         /**< @brief Will retain. */
#define MQTT_CONNECT_FLAG_PASSWORD          ( 6 )         /**< @brief Password present. */
#define MQTT_CONNECT_FLAG_USERNAME          ( 7 )         /**< @brief User name present. */

/**
 * @brief The Remaining Length field of MQTT disconnect packets, per MQTT spec.
 */
#define MQTT_DISCONNECT_REMAINING_LENGTH    ( ( uint8_t ) 0 )

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

#define UINT32_DECODE( ptr )                            \
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 8 ) | \
                    ( ( ( uint32_t ) ptr[1] ) << 16 ) | \
                    ( ( ( uint32_t ) ptr[ 2 ] ) << 24 ) | \
                   ( ( uint32_t ) ptr[ 3 ] ) )

#define UINT32_BYTE3( x )             ( ( uint8_t ) ( ( x ) >> 24 ) )

#define UINT32_BYTE2( x )             ( ( uint8_t ) ( ( x ) >> 16 ) )

#define UINT32_BYTE1( x )             ( ( uint8_t ) ( ( x ) >> 8 ) )

#define UINT32_BYTE0( x )             ( ( uint8_t ) ( ( x ) & 0x000000FFU ) )

#define  MQTT_VERSION_5    (5U)
#define  MQTT_SESSION_EXPIRY_SIZE                     (5U)
#define  MQTT_RECEIVE_MAX_SIZE                        (3U)
#define  MQTT_MAX_PACKET_SIZE                         (5U)
#define  MQTT_TOPIC_ALIAS_SIZE                        (3U)
#define  MQTT_REQUEST_RESPONSE_SIZE                   (2U)
#define  MQTT_REQUEST_PPOBLEM_SIZE                    (2U)

#define  MQTT_SESSION_EXPIRY_ID                     (0x11)
#define  MQTT_RECEIVE_MAX_ID                        (0x21)
#define  MQTT_MAX_PACKET_SIZE_ID                    (0x27)
#define  MQTT_TOPIC_ALIAS_MAX_ID                    (0x22)
#define  MQTT_REQUEST_RESPONSE_ID                   (0x19)
#define  MQTT_REQUEST_PROBLEM_ID                    (0x17)
#define  MQTT_USER_PROPERTY_ID                      (0x26)
#define  MQTT_AUTH_METHOD_ID                        (0x15)
#define  MQTT_AUTH_DATA_ID                          (0x16)

#define  MQTT_WILL_DELAY_ID                         (0x18)
#define  MQTT_PAYLOAD_FORMAT_ID                     (0x01)
#define  MQTT_MSG_EXPIRY_ID                         (0x02)
#define  MQTT_CONTENT_TYPE_ID                       (0x03)
#define  MQTT_RESPONSE_TOPIC_ID                     (0x08)
#define  MQTT_CORRELATION_DATA_ID                   (0x09) 


#define  MQTT_MAX_QOS_ID                            (0x24)
#define  MQTT_RETAIN_AVAILABLE_ID                   (0x25)
#define  MQTT_ASSIGNED_CLIENT_ID                    (0x12)
#define  MQTT_REASON_STRING_ID                      (0x1F)
#define  MQTT_WILDCARD_ID                           (0x28)
#define  MQTT_SUB_AVAILABLE_ID                      (0x29)
#define  MQTT_SHARED_SUB_ID                         (0x2A)
#define  MQTT_SERVER_KEEP_ALIVE_ID                  (0x13)
#define  MQTT_RESPONSE_INFO_ID                      (0x1A)
#define  MQTT_SERVER_REF_ID                         (0x1C)

#define  CORE_MQTT_ID_SIZE                          ( 1U )
#define MQTT_REMAINING_LENGTH_INVALID             ( ( size_t ) 268435456 )




static uint8_t remainingLengthBuffer[ MQTT_REMAINING_BUFFER_MAX_LENGTH ] = { 0 };

static uint8_t encodedStringBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

static uint8_t mqttBuffer[ MQTT_TEST_BUFFER_LENGTH ] = { 0 };

/* ============================   UNITY FIXTURES ============================ */

/* Called before each test method. */
void setUp( void )
{
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
static int32_t mockReceive( NetworkContext_t * pNetworkContext,
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

 static size_t remainingLengthEncodedSize(size_t length)
{
    size_t encodedSize;

    /* Determine how many bytes are needed to encode length.
     * The values below are taken from the MQTT 3.1.1 spec. */

     /* 1 byte is needed to encode lengths between 0 and 127. */
    if (length < 128U)
    {
        encodedSize = 1U;
    }
    /* 2 bytes are needed to encode lengths between 128 and 16,383. */
    else if (length < 16384U)
    {
        encodedSize = 2U;
    }
    /* 3 bytes are needed to encode lengths between 16,384 and 2,097,151. */
    else if (length < 2097152U)
    {
        encodedSize = 3U;
    }
    /* 4 bytes are needed to encode lengths between 2,097,152 and 268,435,455. */
    else
    {
        encodedSize = 4U;
    }

    LogDebug(("Encoded size for length %lu is %lu bytes.",
        (unsigned long)length,
        (unsigned long)encodedSize));

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



static MQTTStatus_t MQTT_GetUserPropertySize(MQTTUserProperty_t* pUserProperty, uint16_t number, size_t* size) {
    MQTTStatus_t status = MQTTSuccess;
    uint16_t i = 0;
    /*Number of user properties can't be more than the max user properties specified*/
    if (number > MAX_USER_PROPERTY || size == NULL) {
        status = MQTTBadParameter;
    }
    else if (number != 0 && pUserProperty == NULL) {
        status = MQTTBadParameter;
    }
    else {
        for (;i < number && status == MQTTSuccess;i++) {
            if ((pUserProperty + i) == NULL || (pUserProperty + i)->keyLength == 0 || (pUserProperty + i)->valueLength == 0 || (pUserProperty + i)->key == NULL || (pUserProperty + i)->value == NULL) {
                status = MQTTBadParameter;
            }
            else{
            *size += (pUserProperty + i)->keyLength + 3U;
            *size += (pUserProperty + i)->valueLength + 2U;
            }
        }
    }
    return status;
}


static MQTTStatus_t MQTT_GetConnectPropertiesSize(MQTTConnectProperties_t* pConnectProperties)
{
    size_t propertyLength = 0;
    MQTTStatus_t status = MQTTSuccess;
    /*Validate the arguments*/
    if(pConnectProperties==NULL){
        status=MQTTBadParameter;
    }
    else if(pConnectProperties->maxPacketSize==0 || pConnectProperties->receiveMax==0){
        status=MQTTBadParameter;
    }
    else{
    /*Add the lengths of the parameters if applicable*/
    if (pConnectProperties->sessionExpiry != 0)
    {
        propertyLength += MQTT_SESSION_EXPIRY_SIZE;
    }
    if (pConnectProperties->receiveMax != UINT16_MAX)
    {
        propertyLength += MQTT_RECEIVE_MAX_SIZE;
    }
    if (pConnectProperties->isMaxPacketSize != false)
    {
        propertyLength += MQTT_MAX_PACKET_SIZE;
    }
    if (pConnectProperties->topicAliasMax != 0U)
    {
        propertyLength += MQTT_TOPIC_ALIAS_SIZE;
    }
    if (pConnectProperties->reqResInfo != 0)
    {
        propertyLength += MQTT_REQUEST_RESPONSE_SIZE;
    }
    if (pConnectProperties->reqProbInfo != 1)
    {
        propertyLength += MQTT_REQUEST_PPOBLEM_SIZE;
    }
    if (pConnectProperties->outgoingAuth != NULL)
    {
        /*Incoming auth cannot be NULL*/ 
        if(pConnectProperties->incomingAuth==NULL){
            status=MQTTBadParameter;
        }
        /*Valid authentication parameters*/
        else if (pConnectProperties->outgoingAuth->authMethodLength == 0U && pConnectProperties->outgoingAuth->authDataLength != 0U)
        {
            status = MQTTBadParameter;
        }
        else if ((pConnectProperties->outgoingAuth->authMethodLength != 0U) && (pConnectProperties->outgoingAuth->authMethod == NULL))
        {
            status = MQTTBadParameter;
        }
        else if ((pConnectProperties->outgoingAuth->authDataLength != 0U) && (pConnectProperties->outgoingAuth->authData == NULL))
        {
            status = MQTTBadParameter;
        }
        else
        {
            propertyLength += pConnectProperties->outgoingAuth->authMethodLength;
            propertyLength += 3U;
            if (pConnectProperties->outgoingAuth->authDataLength != 0U) {
                propertyLength += pConnectProperties->outgoingAuth->authDataLength;
                propertyLength += 3U;
            }
        }
    }
    }
    /*Get the length of the user properties*/
    if (status == MQTTSuccess && pConnectProperties->outgoingUserPropSize != 0) {
        status = MQTT_GetUserPropertySize(pConnectProperties->outgoingUserProperty, pConnectProperties->outgoingUserPropSize, &propertyLength);
    }
    /*Variable length encoded values cannot have valued more than 268435455U*/
    if (status == MQTTSuccess && pConnectProperties->propertyLength > MQTT_MAX_REMAINING_LENGTH) {
        status = MQTTBadParameter;
    }
    if(status==MQTTSuccess){
    pConnectProperties->propertyLength = propertyLength;
    }

    return status;

}

static MQTTStatus_t MQTT_GetWillPropertiesSize(MQTTPublishInfo_t* pWillProperties)
{
    size_t willLength = 0U;
    MQTTStatus_t status = MQTTSuccess;
    /*Validate the arguments*/
    if(pWillProperties==NULL){
        status=MQTTBadParameter;
    }
    else{
    /*Add the length of all the parameters which are applicable*/
    if (pWillProperties->willDelay != 0U)
    {
        willLength += 5U;
    }
    if (pWillProperties->payloadFormat != 0)
    {
        willLength += 2U;
    }
    if (pWillProperties->msgExpiryPresent == true)
    {
        willLength += 5U;
    }
    if (pWillProperties->contentTypeLength != 0U)
    {
        if(pWillProperties->contentType==NULL){
            status= MQTTBadParameter;
        }
        else{
        willLength += pWillProperties->contentTypeLength + 3U;
        }
    }
    }
    /*Validate if length and pointers are valid*/
    if(status==MQTTSuccess && pWillProperties->responseTopicLength != 0U){
        if(pWillProperties->responseTopic==NULL){
            status= MQTTBadParameter;
        }
        else{
        willLength += pWillProperties->responseTopicLength + 3U;
        }
    }
    if (status==MQTTSuccess && pWillProperties->correlationLength != 0U)
    {
        if(pWillProperties->correlationData==NULL){
                   status= MQTTBadParameter;
        }
        else{
        willLength += pWillProperties->correlationLength + 3U;
        }
    }
    /*Get the length of user properties*/ 
    if(status==MQTTSuccess){
        status = MQTT_GetUserPropertySize(pWillProperties->userProperty, pWillProperties->userPropertySize, &willLength);
    }
    /*Variable encoded can't have a value more than 268435455UL*/
    if (willLength > MQTT_MAX_REMAINING_LENGTH) {
        status = MQTTBadParameter;
    }
    if(status==MQTTSuccess){
         pWillProperties->propertyLength = willLength;
    }
    return status;
}

static uint8_t* serializeUserProperties(uint8_t * pIndex,const MQTTUserProperty_t* pUserProperty, uint16_t size){
             uint16_t i = 0;
             assert(pIndex!=NULL);
             uint8_t* pIndexLocal = pIndex;
    for (; i < size; i++)
    {
        *pIndexLocal = MQTT_USER_PROPERTY_ID;
        pIndexLocal++;
        pIndexLocal = encodeString(pIndexLocal,(pUserProperty+i)->key,(pUserProperty+i)->keyLength);
        pIndexLocal = encodeString(pIndexLocal,(pUserProperty+i)->value,(pUserProperty+i)->valueLength);
    }
    return pIndexLocal;
}


static void  serializeConnectPacketV5(const MQTTConnectInfo_t* pConnectInfo,
    const MQTTPublishInfo_t* pWillInfo,
    const MQTTConnectProperties_t* pConnectProperties,
    size_t remainingLength,
    const MQTTFixedBuffer_t* pFixedBuffer)
{
    uint8_t* pIndex = NULL;
    assert(pConnectInfo != NULL);
    assert(pFixedBuffer != NULL);
    assert(pFixedBuffer->pBuffer != NULL);
    assert(pConnectProperties != NULL);

    pIndex = pFixedBuffer->pBuffer;

    /* Serialize the header. */
    pIndex = MQTT_SerializeConnectFixedHeader(pIndex,
        pConnectInfo,
        pWillInfo,
        remainingLength);

    /* Serialize the connect Properties. */
    pIndex= MQTT_SerializeConnectProperties(pIndex,pConnectProperties);

    if( pConnectProperties->outgoingUserPropSize >0){
    pIndex= serializeUserProperties(pIndex, pConnectProperties->outgoingUserProperty, pConnectProperties->outgoingUserPropSize);
    }
    
    MQTTAuthInfo_t *pAuthInfo = pConnectProperties->outgoingAuth;
    if (pAuthInfo != NULL)
    {
        /* Serialize the authentication method  string. */
        *pIndex = MQTT_AUTH_METHOD_ID;
        pIndex++;
        pIndex = encodeString(pIndex,pAuthInfo->authMethod,pAuthInfo->authMethodLength);
        if (pAuthInfo->authDataLength != 0U)
        {
        *pIndex = MQTT_AUTH_DATA_ID;
        pIndex++;
        pIndex = encodeString(pIndex,pAuthInfo->authData,pAuthInfo->authDataLength);
        }
    }

    /* Write the client identifier into the CONNECT packet. */
    pIndex = encodeString(pIndex,
        pConnectInfo->pClientIdentifier,
        pConnectInfo->clientIdentifierLength);

    /* Serialze the will properties,topic name and message into the CONNECT packet if provided. */
    if (pWillInfo != NULL)
    {
        pIndex= MQTT_SerializePublishProperties(pWillInfo,pIndex);
        if (pWillInfo->contentTypeLength != 0U)
        {
        *pIndex = MQTT_CONTENT_TYPE_ID;
        pIndex++;
        pIndex = encodeString(pIndex,pWillInfo->contentType,pWillInfo->contentTypeLength);
        }
        if (pWillInfo->responseTopicLength != 0U)
        {
       *pIndex = MQTT_RESPONSE_TOPIC_ID;
        pIndex++;
        pIndex = encodeString(pIndex,pWillInfo->responseTopic,pWillInfo->responseTopicLength);
        }
        if (pWillInfo->correlationLength != 0U)
        {
        *pIndex = MQTT_CORRELATION_DATA_ID;
        pIndex++;
        pIndex = encodeString(pIndex,pWillInfo->correlationData,pWillInfo->correlationLength);
        }
        if (pWillInfo->userPropertySize != 0)
        {
         pIndex = serializeUserProperties(pIndex,pWillInfo->userProperty,pWillInfo->userPropertySize);
        }
        pIndex = encodeString(pIndex, pWillInfo->pTopicName,pWillInfo->topicNameLength);
        pIndex = encodeString(pIndex,pWillInfo->pPayload,(uint16_t)pWillInfo->payloadLength);
     
    }
    /* Encode the user name if provided. */
    if (pConnectInfo->pUserName != NULL)
    {
        pIndex = encodeString(pIndex, pConnectInfo->pUserName, pConnectInfo->userNameLength);
    }

    /* Encode the password if provided. */
    if (pConnectInfo->pPassword != NULL)
    {
        pIndex = encodeString(pIndex, pConnectInfo->pPassword, pConnectInfo->passwordLength);
    }

    LogDebug(("Length of serialized CONNECT packet is %lu.",
        ((unsigned long)(pIndex - pFixedBuffer->pBuffer))));

    /* Ensure that the difference between the end and beginning of the buffer
     * is less than the buffer size. */
    assert(((size_t)(pIndex - pFixedBuffer->pBuffer)) <= pFixedBuffer->size);
}


 static MQTTStatus_t  logConnackResponseV5(uint8_t responseCode){
    
    MQTTStatus_t status= MQTTServerRefused;
    switch(responseCode){
        case MQTT_REASON_UNSPECIFIED_ERR :
            LogError("Connection refused: Unspecified error");
            break;
        case  MQTT_REASON_MALFORMED_PACKET  :
            LogError("Connection refused: Malformed Packet.");
            break;
        case MQTT_REASON_PROTOCOL_ERR:
            LogError("Connection refused: Protocol Error.");
            break;
        case MQTT_REASON_IMPL_SPECIFIC_ERR :
            LogError("Connection refused: Impementation specific error.");
            break;
        case MQTT_REASON_UNSUP_PROTO_VER :
            LogError("Connection refused: Unsupported Protocol Version.");
            break;
        case MQTT_REASON_CLIENT_ID_NOT_VALID :
            LogError( "Connection refused: Client Identifier not valid.");
            break;
        case MQTT_REASON_BAD_USER_OR_PASS :
            LogError("Connection refused: Bad User Name or Password.");
            break;
        case MQTT_REASON_NOT_AUTHORIZED :
            LogError("Connection refused: Not authorized.");
            break;
        case  MQTT_REASON_SERVER_UNAVAILABLE :
            LogError("Connection refused: Server unavailable.");
            break;
        case MQTT_REASON_SERVER_BUSY :
            LogError( "Connection refused: Server busy." );
            break;
        case MQTT_REASON_BANNED :
            LogError("Connection refused: Banned.");
            break;
        case MQTT_REASON_BAD_AUTH_METHOD :
            LogError("Connection refused: Bad authentication method.");
            break;
        case MQTT_REASON_TOPIC_NAME_INVALID  :
            LogError("Connection refused: Topic Name invalid.");
            break;
        case MQTT_REASON_PACKET_TOO_LARGE :
            LogError( "Connection refused: Packet too large .");
            break;
        case   MQTT_REASON_QUOTA_EXCEEDED :
            LogError("Connection refused: Quota exceeded.");
            break;
        case  MQTT_REASON_PAYLOAD_FORMAT_INVALID :
            LogError( "Connection refused: Payload format invalid." );
            break;
        case  MQTT_REASON_RETAIN_NOT_SUPPORTED  :
            LogError("Connection refused: Retain not supported.");
            break;
        case  MQTT_REASON_QOS_NOT_SUPPORTED :
            LogError("Connection refused: QoS not supported.");
            break;
        case  MQTT_REASON_USE_ANOTHER_SERVER  :
            LogError("Connection refused: Use another server." );
            break;
        case MQTT_REASON_SERVER_MOVED :
            LogError("Connection refused: Server moved.");
            break;
        case MQTT_REASON_CON_RATE_EXCEED :
            LogError("Connection refused: Connection rate exceeded.");
            break;
        default:
            status= MQTTProtocolError;
    }
    return status;
}


static MQTTStatus_t validateConnackParams(const MQTTPacketInfo_t* pIncomingPacket, bool* pSessionPresent)
{

    MQTTStatus_t status = MQTTSuccess;
    if (pIncomingPacket == NULL)
    {
        LogError(("pIncomingPacket cannot be NULL."));
        status = MQTTBadParameter;
    }
    else if (pSessionPresent == NULL)
    {
        LogError(("pSessionPresent cannot be NULL for CONNACK packet."));
        status = MQTTBadParameter;
    }
    else if (pIncomingPacket->pRemainingData == NULL)
    {
        LogError(("Remaining data of incoming packet is NULL."));
        status = MQTTBadParameter;
    }
    else if (pIncomingPacket->type != MQTT_PACKET_TYPE_CONNACK) {

        LogError(("Packet type is invalid."));
        status = MQTTBadParameter;
    }
    else {
        const uint8_t* pRemainingData = NULL;
        pRemainingData = pIncomingPacket->pRemainingData;
        if ((pRemainingData[0] | 0x01U) != 0x01U)
        {
            LogError(("Reserved bits in CONNACK incorrect."));

            status = MQTTBadResponse;
        }
        else
        {
            /* Determine if the "Session Present" bit is set. This is the lowest bit of
             * the third byte in CONNACK. */
            if ((pRemainingData[0] & MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK)
                == MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK)
            {
                LogDebug(("CONNACK session present bit set."));
                *pSessionPresent = true;

                /* MQTT 5 specifies that the fourth byte in CONNACK must be 0 if the
                 * "Session Present" bit is set. */
                if (pRemainingData[1] != 0U)
                {
                    LogError(("Session Present bit is set, but connect return code in CONNACK is %u (nonzero).",
                        (unsigned int)pRemainingData[1]));
                    status = MQTTBadResponse;
                }
            }
            else
            {
                LogDebug(("CONNACK session present bit not set."));
                *pSessionPresent = false;
            }
        }
        if (status == MQTTSuccess)
        {
            /* In MQTT 5, only values 0 , 80 through 9F are valid CONNACK response codes. */
            // Change this
            if(pRemainingData[1]!=0u){
             status= logConnackResponseV5(pRemainingData[1]);
            }

        }
    }

    return status;
}

MQTTStatus_t decodeVariableLength(const uint8_t* pBuffer, size_t* length)
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
        if (multiplier > 2097152U) /* 128 ^ 3 */
        {
            remainingLength = MQTT_REMAINING_LENGTH_INVALID;

            LogError(("Invalid remaining length in the packet.\n"));

            status = MQTTBadResponse;
        }
        else
        {
            /* Get the next byte. It is at the next position after the bytes
             * decoded till now since the header of one byte was read before. */
            encodedByte = pBuffer[bytesDecoded];
            remainingLength += ((size_t)encodedByte & 0x7FU) * multiplier;
            multiplier *= 128U;
            bytesDecoded++;

        }

        /* If the response is incorrect, or no more data is available, then
         * break out of the loop. */
        if ((remainingLength == MQTT_REMAINING_LENGTH_INVALID) ||
            (status != MQTTSuccess))
        {
            break;
        }
    } while ((encodedByte & 0x80U) != 0U);

    if (status == MQTTSuccess)
    {
        /* Check that the decoded remaining length conforms to the MQTT specification. */
        expectedSize = remainingLengthEncodedSize(remainingLength);

        if (bytesDecoded != expectedSize)
        {
            LogError(("Expected and actual length of decoded bytes do not match.\n"));
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
    uint8_t encodedPropertyLength= 0U;
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
    pIndex = MQTT_SerializeConnectProperties(pIndex,pConnectProperties);
    
    if( pConnectProperties->outgoingUserPropSize >0){
    pIndex = serializeUserProperties(pIndex, pConnectProperties->outgoingUserProperty, pConnectProperties->outgoingUserPropSize);
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
        pIndex = MQTT_SerializePublishProperties(pWillInfo,pIndex);
         if (pWillInfo->contentTypeLength != 0U)
        {
            TEST_ASSERT_EQUAL_INT(MQTT_CONTENT_TYPE_ID,*pIndex);
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                            pWillInfo->contentType,
                                            pWillInfo->contentTypeLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }
        if (pWillInfo->responseTopicLength != 0U)
        {
            TEST_ASSERT_EQUAL_INT(MQTT_RESPONSE_TOPIC_ID,*pIndex);
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                            pWillInfo->responseTopic,
                                            pWillInfo->responseTopicLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }
        if (pWillInfo->correlationLength != 0U)
        {
            TEST_ASSERT_EQUAL_INT(MQTT_CORRELATION_DATA_ID,*pIndex);
            pIndex++;
            encodedStringLength = encodeString( encodedStringBuffer,
                                            pWillInfo->correlationData,
                                            pWillInfo->correlationLength );
            TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
            pIndex += encodedStringLength;
        }
        if (pWillInfo->userPropertySize != 0)
        {
         pIndex = serializeUserProperties(pIndex,pWillInfo->userProperty,pWillInfo->userPropertySize);
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


void test_MQTT_GetConnectPropertiesSize(void){
    MQTTConnectInfo_t connectInfo;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPublishInfo_t willInfo = { 0 };

    /* Call MQTT_GetConnectPropertiesSize() with various combinations of
     * incorrect paramters */

    status = MQTT_GetConnectPropertiesSize(NULL);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
   /*
    *Max Packet Size cannot be null
   */ 
    MQTTConnectProperties_t connectProperties;
    connectProperties.receiveMax=24;
    memset(&connectProperties, 0x0, sizeof(connectProperties));
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
    
    /*
    *Receive Maximum cannot be 0
   */ 
    connectProperties.isMaxPacketSize = true;
    connectProperties.maxPacketSize=40;
    connectProperties.receiveMax=0;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
    
     
    connectProperties.receiveMax=24;
    connectProperties.reqProbInfo=1;
     status = MQTT_GetConnectPropertiesSize(&connectProperties);
     TEST_ASSERT_EQUAL (MQTTSuccess,status);
     TEST_ASSERT_EQUAL_size_t (8,connectProperties.propertyLength);

    connectProperties.sessionExpiry=24;
     status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
     TEST_ASSERT_EQUAL_size_t (13,connectProperties.propertyLength);
    

     connectProperties.topicAliasMax=24;
     status = MQTT_GetConnectPropertiesSize(&connectProperties);
     TEST_ASSERT_EQUAL (MQTTSuccess,status);
     TEST_ASSERT_EQUAL_size_t (16,connectProperties.propertyLength);

     connectProperties.reqResInfo=1;
     status = MQTT_GetConnectPropertiesSize(&connectProperties);
     TEST_ASSERT_EQUAL (MQTTSuccess,status);
     TEST_ASSERT_EQUAL_size_t (18,connectProperties.propertyLength);

    connectProperties.reqProbInfo=0;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t (20,connectProperties.propertyLength);

    MQTTUserProperty_t userProperty;
    memset(&userProperty, 0x0, sizeof(userProperty));
    
    connectProperties.outgoingUserPropSize=1;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    connectProperties.outgoingUserProperty= &userProperty;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
   
    userProperty.keyLength=3;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    userProperty.valueLength=1;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    userProperty.value="1";
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);


    userProperty.key="211";
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(29,connectProperties.propertyLength);


    connectProperties.outgoingUserPropSize= 6;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
    connectProperties.outgoingUserPropSize= 1;


    /*
    * Incoming AuthInfo not intialized.
    */
    MQTTAuthInfo_t auth;
    memset(&auth, 0x0, sizeof(auth));
    connectProperties.outgoingAuth=&auth;
    auth.authDataLength=1;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
    
    MQTTAuthInfo_t auth2;
    memset(&auth2, 0x0, sizeof(auth2));
    connectProperties.incomingAuth=&auth2;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    /*
    *  Protocol Error to include Authentication Data if there is no Authentication Method
    */
    auth.authData="1";
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
    

    auth.authMethodLength=3;
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    auth.authMethod="234";
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(39,connectProperties.propertyLength);

}


void test_MQTT_GetWillPropertiesSize(void){
    MQTTConnectInfo_t connectInfo;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPublishInfo_t willInfo;
    /* Call MQTT_GetWillPropertiesSize() with various combinations of
     * incorrect paramters */
    status = MQTT_GetWillPropertiesSize(NULL);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
   /*
    *Max Packet Size cannot be null
   */ 
   
    memset(&willInfo,0x0,sizeof(willInfo));
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(0,willInfo.propertyLength);
    
    willInfo.willDelay =10;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(5,willInfo.propertyLength);

    willInfo.payloadFormat = 1;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(7,willInfo.propertyLength);

    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(12,willInfo.propertyLength);

    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(12,willInfo.propertyLength);

    willInfo.contentTypeLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
     
    willInfo.contentType="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(17,willInfo.propertyLength);
    
    willInfo.responseTopicLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    willInfo.responseTopic="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(22,willInfo.propertyLength);

    willInfo.correlationLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    willInfo.correlationData="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(27,willInfo.propertyLength);

    MQTTUserProperty_t userProperty[2];
    userProperty[0].key="2";
    userProperty[0].keyLength= 1;
    userProperty[0].valueLength=3;
    userProperty[0].value="abc";
    userProperty[1].key="2";
    userProperty[1].keyLength= 1;
    userProperty[1].valueLength=2;
    userProperty[1].value="ab";
    willInfo.userProperty= userProperty;
    willInfo.userPropertySize = 2;
    status = MQTT_GetWillPropertiesSize(&willInfo);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(44,willInfo.propertyLength);

}

void  test_MQTT_SerializeConnectProperties(void){
uint8_t properties[24U];
uint8_t *pIndex = properties;
uint8_t *index = properties;
MQTTConnectProperties_t connect;
MQTTStatus_t status;
size_t propertyLength;
memset(&connect,0x0,sizeof(connect));
connect.sessionExpiry=12;
connect.receiveMax = 32;
connect.isMaxPacketSize =true;
connect.maxPacketSize= 56;
connect.topicAliasMax=11;
connect.reqResInfo = 1;
connect.reqProbInfo = 0;
connect.propertyLength= 20;
pIndex = MQTT_SerializeConnectProperties(pIndex,&connect);
TEST_ASSERT_EQUAL_INT(21,(pIndex-properties));
status = decodeVariableLength(properties, &propertyLength);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
TEST_ASSERT_EQUAL_size_t(connect.propertyLength,propertyLength);
index++;

TEST_ASSERT_EQUAL_INT(MQTT_SESSION_EXPIRY_ID,*index);
index++;
TEST_ASSERT_EQUAL_UINT32(connect.sessionExpiry,UINT32_DECODE(index));
index+=4;

TEST_ASSERT_EQUAL_INT(MQTT_RECEIVE_MAX_ID,*index);
index++;
TEST_ASSERT_EQUAL_UINT32(connect.receiveMax,UINT16_DECODE(index));
index+=2;

TEST_ASSERT_EQUAL_INT(MQTT_MAX_PACKET_SIZE_ID,*index);
index++;
TEST_ASSERT_EQUAL_INT(connect.maxPacketSize,UINT32_DECODE(index));
index+=4;

TEST_ASSERT_EQUAL_INT(MQTT_TOPIC_ALIAS_MAX_ID,*index);
index++;
TEST_ASSERT_EQUAL_INT(connect.topicAliasMax,UINT16_DECODE(index));
index+=2;

TEST_ASSERT_EQUAL_INT(MQTT_REQUEST_RESPONSE_ID,*index);
index++;
TEST_ASSERT_EQUAL_INT(connect.reqResInfo,*index);
index++;

TEST_ASSERT_EQUAL_INT(MQTT_REQUEST_PROBLEM_ID,*index);
index++;
TEST_ASSERT_EQUAL_INT(connect.reqProbInfo,*index);
index++;

};

void  test_MQTT_SerializePublishProperties(void){
    uint8_t properties[16U];
    uint8_t *index = properties;
    MQTTPublishInfo_t will;
    will.payloadFormat = 1;
    will.msgExpiryPresent=true;
    will.msgExpiryInterval= 34;
    will.willDelay=12;
    index= MQTT_SerializePublishProperties(&will,index);
    TEST_ASSERT_EQUAL_INT(13,index-properties);

}

// void test_MQTTV5_DeserializeConnackOnlyStatus(void){
//     MQTTPacketInfo_t packetInfo;
//     MQTTConnectProperties_t properties;
//     MQTTStatus_t status;
//     uint8_t buffer[50];
//     uint8_t* index= buffer;
//     memset(&properties, 0x0, sizeof(properties));
//     memset(&packetInfo, 0x0, sizeof(packetInfo));
//     status = MQTTV5_DeserializeConnack(NULL,NULL,NULL);
//     TEST_ASSERT_EQUAL(MQTTBadParameter,status);

//     bool sessionPresent;
//     status = MQTTV5_DeserializeConnack(NULL,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTBadParameter,status);

//     status = MQTTV5_DeserializeConnack(&properties,NULL,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTBadParameter,status);
    
//     packetInfo.type=MQTT_PACKET_TYPE_CONNACK;
//     status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTBadParameter,status);

//     packetInfo.pRemainingData=index;
//     packetInfo.type =MQTT_PACKET_TYPE_CONNECT;
//     status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTBadParameter,status);

// /*
// * Session Present Bit is set but reason code is not equal to 0;
// */
//     buffer[0] = 0x01;
//     buffer[1] = 0x01;
//     packetInfo.type=MQTT_PACKET_TYPE_CONNACK;
//     status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTBadResponse,status);

//     buffer[0] = 0x00;
//     buffer[1]= 0x00;
//     size_t propertyLength = encodeRemainingLength(buffer[2],0);
//     properties.isMaxPacketSize= true;
//     properties.isMaxPacketSize = 0;
//     packetInfo.remainingLength = 100;
//     status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTProtocolError,status);
//     TEST_ASSERT_EQUAL(false,sessionPresent);
    
//     properties.isMaxPacketSize = 1000;
//     status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     TEST_ASSERT_EQUAL(MQTTMalformedPacket,status);
//     TEST_ASSERT_EQUAL(false,sessionPresent);
    
    
// /* Not a valid response code*/
//     // buffer[1]= 0x03;
//     // status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     // TEST_ASSERT_EQUAL(MQTTProtocolError,status);

//     // // buffer[1] = 0x80;
//     // status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
//     // TEST_ASSERT_EQUAL(MQTTServerRefused,status);


// }

void test_MQTTV5_DeserializeConnack(void){
     MQTTPacketInfo_t packetInfo;
    MQTTConnectProperties_t properties;
    MQTTStatus_t status;
    memset(&properties, 0x0, sizeof(properties));
    memset(&packetInfo, 0x0, sizeof(packetInfo));
    uint8_t buffer[200]={0};
    uint8_t *pIndexLocal= buffer;
    buffer[0]= 0x01;
    buffer[1] = 0x00;
    size_t dummy =0;
    bool session= false;
    packetInfo.pRemainingData=buffer;
    packetInfo.type = MQTT_PACKET_TYPE_CONNACK;
    // property = 49
    // Session expiry 5 bytes
    // receive max  3 bytes
    // max qos 2 bytes
    // retain available  2 bytes
    // max packet size 5 bytes
    //  assigned client id 5
    // Topic alias max  3
    // reason string  5
    // user property 0
    // wildcard  2
    // subscription id  2byte
    // shared subscription   2byte
    //  server keep alive  3 byte
    // response info 5
    // server ref  5
    // auth 0
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
size_t propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_SESSION_EXPIRY_ID;
pIndexLocal++;
pIndexLocal[0] = UINT32_BYTE3(12);
pIndexLocal[1] = UINT32_BYTE2(12);
pIndexLocal[2] = UINT32_BYTE1(12);
pIndexLocal[3] = UINT32_BYTE0(12);
pIndexLocal =  &pIndexLocal[4];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// 8
packetInfo.remainingLength = 6;
pIndexLocal = &buffer[2];
propertyLength = encodeRemainingLength(pIndexLocal,3);
pIndexLocal++;
*pIndexLocal = MQTT_RECEIVE_MAX_ID;
pIndexLocal++;
pIndexLocal[0] = UINT16_HIGH_BYTE(13);
pIndexLocal[1] = UINT16_LOW_BYTE(13);
pIndexLocal = &pIndexLocal[2];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// 11
packetInfo.remainingLength = 5;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,2);
pIndexLocal++;
*pIndexLocal = MQTT_MAX_QOS_ID;
pIndexLocal++;
pIndexLocal[0]= 0;
pIndexLocal++;
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// 13
packetInfo.remainingLength = 5;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,2);
pIndexLocal++;
*pIndexLocal = MQTT_RETAIN_AVAILABLE_ID;
pIndexLocal++;
pIndexLocal[0]= 0;
pIndexLocal++;
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 15
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_MAX_PACKET_SIZE_ID;
pIndexLocal++;
pIndexLocal[0] = UINT32_BYTE3(120);
pIndexLocal[1] = UINT32_BYTE2(120);
pIndexLocal[2] = UINT32_BYTE1(120);
pIndexLocal[3] = UINT32_BYTE0(120);
pIndexLocal =  &pIndexLocal[4];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 20
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_ASSIGNED_CLIENT_ID;
pIndexLocal++;
char string = "ab";
dummy = encodeString(pIndexLocal,&string,2);
pIndexLocal++;
pIndexLocal =  &pIndexLocal[dummy];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);

// // 25
packetInfo.remainingLength = 6;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,3);
pIndexLocal++;
*pIndexLocal = MQTT_TOPIC_ALIAS_MAX_ID;
pIndexLocal++;
pIndexLocal[0] = UINT16_HIGH_BYTE(13);
pIndexLocal[1] = UINT16_LOW_BYTE(13);
pIndexLocal = &pIndexLocal[2];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 28
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_REASON_STRING_ID;
pIndexLocal++;
 string = "ab";
dummy = encodeString(pIndexLocal,&string,2);
pIndexLocal =  &pIndexLocal[dummy];
// status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
// TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 33
packetInfo.remainingLength = 5;
pIndexLocal = &buffer[2];
propertyLength = encodeRemainingLength(pIndexLocal,2);
pIndexLocal++;
*pIndexLocal = MQTT_WILDCARD_ID;
pIndexLocal++;
pIndexLocal[0]= 0;
pIndexLocal++;
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 35
packetInfo.remainingLength = 5;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,2);
pIndexLocal++;
*pIndexLocal = MQTT_SUB_AVAILABLE_ID;
pIndexLocal++;
pIndexLocal[0]= 0;
pIndexLocal++;
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 37
packetInfo.remainingLength = 5;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,2);
pIndexLocal++;
*pIndexLocal = MQTT_SHARED_SUB_ID;
pIndexLocal++;
pIndexLocal[0]= 0;
pIndexLocal++;
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 38
packetInfo.remainingLength = 6;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,3);
pIndexLocal++;
*pIndexLocal = MQTT_SERVER_KEEP_ALIVE_ID;
pIndexLocal[0] = UINT16_HIGH_BYTE(13);
pIndexLocal[1] = UINT16_LOW_BYTE(13);
pIndexLocal = &pIndexLocal[2];
// status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
// TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// // 41
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_RESPONSE_INFO_ID;
pIndexLocal++;
 string = "ab";
dummy = encodeString(pIndexLocal,&string,2);
pIndexLocal =  &pIndexLocal[dummy];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTProtocolError,status);

properties.reqResInfo =1;
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_RESPONSE_INFO_ID;
pIndexLocal++;
 string = "ab";
dummy = encodeString(pIndexLocal,&string,2);
pIndexLocal =  &pIndexLocal[dummy];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);

// // 46
packetInfo.remainingLength = 8;
pIndexLocal = &buffer[2];
 propertyLength = encodeRemainingLength(pIndexLocal,5);
pIndexLocal++;
*pIndexLocal = MQTT_SERVER_REF_ID;
pIndexLocal++;
 string = "ab";
dummy = encodeString(pIndexLocal,&string,2);
pIndexLocal =  &pIndexLocal[dummy];
status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&session);
TEST_ASSERT_EQUAL_INT(MQTTSuccess,status);
// 51
}

void test_MQTTV5_GetConnectPacketSize( void )
{
    MQTTConnectInfo_t connectInfo;
     size_t remainingLength = 0;
    size_t packetSize = 0;
    MQTTStatus_t status = MQTTSuccess;
    MQTTPublishInfo_t willInfo = { 0 };
    MQTTConnectProperties_t properties; 
    memset(&properties, 0x0, sizeof(properties));
    /* Call MQTTV5_GetConnectPacketSize() with various combinations of
     * incorrect paramters */
    properties.receiveMax = 65535U;
    properties.maxPacketSize= UINT32_MAX;
    properties.reqProbInfo = 1;
    status = MQTTV5_GetConnectPacketSize( NULL, NULL, &properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, NULL, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Verify empty connect info fails. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );


    /* Verify empty client identifier fails. */
    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    connectInfo.pClientIdentifier = NULL;
    connectInfo.clientIdentifierLength = CLIENT_IDENTIFIER_LENGTH;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Test a will message payload length that is too large. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.pClientIdentifier = CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = UINT16_MAX;
    connectInfo.pPassword = "";
    connectInfo.passwordLength = UINT16_MAX;
    connectInfo.pUserName = "";
    connectInfo.userNameLength = UINT16_MAX;
    willInfo.pTopicName = TEST_TOPIC_NAME;
    willInfo.topicNameLength = UINT16_MAX;
    /* A valid will message payload is less than the maximum 16 bit integer. */
    willInfo.payloadLength = UINT16_MAX + 2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Verify good case */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.cleanSession = true;
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    TEST_ASSERT_EQUAL_INT( 0, properties.propertyLength );
    /* Make sure remaining size returned is 17. */
    TEST_ASSERT_EQUAL_INT( 17, remainingLength );
    /* Make sure packet size is 19. */
    TEST_ASSERT_EQUAL_INT( 19, packetSize );


    /* With will. These parameters will cause the packet to be
     * 4 + 2 + 8 + 2 = 16 bytes larger. */
    memset( &willInfo, 0x0, sizeof( willInfo ) );
    willInfo.pTopicName = "test";
    willInfo.topicNameLength = 4;
    willInfo.pPayload = "testload";
    willInfo.payloadLength = 8;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
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
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 28 = 16 + 12. */
    TEST_ASSERT_EQUAL_INT( 29, remainingLength );
    /* Make sure packet size is 30 = 18 + 12. */
    TEST_ASSERT_EQUAL_INT( 31, packetSize );

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,NULL, &remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

  /*Connect Properties*/
     properties.sessionExpiry= 22;
     properties.receiveMax =34;
     properties.isMaxPacketSize =true;
     properties.maxPacketSize =32;
     properties.topicAliasMax = 12;
     properties.reqResInfo = 1;
     properties.reqProbInfo = 0;
     MQTTUserProperty_t userProperty;
    memset(&userProperty, 0x0, sizeof(userProperty));
    userProperty.keyLength=3;
    userProperty.valueLength=1;
    userProperty.value="1";
    userProperty.key="211";
    properties.outgoingUserPropSize= 1;
    properties.outgoingUserProperty = &userProperty;

    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 58. */
    TEST_ASSERT_EQUAL_INT( 58, remainingLength );
    /* Make sure packet size is 60. */
    TEST_ASSERT_EQUAL_INT( 60, packetSize );

    willInfo.payloadFormat = 1;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.contentTypeLength =2;  
    willInfo.contentType="ab";
    willInfo.responseTopicLength =2;
    willInfo.responseTopic="ab";
    willInfo.correlationLength =2;
    willInfo.correlationData="ab";
    willInfo.willDelay = 3;
    // 34 + 12 + 29
    status = MQTTV5_GetConnectPacketSize( &connectInfo,&willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    /* Make sure remaining size returned is 50. */
    TEST_ASSERT_EQUAL_INT( 102, remainingLength );
    /* Make sure packet size is 52. */
    TEST_ASSERT_EQUAL_INT( 104, packetSize );
}


// /**
//  * @brief Tests that MQTTV5_SerializeConnect works as intended.
//  */
void test_MQTTV5_SerializeConnect( void )
{
    MQTTConnectInfo_t connectInfo;
    MQTTPublishInfo_t willInfo = { 0 };
    size_t remainingLength = 0;
    uint8_t buffer[ 70 + 2 * BUFFER_PADDING_LENGTH ];
    size_t bufferSize = sizeof( buffer ) - 2 * BUFFER_PADDING_LENGTH;
    size_t packetSize = bufferSize;
    MQTTStatus_t status = MQTTSuccess;
    MQTTConnectProperties_t properties;
    memset(&properties,0x0, sizeof(properties));
    properties.receiveMax = 65535U;
    properties.isMaxPacketSize =true;
    properties.maxPacketSize= UINT32_MAX;
    properties.reqProbInfo = 1;
    MQTTFixedBuffer_t fixedBuffer = { .pBuffer = &buffer[ BUFFER_PADDING_LENGTH ], .size = bufferSize };

    /* Verify bad parameter errors. */
    status = MQTTV5_SerializeConnect( NULL, &willInfo,&properties,remainingLength,&fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    status =MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties, remainingLength, NULL );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, 120, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTNoMemory, status );
    
    status = MQTTV5_SerializeConnect( &connectInfo,&willInfo,NULL, 120, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );
    

    /* Create a good connection info. */
    memset( &connectInfo, 0x0, sizeof( connectInfo ) );
    connectInfo.pClientIdentifier = "TEST";
    connectInfo.clientIdentifierLength = 4;

    /* Inject a invalid fixed buffer test with a good connectInfo. */
    memset( &fixedBuffer, 0x0, sizeof( fixedBuffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTBadParameter, status );

    /* Good case succeeds. */
    /* Set the fixedBuffer properly for the rest of the succeeding test. */
    fixedBuffer.pBuffer = &buffer[ BUFFER_PADDING_LENGTH ];
    fixedBuffer.size = bufferSize;

    /* Calculate a good packet size. */
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL ,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    /* Make sure buffer has enough space */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Make sure test succeeds. */
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL_INT( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Encode user name. Also try clean session. */
    connectInfo.cleanSession = true;
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 4;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Serialize connect with LWT. */
    /* Test for NULL topic name. */
    ( void ) memset( &willInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    willInfo.retain = true;
    willInfo.qos = MQTTQoS1;
    willInfo.pPayload = "test";
    willInfo.payloadLength = ( uint16_t ) strlen( willInfo.pPayload );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    status = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    /* Success. */
    ( void ) memset( &willInfo, 0x00, sizeof( MQTTPublishInfo_t ) );
    willInfo.retain = true;
    willInfo.qos = MQTTQoS1;
    willInfo.pTopicName = "test";
    willInfo.topicNameLength = ( uint16_t ) strlen( willInfo.pTopicName );
    willInfo.pPayload = "test";
    willInfo.payloadLength = ( uint16_t ) strlen( willInfo.pPayload );
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Again with QoS 2 and 0. */

    willInfo.qos = MQTTQoS2;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties,remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    willInfo.qos = MQTTQoS0;
    willInfo.retain = false;
    /* NULL payload is acceptable. */
    willInfo.pPayload = NULL;
    willInfo.payloadLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, &willInfo,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /* Success right on the buffer boundary. */
    connectInfo.pUserName = "USER";
    connectInfo.userNameLength = 4;
    /* Throwing in a possible valid zero length password. */
    connectInfo.pPassword = "PASS";
    connectInfo.passwordLength = 0;
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*Connect Properties*/
     properties.sessionExpiry= 22;
     properties.receiveMax =34;
     properties.maxPacketSize =32;
     properties.topicAliasMax = 12;
     properties.reqResInfo = 1;
     properties.reqProbInfo = 0;
     MQTTUserProperty_t userProperty;
    memset(&userProperty, 0x0, sizeof(userProperty));
    userProperty.keyLength=3;
    userProperty.valueLength=1;
    userProperty.value="1";
    userProperty.key="211";
    properties.outgoingUserPropSize= 1;
    properties.outgoingUserProperty = &userProperty;
    //  29
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

    /*Will Properties*/
    willInfo.payloadFormat = 1;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.contentTypeLength =2;  
    willInfo.contentType="ab";
    willInfo.responseTopicLength =2;
    willInfo.responseTopic="ab";
    willInfo.correlationLength =2;
    willInfo.correlationData="ab";
    willInfo.willDelay = 3;
    // 27
    status = MQTTV5_GetConnectPacketSize( &connectInfo, NULL,&properties, &remainingLength, &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, bufferSize );
    /* Set the fixed buffer to exactly the size of the packet. */
    fixedBuffer.size = packetSize;
    padAndResetBuffer( buffer, sizeof( buffer ) );
    status = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties, remainingLength, &fixedBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    checkBufferOverflow( buffer, sizeof( buffer ) );

}

void test_serializeUserProperties(void){
  MQTTUserProperty_t userProperty[2];
    userProperty[0].key="2";
    userProperty[0].keyLength= 1;
    userProperty[0].valueLength=3;
    userProperty[0].value="abc";
    userProperty[1].key="2";
    userProperty[1].keyLength= 1;
    userProperty[1].valueLength=2;
    userProperty[1].value="ab";
    size_t encodedStringLength= 0U;
    uint8_t buffer[50];
    uint8_t *pIndex= buffer;
    // pIndex= serializeUserProperties(pIndex,&userProperty,1);
    // TEST_ASSERT_EQUAL_INT(MQTT_USER_PROPERTY_ID,*pIndex);
    // pIndex++;
    // encodedStringLength = encodeString( encodedStringBuffer,
    //                                         userProperty->key,
    //                                         userProperty->keyLength);
    // TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    // pIndex += encodedStringLength;
    // encodedStringLength = encodeString( encodedStringBuffer,
    //                                         userProperty->value,
    //                                         userProperty->valueLength);
    // TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    // pIndex += encodedStringLength;

    // TEST_ASSERT_EQUAL_INT(MQTT_USER_PROPERTY_ID,*pIndex);
    // pIndex++;
    // encodedStringLength = encodeString( encodedStringBuffer,
    //                                         (userProperty+1)->key,
    //                                         (userProperty+1)->keyLength);
    // TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    // pIndex += encodedStringLength;

    // encodedStringLength = encodeString( encodedStringBuffer,
    //                                         (userProperty+1)->value,
    //                                         (userProperty+1)->valueLength);
    // TEST_ASSERT_EQUAL_MEMORY( encodedStringBuffer, pIndex, encodedStringLength );
    // pIndex += encodedStringLength;                                     
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
    MQTTConnectInfo_t connectInfo;
    MQTTPublishInfo_t willInfo;
    MQTTConnectProperties_t properties;
    memset(&properties,0x0, sizeof(properties));
    memset(&willInfo,0x0, sizeof(willInfo));
     properties.sessionExpiry= 22;
    properties.receiveMax =34;
    properties.maxPacketSize =32;
    properties.topicAliasMax = 12;
    properties.reqResInfo = 1;
    properties.reqProbInfo = 0;
    willInfo.payloadFormat = 1;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    willInfo.contentTypeLength =2;  
    willInfo.contentType="ab";
    willInfo.responseTopicLength =2;
    willInfo.responseTopic="ab";
    willInfo.correlationLength =2;
    willInfo.correlationData="ab";
    willInfo.willDelay = 3;
    /* Fill structs to pass into methods to be tested. */
    setupNetworkBuffer( &networkBuffer );
    setupConnectInfo( &connectInfo );
    setupPublishInfo( &willInfo );
    willInfo.dup = true;
    willInfo.retain = true;
    /* Get MQTT connect packet size and remaining length. */
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                            &willInfo,
                                            &properties,
                                            &remainingLength,
                                            &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo,&willInfo,&properties,
                                        remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo,&willInfo,&properties,
                                   remainingLength,&networkBuffer );
    
    // /* Repeat with MQTTQoS1. */
    willInfo.qos = MQTTQoS1;
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                            &willInfo,
                                            &properties,
                                            &remainingLength,
                                            &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties,
                                        remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, &willInfo,&properties,
                                   remainingLength, &networkBuffer );


    /* Re-initialize objects for branch coverage. */
    willInfo.pPayload = MQTT_SAMPLE_PAYLOAD;
    willInfo.payloadLength = MQTT_SAMPLE_PAYLOAD_LEN;
    willInfo.pTopicName = MQTT_CLIENT_IDENTIFIER;
    willInfo.topicNameLength = MQTT_CLIENT_IDENTIFIER_LEN;
    willInfo.dup = true;
    willInfo.qos = MQTTQoS2;
    willInfo.retain = false;
    connectInfo.cleanSession = false;
    connectInfo.pClientIdentifier = MQTT_CLIENT_IDENTIFIER;
    connectInfo.clientIdentifierLength = MQTT_CLIENT_IDENTIFIER_LEN;
    connectInfo.pUserName = NULL;
    connectInfo.userNameLength = 0;
    connectInfo.pPassword = NULL;
    connectInfo.passwordLength = 0;

    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                            &willInfo,
                                            &properties,
                                            &remainingLength,
                                            &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, &willInfo,&properties,
                                        remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, &willInfo,&properties,
                                   remainingLength, &networkBuffer );


    /* Repeat with NULL pWillInfo. */
    mqttStatus = MQTTV5_GetConnectPacketSize( &connectInfo,
                                            NULL,
                                            &properties,
                                            &remainingLength,
                                            &packetSize );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    /* Make sure buffer has enough space. */
    TEST_ASSERT_GREATER_OR_EQUAL( packetSize, networkBuffer.size );
    mqttStatus = MQTTV5_SerializeConnect( &connectInfo, NULL,&properties,
                                        remainingLength, &networkBuffer );
    TEST_ASSERT_EQUAL( MQTTSuccess, mqttStatus );
    verifySerializedConnectPacket( &connectInfo, NULL,&properties,
                                   remainingLength, &networkBuffer );
}

