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
 * @file core_mqtt_prop_serializer.c
 * @brief Implements the user-facing functions to serialize user-properties.
 */
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <inttypes.h>

#include "core_mqtt_serializer.h"

#include "private/core_mqtt_serializer_private.h"

/* Include config defaults header to get default values of configs. */
#include "core_mqtt_config_defaults.h"

/**
 * @brief Check whether the provided property is allowed for a packet type.
 *
 * @param[in] mqttPacketType Packet type to check.
 * @param[in] propBitLocation Bit location of the property.
 *
 * @return Whether the property is allowed for the packet type.
 */
static bool isValidPropertyInPacketType( const uint8_t * mqttPacketType,
                                         uint8_t propBitLocation );

/**
 * @brief Add a uint8 property to the property builder.
 *
 * This function adds a uint8 property to the property builder with validation
 * for packet type compatibility and buffer space.
 *
 * @param[in,out] pPropertyBuilder Pointer to the property builder to add the property to.
 * @param[in] property The uint8 property value to add.
 * @param[in] propId The property ID for this property.
 * @param[in] fieldPosition The bit position in the fieldSet for duplicate checking.
 * @param[in] pOptionalMqttPacketType Optional MQTT packet type for validation.
 *                                    Can be NULL to skip packet type validation.
 *
 * @return #MQTTSuccess if the property is successfully added;
 * #MQTTBadParameter if parameters are invalid;
 * #MQTTNoMemory if insufficient buffer space.
 */
static MQTTStatus_t addPropUint8( MQTTPropBuilder_t * pPropertyBuilder,
                                  uint8_t property,
                                  uint8_t propId,
                                  uint8_t fieldPosition,
                                  const uint8_t * pOptionalMqttPacketType );

/**
 * @brief Add a uint16 property to the property builder.
 *
 * This function adds a uint16 property to the property builder with validation
 * for packet type compatibility and buffer space.
 *
 * @param[in,out] pPropertyBuilder Pointer to the property builder to add the property to.
 * @param[in] property The uint16 property value to add.
 * @param[in] propId The property ID for this property.
 * @param[in] fieldPosition The bit position in the fieldSet for duplicate checking.
 * @param[in] pOptionalMqttPacketType Optional MQTT packet type for validation.
 *                                    Can be NULL to skip packet type validation.
 *
 * @return #MQTTSuccess if the property is successfully added;
 * #MQTTBadParameter if parameters are invalid;
 * #MQTTNoMemory if insufficient buffer space.
 */
static MQTTStatus_t addPropUint16( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint16_t property,
                                   uint8_t propId,
                                   uint8_t fieldPosition,
                                   const uint8_t * pOptionalMqttPacketType );

/**
 * @brief Add a uint32 property to the property builder.
 *
 * This function adds a uint32 property to the property builder with validation
 * for packet type compatibility and buffer space.
 *
 * @param[in,out] pPropertyBuilder Pointer to the property builder to add the property to.
 * @param[in] property The uint32 property value to add.
 * @param[in] propId The property ID for this property.
 * @param[in] fieldPosition The bit position in the fieldSet for duplicate checking.
 * @param[in] pOptionalMqttPacketType Optional MQTT packet type for validation.
 *                                    Can be NULL to skip packet type validation.
 *
 * @return #MQTTSuccess if the property is successfully added;
 * #MQTTBadParameter if parameters are invalid;
 * #MQTTNoMemory if insufficient buffer space.
 */
static MQTTStatus_t addPropUint32( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t property,
                                   uint8_t propId,
                                   uint8_t fieldPosition,
                                   const uint8_t * pOptionalMqttPacketType );

/**
 * @brief Add a UTF-8 string property to the property builder.
 *
 * This function adds a UTF-8 string property to the property builder with validation
 * for packet type compatibility and buffer space.
 *
 * @param[in,out] pPropertyBuilder Pointer to the property builder to add the property to.
 * @param[in] property The UTF-8 string property value to add.
 * @param[in] propertyLength The length of the UTF-8 string property.
 * @param[in] propId The property ID for this property.
 * @param[in] fieldPosition The bit position in the fieldSet for duplicate checking.
 * @param[in] pOptionalMqttPacketType Optional MQTT packet type for validation.
 *                                    Can be NULL to skip packet type validation.
 *
 * @return #MQTTSuccess if the property is successfully added;
 * #MQTTBadParameter if parameters are invalid;
 * #MQTTNoMemory if insufficient buffer space.
 */
