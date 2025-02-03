/**
 * Position of each flag in the first byte of the subscribe payload, after topic filter
 */
#define MQTT_SUBSCRIBE_FLAG_QOS1                    ( 0 ) /**< @brief MQTT SUBSCRIBE QoS1 flag. */
#define MQTT_SUBSCRIBE_FLAG_QOS2                    ( 1 ) /**< @brief MQTT SUBSCRIBE QoS2 flag. */
#define MQTT_SUBSCRIBE_FLAG_NO_LOCAL                ( 2 ) /**< @brief MQTT SUBSCRIBE no local flag. */
#define MQTT_SUBSCRIBE_FLAG_RETAIN_AS_PUBLISHED     ( 3 ) /**< @brief MQTT SUBSCRIBE retain as published flag. */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING1             ( 4 ) /**<@brief MQTT SUBSCRIBE Retain Handling Option 1 */
#define MQTT_SUBSCRIBE_RETAIN_HANDLING2             ( 5 ) /**<@brief Retain Handling Option 2   -> in core_mqtt_serializer.c */
// bit 6 and 7 are reserved
typedef enum retainHandling{
    retainSendOnSub = 0,
    retainSendOnSubIfNotPresent = 1,
    retainDoNotSendonSub = 2 
}retainHandling_t; 

// add to mqtt contex -> subscribe properties
typedef struct MQTTSubscribeInfo
{
    /**
     * @brief Quality of Service for subscription. Include protocol error of qos > 2 
     */
    MQTTQoS_t qos;

    /**
     * @brief Topic filter to subscribe to.
     */
    const char * pTopicFilter;

    /**
     * @brief Length of subscription topic filter.
     */
    uint16_t ultopicFilterLength;
    /**
     * @brief no local option for subscription. Include protocol error if noLocalOption = 1 in a shared subscription
     */
    #if (MQTT_VERSION_5_ENABLED)
        bool noLocalOption;
        bool retainAsPublishedOption;
        retainHandling_t retainHandlingOption; 
        /**
         * Length of the property field - variable byte int
         */
        //size_t propertyLength ;
        /**
         * User properties - list of key value pairs
         */
        //MQTTUserProperties_t * pUserProperties ;
        /**
         * Subscription ID - variable byte int, max number of bytes in variable byte int => 4 , 32 bits
         */
        //uint32_t subscriptionId ; 
} MQTTSubscribeInfo_t;

// struct for subscribe properties -> have it in mqtt context ? 

typedef struct MQTTSubscribeProperties
{
    /**
     * Length of the property field - variable byte int
     */
    size_t propertyLength ;
    /**
     * User properties - list of key value pairs
     */
    MQTTUserProperties_t * pUserProperties ;
    /**
     * Subscription ID - variable byte int, max number of bytes in variable byte int => 4 , 32 bits
     */
    uint32_t subscriptionId ; 

} MQTTSubscribeProperties_t; 


