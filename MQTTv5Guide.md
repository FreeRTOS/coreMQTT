# Moving to MQTT v5.0

## Overview

This guide helps you migrate your existing coreMQTT v2.3.1 (MQTT v3.1.1) application to use the new coreMQTT v5.0.0 library. MQTT v5.0 introduces significant enhancements including properties, enhanced reason codes, bidirectional DISCONNECT packets, and improved error handling.

**Key Philosophy**: The coreMQTT v5.0.0 library is designed to be backward compatible in usage patterns. If you don't want to use MQTT v5.0 features, you can pass `NULL` for property parameters and the library will work similarly to v3.1.1.

> **See also:** For a complete list of every changed function signature
> (including serialization APIs), see the
> [coreMQTT Migration Guide](MigrationGuide.md).

---

## Table of Contents

1. [Quick Start: Minimal Changes](#quick-start-minimal-changes)
2. [API Changes Summary](#api-changes-summary)
3. [Detailed Migration Steps](#detailed-migration-steps)
4. [Using MQTT v5.0 Properties](#using-mqtt-v50-properties)
5. [Enhanced Features in v5.0](#enhanced-features-in-v50)
6. [Common Migration Patterns](#common-migration-patterns)
7. [Troubleshooting](#troubleshooting)

---

## Quick Start: Minimal Changes

If you want to migrate with minimal code changes and **not use v5.0 features**, here's what you need to do:

### 1. Update Include Headers
```c
// No change needed - same header
#include "core_mqtt.h"
```

### 2. Update API Calls - Add NULL for Properties

**MQTT_Connect:**
```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_Connect(&context, &connectInfo, &willInfo, timeoutMs, &sessionPresent);

// coreMQTT v5.0.0 - Add two NULL parameters for properties
status = MQTT_Connect(&context, &connectInfo, &willInfo, timeoutMs, &sessionPresent,
                      NULL,  // Connect properties (optional)
                      NULL); // Will properties (optional)
```

**MQTT_Subscribe:**
```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_Subscribe(&context, pSubscriptionList, subscriptionCount, packetId);

// coreMQTT v5.0.0 - Add NULL parameter for properties
status = MQTT_Subscribe(&context, pSubscriptionList, subscriptionCount, packetId,
                        NULL); // Subscribe properties (optional)
```

**MQTT_Publish:**
```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_Publish(&context, &publishInfo, packetId);

// coreMQTT v5.0.0 - Add NULL parameter for properties
status = MQTT_Publish(&context, &publishInfo, packetId,
                      NULL); // Publish properties (optional)
```

**MQTT_Unsubscribe:**
```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_Unsubscribe(&context, pSubscriptionList, subscriptionCount, packetId);

// coreMQTT v5.0.0 - Add NULL parameter for properties
status = MQTT_Unsubscribe(&context, pSubscriptionList, subscriptionCount, packetId,
                          NULL); // Unsubscribe properties (optional)
```

**MQTT_Disconnect:**
```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_Disconnect(&context);

// coreMQTT v5.0.0 - Add parameters for properties and reason code
status = MQTT_Disconnect(&context,
                         NULL,  // Disconnect properties (optional)
                         NULL); // Reason code (optional)
```

### 3. Update Event Callback Signature

The event callback signature has changed significantly:

```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
void eventCallback(MQTTContext_t *pContext,
                   MQTTPacketInfo_t *pPacketInfo,
                   MQTTDeserializedInfo_t *pDeserializedInfo)
{
    // Handle events
}

// coreMQTT v5.0.0 - Returns bool and has additional parameters
bool eventCallback(MQTTContext_t *pContext,
                   MQTTPacketInfo_t *pPacketInfo,
                   MQTTDeserializedInfo_t *pDeserializedInfo,
                   MQTTSuccessFailReasonCode_t *pReasonCode,      // NEW: Reason code
                   MQTTPropBuilder_t *pSendPropsBuffer,           // NEW: Properties to send
                   MQTTPropBuilder_t *pGetPropsBuffer)            // NEW: Properties received
{
    // Handle events
    // NOTE: pReasonCode and pSendPropsBuffer will be NULL for terminating
    // packets (PUBACK, PUBCOMP, SUBACK, UNSUBACK) since the library does
    // not send a response for these. Always check before dereferencing.
    return true;  // Return true if processed successfully
}
```

### 4. Update MQTT_InitStatefulQoS

The coreMQTT v5.0.0 version adds a buffer for ACK properties:

```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
status = MQTT_InitStatefulQoS(&context,
                              outgoingPublishRecords, outgoingPublishCount,
                              incomingPublishRecords, incomingPublishCount);

// coreMQTT v5.0.0 - Add buffer for ACK properties
uint8_t ackPropsBuffer[500];
status = MQTT_InitStatefulQoS(&context,
                              outgoingPublishRecords, outgoingPublishCount,
                              incomingPublishRecords, incomingPublishCount,
                              ackPropsBuffer,         // NEW: Buffer for ACK properties
                              sizeof(ackPropsBuffer)); // NEW: Buffer size
```

### 5. Update MQTTSubscribeInfo Structure

The coreMQTT v5.0.0 version adds subscription options:

```c
// coreMQTT v2.3.1 (MQTT v3.1.1)
MQTTSubscribeInfo_t subscribeInfo = {
    .qos = MQTTQoS1,
    .pTopicFilter = "my/topic",
    .topicFilterLength = strlen("my/topic")
};

// coreMQTT v5.0.0 - Add subscription options (use defaults for v3.1.1 behavior)
MQTTSubscribeInfo_t subscribeInfo = {
    .qos = MQTTQoS1,
    .pTopicFilter = "my/topic",
    .topicFilterLength = strlen("my/topic"),
    .noLocalOption = false,                    // NEW: Receive own publishes
    .retainAsPublishedOption = false,          // NEW: Don't preserve retain flag
    .retainHandlingOption = retainSendOnSub    // NEW: Send retained messages
};
```

---

## API Changes Summary

### Core API Functions

| Function | v2.3.1 Parameters | v5.0.0 Additional Parameters |
|----------|-------------------|----------------------------|
| `MQTT_Connect` | 5 params | +2: `pPropertyBuilder`, `pWillPropertyBuilder` |
| `MQTT_Subscribe` | 4 params | +1: `pPropertyBuilder` |
| `MQTT_Publish` | 3 params | +1: `pPropertyBuilder` |
| `MQTT_Unsubscribe` | 4 params | +1: `pPropertyBuilder` |
| `MQTT_Disconnect` | 1 param | +2: `pPropertyBuilder`, `pReasonCode` |
| `MQTT_InitStatefulQoS` | 5 params | +2: `pAckPropsBuf`, `ackPropsBufLength` |

### Serialization API Functions

All serialization functions now accept property builders:

| Function | v2.3.1 Parameters | v5.0.0 Additional Parameters |
|----------|-------------------|----------------------------|
| `MQTT_GetConnectPacketSize` | 4 params | +2: `pConnectProperties`, `pWillProperties` |
| `MQTT_SerializeConnect` | 4 params | +2: `pConnectProperties`, `pWillProperties` |
| `MQTT_GetSubscribePacketSize` | 4 params | +1: `pPropertyBuilder` |
| `MQTT_SerializeSubscribe` | 5 params | +1: `pPropertyBuilder` |
| `MQTT_GetPublishPacketSize` | 3 params | +1: `pPropertyBuilder` |
| `MQTT_SerializePublish` | 4 params | +1: `pPropertyBuilder` |
| `MQTT_GetUnsubscribePacketSize` | 4 params | +1: `pPropertyBuilder` |
| `MQTT_SerializeUnsubscribe` | 5 params | +1: `pPropertyBuilder` |

### New Structures and Types

- `MQTTPropBuilder_t` - Property builder for adding properties to packets
- `MQTTConnectionProperties_t` - Connection properties from CONNECT/CONNACK
- `MQTTReasonCodeInfo_t` - Reason codes from ACK packets
- `MQTTSuccessFailReasonCode_t` - Enum for v5.0 reason codes
- `MQTTUserProperty_t` - User-defined key-value properties
- `MQTTRetainHandling_t` - Enum for retain handling options

### New Status Codes

- `MQTTEndOfProperties` - Reached end of properties while parsing
- `MQTTEventCallbackFailed` - Event callback returned false

---

## Detailed Migration Steps

### Step 1: Update Context Initialization

The `MQTT_Init` function signature remains the same, but the event callback signature has changed.

```c
// Update your event callback to match the new signature
bool myEventCallback(MQTTContext_t *pContext,
                     MQTTPacketInfo_t *pPacketInfo,
                     MQTTDeserializedInfo_t *pDeserializedInfo,
                     MQTTSuccessFailReasonCode_t *pReasonCode,
                     MQTTPropBuilder_t *pSendPropsBuffer,
                     MQTTPropBuilder_t *pGetPropsBuffer)
{
    // Your existing logic here
    // ...

    // Return true to indicate successful processing
    return true;
}

// MQTT_Init call remains the same
status = MQTT_Init(&context, &transport, getTimeFunction,
                   myEventCallback, &networkBuffer);
```

### Step 2: Initialize Stateful QoS (if using QoS > 0)

```c
// Allocate buffer for ACK properties
uint8_t ackPropsBuffer[500];  // Size depends on your needs

status = MQTT_InitStatefulQoS(&context,
                              outgoingPublishRecords, outgoingPublishCount,
                              incomingPublishRecords, incomingPublishCount,
                              ackPropsBuffer,
                              sizeof(ackPropsBuffer));
```

### Step 3: Update Connection Code

```c
// If not using properties, pass NULL
status = MQTT_Connect(&context, &connectInfo, &willInfo,
                      timeoutMs, &sessionPresent,
                      NULL,  // No connect properties
                      NULL); // No will properties
```

### Step 4: Update Subscription Code

```c
// Update subscription info structure
MQTTSubscribeInfo_t subscriptions[NUM_SUBS];
for (int i = 0; i < NUM_SUBS; i++) {
    subscriptions[i].qos = MQTTQoS1;
    subscriptions[i].pTopicFilter = topics[i];
    subscriptions[i].topicFilterLength = strlen(topics[i]);

    // Add MQTT v5.0 fields with default values
    subscriptions[i].noLocalOption = false;
    subscriptions[i].retainAsPublishedOption = false;
    subscriptions[i].retainHandlingOption = retainSendOnSub;
}

// Subscribe with NULL properties
status = MQTT_Subscribe(&context, subscriptions, NUM_SUBS, packetId, NULL);
```

### Step 5: Update Publish Code

```c
// Publish with NULL properties
status = MQTT_Publish(&context, &publishInfo, packetId, NULL);
```

### Step 6: Update Disconnect Code

```c
// Disconnect with NULL properties and reason code
status = MQTT_Disconnect(&context, NULL, NULL);
```

---

## Using MQTT v5.0 Properties

If you want to leverage v5.0 features, you'll need to use the property builder API.

### Property Builder Basics

1. **Initialize a property builder:**
```c
MQTTPropBuilder_t propBuilder;
uint8_t propBuffer[500];

status = MQTTPropertyBuilder_Init(&propBuilder, propBuffer, sizeof(propBuffer));
```

2. **Add properties using `MQTTPropAdd_*` functions:**
```c
// Add session expiry
uint32_t sessionExpiry = 3600;  // 1 hour
status = MQTTPropAdd_SessionExpiry(&propBuilder, sessionExpiry,
                                   &(uint8_t){MQTT_PACKET_TYPE_CONNECT});

// Add user property
MQTTUserProperty_t userProp = {
    .pKey = "device-id",
    .keyLength = strlen("device-id"),
    .pValue = "sensor-001",
    .valueLength = strlen("sensor-001")
};
status = MQTTPropAdd_UserProp(&propBuilder, &userProp,
                              &(uint8_t){MQTT_PACKET_TYPE_CONNECT});
```

3. **Pass the property builder to API functions:**
```c
status = MQTT_Connect(&context, &connectInfo, &willInfo, timeoutMs,
                      &sessionPresent, &propBuilder, NULL);
```

### Available Property Functions

#### Connect Properties
- `MQTTPropAdd_SessionExpiry()` - Session expiry interval
- `MQTTPropAdd_ReceiveMax()` - Maximum inflight messages
- `MQTTPropAdd_MaxPacketSize()` - Maximum packet size
- `MQTTPropAdd_TopicAliasMax()` - Maximum topic aliases
- `MQTTPropAdd_RequestRespInfo()` - Request response information
- `MQTTPropAdd_RequestProbInfo()` - Request problem information
- `MQTTPropAdd_AuthMethod()` - Authentication method
- `MQTTPropAdd_AuthData()` - Authentication data
- `MQTTPropAdd_UserProp()` - User-defined properties

#### Will Properties
- `MQTTPropAdd_WillDelayInterval()` - Delay before sending will
- `MQTTPropAdd_PayloadFormat()` - Payload format indicator
- `MQTTPropAdd_MessageExpiry()` - Message expiry interval
- `MQTTPropAdd_ContentType()` - Content type
- `MQTTPropAdd_ResponseTopic()` - Response topic
- `MQTTPropAdd_CorrelationData()` - Correlation data
- `MQTTPropAdd_UserProp()` - User-defined properties

#### Publish Properties
- `MQTTPropAdd_PayloadFormat()` - Payload format indicator
- `MQTTPropAdd_MessageExpiry()` - Message expiry interval
- `MQTTPropAdd_TopicAlias()` - Topic alias
- `MQTTPropAdd_ResponseTopic()` - Response topic
- `MQTTPropAdd_CorrelationData()` - Correlation data
- `MQTTPropAdd_ContentType()` - Content type
- `MQTTPropAdd_UserProp()` - User-defined properties

#### Subscribe Properties
- `MQTTPropAdd_SubscriptionId()` - Subscription identifier
- `MQTTPropAdd_UserProp()` - User-defined properties

#### Unsubscribe/Disconnect Properties
- `MQTTPropAdd_UserProp()` - User-defined properties
- `MQTTPropAdd_ReasonString()` - Human-readable reason
- `MQTTPropAdd_SessionExpiry()` - Session expiry (disconnect only)

### Reading Properties from Incoming Packets

In your event callback, use the `pGetPropsBuffer` parameter to read properties:

```c
bool eventCallback(MQTTContext_t *pContext,
                   MQTTPacketInfo_t *pPacketInfo,
                   MQTTDeserializedInfo_t *pDeserializedInfo,
                   MQTTSuccessFailReasonCode_t *pReasonCode,
                   MQTTPropBuilder_t *pSendPropsBuffer,
                   MQTTPropBuilder_t *pGetPropsBuffer)
{
    if (pGetPropsBuffer != NULL && pGetPropsBuffer->bufferLength != 0) {
        size_t index = 0;
        uint8_t propertyType;

        // Iterate through properties
        while (MQTT_GetNextPropertyType(pGetPropsBuffer, &index, &propertyType)
               == MQTTSuccess) {
            switch (propertyType) {
                case MQTT_SESSION_EXPIRY_ID: {
                    uint32_t sessionExpiry;
                    MQTTPropGet_SessionExpiry(pGetPropsBuffer, &index, &sessionExpiry);
                    // Use sessionExpiry value
                    break;
                }
                case MQTT_USER_PROPERTY_ID: {
                    MQTTUserProperty_t userProp;
                    MQTTPropGet_UserProp(pGetPropsBuffer, &index, &userProp);
                    // Use userProp
                    break;
                }
                default:
                    // IMPORTANT: For any property you don't handle, you MUST call
                    // MQTT_SkipNextProperty to advance the index past it. Without
                    // this, the index will not move and the loop will never terminate.
                    MQTT_SkipNextProperty(pGetPropsBuffer, &index);
                    break;
            }
        }
    }

    return true;
}
```

### Property Getter Functions

Use these functions to extract properties from incoming packets:

**CONNACK Properties:**
- `MQTTPropGet_SessionExpiry()`
- `MQTTPropGet_ReceiveMax()`
- `MQTTPropGet_MaxQos()`
- `MQTTPropGet_RetainAvailable()`
- `MQTTPropGet_MaxPacketSize()`
- `MQTTPropGet_AssignedClientId()`
- `MQTTPropGet_TopicAliasMax()`
- `MQTTPropGet_ReasonString()`
- `MQTTPropGet_UserProp()`
- `MQTTPropGet_WildcardId()`
- `MQTTPropGet_SubsIdAvailable()`
- `MQTTPropGet_SharedSubAvailable()`
- `MQTTPropGet_ServerKeepAlive()`
- `MQTTPropGet_ResponseInfo()`
- `MQTTPropGet_ServerRef()`
- `MQTTPropGet_AuthMethod()`
- `MQTTPropGet_AuthData()`

**PUBLISH Properties:**
- `MQTTPropGet_PayloadFormatIndicator()`
- `MQTTPropGet_MessageExpiryInterval()`
- `MQTTPropGet_TopicAlias()`
- `MQTTPropGet_ResponseTopic()`
- `MQTTPropGet_CorrelationData()`
- `MQTTPropGet_ContentType()`
- `MQTTPropGet_SubscriptionId()`
- `MQTTPropGet_UserProp()`

**ACK Properties:**
- `MQTTPropGet_ReasonString()`
- `MQTTPropGet_UserProp()`

---

## Enhanced Features in v5.0

### 1. Reason Codes

MQTT v5.0 provides detailed reason codes for all ACK packets. Access them via `pDeserializedInfo->pReasonCode`:

```c
if (pPacketInfo->type == MQTT_PACKET_TYPE_SUBACK) {
    if (pDeserializedInfo->pReasonCode != NULL) {
        for (size_t i = 0; i < pDeserializedInfo->pReasonCode->reasonCodeLength; i++) {
            uint8_t code = pDeserializedInfo->pReasonCode->reasonCode[i];

            if (code == MQTT_REASON_SUBACK_GRANTED_QOS0 ||
                code == MQTT_REASON_SUBACK_GRANTED_QOS1 ||
                code == MQTT_REASON_SUBACK_GRANTED_QOS2) {
                // Subscription successful
            } else {
                // Subscription failed - check specific reason code
                // e.g., MQTT_REASON_SUBACK_NOT_AUTHORIZED
            }
        }
    }
}
```

### 2. Server-Initiated Disconnect

In v5.0, the server can send DISCONNECT packets. Handle them in your event callback:

```c
if (pPacketInfo->type == MQTT_PACKET_TYPE_DISCONNECT) {
    // Server initiated disconnect
    if (pReasonCode != NULL) {
        // Check reason code to understand why
        switch (*pReasonCode) {
            case MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN:
                // Server is shutting down
                break;
            case MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT:
                // Keep-alive timeout
                break;
            // Handle other reasons...
        }
    }
}
```

### 3. Enhanced Subscription Options

```c
MQTTSubscribeInfo_t subscription = {
    .qos = MQTTQoS1,
    .pTopicFilter = "device/+/telemetry",
    .topicFilterLength = strlen("device/+/telemetry"),

    // Don't receive messages published by this client
    .noLocalOption = true,

    // Preserve the retain flag from original publish
    .retainAsPublishedOption = true,

    // Only send retained messages if subscription doesn't exist
    .retainHandlingOption = retainSendOnSubIfNotPresent
};
```

### 4. Topic Aliases

Reduce bandwidth by using topic aliases for frequently published topics:

```c
// First publish with full topic and alias
MQTTPropBuilder_t pubProps;
uint8_t pubPropsBuf[100];
MQTTPropertyBuilder_Init(&pubProps, pubPropsBuf, sizeof(pubPropsBuf));

uint16_t topicAlias = 1;
MQTTPropAdd_TopicAlias(&pubProps, topicAlias,
                       &(uint8_t){MQTT_PACKET_TYPE_PUBLISH});

MQTTPublishInfo_t publishInfo = {
    .qos = MQTTQoS1,
    .pTopicName = "device/sensor001/temperature",
    .topicNameLength = strlen("device/sensor001/temperature"),
    .pPayload = "25.5",
    .payloadLength = 4
};

MQTT_Publish(&context, &publishInfo, packetId, &pubProps);

// Subsequent publishes can use empty topic with same alias
publishInfo.pTopicName = "";
publishInfo.topicNameLength = 0;
MQTT_Publish(&context, &publishInfo, nextPacketId, &pubProps);
```

### 5. Request/Response Pattern

```c
// Publisher sets response topic
MQTTPropBuilder_t pubProps;
uint8_t pubPropsBuf[100];
MQTTPropertyBuilder_Init(&pubProps, pubPropsBuf, sizeof(pubPropsBuf));

MQTTPropAdd_ResponseTopic(&pubProps, "response/topic",
                          strlen("response/topic"),
                          &(uint8_t){MQTT_PACKET_TYPE_PUBLISH});

MQTTPropAdd_CorrelationData(&pubProps, "req-123", strlen("req-123"),
                            &(uint8_t){MQTT_PACKET_TYPE_PUBLISH});

MQTT_Publish(&context, &publishInfo, packetId, &pubProps);

// Responder reads response topic and correlation data from incoming publish
// and uses them to send response
```

---

## Common Migration Patterns

### Pattern 1: Simple Migration (No v5.0 Features)

```c
// Before (coreMQTT v2.3.1)
void eventCallback(MQTTContext_t *pContext,
                   MQTTPacketInfo_t *pPacketInfo,
                   MQTTDeserializedInfo_t *pDeserializedInfo) {
    // Handle events
}

MQTT_Connect(&context, &connectInfo, NULL, 1000, &sessionPresent);
MQTT_Subscribe(&context, subscriptions, count, packetId);
MQTT_Publish(&context, &publishInfo, packetId);
MQTT_Disconnect(&context);

// After (coreMQTT v5.0.0)
bool eventCallback(MQTTContext_t *pContext,
                   MQTTPacketInfo_t *pPacketInfo,
                   MQTTDeserializedInfo_t *pDeserializedInfo,
                   MQTTSuccessFailReasonCode_t *pReasonCode,
                   MQTTPropBuilder_t *pSendPropsBuffer,
                   MQTTPropBuilder_t *pGetPropsBuffer) {
    // Handle events (same logic)
    return true;
}

MQTT_Connect(&context, &connectInfo, NULL, 1000, &sessionPresent, NULL, NULL);
MQTT_Subscribe(&context, subscriptions, count, packetId, NULL);
MQTT_Publish(&context, &publishInfo, packetId, NULL);
MQTT_Disconnect(&context, NULL, NULL);
```

### Pattern 2: Adding Session Persistence

```c
// Initialize property builder
MQTTPropBuilder_t connectProps;
uint8_t connectPropsBuf[200];
MQTTPropertyBuilder_Init(&connectProps, connectPropsBuf, sizeof(connectPropsBuf));

// Set session expiry to 1 hour
uint32_t sessionExpiry = 3600;
MQTTPropAdd_SessionExpiry(&connectProps, sessionExpiry,
                          &(uint8_t){MQTT_PACKET_TYPE_CONNECT});

// Connect with session expiry
MQTT_Connect(&context, &connectInfo, NULL, 1000, &sessionPresent,
             &connectProps, NULL);
```

### Pattern 3: Using User Properties for Metadata

```c
// Add custom metadata to publishes
MQTTPropBuilder_t pubProps;
uint8_t pubPropsBuf[300];
MQTTPropertyBuilder_Init(&pubProps, pubPropsBuf, sizeof(pubPropsBuf));

MQTTUserProperty_t deviceId = {
    .pKey = "device-id",
    .keyLength = strlen("device-id"),
    .pValue = "sensor-001",
    .valueLength = strlen("sensor-001")
};
MQTTPropAdd_UserProp(&pubProps, &deviceId,
                     &(uint8_t){MQTT_PACKET_TYPE_PUBLISH});

MQTTUserProperty_t timestamp = {
    .pKey = "timestamp",
    .keyLength = strlen("timestamp"),
    .pValue = "2024-01-23T10:30:00Z",
    .valueLength = strlen("2024-01-23T10:30:00Z")
};
MQTTPropAdd_UserProp(&pubProps, &timestamp,
                     &(uint8_t){MQTT_PACKET_TYPE_PUBLISH});

MQTT_Publish(&context, &publishInfo, packetId, &pubProps);
```

### Pattern 4: Graceful Disconnect with Reason

```c
MQTTPropBuilder_t disconnectProps;
uint8_t disconnectPropsBuf[200];
MQTTPropertyBuilder_Init(&disconnectProps, disconnectPropsBuf,
                         sizeof(disconnectPropsBuf));

MQTTPropAdd_ReasonString(&disconnectProps, "Shutting down for maintenance",
                         strlen("Shutting down for maintenance"),
                         &(uint8_t){MQTT_PACKET_TYPE_DISCONNECT});

MQTTSuccessFailReasonCode_t reason = MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION;
MQTT_Disconnect(&context, &disconnectProps, &reason);
```

---

## Troubleshooting

### Issue: Compilation Errors About Missing Parameters

**Solution**: You're likely calling v5.0.0 APIs with v2.3.1 signatures. Add the additional parameters (use `NULL` if not using properties).

### Issue: Event Callback Not Being Called

**Solution**: Check that your event callback returns `bool` and returns `true`. If it returns `false`, the library treats it as an error.

### Issue: Properties Not Being Sent

**Solution**: Ensure you:
1. Initialize the property builder with `MQTTPropertyBuilder_Init()`
2. Add properties with the correct packet type parameter
3. Pass the property builder (not `NULL`) to the API function

### Issue: Buffer Too Small for Properties

**Solution**: Increase the size of your property buffer. The required size depends on how many properties you're adding. Start with 500 bytes and adjust as needed.

### Issue: Subscription Options Not Working

**Solution**: Ensure your broker supports MQTT v5.0. Check the CONNACK properties to see what features the broker supports:
- `retainAvailable`
- `isWildcardAvailable`
- `isSharedAvailable`
- `isSubscriptionIdAvailable`

### Issue: Reason Codes Always NULL

**Solution**: Reason codes are only present in v5.0 brokers. If connecting to a v3.1.1 broker, reason codes will be NULL.

---

## Additional Resources

- **MQTT v5.0 Specification**: [OASIS MQTT Version 5.0](https://docs.oasis-open.org/mqtt/mqtt/v5.0/mqtt-v5.0.html)
- **coreMQTT Documentation**: Check the `docs/` directory in the repository
- **Example Code**: See `main.c` in the repository root for a complete v5.0.0 example

---

## Summary Checklist

- [ ] Update event callback signature to return `bool` and accept new parameters
- [ ] Add `NULL` parameters to all API calls if not using v5.0 features
- [ ] Update `MQTT_InitStatefulQoS` to include ACK properties buffer
- [ ] Update `MQTTSubscribeInfo_t` structures with new subscription options
- [ ] Test with your MQTT broker (ensure it supports v5.0 if using v5.0 features)
- [ ] Handle new reason codes in event callback
- [ ] Handle server-initiated DISCONNECT packets
- [ ] Consider using v5.0 features like properties, topic aliases, and enhanced subscriptions

---

**Note**: This migration guide assumes you're familiar with the coreMQTT v2.3.1 API. If you're new to coreMQTT, refer to the main documentation and examples in the repository.
