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
#include "core_mqtt_config_defaults.h"
#include "core_mqtt_serializer.h"
#include "core_mqtt.h"
#include "core_mqtt_serializer_private.h"

void test_MQTTPropAdd_SubscriptionId_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint32_t subscriptionId;
    uint8_t OptionalMqttPacketType;
    MQTTStatus_t status;
    uint32_t val = 0;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, 0, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    subscriptionId = 1;
    status = MQTTPropAdd_SubscriptionId( NULL, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_SUBSCRIPTION_ID_POS );
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    subscriptionId = ~( ( uint32_t ) ( 0UL ) );
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    subscriptionId = 1;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( PropertyBuilder.fieldSet & ( 1 << MQTT_SUBSCRIPTION_ID_POS ), 1 << MQTT_SUBSCRIPTION_ID_POS );
    TEST_ASSERT_EQUAL( PropertyBuilder.pBuffer[ 0 ], MQTT_SUBSCRIPTION_ID_ID );
    /* Only one byte required to encode value of 1. */
    TEST_ASSERT_EQUAL( PropertyBuilder.pBuffer[ 1 ], 1 );
    TEST_ASSERT_EQUAL( PropertyBuilder.currentIndex, 2 );

    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    subscriptionId = 900;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( PropertyBuilder.fieldSet & ( 1 << MQTT_SUBSCRIPTION_ID_POS ), 1 << MQTT_SUBSCRIPTION_ID_POS );
    TEST_ASSERT_EQUAL( PropertyBuilder.pBuffer[ 0 ], MQTT_SUBSCRIPTION_ID_ID );
    /* Only two bytes required to encode value of 900. */
    TEST_ASSERT_EQUAL( 2 + 1, PropertyBuilder.currentIndex );
    TEST_ASSERT_EQUAL( 128 | 4, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL( 7, PropertyBuilder.pBuffer[ 2 ] );
    status = decodeVariableLength( &PropertyBuilder.pBuffer[ 1 ], 4, &val );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( 900, val );

    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    subscriptionId = 9000;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( PropertyBuilder.fieldSet & ( 1 << MQTT_SUBSCRIPTION_ID_POS ), 1 << MQTT_SUBSCRIPTION_ID_POS );
    TEST_ASSERT_EQUAL( PropertyBuilder.pBuffer[ 0 ], MQTT_SUBSCRIPTION_ID_ID );
    /* Only two bytes required to encode value of 9000. */
    TEST_ASSERT_EQUAL( 2 + 1, PropertyBuilder.currentIndex );
    TEST_ASSERT_EQUAL( 128 | 40, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL( 70, PropertyBuilder.pBuffer[ 2 ] );
    status = decodeVariableLength( &PropertyBuilder.pBuffer[ 1 ], 4, &val );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( 9000, val );

    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    subscriptionId = 2097152U;
    status = MQTTPropAdd_SubscriptionId( &PropertyBuilder, subscriptionId, NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( PropertyBuilder.fieldSet & ( 1 << MQTT_SUBSCRIPTION_ID_POS ), 1 << MQTT_SUBSCRIPTION_ID_POS );
    TEST_ASSERT_EQUAL( PropertyBuilder.pBuffer[ 0 ], MQTT_SUBSCRIPTION_ID_ID );
    /* Four bytes required to encode value. */
    TEST_ASSERT_EQUAL( 4 + 1, PropertyBuilder.currentIndex );
    TEST_ASSERT_EQUAL( 128 | 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL( 128 | 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 128 | 0, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL( 1, PropertyBuilder.pBuffer[ 4 ] );
    status = decodeVariableLength( &PropertyBuilder.pBuffer[ 1 ], 4, &val );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( 2097152U, val );
}

void test_MQTTPropAdd_UserProp_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    MQTTUserProperty_t userProperty = { 0 };
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_UserProp( NULL, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.bufferLength = 100;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, NULL, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = NULL;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = "Key";
    userProperty.pValue = NULL;
    userProperty.keyLength = 3;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 0;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 3;
    userProperty.valueLength = 0;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 3;
    userProperty.valueLength = 5;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 3;
    userProperty.valueLength = 5;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 3;
    userProperty.valueLength = 5;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID;
    PropertyBuilder.currentIndex -= 7;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 10;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    userProperty.pKey = "Key";
    userProperty.pValue = "Value";
    userProperty.keyLength = 3;
    userProperty.valueLength = 5;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    status = MQTTPropAdd_UserProp( &PropertyBuilder, &userProperty, NULL );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_USER_PROPERTY_ID, PropertyBuilder.pBuffer[ 0 ] );
    /* Encoded key length. */
    TEST_ASSERT_EQUAL( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL( 3, PropertyBuilder.pBuffer[ 2 ] );
    /* Key itself. */
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Key", 3 ) );

    /* Encoded key length. */
    TEST_ASSERT_EQUAL( 0, PropertyBuilder.pBuffer[ 6 ] );
    TEST_ASSERT_EQUAL( 5, PropertyBuilder.pBuffer[ 7 ] );
    /* Key itself. */
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 8 ], "Value", 5 ) );
}

void test_MQTTPropAdd_SessionExpiry_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint32_t sessionExpiry = 0;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_SessionExpiry( NULL, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_SESSION_EXPIRY_INTERVAL_POS );
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    sessionExpiry = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_SESSION_EXPIRY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 4 ] );


    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    sessionExpiry = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTPropAdd_SessionExpiry( &PropertyBuilder, sessionExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_SESSION_EXPIRY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_ReceiveMax_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint16_t receiveMax = 10;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTPropAdd_ReceiveMax( NULL, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_RECEIVE_MAXIMUM_POS );
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    receiveMax = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_RECEIVE_MAX_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    receiveMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTPropAdd_ReceiveMax( &PropertyBuilder, receiveMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_RECEIVE_MAX_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_MaxPacketSize_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint32_t maxPacketSize = 10;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_MaxPacketSize( NULL, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_MAX_PACKET_SIZE_POS );
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    maxPacketSize = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_MAX_PACKET_SIZE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 4 ] );


    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    maxPacketSize = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTPropAdd_MaxPacketSize( &PropertyBuilder, maxPacketSize, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_MAX_PACKET_SIZE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_MessageExpiry_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint32_t messageExpiry = 10;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_MessageExpiry( NULL, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_MESSAGE_EXPIRY_INTERVAL_POS );
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    messageExpiry = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_MSG_EXPIRY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    messageExpiry = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_MSG_EXPIRY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    messageExpiry = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_MessageExpiry( &PropertyBuilder, messageExpiry, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTTPropAdd_WillDelayInterval_AllInputs( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint32_t willDelayInterVal = 10;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_WillDelayInterval( NULL, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_WILL_DELAY_POS );
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    willDelayInterVal = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_WILL_DELAY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 4 ] );


    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    willDelayInterVal = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_WILL_DELAY_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    willDelayInterVal = 1000;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_WillDelayInterval( &PropertyBuilder, willDelayInterVal, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
}

void test_MQTTPropAdd_TopicAliasMax_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint16_t topicAliasMax = 10;
    uint8_t OptionalMqttPacketType = 0;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_TopicAliasMax( NULL, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_TOPIC_ALIAS_MAX_POS );
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TOPIC_ALIAS_MAX_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TOPIC_ALIAS_MAX_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNACK;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TOPIC_ALIAS_MAX_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PINGREQ;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PINGRESP;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_TopicAliasMax( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_TopicAlias_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint16_t topicAliasMax = 10;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_TopicAlias( NULL, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_TOPIC_ALIAS_POS );
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 30;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TOPIC_ALIAS_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 30, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL( 0xAA, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_TOPIC_ALIAS_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 3, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 232, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    topicAliasMax = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_TopicAlias( &PropertyBuilder, topicAliasMax, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_RequestRespInfo_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    bool requestRespInfo = false;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_RequestRespInfo( NULL, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_REQUEST_RESPONSE_INFO_POS );
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = false;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REQUEST_RESPONSE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = true;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REQUEST_RESPONSE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 1, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_RequestRespInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_RequestProbInfo_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    bool requestRespInfo = false;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_RequestProbInfo( NULL, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_REQUEST_PROBLEM_INFO_POS );
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = false;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REQUEST_PROBLEM_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = true;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REQUEST_PROBLEM_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 1, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_RequestProbInfo( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_PayloadFormat_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    bool requestRespInfo = false;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    MQTTStatus_t status;
    uint8_t buffer[ 100 ];

    status = MQTTPropAdd_PayloadFormat( NULL, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_PAYLOAD_FORMAT_INDICATOR_POS );
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = 0;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID - 1;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 20;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 0;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = false;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_PAYLOAD_FORMAT_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = true;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    memset( buffer, 0xAA, 100 );
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_PAYLOAD_FORMAT_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 1, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    requestRespInfo = 1000;
    memset( buffer, 0xAA, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_UNSUBSCRIBE;
    status = MQTTPropAdd_PayloadFormat( &PropertyBuilder, requestRespInfo, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 3 ] );
    TEST_ASSERT_EQUAL_UINT8( 0xAA, PropertyBuilder.pBuffer[ 4 ] );
}

void test_MQTTPropAdd_AuthMethod_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ];
    MQTTStatus_t status;

    status = MQTTPropAdd_AuthMethod( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = MQTT_REMAINING_LENGTH_INVALID;
    PropertyBuilder.bufferLength = MQTT_REMAINING_LENGTH_INVALID + 50;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_AUTH_METHOD_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_AUTH;
    status = MQTTPropAdd_AuthMethod( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_AUTH_METHOD_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}

void test_MQTTPropAdd_AuthData_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ];
    MQTTStatus_t status;

    status = MQTTPropAdd_AuthData( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_DATA_POS );
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_AUTH_DATA_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_AUTHENTICATION_METHOD_POS );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_AUTH;
    status = MQTTPropAdd_AuthData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_AUTH_DATA_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}

void test_MQTTPropAdd_ReasonString_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ];
    MQTTStatus_t status;

    status = MQTTPropAdd_ReasonString( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_REASON_STRING_POS );
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBACK;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBACK;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_UNSUBACK;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_DISCONNECT;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REASON_STRING_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_AUTH;
    status = MQTTPropAdd_ReasonString( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_REASON_STRING_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}

