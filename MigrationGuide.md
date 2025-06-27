# coreMQTT Migration Guide

## Table of Contents
* [coreMQTT version v2.x Migration Guide](#coremqtt-version-v2x-migration-guide)
    * [Breaking Changes](#breaking-changes)
        * [Updated `MQTT_ProcessLoop` API](#updated-mqtt_processloop-api)
        * [Updated `MQTT_ReceiveLoop` API](#updated-mqtt_receiveloop-api)
        * [Updated `TransportInterface_t` struct](#updated-transportinterface_t-struct)
        * [`MQTT_InitStatefulQos` Required for QoS>0](#mqtt_initstatefulqos-required-for-qos0)
        * [Replacing `MQTT_SEND_RETRY_TIMEOUT_MS`](#replacing-mqtt_send_retry_timeout_ms)
    * [Additional Changes](#additional-changes)
        * [New API `MQTT_CancelCallback`](#new-api-mqtt_cancelcallback)
* [coreMQTT version >=v3.0.0 Migration Guide](#coremqtt-version-v300-migration-guide)
    * [Breaking Changes](#breaking-changes-1)
        * [Updated `MQTTEventCallback_t` Callback Function](#updated-mqtteventcallback_t-callback-function)
        * [Updated `MQTT_InitStatefulQoS` API](#updated-mqtt_initstatefulqos-api)
        * [Updated `MQTT_Connect` API](#updated-mqtt_connect-api)
        * [Updated `MQTT_Subscribe` API](#updated-mqtt_subscribe-api)
        * [Updated `MQTT_Publish` API](#updated-mqtt_publish-api)
        * [Updated `MQTT_Unsubscribe` API](#updated-mqtt_unsubscribe-api)
        * [Updated `MQTT_Disconnect` API](#updated-mqtt_disconnect-api)
        * [Updated `MQTT_GetConnectPacketSize` API](#updated-mqtt_getconnectpacketsize-api)
        * [Updated `MQTT_GetPublishPacketSize` API](#updated-mqtt_getpublishpacketsize-api)
        * [Updated `MQTT_GetSubscribePacketSize` API](#updated-mqtt_getsubscribepacketsize-api)
        * [Updated `MQTT_GetUnsubscribePacketSize` API](#updated-mqtt_getunsubscribepacketsize-api)
        * [Updated `MQTT_GetDisconnectPacketSize` API](#updated-mqtt_getdisconnectpacketsize-api)
        * [Updated `MQTT_SerializeConnect` API](#updated-mqtt_serializeconnect-api)
        * [Updated `MQTT_SerializePublish` API](#updated-mqtt_serializepublish-api)
        * [Updated `MQTT_SerializePublishHeader` API](#updated-mqtt_serializepublishheader-api)
        * [Updated `MQTT_SerializeSubscribe` API](#updated-mqtt_serializesubscribe-api)
        * [Updated `MQTT_SerializeUnsubscribe` API](#updated-mqtt_serializeunsubscribe-api)
        * [Updated `MQTT_SerializeDisconnect` API](#updated-mqtt_serializedisconnect-api)
        * [Updated `MQTT_DeserializePublish` API](#updated-mqtt_deserializepublish-api)
        * [Updated `MQTT_DeserializeAck` API](#updated-mqtt_deserializeack-api)




## coreMQTT version v2.x Migration Guide

With coreMQTT versions >=v2.x, there are some breaking changes that need to be addressed when upgrading.

### Breaking Changes

#### Updated `MQTT_ProcessLoop` API

The `MQTT_ProcessLoop` function no longer uses a timeout as this led to unavoidable busy-waiting. Thus, the signature of `MQTT_ProcessLoop` changed

from
```c
MQTTStatus_t MQTT_ProcessLoop( MQTTContext_t * pContext, uint32_t timeoutMs )
```
to
```c
MQTTStatus_t MQTT_ProcessLoop( MQTTContext_t * pContext )
```
Additionally, `MQTT_ProcessLoop` can now return `MQTTNeedMoreBytes`. A return of `MQTTNeedMoreBytes` means that `MQTT_ProcessLoop` received only a part of an MQTT packet and will need to be called again (probably after a bit of delay) in order to finish receiving the MQTT packet. Thus, to migrate, simply remove the timeout from the `MQTT_ProcessLoop` call and additionally check for if `MQTTNeedMoreBytes` has been returned when checking the status of `MQTT_ProcessLoop`. For example:

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
uint32_t timeoutMs = 100;
// This context is assumed to be initialized and connected.
MQTTContext_t * pContext;

while( true )
{
    status = MQTT_ProcessLoop( pContext, timeoutMs );

    if( status != MQTTSuccess )
    {
        // Determine the error. It's possible we might need to disconnect
        // the underlying transport connection.
    }
    else
    {
        // Other application functions.
    }
}
```
**New Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
// This context is assumed to be initialized and connected.
MQTTContext_t * pContext;

while( true )
{
    status = MQTT_ProcessLoop( pContext );

    if( status != MQTTSuccess && status != MQTTNeedMoreBytes )
    {
        // Determine the error. It's possible we might need to disconnect
        // the underlying transport connection.
    }
    /* else if only required if the terminating else is empty and the application doesn't want a scenario akin to busy waiting. */
    else if( status == MQTTNeedMoreBytes )
    {
        /* Only a partial MQTT packet is received. Call MQTT_ProcessLoop again; ideally after a small delay. */
    }
    else
    {
        // Other application functions.
    }
}
```
#### Updated `MQTT_ReceiveLoop` API

The `MQTT_ReceiveLoop` function no longer uses a timeout as this led to unavoidable busy-waiting. Thus, the signature of `MQTT_ReceiveLoop` changed

from
```c
MQTTStatus_t MQTT_ReceiveLoop( MQTTContext_t * pContext, uint32_t timeoutMs )
```
to
```c
MQTTStatus_t MQTT_ReceiveLoop( MQTTContext_t * pContext )
```

Additionally, `MQTT_ReceiveLoop` can now return `MQTTNeedMoreBytes`. A return of `MQTTNeedMoreBytes` means that `MQTT_ReceiveLoop` received only a part of an MQTT packet and will need to be called again (probably after a bit of delay) in order to finish receiving the MQTT packet. Thus, to migrate, simply remove the timeout from the `MQTT_ReceiveLoop` call and additionally check for if `MQTTNeedMoreBytes` has been returned when checking the status of `MQTT_ReceiveLoop`. For example:

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
uint32_t timeoutMs = 100;
// This context is assumed to be initialized and connected.
MQTTContext_t * pContext;

while( true )
{
    status = MQTT_ReceiveLoop( pContext, timeoutMs );

    if( status != MQTTSuccess )
    {
        // Determine the error. It's possible we might need to disconnect
        // the underlying transport connection.
    }
    else
    {
        // Other application functions.
    }
}
```
**New Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
// This context is assumed to be initialized and connected.
MQTTContext_t * pContext;

while( true )
{
    status = MQTT_ReceiveLoop( pContext );

    if( status != MQTTSuccess && status != MQTTNeedMoreBytes )
    {
        // Determine the error. It's possible we might need to disconnect
        // the underlying transport connection.
    }
    /* else if only required if the terminating else is empty and the application doesn't want a scenario akin to busy waiting. */
    else if( status == MQTTNeedMoreBytes )
    {
        /* Only a partial MQTT packet is received. Call MQTT_ReceiveLoop again; ideally after a small delay. */
    }
    else
    {
        // Other application functions.
    }
}
```
#### Updated `TransportInterface_t` struct

The `TransportInterface_t` structure now has a new member `writev`. It uses scatter-gather approach to send multiple MQTT packet components as a single packet to reduce overhead and improve performance. However, it is COMPLETELY OPTIONAL to implement. To that end, when application(s) initialize a `TransportInterface_t` structure, they **MUST** either set `writev` to a working implementation or set it `NULL`. Not doing this will lead to undefined behavior as the coreMQTT library checks if `writev` is `NULL` to determine if it should be used. For example:

**Old Code Snippet**:
```c
TransportInterface_t transport;
// Set transport interface members.
transport.pNetworkInterface = &someNetworkInterface;
transport.send = networkSend;
transport.recv = networkRecv;
```
**New Code Snippet**:
```c
TransportInterface_t transport;
// Set transport interface members.
transport.pNetworkInterface = &someNetworkInterface;
transport.send = networkSend;
transport.recv = networkRecv;
transport.writev = NULL;
```
#### `MQTT_InitStatefulQos` Required for QoS>0

The `MQTT_Init` function no longer creates buffers to handle QoS > 0 packets, so if planning to use QoS > 0, the `MQTT_InitStatefulQoS` function must also be called on an `MQTTContext_t` after calling `MQTT_Init` on it and before using any other coreMQTT functions with it. If not using QoS > 0, `MQTT_InitStatefulQoS` does not need to be called. For example (code that uses QoS > 0):

**Old Code Snippet**:
```c
// Function for obtaining a timestamp.
uint32_t getTimeStampMs();
// Callback function for receiving packets.
void eventCallback(
    MQTTContext_t * pContext,
    MQTTPacketInfo_t * pPacketInfo,
    MQTTDeserializedInfo_t * pDeserializedInfo
);
// Network send.
int32_t networkSend( NetworkContext_t * pContext, const void * pBuffer, size_t bytes );
// Network receive.
int32_t networkRecv( NetworkContext_t * pContext, void * pBuffer, size_t bytes );

MQTTContext_t mqttContext;
TransportInterface_t transport;
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[ 1024 ];

// Clear context.
memset( ( void * ) &mqttContext, 0x00, sizeof( MQTTContext_t ) );

// Set transport interface members.
transport.pNetworkInterface = &someNetworkInterface;
transport.send = networkSend;
transport.recv = networkRecv;

// Set buffer members.
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = 1024;

status = MQTT_Init( &mqttContext, &transport, getTimeStampMs, eventCallback, &fixedBuffer );

if( status == MQTTSuccess )
{
    // Do something with mqttContext. The transport and fixedBuffer structs were
    // copied into the context, so the original structs do not need to stay in scope.
}
```
**New Code Snippet**:
```c
// Function for obtaining a timestamp.
uint32_t getTimeStampMs();
// Callback function for receiving packets.
void eventCallback(
    MQTTContext_t * pContext,
    MQTTPacketInfo_t * pPacketInfo,
    MQTTDeserializedInfo_t * pDeserializedInfo
);
// Network send.
int32_t networkSend( NetworkContext_t * pContext, const void * pBuffer, size_t bytes );
// Network receive.
int32_t networkRecv( NetworkContext_t * pContext, void * pBuffer, size_t bytes );

MQTTContext_t mqttContext;
TransportInterface_t transport;
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[ 1024 ];
static MQTTPubAckInfo_t pOutgoingPublishRecords[ OUTGOING_PUBLISH_RECORD_COUNT ];
static MQTTPubAckInfo_t pIncomingPublishRecords[ INCOMING_PUBLISH_RECORD_COUNT ];

// Clear context.
memset( ( void * ) &mqttContext, 0x00, sizeof( MQTTContext_t ) );

// Set transport interface members.
transport.pNetworkInterface = &someNetworkInterface;
transport.send = networkSend;
transport.recv = networkRecv;
transport.writev = NULL;

// Set buffer members.
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = 1024;

status = MQTT_Init( &mqttContext, &transport, getTimeStampMs, eventCallback, &fixedBuffer );

if( status == MQTTSuccess )
{
// Initialize MQTT context for QoS > 0. This only has to be done if
// performing QoS > 0 operations.
status = MQTT_InitStatefulQoS(pxMQTTContext,
                                pOutgoingPublishRecords,
                                OUTGOING_PUBLISH_RECORD_COUNT,
                                pIncomingPublishRecords,
                                INCOMING_PUBLISH_RECORD_COUNT);
}

if( status == MQTTSuccess )
{
    // Do something with mqttContext. The transport and fixedBuffer structs were
    // copied into the context, so the original structs do not need to stay in scope.
}
```

#### Replacing `MQTT_SEND_RETRY_TIMEOUT_MS`

For coreMQTT version >=v2.1.0, the `MQTT_SEND_RETRY_TIMEOUT_MS` macro configuration has been deprecated. It has been replaced with `MQTT_SEND_TIMEOUT_MS`. `MQTT_SEND_RETRY_TIMEOUT_MS` was formerly used to timeout sending an MQTT packet after the transport `send` function failed to transmit any bytes for too long. This timeout would reset each time the transport `send` function sent any bytes, leading to the actual timeout scaling with the number of bytes being sent. In other words, the maximum time for sending a packet was variable. In order to remedy this, the `MQTT_SEND_RETRY_TIMEOUT_MS` macro was replaced with `MQTT_SEND_TIMEOUT_MS`. `MQTT_SEND_TIMEOUT_MS` now sets the maximum duration that coreMQTT will attempt to send an MQTT packet, leading to more consistent timeout behavior across various APIs and packet lengths.

### Additional Changes

#### New API `MQTT_CancelCallback`

The `MQTT_CancelCallback` function has been added to allow a program to prevent the event callback from being called when receiving an ACK for a sent packet. For example, if a program sends a publish with packet ID 2 and QoS > 0 using `MQTT_Publish`, the program could then call `MQTT_CancelCallback` on packet ID 2 to prevent coreMQTT from calling the event callback when it receives the `PUBACK` for packet ID 2.

## coreMQTT version >=v3.0.0 Migration Guide

With coreMQTT versions >=v3.0.0, there are some breaking changes that need to be addressed when upgrading.

### Breaking Changes

#### Updated `MQTTEventCallback_t` Callback Function

The `MQTTEventCallback_t` function signature used in `MQTT_Init` has changed to support MQTT v5 properties and reason codes. The signature changed

from

```c
void (* MQTTEventCallback_t)( struct MQTTContext * pContext,
                                struct MQTTPacketInfo * pPacketInfo,
                                struct MQTTDeserializedInfo * pDeserializedInfo )
```

to

```c
void (* MQTTEventCallback_t)( struct MQTTContext * pContext,
                                struct MQTTPacketInfo * pPacketInfo,
                                struct MQTTDeserializedInfo * pDeserializedInfo,
                                enum MQTTSuccessFailReasonCode * pReasonCode,
                                struct MQTTPropBuilder * pSendPropsBuffer,
                                struct MQTTPropBuilder * pGetPropsBuffer )
```
For example:

**Old Code Snippet**:

```c
// Callback function implementation
static void eventCallback(
    MQTTContext_t * pContext,
    MQTTPacketInfo_t * pPacketInfo,
    MQTTDeserializedInfo_t * pDeserializedInfo )
{
    /* Handle incoming publish. The lower 4 bits of the publish packet
    * type is used for the dup, QoS, and retain flags. Hence masking
    * out the lower bits to check if the packet is publish. */
    if( ( pPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        /* Handle incoming publish. */
    }
    else
    {
        /* Handle other packets. */
    }
}

// MQTT_Init call
status = MQTT_Init( &mqttContext,
                    &transport,
                    getTimeMs,
                    eventCallback,
                    &fixedBuffer );
```

**New Code Snippet**:

```c
// Callback function implementation
static void eventCallback(
    MQTTContext_t * pContext,
    MQTTPacketInfo_t * pPacketInfo,
    MQTTDeserializedInfo_t * pDeserializedInfo,
    MQTTSuccessFailReasonCode * pReasonCode,
    MQTTPropBuilder_t * pSendPropsBuffer,
    MQTTPropBuilder_t * pGetPropsBuffer )
{
    /* Handle incoming publish. The lower 4 bits of the publish packet
    * type is used for the dup, QoS, and retain flags. Hence masking
    * out the lower bits to check if the packet is publish. */
    if( ( pPacketInfo->type & 0xF0U ) == MQTT_PACKET_TYPE_PUBLISH )
    {
        /* Handle incoming publish. */

        /* Access publish properties if needed */
        uint16_t topicAlias;
        if( MQTTPropGet_PubTopicAlias( pGetPropsBuffer, &topicAlias ) == MQTTSuccess )
        {
            /* Handle topic alias */
        }
    }
    else
    {
        /* Handle other packets. */

        /* Add properties to outgoing ack packets if needed */
        if( pPacketInfo->type == MQTT_PACKET_TYPE_PUBACK )
        {
            MQTTPropAdd_ReasonString( pSendPropsBuffer, "Success", 7 );
        }
    }
}

// MQTT_Init call
status = MQTT_Init( &mqttContext,
                    &transport,
                    getTimeMs,
                    eventCallback,
                    &fixedBuffer );
```

#### Updated `MQTT_InitStatefulQoS` API

The `MQTT_InitStatefulQoS` function now includes support for MQTT v5 properties in outgoing publish acknowledgments with two additional parameters. Thus, the signature of `MQTT_InitStatefulQoS` changed

from

```c
MQTTStatus_t MQTT_InitStatefulQoS( MQTTContext_t * pContext, MQTTPubAckInfo_t * pOutgoingPublishRecords,
                                    size_t outgoingPublishCount,
                                    MQTTPubAckInfo_t * pIncomingPublishRecords,
                                    size_t incomingPublishCount )
```

to

```c
MQTTStatus_t MQTT_InitStatefulQoS( MQTTContext_t * pContext,
                                    MQTTPubAckInfo_t * pOutgoingPublishRecords,
                                    size_t outgoingPublishCount,
                                    MQTTPubAckInfo_t * pIncomingPublishRecords,
                                    size_t incomingPublishCount,
                                    uint8_t * pAckPropsBuf,
                                    size_t ackPropsBufLength )
```
The new parameters can be set to NULL and 0 respectively if not using MQTT v5 properties in publish acknowledgments.

For example:

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTContext_t mqttContext;
const size_t outgoingPublishCount = 10;
const size_t incomingPublishCount = 10;
MQTTPubAckInfo_t pOutgoingPublishRecords[ outgoingPublishCount ];
MQTTPubAckInfo_t pIncomingPublishRecords[ incomingPublishCount ];

status = MQTT_InitStatefulQoS( &mqttContext,
                            pOutgoingPublishRecords,
                            outgoingPublishCount,
                            pIncomingPublishRecords,
                            incomingPublishCount );

if( status == MQTTSuccess )
{
    // QoS > 0 operations can now be performed
}
```
**New Code snippet**:
```c
/ Variables used in this example.
MQTTContext_t mqttContext;
const size_t outgoingPublishCount = 10;
const size_t incomingPublishCount = 10;
MQTTPubAckInfo_t pOutgoingPublishRecords[ outgoingPublishCount ];
MQTTPubAckInfo_t pIncomingPublishRecords[ incomingPublishCount ];

// Option 1: Without MQTT v5 properties in publish acknowledgments
status = MQTT_InitStatefulQoS( &mqttContext,
                            pOutgoingPublishRecords,
                            outgoingPublishCount,
                            pIncomingPublishRecords,
                            incomingPublishCount,
                            NULL,  // No buffer for properties
                            0 );   // No buffer length

// Option 2: With MQTT v5 properties in publish acknowledgments
uint8_t propertyBuffer[500];
status = MQTT_InitStatefulQoS( &mqttContext,
                            pOutgoingPublishRecords,
                            outgoingPublishCount,
                            pIncomingPublishRecords,
                            incomingPublishCount,
                            propertyBuffer,     // Buffer for properties
                            sizeof(propertyBuffer) );

if( status == MQTTSuccess )
{
    // QoS > 0 operations can now be performed
}
```

#### Updated `MQTT_Connect` API

The `MQTT_Connect` function now includes MQTT v5 property support with two additional parameters. Thus, the signature of `MQTT_Connect` changed

from

```c
MQTTStatus_t MQTT_Connect( MQTTContext_t * pContext,
                            const MQTTConnectInfo_t * pConnectInfo,
                            const MQTTPublishInfo_t * pWillInfo,
                            uint32_t timeoutMs,
                            bool * pSessionPresent )
```
to

```c
MQTTStatus_t MQTT_Connect( MQTTContext_t * pContext,
                            const MQTTConnectInfo_t * pConnectInfo,
                            const MQTTPublishInfo_t * pWillInfo,
                            uint32_t timeoutMs,
                            bool * pSessionPresent,
                            const MQTTPropBuilder_t * pPropertyBuilder,
                            const MQTTPropBuilder_t * pWillPropertyBuilder )`.
```

`pPropertyBuilder` and `pWillPropertyBuilder` can be set to NULL if the user does not want to send any properties.

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
MQTTConnectInfo_t connectInfo = { 0 };
bool sessionPresent;
// This context is assumed to be initialized.
MQTTContext_t * pContext;

// Set connection info.
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

status = MQTT_Connect( pContext,
                    &connectInfo,
                    NULL, /* No will message */
                    MQTT_TIMEOUT_MS,
                    &sessionPresent );

if( status == MQTTSuccess )
{
    // Connection successful
}
```
**New Code Snippet**:
```c
// Variables used in this example.
MQTTStatus_t status;
MQTTConnectInfo_t connectInfo = { 0 };
bool sessionPresent;
// This context is assumed to be initialized.
MQTTContext_t * pContext;

// Set connection info.
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

status = MQTT_Connect( pContext,
                    &connectInfo,
                    NULL, /* No will message */
                    MQTT_TIMEOUT_MS,
                    &sessionPresent,
                    NULL, /* No connect properties */
                    NULL  /* No will properties */ );

if( status == MQTTSuccess )
{
    // Connection successful
}

// To use MQTT v5 properties, initialize and use the property builders:

// Variables used in this example.
MQTTStatus_t status;
MQTTConnectInfo_t connectInfo = { 0 };
bool sessionPresent;
MQTTPropBuilder_t connectionProperties = { 0 };
// This context is assumed to be initialized.
MQTTContext_t * pContext;

// Set connection info.
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

// Initialize and add connect properties
MQTTPropBuilder_t connectionProperties ;
uint8_t buf[500] ;
size bufLength = sizeof(buf);
MQTTPropertyBuilder_Init(&connectionProperties, buf, bufLength) ;

uint32_t sessionExpiryInterval = 100 ; // 100ms
MQTTPropAdd_SessionExpiry(&connectionProperties, sessionExpiryInterval );

// Can also use the will properties in a similar way.

status = MQTT_Connect( pContext,
                    &connectInfo,
                    NULL, /* No will message */
                    MQTT_TIMEOUT_MS,
                    &sessionPresent,
                    &connectionProperties,
                    NULL /* No will properties */ );
if( status == MQTTSuccess )
{
    // Connection successful
}
```

#### Updated `MQTT_Subscribe` API

The `MQTT_Subscribe` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_Subscribe` changed

from

```c
MQTTStatus_t MQTT_Subscribe( MQTTContext_t * pContext,
                                const MQTTSubscribeInfo_t * pSubscriptionList,
                                size_t subscriptionCount,
                                uint16_t packetId )
```

to

```c
MQTTStatus_t MQTT_Subscribe( MQTTContext_t * pContext,
                                const MQTTSubscribeInfo_t * pSubscriptionList,
                                size_t subscriptionCount,
                                uint16_t packetId,
                                const MQTTPropBuilder_t * pPropertyBuilder )
```
The new parameter can be set to NULL if not using MQTT v5 properties.

For example:

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTSubscribeInfo_t subscriptionList[1];
uint16_t packetId;

// Configure subscription
subscriptionList[0].qos = MQTTQoS0;
subscriptionList[0].pTopicFilter = "topic/example";
subscriptionList[0].topicFilterLength = strlen("topic/example");

// Get packet id
packetId = MQTT_GetPacketId(&mqttContext);

status = MQTT_Subscribe(&mqttContext,
                    subscriptionList,
                    1,
                    packetId);

if(status == MQTTSuccess)
{
    // Subscription request sent successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTSubscribeInfo_t subscriptionList[1];
uint16_t packetId;

// Configure subscription
subscriptionList[0].qos = MQTTQoS0;
subscriptionList[0].pTopicFilter = "topic/example";
subscriptionList[0].topicFilterLength = strlen("topic/example");

// Get packet id
packetId = MQTT_GetPacketId(&mqttContext);

// Option 1: Without MQTT v5 properties
status = MQTT_Subscribe(&mqttContext,
                    subscriptionList,
                    1,
                    packetId,
                    NULL);  // No properties

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t propertyBuilder = { 0 };
uint8_t propertyBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&propertyBuilder,
                        propertyBuffer,
                        sizeof(propertyBuffer));

// Add subscription identifier property
MQTTPropAdd_SubscribeId(&propertyBuilder, 1);

status = MQTT_Subscribe(&mqttContext,
                    subscriptionList,
                    1,
                    packetId,
                    &propertyBuilder);

if(status == MQTTSuccess)
{
    // Subscription request sent successfully
}
```

#### Updated `MQTT_Publish` API

The `MQTT_Publish` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_Publish` changed

from

```c
MQTTStatus_t MQTT_Publish( MQTTContext_t * pContext,
                            const MQTTPublishInfo_t * pPublishInfo,
                            uint16_t packetId )
```

to

```c
MQTTStatus_t MQTT_Publish( MQTTContext_t * pContext,
                            const MQTTPublishInfo_t * pPublishInfo,
                            uint16_t packetId,
                            const MQTTPropBuilder_t * pPropertyBuilder )
```
The new parameter can be set to NULL if not using MQTT v5 properties.

For example:

**Old Code Snippet**:
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTPublishInfo_t publishInfo = { 0 };
uint16_t packetId;

// Configure publish message
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Get packet id for QoS > 0
packetId = MQTT_GetPacketId(&mqttContext);

status = MQTT_Publish(&mqttContext,
                    &publishInfo,
                    packetId);

if(status == MQTTSuccess)
{
    // Message published successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTPublishInfo_t publishInfo = { 0 };
uint16_t packetId;

// Configure publish message
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Get packet id for QoS > 0
packetId = MQTT_GetPacketId(&mqttContext);

// Option 1: Without MQTT v5 properties
status = MQTT_Publish(&mqttContext,
                    &publishInfo,
                    packetId,
                    NULL);  // No properties

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t propertyBuilder = { 0 };
uint8_t propertyBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&propertyBuilder,
                        propertyBuffer,
                        sizeof(propertyBuffer));

// Add publish properties
MQTTPropAdd_PubPayloadFormat(&propertyBuilder, 1);
MQTTPropAdd_PubTopicAlias(&propertyBuilder, 1);

status = MQTT_Publish(&mqttContext,
                    &publishInfo,
                    packetId,
                    &propertyBuilder);

if(status == MQTTSuccess)
{
    // Message published successfully
}
```

#### Updated `MQTT_Unsubscribe` API

The `MQTT_Unsubscribe` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_Unsubscribe` changed

from

```c
MQTTStatus_t MQTT_Unsubscribe( MQTTContext_t * pContext,
                                const MQTTSubscribeInfo_t * pSubscriptionList,
                                size_t subscriptionCount,
                                uint16_t packetId )
```

to

```c
MQTTStatus_t MQTT_Unsubscribe( MQTTContext_t * pContext,
                                const MQTTSubscribeInfo_t * pSubscriptionList,
                                size_t subscriptionCount,
                                uint16_t packetId,
                                const MQTTPropBuilder_t * pPropertyBuilder )
```
The new parameter can be set to NULL if not using MQTT v5 properties.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTSubscribeInfo_t unsubscribeList[1];
uint16_t packetId;

// Configure unsubscribe info
unsubscribeList[0].pTopicFilter = "topic/example";
unsubscribeList[0].topicFilterLength = strlen("topic/example");
// QoS field is unused for unsubscribe

// Get packet id
packetId = MQTT_GetPacketId(&mqttContext);

status = MQTT_Unsubscribe(&mqttContext,
                        unsubscribeList,
                        1,
                        packetId);

if(status == MQTTSuccess)
{
    // Unsubscribe request sent successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;
MQTTSubscribeInfo_t unsubscribeList[1];
uint16_t packetId;

// Configure unsubscribe info
unsubscribeList[0].pTopicFilter = "topic/example";
unsubscribeList[0].topicFilterLength = strlen("topic/example");
// QoS field is unused for unsubscribe

// Get packet id
packetId = MQTT_GetPacketId(&mqttContext);

// Option 1: Without MQTT v5 properties
status = MQTT_Unsubscribe(&mqttContext,
                        unsubscribeList,
                        1,
                        packetId,
                        NULL);  // No properties

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t propertyBuilder = { 0 };
uint8_t propertyBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&propertyBuilder,
                        propertyBuffer,
                        sizeof(propertyBuffer));

// Add user property
MQTTUserProperty_t userProperty = {
    .pKey = "key",
    .keyLength = strlen("key"),
    .pValue = "value",
    .valueLength = strlen("value")
};
MQTTPropAdd_UserProp(&propertyBuilder, &userProperty);

status = MQTT_Unsubscribe(&mqttContext,
                        unsubscribeList,
                        1,
                        packetId,
                        &propertyBuilder);

if(status == MQTTSuccess)
{
    // Unsubscribe request sent successfully
}
```

#### Updated `MQTT_Disconnect` API

The `MQTT_Disconnect` function now includes support for MQTT v5 properties and reason codes with two additional parameters. Thus, the signature of `MQTT_Disconnect` changed

from

```c
MQTTStatus_t MQTT_Disconnect( MQTTContext_t * pContext )
```

to

```c
MQTTStatus_t MQTT_Disconnect( MQTTContext_t * pContext,
                                const MQTTPropBuilder_t * pPropertyBuilder,
                                MQTTSuccessFailReasonCode_t reasonCode )
```
The new parameters can be set to NULL and 0 respectively if not using MQTT v5 features.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;

status = MQTT_Disconnect(&mqttContext);

if(status == MQTTSuccess)
{
    // Disconnected successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTContext_t mqttContext;

// Option 1: Without MQTT v5 properties and reason code
status = MQTT_Disconnect(&mqttContext,
                        NULL,  // No properties
                        0);    // No reason code

// Option 2: With MQTT v5 properties and reason code
MQTTPropBuilder_t propertyBuilder = { 0 };
uint8_t propertyBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&propertyBuilder,
                        propertyBuffer,
                        sizeof(propertyBuffer));

// Add disconnect properties
MQTTPropAdd_ReasonString(&propertyBuilder,
                        "Normal shutdown",
                        strlen("Normal shutdown"));

status = MQTT_Disconnect(&mqttContext,
                        &propertyBuilder,
                        MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);

if(status == MQTTSuccess)
{
    // Disconnected successfully
}
```
#### Updated `MQTT_GetConnectPacketSize` API

The `MQTT_GetConnectPacketSize` function now includes support for MQTT v5 properties with two additional parameters. Thus, the signature of `MQTT_GetConnectPacketSize` changed

from

```c
MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
```

to

```c
MQTTStatus_t MQTT_GetConnectPacketSize( const MQTTConnectInfo_t * pConnectInfo,
                                        const MQTTPublishInfo_t * pWillInfo,
                                        const MQTTPropBuilder_t *pConnectProperties,
                                        const MQTTPropBuilder_t *pWillProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
```
The new parameters can be set to NULL if not using MQTT v5 properties.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTConnectInfo_t connectInfo = { 0 };
MQTTPublishInfo_t willInfo = { 0 };
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure connect and will info
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                &remainingLength,
                                &packetSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTConnectInfo_t connectInfo = { 0 };
MQTTPublishInfo_t willInfo = { 0 };
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure connect and will info
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

// Option 1: Without MQTT v5 properties
status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                NULL,  // No connect properties
                                NULL,  // No will properties
                                &remainingLength,
                                &packetSize);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t connectionProperties = { 0 };
MQTTPropBuilder_t willProperties = { 0 };
uint8_t connectPropBuffer[100];
uint8_t willPropBuffer[100];

// Initialize property builders
MQTTPropertyBuilder_Init(&connectionProperties,
                        connectPropBuffer,
                        sizeof(connectPropBuffer));
MQTTPropertyBuilder_Init(&willProperties,
                        willPropBuffer,
                        sizeof(willPropBuffer));

// Add properties as needed
MQTTPropAdd_SessionExpiry(&connectionProperties, 3600);
MQTTPropAdd_WillDelayInterval(&willProperties, 60);

status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                &connectionProperties,
                                &willProperties,
                                &remainingLength,
                                &packetSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```

#### Updated `MQTT_GetPublishPacketSize` API

The `MQTT_GetPublishPacketSize` function now includes support for MQTT v5 properties and maximum packet size with two additional parameters. Thus, the signature of `MQTT_GetPublishPacketSize` changed

from

```c
MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize )
```

to

```c
MQTTStatus_t MQTT_GetPublishPacketSize( const MQTTPublishInfo_t * pPublishInfo,
                                        const MQTTPropBuilder_t * pPublishProperties,
                                        size_t * pRemainingLength,
                                        size_t * pPacketSize,
                                        uint32_t maxPacketSize )
```
The new parameters include support for MQTT v5 properties and server-imposed packet size limitations.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

status = MQTT_GetPublishPacketSize(&publishInfo,
                                &remainingLength,
                                &packetSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
size_t remainingLength = 0;
size_t packetSize = 0;
uint32_t maxPacketSize = 268435455; // Default max packet size

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Option 1: Without MQTT v5 properties
status = MQTT_GetPublishPacketSize(&publishInfo,
                                NULL,  // No publish properties
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t publishProperties = { 0 };
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&publishProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add publish properties
MQTTPropAdd_PubTopicAlias(&publishProperties, 1);
MQTTPropAdd_PubPayloadFormat(&publishProperties, 1);

// Get max packet size from CONNACK properties
uint32_t serverMaxPacketSize = pContext->connectionProperties.serverMaxPacketSize; // Value from server

status = MQTT_GetPublishPacketSize(&publishInfo,
                                &publishProperties,
                                &remainingLength,
                                &packetSize,
                                serverMaxPacketSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```

#### Updated `MQTT_GetSubscribePacketSize` API

The `MQTT_GetSubscribePacketSize` function now includes support for MQTT v5 properties and maximum packet size with two additional parameters. Thus, the signature of `MQTT_GetSubscribePacketSize` changed

from

```c
MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize )
```

to

```c
MQTTStatus_t MQTT_GetSubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            const MQTTPropBuilder_t * pSubscribeProperties,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize )
```
`pSubscribeProperties` can be set to NULL if no properties are sent in the SUBSCRIBE packet.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[2];
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure subscription list
subscriptionList[0].qos = MQTTQoS1;
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

subscriptionList[1].qos = MQTTQoS0;
subscriptionList[1].pTopicFilter = "topic/2";
subscriptionList[1].topicFilterLength = strlen("topic/2");

status = MQTT_GetSubscribePacketSize(subscriptionList,
                                2,
                                &remainingLength,
                                &packetSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure subscription list
subscriptionList[0].qos = MQTTQoS1;
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");
subscriptionList[0].noLocalOption = false;
subscriptionList[0].retainAsPublishedOption = false;
subscriptionList[0].retainHandlingOption = retainSendOnSub;

MQTTPropBuilder_t subscribeProperties = { 0 };
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&subscribeProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add subscription identifier
MQTTPropAdd_SubscribeId(&subscribeProperties, 1);

// Get max packet size from CONNACK properties
uint32_t serverMaxPacketSize = pContext->connectionProperties.serverMaxPacketSize; // value from server

status = MQTT_GetSubscribePacketSize(subscriptionList,
                                1,
                                &subscribeProperties,
                                &remainingLength,
                                &packetSize,
                                serverMaxPacketSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```


#### Updated `MQTT_GetUnsubscribePacketSize` API

The `MQTT_GetUnsubscribePacketSize` function now includes support for MQTT v5 properties with two additional parameters. Thus, the signature of `MQTT_GetUnsubscribePacketSize` changed

from

```c
MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize )
```

to

```c
MQTTStatus_t MQTT_GetUnsubscribePacketSize( const MQTTSubscribeInfo_t * pSubscriptionList,
                                            size_t subscriptionCount,
                                            const MQTTPropBuilder_t * pUnsubscribeProperties,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize )
```
`pUnsubscribeProperties` can be set to NULL if no properties are sent in the UNSUBSCRIBE packet.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
size_t remainingLength = 0;
size_t packetSize = 0;

// Configure unsubscribe list
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    1,
                                    &remainingLength,
                                    &packetSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```
**New Code Snippet**
```C
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
size_t remainingLength = 0;
size_t packetSize = 0;
uint32_t maxPacketSize = 268435455; // Default max packet size

// Configure unsubscribe list
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

// Option 1: Without MQTT v5 properties
status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    1,
                                    NULL,  // No unsubscribe properties
                                    &remainingLength,
                                    &packetSize,
                                    maxPacketSize);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t unsubscribeProperties;
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&unsubscribeProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add user property
MQTTUserProperty_t userProperty = {
    .pKey = "key",
    .keyLength = strlen("key"),
    .pValue = "value",
    .valueLength = strlen("value")
};
MQTTPropAdd_UserProp(&unsubscribeProperties, &userProperty);

// Get max packet size from CONNACK properties
uint32_t serverMaxPacketSize = pContext->connectionProperties.serverMaxPacketSize; // Value from server

status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    2,
                                    &unsubscribeProperties,
                                    &remainingLength,
                                    &packetSize,
                                    serverMaxPacketSize);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```

#### Updated `MQTT_GetDisconnectPacketSize` API

The `MQTT_GetDisconnectPacketSize` function now includes support for MQTT v5 properties and reason code validation with four additional parameters. Thus, the signature of `MQTT_GetDisconnectPacketSize` changed

from

```c
MQTTStatus_t MQTT_GetDisconnectPacketSize( size_t * pPacketSize )
```

to

```c
MQTTStatus_t MQTT_GetDisconnectPacketSize( const MQTTPropBuilder_t * pDisconnectProperties,
                                            size_t * pRemainingLength,
                                            size_t * pPacketSize,
                                            uint32_t maxPacketSize,
                                            MQTTSuccessFailReasonCode_t reasonCode )
```

`pDisconnectProperties` can be set to NULL if no properties are sent in the DISCONNECT packet.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
size_t packetSize = 0;

status = MQTT_GetDisconnectPacketSize(&packetSize);

if(status == MQTTSuccess)
{
    // Packet Size was always 2.
}
```
**New Code Snippet**
```c
// Variables used in this example.
size_t remainingLength = 0;
size_t packetSize = 0;
uint32_t serverMaxPacketSize = pContext->connectionProperties.serverMaxPacketSize ;

// Option 1: Without MQTT v5 properties and default reason code
status = MQTT_GetDisconnectPacketSize(NULL,  // No disconnect properties
                                    &remainingLength,
                                    &packetSize,
                                    serverMaxPacketSize,
                                    0);  // Default reason code

// Option 2: With MQTT v5 properties and specific reason code
MQTTPropBuilder_t disconnectProperties;
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&disconnectProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add disconnect properties
MQTTPropAdd_SessionExpiry(&disconnectProperties, 0);
MQTTPropAdd_ReasonString(&disconnectProperties,
                        "Normal shutdown",
                        strlen("Normal shutdown"));


status = MQTT_GetDisconnectPacketSize(&disconnectProperties,
                                    &remainingLength,
                                    &packetSize,
                                    serverMaxPacketSize,
                                    MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);

if(status == MQTTSuccess)
{
    // Use packet size information
}
```

#### Updated `MQTT_SerializeConnect` API

The `MQTT_SerializeConnect` function now includes support for MQTT v5 properties with two additional parameters. Thus, the signature of `MQTT_SerializeConnect` changed

from

```c
MQTTStatus_t MQTT_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
```

to

```c
MQTTStatus_t MQTT_SerializeConnect( const MQTTConnectInfo_t * pConnectInfo,
                                    const MQTTPublishInfo_t * pWillInfo,
                                    const MQTTPropBuilder_t * pConnectProperties,
                                    const MQTTPropBuilder_t * pWillProperties,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
```
`pConnectProperties` and `pWillProperties` can be set to NULL if these are not being used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTConnectInfo_t connectInfo = { 0 };
MQTTPublishInfo_t willInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;

// Configure connect info
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Get remaining length first
status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                &remainingLength,
                                &packetSize);

// Serialize connect packet
status = MQTT_SerializeConnect(&connectInfo,
                            &willInfo,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Connect packet serialized successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTConnectInfo_t connectInfo = { 0 };
MQTTPublishInfo_t willInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;

// Configure connect info
connectInfo.keepAliveSeconds = 60;
connectInfo.cleanSession = true;
connectInfo.pClientIdentifier = "clientId";
connectInfo.clientIdentifierLength = strlen("clientId");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                NULL,  // No connect properties
                                NULL,  // No will properties
                                &remainingLength,
                                &packetSize);

// Serialize connect packet
status = MQTT_SerializeConnect(&connectInfo,
                            &willInfo,
                            NULL,  // No connect properties
                            NULL,  // No will properties
                            remainingLength,
                            &fixedBuffer);
// Option 2: With MQTT v5 properties
MQTTPropBuilder_t connectionProperties ;
MQTTPropBuilder_t willProperties ;
uint8_t connectPropBuffer[100];
uint8_t willPropBuffer[100];

// Initialize property builders
MQTTPropertyBuilder_Init(&connectionProperties,
                        connectPropBuffer,
                        sizeof(connectPropBuffer));
MQTTPropertyBuilder_Init(&willProperties,
                        willPropBuffer,
                        sizeof(willPropBuffer));

// Add connect properties
MQTTPropAdd_SessionExpiry(&connectionProperties, 3600);
MQTTPropAdd_MaxPacketSize(&connectionProperties, 1024);

// Add will properties if using will message
MQTTPropAdd_WillDelayInterval(&willProperties, 60);

// Get remaining length first
status = MQTT_GetConnectPacketSize(&connectInfo,
                                &willInfo,
                                &connectionProperties,
                                &willProperties,
                                &remainingLength,
                                &packetSize);

// Serialize connect packet
status = MQTT_SerializeConnect(&connectInfo,
                            &willInfo,
                            &connectionProperties,
                            &willProperties,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Connect packet serialized successfully
}
```

#### Updated `MQTT_SerializePublish` API

The `MQTT_SerializePublish` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_SerializePublish` changed

from

```c
MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
                                    uint16_t packetId,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
```

to

```c
MQTTStatus_t MQTT_SerializePublish( const MQTTPublishInfo_t * pPublishInfo,
                                    const MQTTPropBuilder_t * pPublishProperties,
                                    uint16_t packetId,
                                    size_t remainingLength,
                                    const MQTTFixedBuffer_t * pFixedBuffer )
```
`pPublishProperties` can be set to NULL if not used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                &remainingLength,
                                &packetSize);

// Serialize publish packet
status = MQTT_SerializePublish(&publishInfo,
                            packetId,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Publish packet serialized successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;
uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize ;

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                NULL,  // No publish properties
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize publish packet
status = MQTT_SerializePublish(&publishInfo,
                            NULL,  // No publish properties
                            packetId,
                            remainingLength,
                            &fixedBuffer);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t publishProperties = { 0 };
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&publishProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add publish properties
MQTTPropAdd_PubPayloadFormat(&publishProperties, 1);
MQTTPropAdd_PubTopicAlias(&publishProperties, 1);
MQTTPropAdd_PubMessageExpiry(&publishProperties, 3600);

// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                &publishProperties,
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize publish packet
status = MQTT_SerializePublish(&publishInfo,
                            &publishProperties,
                            packetId,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Publish packet serialized successfully
}
```

#### Updated `MQTT_SerializePublishHeader` API

The `MQTT_SerializePublishHeader` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_SerializePublishHeader` changed

from

```c
MQTTStatus_t MQTT_SerializePublishHeader( const MQTTPublishInfo_t * pPublishInfo,
                                            uint16_t packetId,
                                            size_t remainingLength,
                                            const MQTTFixedBuffer_t * pFixedBuffer,
                                            size_t * pHeaderSize )
```

to

```c
MQTTStatus_t MQTT_SerializePublishHeader( const MQTTPublishInfo_t * pPublishInfo,
                                            const MQTTPropBuilder_t * pPublishProperties,
                                            uint16_t packetId,
                                            size_t remainingLength,
                                            const MQTTFixedBuffer_t * pFixedBuffer,
                                            size_t * pHeaderSize )
```
`pPublishInfo` can be set to NULL if not used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
size_t headerSize = 0;
uint16_t packetId = 1;

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                &remainingLength,
                                &packetSize);

// Serialize publish header
status = MQTT_SerializePublishHeader(&publishInfo,
                                packetId,
                                remainingLength,
                                &fixedBuffer,
                                &headerSize);

if(status == MQTTSuccess)
{
    // Send header and payload separately
    send(socket, fixedBuffer.pBuffer, headerSize, 0);
    send(socket, publishInfo.pPayload, publishInfo.payloadLength, 0);
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTPublishInfo_t publishInfo = { 0 };
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
size_t headerSize = 0;
uint16_t packetId = 1;
uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize;

// Configure publish info
publishInfo.qos = MQTTQoS1;
publishInfo.pTopicName = "topic/example";
publishInfo.topicNameLength = strlen("topic/example");
publishInfo.pPayload = "Hello World!";
publishInfo.payloadLength = strlen("Hello World!");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                NULL,  // No publish properties
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize publish header
status = MQTT_SerializePublishHeader(&publishInfo,
                                NULL,  // No publish properties
                                packetId,
                                remainingLength,
                                &fixedBuffer,
                                &headerSize);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t publishProperties ;
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&publishProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add publish properties
MQTTPropAdd_PubPayloadFormat(&publishProperties, 1);
MQTTPropAdd_PubTopicAlias(&publishProperties, 1);
MQTTPropAdd_PubMessageExpiry(&publishProperties, 3600);

// Get remaining length first
status = MQTT_GetPublishPacketSize(&publishInfo,
                                &publishProperties,
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize publish header
status = MQTT_SerializePublishHeader(&publishInfo,
                                &publishProperties,
                                packetId,
                                remainingLength,
                                &fixedBuffer,
                                &headerSize);

if(status == MQTTSuccess)
{
    // Send header and payload separately
    send(socket, fixedBuffer.pBuffer, headerSize, 0);
    send(socket, publishInfo.pPayload, publishInfo.payloadLength, 0);
}
```

#### Updated `MQTT_SerializeSubscribe` API

The `MQTT_SerializeSubscribe` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_SerializeSubscribe` changed

from

```c
MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
```

to

```c
MQTTStatus_t MQTT_SerializeSubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        const MQTTPropBuilder_t * pSubscribeProperties,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
```
`pSubscribeProperties` can be set to NULL if not used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;

// Configure subscription list
subscriptionList[0].qos = MQTTQoS1;
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Get remaining length first
status = MQTT_GetSubscribePacketSize(subscriptionList,
                                1,
                                &remainingLength,
                                &packetSize);

// Serialize subscribe packet
status = MQTT_SerializeSubscribe(subscriptionList,
                            1,
                            packetId,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Subscribe packet serialized successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[11];
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;
uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize ;

// Configure subscription list
subscriptionList[0].qos = MQTTQoS1;
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");
subscriptionList[0].noLocalOption = false;
subscriptionList[0].retainAsPublishedOption = false;
subscriptionList[0].retainHandlingOption = retainSendOnSub;

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetSubscribePacketSize(subscriptionList,
                                1,
                                NULL,  // No subscribe properties
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize subscribe packet
status = MQTT_SerializeSubscribe(subscriptionList,
                            1,
                            NULL,  // No subscribe properties
                            packetId,
                            remainingLength,
                            &fixedBuffer);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t subscribeProperties = { 0 };
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&subscribeProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add subscription identifier
MQTTPropAdd_SubscribeId(&subscribeProperties, 1);

// Get remaining length first
status = MQTT_GetSubscribePacketSize(subscriptionList,
                                1,
                                &subscribeProperties,
                                &remainingLength,
                                &packetSize,
                                maxPacketSize);

// Serialize subscribe packet
status = MQTT_SerializeSubscribe(subscriptionList,
                            1,
                            &subscribeProperties,
                            packetId,
                            remainingLength,
                            &fixedBuffer);

if(status == MQTTSuccess)
{
    // Subscribe packet serialized successfully
}
```

#### Updated `MQTT_SerializeUnsubscribe` API

The `MQTT_SerializeUnsubscribe` function now includes support for MQTT v5 properties with an additional parameter. Thus, the signature of `MQTT_SerializeUnsubscribe` changed

from

```c
MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
```

to

```c
MQTTStatus_t MQTT_SerializeUnsubscribe( const MQTTSubscribeInfo_t * pSubscriptionList,
                                        size_t subscriptionCount,
                                        const MQTTPropBuilder_t * pUnsubscribeProperties,
                                        uint16_t packetId,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
```
`pUnsubscribeProperties` can be set to NULL if not used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;

// Configure unsubscribe list
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Get remaining length first
status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    1,
                                    &remainingLength,
                                    &packetSize);

// Serialize unsubscribe packet
status = MQTT_SerializeUnsubscribe(subscriptionList,
                                1,
                                packetId,
                                remainingLength,
                                &fixedBuffer);

if(status == MQTTSuccess)
{
// Unsubscribe packet serialized successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTSubscribeInfo_t subscriptionList[1];
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint16_t packetId = 1;
uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize ;

// Configure unsubscribe list
subscriptionList[0].pTopicFilter = "topic/1";
subscriptionList[0].topicFilterLength = strlen("topic/1");

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    1,
                                    NULL,  // No unsubscribe properties
                                    &remainingLength,
                                    &packetSize,
                                    maxPacketSize);

// Serialize unsubscribe packet
status = MQTT_SerializeUnsubscribe(subscriptionList,
                                1,
                                NULL,  // No unsubscribe properties
                                packetId,
                                remainingLength,
                                &fixedBuffer);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t unsubscribeProperties ;
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&unsubscribeProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add user property
MQTTUserProperty_t userProperty = {
.pKey = "key",
.keyLength = strlen("key"),
.pValue = "value",
.valueLength = strlen("value")
};
MQTTPropAdd_UserProp(&unsubscribeProperties, &userProperty);

// Get remaining length first
status = MQTT_GetUnsubscribePacketSize(subscriptionList,
                                    1,
                                    &unsubscribeProperties,
                                    &remainingLength,
                                    &packetSize,
                                    maxPacketSize);

// Serialize unsubscribe packet
status = MQTT_SerializeUnsubscribe(subscriptionList,
                                1,
                                &unsubscribeProperties,
                                packetId,
                                remainingLength,
                                &fixedBuffer);

if(status == MQTTSuccess)
{
// Unsubscribe packet serialized successfully
}
```

#### Updated `MQTT_SerializeDisconnect` API

The `MQTT_SerializeDisconnect` function now includes support for MQTT v5 properties and reason codes with three additional parameters. Thus, the signature of `MQTT_SerializeDisconnect` changed

from

```c
MQTTStatus_t MQTT_SerializeDisconnect( const MQTTFixedBuffer_t * pFixedBuffer )
```

to

```c
MQTTStatus_t MQTT_SerializeDisconnect( const MQTTPropBuilder_t *pDisconnectProperties,
                                        MQTTSuccessFailReasonCode_t reasonCode,
                                        size_t remainingLength,
                                        const MQTTFixedBuffer_t * pFixedBuffer )
```
`pDisconnectProperties` can be set to NULL if not used.

For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Serialize disconnect packet
status = MQTT_SerializeDisconnect(&fixedBuffer);

if(status == MQTTSuccess)
{
    // Disconnect packet serialized successfully
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTFixedBuffer_t fixedBuffer;
uint8_t buffer[BUFFER_SIZE];
size_t remainingLength = 0;
uint32_t maxPacketSize = pContext->connectionProperties.serverMaxPacketSize ;

// Configure fixed buffer
fixedBuffer.pBuffer = buffer;
fixedBuffer.size = BUFFER_SIZE;

// Option 1: Without MQTT v5 properties
// Get remaining length first
status = MQTT_GetDisconnectPacketSize(NULL,  // No disconnect properties
                                    &remainingLength,
                                    &packetSize,
                                    maxPacketSize,
                                    0);  // Default reason code

// Serialize disconnect packet
status = MQTT_SerializeDisconnect(NULL,  // No disconnect properties
                                0,      // Default reason code
                                remainingLength,
                                &fixedBuffer);

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t disconnectProperties ;
uint8_t propBuffer[100];

// Initialize property builder
MQTTPropertyBuilder_Init(&disconnectProperties,
                        propBuffer,
                        sizeof(propBuffer));

// Add disconnect properties
MQTTPropAdd_SessionExpiry(&disconnectProperties, 0);
MQTTPropAdd_ReasonString(&disconnectProperties,
                        "Normal shutdown",
                        strlen("Normal shutdown"));

// Get remaining length first
status = MQTT_GetDisconnectPacketSize(&disconnectProperties,
                                    &remainingLength,
                                    &packetSize,
                                    maxPacketSize,
                                    MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION);

// Serialize disconnect packet
status = MQTT_SerializeDisconnect(&disconnectProperties,
                                MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION,
                                remainingLength,
                                &fixedBuffer);

if(status == MQTTSuccess)
{
    // Disconnect packet serialized successfully
}
```

#### Updated `MQTT_DeserializePublish` API

The `MQTT_DeserializePublish` function now includes support for MQTT v5 properties with three additional parameters. Thus, the signature of `MQTT_DeserializePublish` changed

from

```c
MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo )
```

to

```c
MQTTStatus_t MQTT_DeserializePublish( const MQTTPacketInfo_t * pIncomingPacket,
                                        uint16_t * pPacketId,
                                        MQTTPublishInfo_t * pPublishInfo,
                                        MQTTPropBuilder_t * propBuffer,
                                        uint32_t maxPacketSize,
                                        uint16_t topicAliasMax )
```
For example:

**Old Code Snippet**
```c
// Variables used in this example.
MQTTPacketInfo_t incomingPacket;
MQTTPublishInfo_t publishInfo = { 0 };
uint16_t packetId;

// Assume incomingPacket is populated from network

if((incomingPacket.type & 0xF0) == MQTT_PACKET_TYPE_PUBLISH)
{
    status = MQTT_DeserializePublish(&incomingPacket,
                                &packetId,
                                &publishInfo);

    if(status == MQTTSuccess)
    {
        // Handle received publish
        handlePublish(&publishInfo);
    }
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTPacketInfo_t incomingPacket;
MQTTPublishInfo_t publishInfo = { 0 };
uint16_t packetId;

uint16_t topicAliasMax = pContext->connectionProperties.topicAliasMax ;
uint32_t maxPacketSize = pContext->connectionProperties.maxPacketSize ;

// Option 1: Without MQTT v5 properties
if((incomingPacket.type & 0xF0) == MQTT_PACKET_TYPE_PUBLISH)
{
    status = MQTT_DeserializePublish(&incomingPacket,
                                &packetId,
                                &publishInfo,
                                NULL,  // No property buffer
                                maxPacketSize,     // No max packet size
                                topicAliasMax);    // No topic alias maximum

    if(status == MQTTSuccess)
    {
        // Handle received publish
        handlePublish(&publishInfo);
    }
}

// Option 2: With MQTT v5 properties
MQTTPropBuilder_t propBuffer = { 0 };
uint8_t buffer[100];

// Initialize property buffer
MQTTPropertyBuilder_Init(&propBuffer,
                        buffer,
                        sizeof(buffer));

if((incomingPacket.type & 0xF0) == MQTT_PACKET_TYPE_PUBLISH)
{
    status = MQTT_DeserializePublish(&incomingPacket,
                                &packetId,
                                &publishInfo,
                                &propBuffer,
                                maxPacketSize,
                                topicAliasMax);

    if(status == MQTTSuccess)
    {
        // Access publish properties if needed
        uint8_t payloadFormat;
        if(MQTTPropGet_PubPayloadFormat(&propBuffer, &payloadFormat) == MQTTSuccess)
        {
            // Handle payload format
        }

        uint16_t topicAlias;
        if(MQTTPropGet_PubTopicAlias(&propBuffer, &topicAlias) == MQTTSuccess)
        {
            // Handle topic alias
        }

        // Handle received publish
        handlePublish(&publishInfo);
    }
}
```

#### Updated `MQTT_DeserializeAck` API

The `MQTT_DeserializeAck` function now includes support for MQTT v5 properties and reason codes with five additional parameters. Thus, the signature of `MQTT_DeserializeAck` changed

from

```c

MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                    uint16_t * pPacketId,
                                    bool * pSessionPresent )
```

to

```c
MQTTStatus_t MQTT_DeserializeAck( const MQTTPacketInfo_t * pIncomingPacket,
                                    uint16_t * pPacketId,
                                    bool * pSessionPresent,
                                    MQTTReasonCodeInfo_t * pReasonCode,
                                    MQTTPropBuilder_t * propBuffer,
                                    MQTTConnectionProperties_t * pConnectProperties )
```
For example:

**Old Code Snippet**
```c
/ Variables used in this example.
MQTTPacketInfo_t incomingPacket;
uint16_t packetId;
bool sessionPresent;

// Assume incomingPacket is populated from network

status = MQTT_DeserializeAck(&incomingPacket,
                            &packetId,
                            &sessionPresent);

if(status == MQTTSuccess)
{
    // Handle acknowledgment
}
```
**New Code Snippet**
```c
// Variables used in this example.
MQTTPacketInfo_t incomingPacket;
uint16_t packetId;
bool sessionPresent;
MQTTPropBuilder_t propBuffer ; // Can be set to NULL if the user does not want any incoming properties.

MQTTReasonCodeInfo_t reasonCode ; // Can be set to NULL if the incoming packet is CONNACK or PINGRESP

MQTTConnectionProperties_t connectionProperties = pContext->connectionProperties;  // Cannot be set to NULL.

status = MQTT_DeserializeAck(&incomingPacket,
                            &packetId,
                            &sessionPresent,
                            &reasonCode,
                            &propBuffer,
                            &connectionProperties);

if(status == MQTTSuccess)
{
    // Handle acknowledgment based on packet type

}
```