static MQTTStatus_t validateSubscribeUnsubscribeParams( const MQTTContext_t * pContext,
                                                        const MQTTSubscribeInfo_t * pSubscriptionList,
                                                        const MQTTSubscribeProperties_t *pSubscribeProperties
                                                        size_t subscriptionCount,
                                                        uint16_t packetId )
{
    MQTTStatus_t status = MQTTSuccess;
    size_t iterator;

    /* Validate all the parameters. */
    if( ( pContext == NULL ) || ( pSubscriptionList == NULL ) )
    {
        LogError( ( "Argument cannot be NULL: pContext=%p, "
                    "pSubscriptionList=%p.",
                    ( void * ) pContext,
                    ( void * ) pSubscriptionList ) );
        status = MQTTBadParameter;
    }
    else if( subscriptionCount == 0UL )
    {
        LogError( ( "Subscription count is 0." ) );
        status = MQTTBadParameter;
    }
    else if( packetId == 0U )
    {
        LogError( ( "Packet Id for subscription packet is 0." ) );
        status = MQTTBadParameter;
    }
    else
    {
        if( pContext->incomingPublishRecords == NULL )
        {
            for( iterator = 0; iterator < subscriptionCount; iterator++ )
            {
                if( pSubscriptionList[ iterator ].qos > MQTTQoS0 )
                {
                    LogError( ( "The incoming publish record list is not "
                                "initialised for QoS1/QoS2 records. Please call "
                                " MQTT_InitStatefulQoS to enable use of QoS1 and "
                                " QoS2 packets." ) );
                    status = MQTTBadParameter;
                    break;
                }
                if((pSubscriptionList[ iterator ].ultopicFilterLength == 0U) || (pSubscriptionList[iterator].pTopicFilter == NULL)){
                    LogError("Argument cannot be null : pTopicFilter") ;
                    status = MQTTBadParameter;
                    break ;
                }
                if(pSubscriptionList[iterator].qos > MQTTQoS2){
                    LogError("Protocol Error : QoS cannot be greater than 2");
                    status = MQTTBadParameter;
                    break ;
                }
                #if (MQTT_VERSION_5_ENABLED)
                    bool isSharedSub = strncmp(pSubscriptionList[iterator].pTopicFilter, "$share/", 7)==0;
                    if(isSharedSub){
                        /**
                         * ensuring ShareName is present and does not contain invalid characters
                         */
                        const char *shareNameEnd = strche(pSubscriptionList[iterator].pTopicFilter+7, '/');
                        if((shareNameEnd == NULL) || (shareNameEnd == pSubscriptionList[iterator].pTopicFilter + 7)){
                            LogError("Protocol Error : ShareName is not present , missing or empty");
                            status = MQTTBadParameter;
                            break ;
                        }
                        if(strpbrk(pSubscriptionList[iterator].pTopicFilter + 7, "+#/") < shareNameEnd){
                            LogError("Protocol Error : ShareName contains invalid characters");
                            status = MQTTBadParameter;
                            break ;
                        }
                        if(pSubscriptionList[iterator].noLocalOption == 1){
                            LogError("Protocol Error : noLocalOption cannot be 1 for shared subscriptions");
                            status = MQTTBadParameter;
                            break;
                        }
                    }
                    if(pSubscriptionList[iterator].retainHandlingOption > 2) // Application message must not be forwarded to)
                    {
                        LogError("Protocol Error : retainHandlingOption cannot be greater than 2");
                        status = MQTTBadParameter;
                        break;
                    }
                #endif
            }
        }
    }
    if( status == MQTTSuccess )
    {
        #if (MQTT_VERSION_5_ENABLED)
            if(pSubscribeProperties==NULL){
                LogError("Argument cannot be null : pSubscriberProperties") ; 
                status = MQTTBadParameter; 
                break ; 
            }
            if(pSubscribeProperties->subscriptionId == 0){
                    LogError("Subscription Id cannot 0 for subscribe properties : Protocol Error ") ;
                    status = MQTTBadParameter;
            }

        #endif

    }
    return status;
}

// in core_mqtt_serializer.c 
/*
uint8_t * MQTT_SerializeSubscribeProperties(const MQTTSubscribeProperties_t *pSubscribeProperties , unint_8 *propertyBuffer){
    uint8_t * pIndex = propertyBuffer; 

    pIndex = encodeRemainingLength(pIndex, pSubscribeProperties->propertyLength ) ; 
    if(pSubscribeProperties->pUserProperties != NULL){
        *pIndex = MQTT_USER_PROPERTY_ID;
        pIndex ++ ; 
        /**
         * Write down function to encode user properties, 
         * key-value pairs in a utf - 8 encoded format
         */
/*        uint8_t encodedKeyLength[CORE_MQTT_SERIALIZED_LENGTH_FIELD_BYTES] ; 



    }

    if(pSubscribeProperties->subscriptionId != NULL){
        *pIndex = MQTT_SUBSCRIPTION_ID_ID;
        pIndex ++ ;
        pIndex = encodeRemainingLength(pIndex, pProperties->subscriptionId) ; 
    }
    return pIndex ; 

} */