void test_MQTTPropAdd_ContentType_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ];
    MQTTStatus_t status;

    status = MQTTPropAdd_ContentType( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_CONTENT_TYPE_POS );
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_CONTENT_TYPE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_ContentType( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_CONTENT_TYPE_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}

void test_MQTTPropAdd_CorrelationData_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ];
    MQTTStatus_t status;

    status = MQTTPropAdd_CorrelationData( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_CORRELATION_DATA_POS );
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_CORRELATION_DATA_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_CorrelationData( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_CORRELATION_DATA_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}

void test_MQTTPropAdd_ResponseTopic_AllCases( void )
{
    MQTTPropBuilder_t PropertyBuilder;
    uint8_t OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    uint8_t buffer[ 100 ] = { 0 };
    MQTTStatus_t status;

    PropertyBuilder.pBuffer = buffer;
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, NULL, 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 0, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    memset( buffer, 0, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "H+llo", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    memset( buffer, 0, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "H#llo", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    status = MQTTPropAdd_ResponseTopic( NULL, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = NULL;
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet |= ( 1 << MQTT_RESPONSE_TOPIC_POS );
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_SUBSCRIBE;
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTBadParameter, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 1;
    PropertyBuilder.fieldSet = 0;
    memset( buffer, 0, 100 );
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, NULL );
    TEST_ASSERT_EQUAL( MQTTNoMemory, status );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    memset( buffer, 0, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_CONNECT;
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_RESPONSE_TOPIC_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );

    PropertyBuilder.pBuffer = buffer;
    PropertyBuilder.currentIndex = 0;
    PropertyBuilder.bufferLength = 100;
    PropertyBuilder.fieldSet = 0;
    memset( buffer, 0, 100 );
    OptionalMqttPacketType = MQTT_PACKET_TYPE_PUBLISH;
    status = MQTTPropAdd_ResponseTopic( &PropertyBuilder, "Hello", 5, &OptionalMqttPacketType );
    TEST_ASSERT_EQUAL( MQTTSuccess, status );
    TEST_ASSERT_EQUAL( MQTT_RESPONSE_TOPIC_ID, PropertyBuilder.pBuffer[ 0 ] );
    TEST_ASSERT_EQUAL_UINT8( 0, PropertyBuilder.pBuffer[ 1 ] );
    TEST_ASSERT_EQUAL_UINT8( 5, PropertyBuilder.pBuffer[ 2 ] );
    TEST_ASSERT_EQUAL( 0, memcmp( &PropertyBuilder.pBuffer[ 3 ], "Hello", 5 ) );
}
