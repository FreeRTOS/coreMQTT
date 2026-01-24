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
 * @file core_mqtt_prop_deserializer.c
 * @brief Implements the user-facing functions to de-serialize user-properties.
 */
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>

#include "core_mqtt_serializer.h"

#include "private/core_mqtt_serializer_private.h"

/* Include config defaults header to get default values of configs. */
#include "core_mqtt_config_defaults.h"

/**
 * @brief Checks the parameters to all the internal/external 'get' property
 * functions.
 *
 * @param[in] mqttPropBuilder The property builder struct.
 * @param[in] currentIndex The index of the property in the buffer.
 *
 * @return MQTTSuccess if all the checks pass;
 */
static inline MQTTStatus_t checkPropBuilderParams( MQTTPropBuilder_t * mqttPropBuilder,
                                                   uint32_t * currentIndex );

/**
 * @brief Get a uint8 property value from the property builder.
 *
 * This function extracts a uint8 property value from the property builder
 * at the specified index and validates that it matches the expected property ID.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder containing the properties.
 * @param[in,out] currentIndex Pointer to the current index in the property buffer.
 *                             Updated to point past the property on success.
 * @param[in] propertyId Expected property ID to validate against.
 * @param[out] property Pointer to store the extracted uint8 property value.
 *
 * @return #MQTTSuccess if the property is successfully extracted;
 * #MQTTBadParameter if parameters are invalid or property ID doesn't match.
 */
static MQTTStatus_t getPropUint8( MQTTPropBuilder_t * pPropertyBuilder,
                                  uint32_t * currentIndex,
                                  uint8_t propertyId,
                                  uint8_t * property );

/**
 * @brief Get a uint16 property value from the property builder.
 *
 * This function extracts a uint16 property value from the property builder
 * at the specified index and validates that it matches the expected property ID.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder containing the properties.
 * @param[in,out] currentIndex Pointer to the current index in the property buffer.
 *                             Updated to point past the property on success.
 * @param[in] propertyId Expected property ID to validate against.
 * @param[out] property Pointer to store the extracted uint16 property value.
 *
 * @return #MQTTSuccess if the property is successfully extracted;
 * #MQTTBadParameter if parameters are invalid or property ID doesn't match.
 */
static MQTTStatus_t getPropUint16( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   uint8_t propertyId,
                                   uint16_t * property );

/**
 * @brief Get a uint32 property value from the property builder.
 *
 * This function extracts a uint32 property value from the property builder
 * at the specified index and validates that it matches the expected property ID.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder containing the properties.
 * @param[in,out] currentIndex Pointer to the current index in the property buffer.
 *                             Updated to point past the property on success.
 * @param[in] propertyId Expected property ID to validate against.
 * @param[out] property Pointer to store the extracted uint32 property value.
 *
 * @return #MQTTSuccess if the property is successfully extracted;
 * #MQTTBadParameter if parameters are invalid or property ID doesn't match.
 */
static MQTTStatus_t getPropUint32( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   uint8_t propertyId,
                                   uint32_t * property );

/**
 * @brief Get a UTF-8 string property value from the property builder.
 *
 * This function extracts a UTF-8 string property value from the property builder
 * at the specified index and validates that it matches the expected property ID.
 *
 * @param[in] pPropertyBuilder Pointer to the property builder containing the properties.
 * @param[in,out] currentIndex Pointer to the current index in the property buffer.
 *                             Updated to point past the property on success.
 * @param[in] propertyId Expected property ID to validate against.
 * @param[out] property Pointer to store the extracted UTF-8 string pointer.
 * @param[out] propertyLength Pointer to store the length of the extracted string.
 *
 * @return #MQTTSuccess if the property is successfully extracted;
 * #MQTTBadParameter if parameters are invalid or property ID doesn't match.
 */
static MQTTStatus_t getPropUtf8( MQTTPropBuilder_t * pPropertyBuilder,
                                 uint32_t * currentIndex,
                                 uint8_t propertyId,
                                 const char ** property,
                                 uint16_t * propertyLength );

/*-----------------------------------------------------------*/