static MQTTStatus_t sendSubscribeWithoutCopyV5( MQTTContext_t * pContext,
                                              const MQTTSubscribeInfo_t * pSubscriptionList,
                                              const MQTTSubscribeProperties_t *pSubscribeProperties,
                                              size_t subscriptionCount,
                                              uint16_t packetId,
                                              size_t remainingLength ){
    MQTTStatus_t status = MQTTSuccess;
    uint8_t * pIndex;
    /**
     * Fixed Size Properties 
     */
    TransportOutVector_t pIoVector[ MQTT_SUB_UNSUB_MAX_VECTORS ];
    TransportOutVector_t * pIterator;
    uint8_t serializedTopicFieldLength[ MQTT_SUB_UNSUB_MAX_VECTORS ][ CORE_MQTT_SERIALIZED_LENGTH_FIELD_BYTES ];
    size_t totalPacketLength = 0U;
    size_t ioVectorLength = 0U;
    size_t subscriptionsSent = 0U;
    size_t vectorsAdded;
    size_t topicFieldLengthIndex;
    /**
     * Maximum number of bytes by the fixed header of a SUBSCRIBE packet.
     * MQTT Control Byte 0 + 1 = 1 
     * Remaining Length    + 4 = 5 
     * Packet Id           + 2 = 7 
     */
    uint8_t subscribeHeader[7] ; 
    pIndex = MQTT_SerializeSubscribeHeader(remainingLength , subscribeHeader , packetId) ; 
    pIterator = pIoVector ; 
    pIterator->iov_base = subscribeHeader ;
    pIterator->iov_len = (size_t)(pIndex - subscribeHeader) ; 
    totalPacketLength += pIterator->iov_len ;
    pIterator++ ; 
    ioVectorLength++ ; 

    /*Encode subscription properties*/
    // SART
    uint8_t propertyBuffer[MQTT_MAX_PACKET_PROPERTY_SIZE] ;
    pIndex = propertyBuffer ; 
    pIndex = encodeRemainingLength(pIndex, pSubscribeProperties->propertyLength) ;
    if(pSubscribeProperties->pUserProperties != NULL){
        *pIndex = MQTT_USER_PROPERTY_ID;
        pIndex ++ ;
        for(int i = 0; i < pSubscribeProperties->pUserProperties->count; i++){
            pIndex = encodeString(pIndex, pSubscribeProperties->pUserProperties->pUserProperties[i].pKey, strlen(pSubscribeProperties->pUserProperties->pUserProperties[i].pKey), encodedKeyLength);
            pIndex = encodeString(pIndex, pSubscribeProperties->pUserProperties->pUserProperties[i].pValue, strlen(pSubscribeProperties->pUserProperties->pUserProperties[i].pValue), encodedKeyLength);
        }
    }
    if(pSubscribeProperties->subscriptionId != NULL){
        *pIndex = MQTT_SUBSCRIPTION_ID_ID;
        pIndex ++ ;
        pIndex = encodeRemainingLength(pIndex, pProperties->subscriptionId) ;
    }

    pIterator->iov_base = propertyBuffer ; 
    pIterator->iov_len = (size_t)(pIndex - propertyBuffer) ;

    totalPacketLength += pIterator->iov_len ;
    pIterator ++ ; 
    ioVectorLength++ ; 

// end
    /**
     * Fixed Header 0 + 1 = 1 
     * User Property  + 5 * MAX_USER_PROPERTY 
     * 
     */
    uint8_t propertyBuffer[MQTT_MAX_PACKET_PROPERTY_SIZE] ; 
    uint8_t encodedProperties = MQTT_SerializeSubscribeProperties(pSubscribeProperties, propertyBuffer);
    pIterator->iov_base = propertyBuffer ;
    pIterator->iov_len = (size_t) (encodedProperties - propertyBuffer) ;
    totalPacketLength += pIterator->iov_len ;
    pIterator ++ ; 
    ioVectorLength ++ ; 

    while( ( status == MQTTSuccess ) && ( subscriptionsSent < subscriptionCount ) )
    {
        /* Reset the index for next iteration. */
        topicFieldLengthIndex = 0;

        /* Check whether the subscription topic (with QoS) will fit in the
         * given vector. */
        while( ( ioVectorLength <= ( MQTT_SUB_UNSUB_MAX_VECTORS - CORE_MQTT_SUBSCRIBE_PER_TOPIC_VECTOR_LENGTH ) ) &&
               ( subscriptionsSent < subscriptionCount ) )
        {
            /* The topic filter and the filter length gets sent next. (filter length - 2 bytes , topic filter - utf - 8 ) */
            vectorsAdded = addEncodedStringToVector( serializedTopicFieldLength[ topicFieldLengthIndex ],
                                                     pSubscriptionList[ subscriptionsSent ].pTopicFilter,
                                                     pSubscriptionList[ subscriptionsSent ].topicFilterLength,
                                                     pIterator,
                                                     &totalPacketLength );

            /* Update the pointer after the above operation. */
            pIterator = &pIterator[ vectorsAdded ];
            ioVectorLength += vectorsAdded;

            /* Lastly, send the susbcription Options */
            //pIterator->iov_base = &( pSubscriptionList[ subscriptionsSent ].qos ); -> send it as bits no? why even go for subscriptionOptions

            unint_8 subscriptionOptions = 0U; 
            if(pSubscribeInfo->qos == MQTTQoS1){
                LogDebug("Adding QoS as QoS 1 in SUBSCRIBE payload") ; 
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_QOS1) ; 
            }else if(pSubscribeInfo->qos == MQTTQoS2){
                LogDebug("Adding QoS as QoS 2 in SUBSCRIBE payload") ;
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_QOS2) ;
            }else{
                LogDebug("Adding QoS as QoS 0 in SUBSCRIBE payload") ;
            }
            if(pSubscribeInfo->noLocalOption == 1){
                LogDebug("Adding noLocalOption in SUBSCRIBE payload") ;
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_NO_LOCAL) ;
            }else LogDebug("Adding noLocalOption as 0 in SUBSCRIBE payload") ; 

            if(pSubscribeInfo->retainAsPublishedOption == 1){
                LogDebug("Adding retainAsPublishedOption in SUBSCRIBE payload") ;
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_AS_PUBLISHED) ;
            }else LogDebug("Adding retainAsPublishedOption as 0 in SUBSCRIBE payload") ;

            if(pSubscribeInfo->retainHandling == 0){
                LogDebug("Send Retain messages at the time of subscribe") ; 
            }else if(pSubscribeInfo->retainHandling == 1){
                LogDebug("Send retained messages at subscribe only if the subscription does not currently exist") ;
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_HANDLING1) ;

            }else if(pSubscribeInfo->retainHandling == 2){
                LogDebug("Do not send retained messages at subscribe") ;
                UINT8_SET_BIT(subscriptionOptions, MQTT_SUBSCRIBE_RETAIN_HANDLING2) ;
            }else{
                LogDebug("Protocol Error, retain handling is 3") ; 
                status = MQTTBadParameter ;
                break ; 
            }
            pIterator->iov_base = &( subscriptionOptions );
            pIterator->iov_len = 1U ;
            totalPacketLength += 1U ; 
            pIterator ++ ; 
            ioVectorLength ++ ;
            subcriptionsSent ++ ;
        }
    }
    int32_t byteSent = sendMessageVector(pContext, pIoVector, ioVectorLength, ioVectorLength) ; 
    if(byteSent != (int32_t)totalPacketLength){
        LogError("Error in sending SUBSCRIBE packet") ; 
        status = MQTTTransportSendFailed ;
    }else{
        status = MQTTSuccess ;
    }
   
    return status ;                                        
}