static MQTTStatus_t addPropUtf8( MQTTPropBuilder_t * pPropertyBuilder,
                                 const char * property,
                                 size_t propertyLength,
                                 uint8_t propId,
                                 uint8_t fieldPosition,
                                 const uint8_t * pOptionalMqttPacketType );

/*-----------------------------------------------------------*/

static bool isValidPropertyInPacketType( const uint8_t * mqttPacketType,
                                         uint8_t propBitLocation )
{
    bool isAllowed = false;
    uint32_t allowedPropertiesMask = 0U;

    switch( *mqttPacketType )
    {
        case MQTT_PACKET_TYPE_CONNECT:

            /* CONNECT properties:
             * - Session Expiry Interval
             * - Receive Maximum
             * - Maximum Packet Size
             * - Topic Alias Maximum
             * - Request Response Information
             * - Request Problem Information
             * - User Property
             * - Authentication Method
             * - Authentication Data
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SESSION_EXPIRY_INTERVAL_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RECEIVE_MAXIMUM_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_MAX_PACKET_SIZE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_TOPIC_ALIAS_MAX_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REQUEST_RESPONSE_INFO_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REQUEST_PROBLEM_INFO_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_METHOD_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_DATA_POS );

            /* Will properties. */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_WILL_DELAY_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_PAYLOAD_FORMAT_INDICATOR_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_MESSAGE_EXPIRY_INTERVAL_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_CONTENT_TYPE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RESPONSE_TOPIC_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_CORRELATION_DATA_POS );

            break;

        case MQTT_PACKET_TYPE_CONNACK:

            /* CONNACK properties:
             * - Session Expiry Interval
             * - Receive Maximum
             * - Maximum QoS
             * - Retain Available
             * - Maximum Packet Size
             * - Assigned Client Identifier
             * - Topic Alias Maximum
             * - Reason String
             * - User Property
             * - Wildcard Subscription Available
             * - Subscription Identifier Available
             * - Shared Subscription Available
             * - Server Keep Alive
             * - Response Information
             * - Server Reference
             * - Authentication Method
             * - Authentication Data
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SESSION_EXPIRY_INTERVAL_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RECEIVE_MAXIMUM_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_MAX_QOS_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RETAIN_AVAILABLE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_MAX_PACKET_SIZE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_ASSIGNED_CLIENT_ID_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_TOPIC_ALIAS_MAX_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_WILDCARD_SUBSCRIPTION_AVAILABLE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SUBSCRIPTION_ID_AVAILABLE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SHARED_SUBSCRIPTION_AVAILABLE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SERVER_KEEP_ALIVE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RESPONSE_INFORMATION_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SERVER_REFERENCE_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_METHOD_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_DATA_POS );
            break;

        case MQTT_PACKET_TYPE_PUBLISH:

            /* PUBLISH properties:
             * - Payload Format Indicator
             * - Message Expiry Interval
             * - Topic Alias
             * - Response Topic
             * - Correlation Data
             * - User Property
             * - Subscription Identifier (only in server-to-client PUBLISH)
             * - Content Type
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_PAYLOAD_FORMAT_INDICATOR_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_MESSAGE_EXPIRY_INTERVAL_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_TOPIC_ALIAS_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_RESPONSE_TOPIC_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_CORRELATION_DATA_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SUBSCRIPTION_ID_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_CONTENT_TYPE_POS );
            break;

        case MQTT_PACKET_TYPE_PUBACK:
        case MQTT_PACKET_TYPE_PUBREC:
        case MQTT_PACKET_TYPE_PUBREL:
        case MQTT_PACKET_TYPE_PUBCOMP:

            /* PUBACK, PUBREC, PUBREL, PUBCOMP properties:
             * - Reason String
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            break;

        case MQTT_PACKET_TYPE_SUBSCRIBE:

            /* SUBSCRIBE properties:
             * - Subscription Identifier
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SUBSCRIPTION_ID_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            break;

        case MQTT_PACKET_TYPE_SUBACK:

            /* SUBACK properties:
             * - Reason String
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            break;

        case MQTT_PACKET_TYPE_UNSUBSCRIBE:

            /* UNSUBSCRIBE properties:
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            break;

        case MQTT_PACKET_TYPE_UNSUBACK:

            /* UNSUBACK properties:
             * - Reason String
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            break;

        case MQTT_PACKET_TYPE_DISCONNECT:

            /* DISCONNECT properties:
             * - Session Expiry Interval
             * - Reason String
             * - User Property
             * - Server Reference
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SESSION_EXPIRY_INTERVAL_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_SERVER_REFERENCE_POS );
            break;

        case MQTT_PACKET_TYPE_PINGREQ:
        case MQTT_PACKET_TYPE_PINGRESP:

            /* PINGREQ and PINGRESP have no properties section.
             * allowedPropertiesMask remains 0. */
            break;

        case MQTT_PACKET_TYPE_AUTH:

            /* AUTH properties:
             * - Authentication Method
             * - Authentication Data
             * - Reason String
             * - User Property
             */
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_METHOD_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_AUTHENTICATION_DATA_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_REASON_STRING_POS );
            UINT32_SET_BIT( allowedPropertiesMask, MQTT_USER_PROP_POS );

        default:
            /* Unknown packet type - no properties allowed. */
            break;
    }

    /* Check if the property bit is set in the allowed mask. */
    isAllowed = UINT32_CHECK_BIT( allowedPropertiesMask, propBitLocation );

    return isAllowed;
}