static inline MQTTStatus_t checkPropBuilderParams( MQTTPropBuilder_t * mqttPropBuilder,
                                                   uint32_t * currentIndex )
{
    MQTTStatus_t status = MQTTSuccess;

    if( ( mqttPropBuilder == NULL ) || ( mqttPropBuilder->pBuffer == NULL ) ||
        ( currentIndex == NULL ) )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else if( *currentIndex >= mqttPropBuilder->currentIndex )
    {
        LogWarn( ( "Index out of bounds." ) );
        status = MQTTEndOfProperties;
    }
    else
    {
        /* Nothing to do. All good. */
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t getPropUint8( MQTTPropBuilder_t * pPropertyBuilder,
                                  uint32_t * currentIndex,
                                  uint8_t propertyId,
                                  uint8_t * property )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( property == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t localPropertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == localPropertyId )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            uint32_t remainingPropLength = ( uint32_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );
            bool tempBool = false;

            pLocalIndex++;

            status = decodeUint8t( property,
                                   &remainingPropLength,
                                   &tempBool,
                                   &pLocalIndex );

            if( status == MQTTSuccess )
            {
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not %" PRIu8, propertyId ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t getPropUint16( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   uint8_t propertyId,
                                   uint16_t * property )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( property == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t localPropertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == localPropertyId )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            /* Remaining properties = Total properties - current location in the buffer - 1 ( for property ID ). */
            uint32_t remainingPropLength = ( uint32_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );
            bool tempBool = false;

            /* Move index to the beginning of the key. */
            pLocalIndex++;

            status = decodeUint16t( property,
                                    &remainingPropLength,
                                    &tempBool,
                                    &pLocalIndex );

            if( status == MQTTSuccess )
            {
                /* Move the index to the end of the property. */
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not %" PRIu8, propertyId ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t getPropUint32( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   uint8_t propertyId,
                                   uint32_t * property )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( property == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t localPropertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == localPropertyId )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            /* Remaining properties = Total properties - current location in the buffer - 1 ( for property ID ). */
            uint32_t remainingPropLength = ( uint32_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );
            bool tempBool = false;

            /* Move index to the beginning of the key. */
            pLocalIndex++;

            status = decodeUint32t( property,
                                    &remainingPropLength,
                                    &tempBool,
                                    &pLocalIndex );

            if( status == MQTTSuccess )
            {
                /* Move the index to the end of the property. */
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not %" PRIu8 ".", propertyId ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

static MQTTStatus_t getPropUtf8( MQTTPropBuilder_t * pPropertyBuilder,
                                 uint32_t * currentIndex,
                                 uint8_t propertyId,
                                 const char ** property,
                                 uint16_t * propertyLength )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( ( property == NULL ) || ( propertyLength == NULL ) )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t localPropertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == localPropertyId )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            uint32_t remainingPropLength = ( uint32_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );
            bool tempBool = false;

            pLocalIndex++;

            status = decodeUtf8( property,
                                 propertyLength,
                                 &remainingPropLength,
                                 &tempBool,
                                 &pLocalIndex );

            if( status == MQTTSuccess )
            {
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not %" PRIu8 ".", propertyId ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_GetNextPropertyType( MQTTPropBuilder_t * pPropertyBuilder,
                                       uint32_t * currentIndex,
                                       uint8_t * property )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        /* Do nothing. checkPropBuilderParams will log the warning/error. */
    }
    else if( property == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        *property = pPropertyBuilder->pBuffer[ *currentIndex ];

        switch( *property )
        {
            case MQTT_SESSION_EXPIRY_ID:
            case MQTT_RECEIVE_MAX_ID:
            case MQTT_MAX_PACKET_SIZE_ID:
            case MQTT_TOPIC_ALIAS_MAX_ID:
            case MQTT_REQUEST_RESPONSE_ID:
            case MQTT_REQUEST_PROBLEM_ID:
            case MQTT_USER_PROPERTY_ID:
            case MQTT_AUTH_METHOD_ID:
            case MQTT_AUTH_DATA_ID:
            case MQTT_WILL_DELAY_ID:
            case MQTT_PAYLOAD_FORMAT_ID:
            case MQTT_MSG_EXPIRY_ID:
            case MQTT_CONTENT_TYPE_ID:
            case MQTT_RESPONSE_TOPIC_ID:
            case MQTT_CORRELATION_DATA_ID:
            case MQTT_TOPIC_ALIAS_ID:
            case MQTT_MAX_QOS_ID:
            case MQTT_RETAIN_AVAILABLE_ID:
            case MQTT_ASSIGNED_CLIENT_ID:
            case MQTT_REASON_STRING_ID:
            case MQTT_WILDCARD_ID:
            case MQTT_SUB_AVAILABLE_ID:
            case MQTT_SHARED_SUB_ID:
            case MQTT_SERVER_KEEP_ALIVE_ID:
            case MQTT_RESPONSE_INFO_ID:
            case MQTT_SERVER_REF_ID:
            case MQTT_SUBSCRIPTION_ID_ID:
                break;

            default:
                LogError( ( "Unknown property ID: %d", *property ) );
                status = MQTTBadParameter;
                break;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTT_SkipNextProperty( MQTTPropBuilder_t * pPropertyBuilder,
                                    uint32_t * currentIndex )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );
    uint8_t property;
    uint8_t * pLocalIndex;
    uint32_t remainingPropLength;
    bool tempBool = false;
    uint32_t dummyUint32;
    uint16_t dummyUint16;
    uint8_t dummyUint8;
    const char * dummyString;
    uint16_t dummyStringLen;

    if( status != MQTTSuccess )
    {
        /* Do nothing. checkPropBuilderParams will log the warning/error. */
    }
    else
    {
        property = pPropertyBuilder->pBuffer[ *currentIndex ];
        pLocalIndex = &pPropertyBuilder->pBuffer[ *currentIndex ];
        remainingPropLength = pPropertyBuilder->currentIndex - *currentIndex - 1U;

        /* Skip the property ID byte */
        pLocalIndex++;

        switch( property )
        {
            /* Four-byte integer properties */
            case MQTT_SESSION_EXPIRY_ID:
            case MQTT_MAX_PACKET_SIZE_ID:
            case MQTT_WILL_DELAY_ID:
            case MQTT_MSG_EXPIRY_ID:
                status = decodeUint32t( &dummyUint32, &remainingPropLength, &tempBool, &pLocalIndex );
                break;

            /* Two-byte integer properties */
            case MQTT_RECEIVE_MAX_ID:
            case MQTT_TOPIC_ALIAS_MAX_ID:
            case MQTT_TOPIC_ALIAS_ID:
            case MQTT_SERVER_KEEP_ALIVE_ID:
                status = decodeUint16t( &dummyUint16, &remainingPropLength, &tempBool, &pLocalIndex );
                break;

            /* One-byte integer properties */
            case MQTT_REQUEST_RESPONSE_ID:
            case MQTT_REQUEST_PROBLEM_ID:
            case MQTT_PAYLOAD_FORMAT_ID:
            case MQTT_MAX_QOS_ID:
            case MQTT_RETAIN_AVAILABLE_ID:
            case MQTT_WILDCARD_ID:
            case MQTT_SUB_AVAILABLE_ID:
            case MQTT_SHARED_SUB_ID:
                status = decodeUint8t( &dummyUint8, &remainingPropLength, &tempBool, &pLocalIndex );
                break;

            /* UTF-8 string properties */
            case MQTT_AUTH_METHOD_ID:
            case MQTT_CONTENT_TYPE_ID:
            case MQTT_RESPONSE_TOPIC_ID:
            case MQTT_ASSIGNED_CLIENT_ID:
            case MQTT_REASON_STRING_ID:
            case MQTT_RESPONSE_INFO_ID:
            case MQTT_SERVER_REF_ID:
            case MQTT_AUTH_DATA_ID:
            case MQTT_CORRELATION_DATA_ID:
                status = decodeUtf8( &dummyString, &dummyStringLen, &remainingPropLength, &tempBool, &pLocalIndex );
                break;

            /* User property (two UTF-8 strings: key and value) */
            case MQTT_USER_PROPERTY_ID:
                status = decodeUserProp( &dummyString, &dummyStringLen,
                                         &dummyString, &dummyStringLen,
                                         &remainingPropLength, &pLocalIndex );
                break;

            /* Variable byte integer property (subscription identifier) */
            case MQTT_SUBSCRIPTION_ID_ID:
               {
                   uint32_t subscriptionId;
                   size_t varIntSize;

                   status = decodeVariableLength( pLocalIndex, remainingPropLength, &subscriptionId );

                   if( status == MQTTSuccess )
                   {
                       varIntSize = variableLengthEncodedSize( subscriptionId );
                       pLocalIndex = &pLocalIndex[ varIntSize ];
                   }
               }
               break;

            default:
                LogError( ( "Unknown property ID: %d", property ) );
                status = MQTTBadParameter;
                break;
        }

        if( status == MQTTSuccess )
        {
            /* Update the current index to point past the skipped property */
            *currentIndex = ( uint32_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_UserProp( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   MQTTUserProperty_t * pUserProperty )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( pUserProperty == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t propertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == MQTT_USER_PROPERTY_ID )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            /* Remaining properties = Total properties - current location in the buffer - 1 ( for property ID ). */
            uint32_t remainingPropLength = ( uint32_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );

            /* Move index to the beginning of the key. */
            pLocalIndex++;

            status = decodeUserProp( &pUserProperty->pKey,
                                     &pUserProperty->keyLength,
                                     &pUserProperty->pValue,
                                     &pUserProperty->valueLength,
                                     &remainingPropLength,
                                     &pLocalIndex );

            if( status == MQTTSuccess )
            {
                /* Move the index to the end of the property. */
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not a User Property." ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_SessionExpiry( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t * currentIndex,
                                        uint32_t * pSessionExpiry )
{
    return getPropUint32( pPropertyBuilder, currentIndex, MQTT_SESSION_EXPIRY_ID, pSessionExpiry );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ReceiveMax( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint32_t * currentIndex,
                                     uint16_t * pReceiveMax )
{
    return getPropUint16( pPropertyBuilder, currentIndex, MQTT_RECEIVE_MAX_ID, pReceiveMax );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_MaxQos( MQTTPropBuilder_t * pPropertyBuilder,
                                 uint32_t * currentIndex,
                                 uint8_t * pMaxQos )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_MAX_QOS_ID, pMaxQos );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_RetainAvailable( MQTTPropBuilder_t * pPropertyBuilder,
                                          uint32_t * currentIndex,
                                          uint8_t * pRetainAvailable )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_RETAIN_AVAILABLE_ID, pRetainAvailable );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_MaxPacketSize( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t * currentIndex,
                                        uint32_t * pMaxPacketSize )
{
    return getPropUint32( pPropertyBuilder, currentIndex, MQTT_MAX_PACKET_SIZE_ID, pMaxPacketSize );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_AssignedClientId( MQTTPropBuilder_t * pPropertyBuilder,
                                           uint32_t * currentIndex,
                                           const char ** pClientId,
                                           uint16_t * pClientIdLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_ASSIGNED_CLIENT_ID, pClientId, pClientIdLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_TopicAliasMax( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t * currentIndex,
                                        uint16_t * pTopicAliasMax )
{
    return getPropUint16( pPropertyBuilder, currentIndex, MQTT_TOPIC_ALIAS_MAX_ID, pTopicAliasMax );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ReasonString( MQTTPropBuilder_t * pPropertyBuilder,
                                       uint32_t * currentIndex,
                                       const char ** pReasonString,
                                       uint16_t * pReasonStringLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_REASON_STRING_ID, pReasonString, pReasonStringLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_WildcardId( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint32_t * currentIndex,
                                     uint8_t * pWildcardAvailable )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_WILDCARD_ID, pWildcardAvailable );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_SubsIdAvailable( MQTTPropBuilder_t * pPropertyBuilder,
                                          uint32_t * currentIndex,
                                          uint8_t * pSubsIdAvailable )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_SUB_AVAILABLE_ID, pSubsIdAvailable );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_SharedSubAvailable( MQTTPropBuilder_t * pPropertyBuilder,
                                             uint32_t * currentIndex,
                                             uint8_t * pSharedSubAvailable )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_SHARED_SUB_ID, pSharedSubAvailable );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ServerKeepAlive( MQTTPropBuilder_t * pPropertyBuilder,
                                          uint32_t * currentIndex,
                                          uint16_t * pServerKeepAlive )
{
    return getPropUint16( pPropertyBuilder, currentIndex, MQTT_SERVER_KEEP_ALIVE_ID, pServerKeepAlive );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ResponseInfo( MQTTPropBuilder_t * pPropertyBuilder,
                                       uint32_t * currentIndex,
                                       const char ** pResponseInfo,
                                       uint16_t * pResponseInfoLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_RESPONSE_INFO_ID, pResponseInfo, pResponseInfoLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ServerRef( MQTTPropBuilder_t * pPropertyBuilder,
                                    uint32_t * currentIndex,
                                    const char ** pServerRef,
                                    uint16_t * pServerRefLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_SERVER_REF_ID, pServerRef, pServerRefLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_AuthMethod( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint32_t * currentIndex,
                                     const char ** pAuthMethod,
                                     uint16_t * pAuthMethodLen )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_AUTH_METHOD_ID, pAuthMethod, pAuthMethodLen );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_AuthData( MQTTPropBuilder_t * pPropertyBuilder,
                                   uint32_t * currentIndex,
                                   const char ** pAuthData,
                                   uint16_t * pAuthDataLen )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_AUTH_DATA_ID, pAuthData, pAuthDataLen );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_PayloadFormatIndicator( MQTTPropBuilder_t * pPropertyBuilder,
                                                 uint32_t * currentIndex,
                                                 uint8_t * pPayloadFormat )
{
    return getPropUint8( pPropertyBuilder, currentIndex, MQTT_PAYLOAD_FORMAT_ID, pPayloadFormat );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_MessageExpiryInterval( MQTTPropBuilder_t * pPropertyBuilder,
                                                uint32_t * currentIndex,
                                                uint32_t * pMessageExpiry )
{
    return getPropUint32( pPropertyBuilder, currentIndex, MQTT_MSG_EXPIRY_ID, pMessageExpiry );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_TopicAlias( MQTTPropBuilder_t * pPropertyBuilder,
                                     uint32_t * currentIndex,
                                     uint16_t * pTopicAlias )
{
    return getPropUint16( pPropertyBuilder, currentIndex, MQTT_TOPIC_ALIAS_ID, pTopicAlias );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ResponseTopic( MQTTPropBuilder_t * pPropertyBuilder,
                                        uint32_t * currentIndex,
                                        const char ** pResponseTopic,
                                        uint16_t * pResponseTopicLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_RESPONSE_TOPIC_ID, pResponseTopic, pResponseTopicLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_CorrelationData( MQTTPropBuilder_t * pPropertyBuilder,
                                          uint32_t * currentIndex,
                                          const char ** pCorrelationData,
                                          uint16_t * pCorrelationDataLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_CORRELATION_DATA_ID, pCorrelationData, pCorrelationDataLength );
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_SubscriptionId( MQTTPropBuilder_t * pPropertyBuilder,
                                         uint32_t * currentIndex,
                                         uint32_t * pSubscriptionId )
{
    MQTTStatus_t status = checkPropBuilderParams( pPropertyBuilder, currentIndex );

    if( status != MQTTSuccess )
    {
        LogError( ( "Property type is invalid." ) );
    }
    else if( pSubscriptionId == NULL )
    {
        LogError( ( "Argument cannot be NULL." ) );
        status = MQTTBadParameter;
    }
    else
    {
        uint8_t propertyId = pPropertyBuilder->pBuffer[ *currentIndex ];

        if( propertyId == MQTT_SUBSCRIPTION_ID_ID )
        {
            uint8_t * pLocalIndex = ( uint8_t * ) &pPropertyBuilder->pBuffer[ *currentIndex ];
            size_t remainingPropLength = ( size_t ) ( pPropertyBuilder->currentIndex - *currentIndex - 1U );

            pLocalIndex++;

            status = decodeVariableLength( pLocalIndex, remainingPropLength, pSubscriptionId );

            if( status == MQTTSuccess )
            {
                size_t encodedSize = variableLengthEncodedSize( *pSubscriptionId );
                pLocalIndex = &pLocalIndex[ encodedSize ];
                *currentIndex = ( size_t ) ( pLocalIndex - pPropertyBuilder->pBuffer );
            }
        }
        else
        {
            LogError( ( "Property is not a Subscription Identifier Property." ) );
            status = MQTTBadParameter;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

MQTTStatus_t MQTTPropGet_ContentType( MQTTPropBuilder_t * pPropertyBuilder,
                                      uint32_t * currentIndex,
                                      const char ** pContentType,
                                      uint16_t * pContentTypeLength )
{
    return getPropUtf8( pPropertyBuilder, currentIndex, MQTT_CONTENT_TYPE_ID, pContentType, pContentTypeLength );
}

/*-----------------------------------------------------------*/