MQTTStatus_t MQTT_Subscribe( MQTTContext_t * pContext,
                             const MQTTSubscribeInfo_t * pSubscriptionList,
                             size_t subscriptionCount,
                             uint16_t packetId )
{

    MQTTConnectionStatus_t connectStatus;
    size_t remainingLength = 0UL, packetSize = 0UL;

    MQTTStatus_t status = validateSubscribeUnsubscribeParams( pContext,
                                                              pSubscriptionList,
                                                              subscriptionCount,
                                                              packetId );
    
    if( status == MQTTSuccess )
    {
        /* Get the remaining length and packet size.*/
        status = MQTT_GetSubscribePacketSize( pSubscriptionList,
                                              subscriptionCount,
                                              &remainingLength,
                                              &packetSize );
        LogDebug( ( "SUBSCRIBE packet size is %lu and remaining length is %lu.",
                    ( unsigned long ) packetSize,
                    ( unsigned long ) remainingLength ) );
    }   
    if(status == MQTTStatus){
        MQTT_PRE_STATE_HOOK(pContext); 
        connectStatus = pContext->connectStatus ;
        if( connectStatus != MQTTConnected )
        {
            status = ( connectStatus == MQTTNotConnected ) ? MQTTStatusNotConnected : MQTTStatusDisconnectPending;
        }

        if( status == MQTTSuccess )
        {
            /* Send MQTT SUBSCRIBE packet. */
            status = sendSubscribeWithoutCopy( pContext,
                                               pSubscriptionList,
                                               subscriptionCount,
                                               packetId,
                                               remainingLength );
        }

        MQTT_POST_STATE_UPDATE_HOOK( pContext );
    }
    return status;
}
