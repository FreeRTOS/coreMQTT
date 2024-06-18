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
    MQTT_GetUserPropertySize_CMockExpectAndReturn(NULL,&userProperty, connectProperties.outgoingUserPropSize,NULL, MQTTBadParameter);
    MQTT_GetUserPropertySize_CMockIgnoreArg_length();
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    userProperty.valueLength=1;
    MQTT_GetUserPropertySize_CMockExpectAndReturn(NULL,&userProperty, connectProperties.outgoingUserPropSize,NULL, MQTTBadParameter);
    MQTT_GetUserPropertySize_CMockIgnoreArg_length();
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    userProperty.value="1";
    MQTT_GetUserPropertySize_CMockExpectAndReturn(NULL,&userProperty, connectProperties.outgoingUserPropSize,NULL, MQTTBadParameter);
    MQTT_GetUserPropertySize_CMockIgnoreArg_length();
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);


    userProperty.key="211";
    status = MQTT_GetConnectPropertiesSize(&connectProperties);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(29,connectProperties.propertyLength);


    connectProperties.outgoingUserPropSize= 6;
    MQTT_GetUserPropertySize_CMockExpectAndReturn(NULL,&userProperty, connectProperties.outgoingUserPropSize,NULL, MQTTBadParameter);
    MQTT_GetUserPropertySize_CMockIgnoreArg_length();
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
    uint32_t willDelay= 10;

    /* Call MQTT_GetWillPropertiesSize() with various combinations of
     * incorrect paramters */

    status = MQTT_GetWillPropertiesSize(NULL,1);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
   /*
    *Max Packet Size cannot be null
   */ 
   
    memset(&willInfo,0x0,sizeof(willInfo));
    status = MQTT_GetWillPropertiesSize(&willInfo,0);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(0,willInfo.propertyLength);
    

    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(5,willInfo.propertyLength);

    willInfo.payloadFormat = 1;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(7,willInfo.propertyLength);

    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(12,willInfo.propertyLength);

    willInfo.msgExpiryPresent = 1;
    willInfo.msgExpiryInterval= 10;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(12,willInfo.propertyLength);

    willInfo.contentTypeLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);
     
    willInfo.contentType="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(17,willInfo.propertyLength);
    
    willInfo.responseTopicLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    willInfo.responseTopic="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTSuccess,status);
    TEST_ASSERT_EQUAL_size_t(22,willInfo.propertyLength);

    willInfo.correlationLength =2;
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
    TEST_ASSERT_EQUAL (MQTTBadParameter,status);

    willInfo.correlationData="ab";
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
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
    status = MQTT_GetWillPropertiesSize(&willInfo,willDelay);
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
    index= MQTT_SerializePublishProperties(&will,index,12);
    TEST_ASSERT_EQUAL_INT(13,index-properties);

}

void test_MQTTV5_DeserializeConnackOnlyStatus(void){
    MQTTPacketInfo_t packetInfo;
    MQTTConnectProperties_t properties;
    MQTTStatus_t status;
    uint8_t buffer[50];
    uint8_t* index= buffer;
    memset(&properties, 0x0, sizeof(properties));
    memset(&packetInfo, 0x0, sizeof(packetInfo));
    status = MQTTV5_DeserializeConnack(NULL,NULL,NULL);
    TEST_ASSERT_EQUAL(MQTTBadParameter,status);

    bool sessionPresent;
    status = MQTTV5_DeserializeConnack(NULL,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTBadParameter,status);

    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTBadParameter,status);
    
    packetInfo.type=MQTT_PACKET_TYPE_CONNACK;
    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTBadParameter,status);

    packetInfo.pRemainingData=index;
    packetInfo.type =MQTT_PACKET_TYPE_CONNECT;
    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTBadParameter,status);

/*
* Session Present Bit is set but reason code is not equal to 0;
*/
    buffer[0] = 0x01;
    buffer[1] = 0x01;
    packetInfo.type=MQTT_PACKET_TYPE_CONNACK;
    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTBadResponse,status);

    buffer[0] = 0x00;
    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTSuccess,status);
    TEST_ASSERT_EQUAL(false,sessionPresent);

    buffer[1] = 0x01;
    status = MQTTV5_DeserializeConnack(&properties,&packetInfo,&sessionPresent);
    TEST_ASSERT_EQUAL(MQTTSuccess,status);
    TEST_ASSERT_EQUAL(true,sessionPresent);


    buffer[1] = 0x01;
    



}