/*-----------------------------------------------------------*/

static MQTTStatus_t addPropUint8( MQTTPropBuilder_t * pPropertyBuilder,
                                  uint8_t property,
                                  uint8_t propId,
                                  uint8_t fieldPosition,
                                  const uint8_t * pOptionalMqttPacketType )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "pPropertyBuilder cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "pPropertyBuilder->pBuffer cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, fieldPosition ) )
    {
        LogError( ( "%" PRIu8 " already set.", propId ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, fieldPosition ) == false ) )
    {
        LogError( ( "%" PRIu8 " not allowed in %d packet type.", propId, *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }
    /* We need 1 byte to encode the property and 1 byte to encode the property ID. */
    else if( ( pPropertyBuilder->bufferLength < 2U ) ||
             ( pPropertyBuilder->currentIndex > ( pPropertyBuilder->bufferLength - 2U ) ) )
    {
        LogError( ( "Buffer too small to add property" ) );
        status = MQTTNoMemory;
    }
    else if( pPropertyBuilder->currentIndex > ( MQTT_REMAINING_LENGTH_INVALID - 2U ) )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = propId;
        pIndex++;
        *pIndex = ( property ? 1U : 0U );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, fieldPosition );
        pPropertyBuilder->currentIndex += 2U;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t addPropUint16( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint16_t property,
                                   uint8_t propId,
                                   uint8_t fieldPosition,
                                   const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Argument pPropertyBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Argument pPropertyBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, fieldPosition ) )
    {
        LogError( ( "%" PRIu8 " already set.", propId ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, fieldPosition ) == false ) )
    {
        LogError( ( "Receive Maximum not allowed in %d packet type.", *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }

    /* We need 1 byte to encode property ID and 2 bytes to encode the property
     * itself. */
    else if( ( pPropertyBuilder->bufferLength < 3U ) ||
             ( pPropertyBuilder->currentIndex > ( pPropertyBuilder->bufferLength - 3U ) ) )
    {
        LogError( ( "Buffer too small to add property." ) );
        status = MQTTNoMemory;
    }
    else if( pPropertyBuilder->currentIndex > ( MQTT_REMAINING_LENGTH_INVALID - 3U ) )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t * pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = propId;
        pIndex++;
        pIndex[ 0 ] = UINT16_HIGH_BYTE( property );
        pIndex[ 1 ] = UINT16_LOW_BYTE( property );

        UINT32_SET_BIT( pPropertyBuilder->fieldSet, fieldPosition );
        pPropertyBuilder->currentIndex += 3U;
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t addPropUint32( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t property,
                                   uint8_t propId,
                                   uint8_t fieldPosition,
                                   const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Argument pPropertyBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Argument pPropertyBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, fieldPosition ) )
    {
        LogError( ( "Subscription Id already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, fieldPosition ) == false ) )
    {
        LogError( ( "Subscription Id not allowed in %d packet type.", *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }

    /* We need 1 byte to encode property ID and 4 bytes to encode the property
     * itself. */
    else if( ( pPropertyBuilder->bufferLength < 5U ) ||
             ( pPropertyBuilder->currentIndex > ( pPropertyBuilder->bufferLength - 5U ) ) )
    {
        LogError( ( "Buffer too small to add subscription id." ) );
        status = MQTTNoMemory;
    }
    else if( pPropertyBuilder->currentIndex > ( MQTT_REMAINING_LENGTH_INVALID - 5U ) )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = propId;
        pIndex++;
        WRITE_UINT32( &( pIndex[ 0 ] ), property );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += 5U;
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, fieldPosition );
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t addPropUtf8( MQTTPropBuilder_t * pPropertyBuilder,
                                 const char * property,
                                 size_t propertyLength,
                                 uint8_t propId,
                                 uint8_t fieldPosition,
                                 const uint8_t * pOptionalMqttPacketType )
{
    uint8_t * pIndex;
    MQTTStatus_t status = MQTTSuccess;

    assert( !CHECK_SIZE_T_OVERFLOWS_16BIT( propertyLength ) );

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Argument pPropertyBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Argument pPropertyBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( property == NULL )
    {
        LogError( ( "property cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( propertyLength == 0U )
    {
        LogError( ( "Property Length cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, fieldPosition ) )
    {
        LogError( ( "Reason String already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, fieldPosition ) == false ) )
    {
        LogError( ( "Reason String not allowed in %d packet type.", *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }

    /* We need to make sure that adding 2 to property length will not overflow. Then, we need to make
     * sure that buffer is greater than the size needed to fit the property ( 2U + property length ).
     * Then, we need to make sure that buffer is big enough to handle the existing data and this property. */
    else if( ADDITION_WILL_OVERFLOW_SIZE_T( propertyLength, 2U ) ||
             ( pPropertyBuilder->bufferLength < ( propertyLength + 2U ) ) ||
             ( pPropertyBuilder->currentIndex > ( pPropertyBuilder->bufferLength - ( propertyLength + 2U ) ) ) )
    {
        LogError( ( "Buffer too small to add property." ) );
        status = MQTTNoMemory;
    }
    else if( ( MQTT_REMAINING_LENGTH_INVALID < ( propertyLength + 2U ) ) ||
             ( pPropertyBuilder->currentIndex > ( MQTT_REMAINING_LENGTH_INVALID - ( propertyLength + 2U ) ) ) )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = propId;
        pIndex++;
        pIndex = encodeString( pIndex, property, propertyLength );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, fieldPosition );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - ( &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] ) );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_SubscriptionId( MQTTPropBuilder_t * pPropertyBuilder,
                                         uint32_t subscriptionId,
                                         const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;

    if( subscriptionId == 0U )
    {
        LogError( ( "Subscription Id cannot be 0 for subscribe properties: Protocol Error." ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionId > MAX_VARIABLE_LENGTH_INT_VALUE )
    {
        LogError( ( "Subscription Id cannot be greater than 268435455: Protocol Error." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder == NULL )
    {
        LogError( ( "Argument pPropertyBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Argument pPropertyBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_SUBSCRIPTION_ID_POS ) )
    {
        LogError( ( "Subscription Id already set." ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, MQTT_SUBSCRIPTION_ID_POS ) == false ) )
    {
        LogError( ( "Subscription Id not allowed in %d packet type.", *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }
    /* variableLengthEncodedSize always returns a number in the range of [1,4]. The addition will not overflow. */
    else if( ( pPropertyBuilder->bufferLength < ( variableLengthEncodedSize( subscriptionId ) + 1U ) ) ||
             ( pPropertyBuilder->currentIndex > pPropertyBuilder->bufferLength - ( variableLengthEncodedSize( subscriptionId ) + 1U ) ) )
    {
        LogError( ( "Buffer too small to add subscription id." ) );
        status = MQTTNoMemory;
    }
    else if( pPropertyBuilder->currentIndex > ( MQTT_REMAINING_LENGTH_INVALID - ( variableLengthEncodedSize( subscriptionId ) + 1U ) ) )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        *pIndex = MQTT_SUBSCRIPTION_ID_ID;
        pIndex++;
        pIndex = encodeVariableLength( pIndex, subscriptionId );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ] );
        UINT32_SET_BIT( pPropertyBuilder->fieldSet, MQTT_SUBSCRIPTION_ID_POS );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_UserProp( MQTTPropBuilder_t * pPropertyBuilder,
                                   const MQTTUserProperty_t * userProperty,
                                   const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "Argument pPropertyBuilder cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "Argument pPropertyBuilder->pBuffer cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( userProperty == NULL )
    {
        LogError( ( "Argument userProperty cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( ( userProperty->pKey == NULL ) ||
             ( userProperty->pValue == NULL ) ||
             ( userProperty->keyLength == 0U ) ||
             ( userProperty->valueLength == 0U ) )
    {
        LogError( ( "Arguments cannot be NULL: pUserProperties->userProperty->pKey=%p, "
                    "pUserProperties->userProperty->pValue=%p, "
                    "Key Length = %d, Value Length = %d",
                    ( void * ) userProperty->pKey,
                    ( void * ) userProperty->pValue,
                    ( int ) userProperty->keyLength,
                    ( int ) userProperty->valueLength ) );
        status = MQTTBadParameter;
    }
    else if( ( pOptionalMqttPacketType != NULL ) &&
             ( isValidPropertyInPacketType( pOptionalMqttPacketType, MQTT_USER_PROP_POS ) == false ) )
    {
        LogError( ( "User property not allowed in %d packet type.", *pOptionalMqttPacketType ) );
        status = MQTTBadParameter;
    }
    /* 1 byte Property ID + 2 byte key length + 2 byte value length. */
    else if( ADDITION_WILL_OVERFLOW_SIZE_T( pPropertyBuilder->currentIndex, 5U ) )
    {
        LogError( ( "Property builder buffer will overflow." ) );
        status = MQTTBadParameter;
    }
    /* 1 byte Property ID + 2 byte key length + 2 byte value length + Actual key */
    else if( ADDITION_WILL_OVERFLOW_SIZE_T( pPropertyBuilder->currentIndex + 5U, userProperty->keyLength ) )
    {
        LogError( ( "Property builder buffer will overflow." ) );
        status = MQTTBadParameter;
    }
    /* 1 byte Property ID + 2 byte key length + 2 byte value length + Actual key + Actual value. */
    else if( ADDITION_WILL_OVERFLOW_SIZE_T( pPropertyBuilder->currentIndex + 5U + userProperty->keyLength,
                                            userProperty->valueLength ) )
    {
        LogError( ( "Property builder buffer will overflow." ) );
        status = MQTTBadParameter;
    }

    /*
     * User property is encoded as a key-value pair of UTF-8 strings.
     * 2-bytes for string length of key + key length +
     * 2-bytes for string length of value + value length +
     * 1 byte for property identifier.
     */
    else if( ( pPropertyBuilder->currentIndex +
               sizeof( uint16_t ) + userProperty->keyLength +
               sizeof( uint16_t ) + userProperty->valueLength +
               sizeof( uint8_t ) ) > pPropertyBuilder->bufferLength )
    {
        LogError( ( "Buffer too small to add property." ) );
        status = MQTTNoMemory;
    }
    else if( ( pPropertyBuilder->currentIndex +
               sizeof( uint16_t ) + userProperty->keyLength +
               sizeof( uint16_t ) + userProperty->valueLength +
               sizeof( uint8_t ) ) > MQTT_REMAINING_LENGTH_INVALID )
    {
        LogError( ( "MQTT packets must be smaller than %" PRIu32 ".",
                    ( uint32_t ) MQTT_REMAINING_LENGTH_INVALID ) );
        status = MQTTBadParameter;
    }
    else
    {
        const uint8_t * start = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];
        uint8_t * pIndex = &pPropertyBuilder->pBuffer[ pPropertyBuilder->currentIndex ];

        *pIndex = MQTT_USER_PROPERTY_ID;
        pIndex++;

        /* Encoding key. */
        pIndex = encodeString( pIndex, userProperty->pKey, userProperty->keyLength );
        pIndex = encodeString( pIndex, userProperty->pValue, userProperty->valueLength );
        /* More details at: https://github.com/FreeRTOS/coreMQTT/blob/main/MISRA.md#rule-108 */
        /* coverity[misra_c_2012_rule_10_8_violation] */
        pPropertyBuilder->currentIndex += ( size_t ) ( pIndex - start );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_SessionExpiry( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t sessionExpiry,
                                        const uint8_t * pOptionalMqttPacketType )
{
    /* Any value of session expiry is valid. */
    return addPropUint32( pPropertyBuilder,
                          sessionExpiry,
                          MQTT_SESSION_EXPIRY_ID,
                          MQTT_SESSION_EXPIRY_INTERVAL_POS,
                          pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ReceiveMax( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint16_t receiveMax,
                                     const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( receiveMax == 0U )
    {
        LogError( ( "Receive max cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = addPropUint16( pPropertyBuilder,
                                receiveMax,
                                MQTT_RECEIVE_MAX_ID,
                                MQTT_RECEIVE_MAXIMUM_POS,
                                pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_MaxPacketSize( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t maxPacketSize,
                                        const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;

    if( maxPacketSize == 0U )
    {
        LogError( ( "Max packet size cannot be set to 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = addPropUint32( pPropertyBuilder,
                                maxPacketSize,
                                MQTT_MAX_PACKET_SIZE_ID,
                                MQTT_MAX_PACKET_SIZE_POS,
                                pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_TopicAliasMax( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint16_t topicAliasMax,
                                        const uint8_t * pOptionalMqttPacketType )
{
    /* All values of topic alias maximum are valid. */
    return addPropUint16( pPropertyBuilder,
                          topicAliasMax,
                          MQTT_TOPIC_ALIAS_MAX_ID,
                          MQTT_TOPIC_ALIAS_MAX_POS,
                          pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_RequestRespInfo( MQTTPropBuilder_t * pPropertyBuilder,
                                          bool requestResponseInfo,
                                          const uint8_t * pOptionalMqttPacketType )
{
    return addPropUint8( pPropertyBuilder,
                         ( uint8_t ) requestResponseInfo,
                         MQTT_REQUEST_RESPONSE_ID,
                         MQTT_REQUEST_RESPONSE_INFO_POS,
                         pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_RequestProbInfo( MQTTPropBuilder_t * pPropertyBuilder,
                                          bool requestProblemInfo,
                                          const uint8_t * pOptionalMqttPacketType )
{
    return addPropUint8( pPropertyBuilder,
                         ( uint8_t ) requestProblemInfo,
                         MQTT_REQUEST_PROBLEM_ID,
                         MQTT_REQUEST_PROBLEM_INFO_POS,
                         pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_AuthMethod( MQTTPropBuilder_t * pPropertyBuilder,
                                     const char * authMethod,
                                     size_t authMethodLength,
                                     const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( CHECK_SIZE_T_OVERFLOWS_16BIT( authMethodLength ) )
    {
        LogError( ( "Auth method length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Auth method has no restrictions and hence no additional checks required. */
        status = addPropUtf8( pPropertyBuilder,
                              authMethod,
                              authMethodLength,
                              MQTT_AUTH_METHOD_ID,
                              MQTT_AUTHENTICATION_METHOD_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_AuthData( MQTTPropBuilder_t * pPropertyBuilder,
                                   const char * authData,
                                   size_t authDataLength,
                                   const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;

    if( pPropertyBuilder == NULL )
    {
        LogError( ( "pPropertyBuilder cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( pPropertyBuilder->pBuffer == NULL )
    {
        LogError( ( "pPropertyBuilder->pBuffer cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( authData == NULL )
    {
        LogError( ( "authData cannot be NULL" ) );
        status = MQTTBadParameter;
    }
    else if( UINT32_CHECK_BIT( pPropertyBuilder->fieldSet, MQTT_AUTHENTICATION_METHOD_POS ) == false )
    {
        LogError( ( "Auth method must be added before authentication data. "
                    "Not a protocol violation but a practice enforced by coreMQTT." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( authDataLength ) )
    {
        LogError( ( "Auth data length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = addPropUtf8( pPropertyBuilder,
                              authData,
                              authDataLength,
                              MQTT_AUTH_DATA_ID,
                              MQTT_AUTHENTICATION_DATA_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_PayloadFormat( MQTTPropBuilder_t * pPropertyBuilder,
                                        bool payloadFormat,
                                        const uint8_t * pOptionalMqttPacketType )
{
    return addPropUint8( pPropertyBuilder,
                         ( uint8_t ) payloadFormat,
                         MQTT_PAYLOAD_FORMAT_ID,
                         MQTT_PAYLOAD_FORMAT_INDICATOR_POS,
                         pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_MessageExpiry( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t messageExpiry,
                                        const uint8_t * pOptionalMqttPacketType )
{
    return addPropUint32( pPropertyBuilder,
                          messageExpiry,
                          MQTT_MSG_EXPIRY_ID,
                          MQTT_MESSAGE_EXPIRY_INTERVAL_POS,
                          pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_WillDelayInterval( MQTTPropBuilder_t * pPropertyBuilder,
                                            uint32_t willDelayInterval,
                                            const uint8_t * pOptionalMqttPacketType )
{
    return addPropUint32( pPropertyBuilder,
                          willDelayInterval,
                          MQTT_WILL_DELAY_ID,
                          MQTT_WILL_DELAY_POS,
                          pOptionalMqttPacketType );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_TopicAlias( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint16_t topicAlias,
                                     const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status = MQTTSuccess;

    if( topicAlias == 0U )
    {
        LogError( ( "Topic Alias cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = addPropUint16( pPropertyBuilder,
                                topicAlias,
                                MQTT_TOPIC_ALIAS_ID,
                                MQTT_TOPIC_ALIAS_POS,
                                pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ResponseTopic( MQTTPropBuilder_t * pPropertyBuilder,
                                        const char * responseTopic,
                                        size_t responseTopicLength,
                                        const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( responseTopic == NULL )
    {
        LogError( ( "Arguments cannot be NULL : responseTopic=%p.", ( void * ) responseTopic ) );
        status = MQTTBadParameter;
    }
    else if( responseTopicLength == 0U )
    {
        LogError( ( "Response Topic Length cannot be 0." ) );
        status = MQTTBadParameter;
    }
    else if( ( memchr( ( void * ) responseTopic, ( int ) '#', responseTopicLength ) != NULL ) ||
             ( memchr( ( void * ) responseTopic, ( int ) '+', responseTopicLength ) != NULL ) )
    {
        LogError( ( "Protocol Error : Response Topic contains wildcards (such as # or +)." ) );
        status = MQTTBadParameter;
    }
    else if( CHECK_SIZE_T_OVERFLOWS_16BIT( responseTopicLength ) )
    {
        LogError( ( "Response topic length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        status = addPropUtf8( pPropertyBuilder,
                              responseTopic,
                              responseTopicLength,
                              MQTT_RESPONSE_TOPIC_ID,
                              MQTT_RESPONSE_TOPIC_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_CorrelationData( MQTTPropBuilder_t * pPropertyBuilder,
                                          const void * pCorrelationData,
                                          size_t correlationLength,
                                          const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( CHECK_SIZE_T_OVERFLOWS_16BIT( correlationLength ) )
    {
        LogError( ( "Correlation data length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* Encoding binary and UTF-8 strings behaves in the same way. */
        status = addPropUtf8( pPropertyBuilder,
                              ( const char * ) pCorrelationData,
                              correlationLength,
                              MQTT_CORRELATION_DATA_ID,
                              MQTT_CORRELATION_DATA_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ContentType( MQTTPropBuilder_t * pPropertyBuilder,
                                      const char * contentType,
                                      size_t contentTypeLength,
                                      const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( CHECK_SIZE_T_OVERFLOWS_16BIT( contentTypeLength ) )
    {
        LogError( ( "Content type string length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* No restriction and hence no additional checks on the content type. */
        status = addPropUtf8( pPropertyBuilder,
                              contentType,
                              contentTypeLength,
                              MQTT_CONTENT_TYPE_ID,
                              MQTT_CONTENT_TYPE_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropAdd_ReasonString( MQTTPropBuilder_t * pPropertyBuilder,
                                       const char * pReasonString,
                                       size_t reasonStringLength,
                                       const uint8_t * pOptionalMqttPacketType )
{
    MQTTStatus_t status;

    if( CHECK_SIZE_T_OVERFLOWS_16BIT( reasonStringLength ) )
    {
        LogError( ( "Reason string length must be less than 65536 according to MQTT spec." ) );
        status = MQTTBadParameter;
    }
    else
    {
        /* No restriction and hence no additional checks on the reason string. */
        status = addPropUtf8( pPropertyBuilder,
                              pReasonString,
                              reasonStringLength,
                              MQTT_REASON_STRING_ID,
                              MQTT_REASON_STRING_POS,
                              pOptionalMqttPacketType );
    }

    return status;
}
