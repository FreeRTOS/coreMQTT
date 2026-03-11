<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.6" doxygen_gitid="4586b5cfaa3d46d51f6a51882951d15644c49edf">
  <compound kind="file">
    <name>core_mqtt.c</name>
    <path>source/</path>
    <filename>core__mqtt_8c.html</filename>
    <includes id="core__mqtt_8h" name="core_mqtt.h" local="yes" imported="no">core_mqtt.h</includes>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="transport__interface_8h" name="transport_interface.h" local="yes" imported="no">transport_interface.h</includes>
    <includes id="core__mqtt__state_8h" name="core_mqtt_state.h" local="yes" imported="no">core_mqtt_state.h</includes>
    <includes id="core__mqtt__serializer__private_8h" name="core_mqtt_serializer_private.h" local="yes" imported="no">private/core_mqtt_serializer_private.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <class kind="struct">MQTTVec</class>
    <class kind="struct">IoVecState_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PRE_STATE_UPDATE_HOOK</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>acba4b6e51723d384aa9140313effdf8b</anchor>
      <arglist>(pContext)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_POST_STATE_UPDATE_HOOK</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a436983fba04e3d13cabea35efc4e9bf8</anchor>
      <arglist>(pContext)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CORE_MQTT_SERIALIZED_LENGTH_FIELD_BYTES</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a989426922a1f5f04ea8b612fd1f4b185</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CORE_MQTT_SUBSCRIBE_PER_TOPIC_VECTOR_LENGTH</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a97f180c9cc32ca9e354e7c22378a386b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CORE_MQTT_UNSUBSCRIBE_PER_TOPIC_VECTOR_LENGTH</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a42477ec456354f2b944b47646ee5a9ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SET_INCOMING_PUB_FLAG</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a058b32a397e7f6e6ac1be106b7df5346</anchor>
      <arglist>(packetID)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>sendBuffer</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a7f4f9871c75f8f987e3c86ae910bd982</anchor>
      <arglist>(MQTTContext_t *pContext, const uint8_t *pBufferToSend, size_t bytesToSend)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendConnectWithoutCopy</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>aad5f0493d1808eff99d088ea5ac8b3b5</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, uint32_t remainingLength, const MQTTPropBuilder_t *pPropertyBuilder, const MQTTPropBuilder_t *pWillPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>sendMessageVector</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a39f478d2bb0366a5f14bfa90316d8d26</anchor>
      <arglist>(MQTTContext_t *pContext, TransportOutVector_t *pIoVec, size_t ioVecCount)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>addEncodedStringToVector</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ae881496b08f50a2abe55fc80213b584b</anchor>
      <arglist>(uint8_t serializedLength[CORE_MQTT_SERIALIZED_LENGTH_FIELD_BYTES], const char *const string, uint16_t length, TransportOutVector_t *iterator, uint32_t *updatedLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendSubscribeWithoutCopy</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a6e737e481f0484cae194fa5c05bd9417</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, uint32_t remainingLength, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendUnsubscribeWithoutCopy</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a5d576e125ba4854761b1900f6332e076</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, uint32_t remainingLength, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>calculateElapsedTime</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a04f9f5742bc28fe29e61f3f46d20d3d6</anchor>
      <arglist>(uint32_t later, uint32_t start)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTPubAckType_t</type>
      <name>getAckFromPacketType</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>aa1f35063dbe3b2c35f278ea6aa347a0e</anchor>
      <arglist>(uint8_t packetType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>recvExact</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>abe9fbe1e681fa7b248489ab4a22be49c</anchor>
      <arglist>(MQTTContext_t *pContext, size_t bytesToRecv)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>receiveConnackPacket</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a77c022626dc046914609d80ccfc1e955</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t incomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint8_t</type>
      <name>getAckTypeToSend</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a02f112e21a6d0b87a3c69ef300d264de</anchor>
      <arglist>(MQTTPublishState_t state)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendPublishAcks</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab4b719d2f726b049c279dcb37fcba840</anchor>
      <arglist>(MQTTContext_t *pContext, uint16_t packetId, MQTTPublishState_t publishState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleKeepAlive</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ae0e50de302a1aa66e3c5b2cdcacc4f3f</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleIncomingPublish</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a4ac6e6829500c4f522eae413c9470e4d</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handlePublishAcks</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a2363868c0417261c27c750251aad13e5</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleIncomingAck</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a264afa489cbfbd96086614d335969115</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t *pIncomingPacket, bool manageKeepAlive)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>receiveSingleIteration</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a14d3be6806a945c14c0529daa1714e10</anchor>
      <arglist>(MQTTContext_t *pContext, bool manageKeepAlive)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateSubscribeUnsubscribeParams</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a136d2a0b080ba8e5150dff7a9cfe8617</anchor>
      <arglist>(const MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, MQTTSubscriptionType_t subscriptionType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>receiveConnack</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a977ee0cfe6c2f856ec904a7ae68e2ef6</anchor>
      <arglist>(MQTTContext_t *pContext, uint32_t timeoutMs, bool cleanSession, MQTTPacketInfo_t *pIncomingPacket, bool *pSessionPresent)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleUncleanSessionResumption</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ac9e041563743306d9c9c1cfa01551818</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleCleanSession</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a91519103b8929035e95a218d54ce1407</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendPublishWithoutCopy</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a750d27d7f4902668e09d50300cfc65ba</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTPublishInfo_t *pPublishInfo, uint8_t *pMqttHeader, size_t headerSize, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validatePublishParams</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ad7eda8c9d4a5afa7b3f830dbd8cf4de4</anchor>
      <arglist>(const MQTTContext_t *pContext, const MQTTPublishInfo_t *pPublishInfo, uint16_t packetId)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>matchEndWildcardsSpecialCases</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab29bb66fe7385c52452a3087bcfbc98e</anchor>
      <arglist>(const char *pTopicFilter, uint16_t topicFilterLength, uint16_t filterIndex)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>matchWildcards</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab1f061741c445d07454cfa03786a5eea</anchor>
      <arglist>(const char *pTopicName, uint16_t topicNameLength, const char *pTopicFilter, uint16_t topicFilterLength, uint16_t *pNameIndex, uint16_t *pFilterIndex, bool *pMatch)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>matchTopicFilter</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a4c052d9dd6a81e866121c24a2ee2aa0b</anchor>
      <arglist>(const char *pTopicName, uint16_t topicNameLength, const char *pTopicFilter, uint16_t topicFilterLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateTopicFilter</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ad9744a59e52744e2e6c8f3c024eab5be</anchor>
      <arglist>(const MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t iterator, MQTTSubscriptionType_t subscriptionType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>checkWildcardSubscriptions</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a3be6306db7a810bd7fa88a12fb4e667d</anchor>
      <arglist>(uint8_t isWildcardAvailable, const MQTTSubscribeInfo_t *pSubscriptionList, size_t iterator)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateSharedSubscriptions</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a6ecd4a82277da5646048263885b2ef1b</anchor>
      <arglist>(const MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, const size_t iterator)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>addSubscriptionOptions</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a7e0dbed0a29728cde3eec4acaa4973b8</anchor>
      <arglist>(const MQTTSubscribeInfo_t subscriptionInfo, uint8_t *pSubscriptionOptionsArray, size_t currentOptionIndex)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleSubUnsubAck</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a38fbb99590eea1dc363e996a7e889aeb</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendPublishAcksWithoutProperty</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>acceb15d0cd3fe0ce049caadb547879a3</anchor>
      <arglist>(MQTTContext_t *pContext, uint16_t packetId, MQTTPublishState_t publishState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendPublishAcksWithProperty</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a2f1219a4667444a4777afc54d5304033</anchor>
      <arglist>(MQTTContext_t *pContext, uint16_t packetId, MQTTPublishState_t publishState, MQTTSuccessFailReasonCode_t reasonCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validatePublishAckReasonCode</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a9c2dc0cf3431f5deb86581b8448f13a1</anchor>
      <arglist>(MQTTSuccessFailReasonCode_t reasonCode, uint8_t packetType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>sendDisconnectWithoutCopy</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ac792ddfd7727a29ee18a26d46b923652</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSuccessFailReasonCode_t *pReasonCode, uint32_t remainingLength, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>handleIncomingDisconnect</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ae9341249f41a4e230c3baba01d4dac4c</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>buildAndSendAckWithProps</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab2cb07b34f25d3c894c1ed732265a6cc</anchor>
      <arglist>(MQTTContext_t *pContext, uint8_t packetTypeByte, uint16_t packetId, MQTTSuccessFailReasonCode_t reasonCode, uint32_t remainingLength, size_t ackPropertyLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addStringToVectorChecked</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>aa1140006f8819efaf9379c4f690b1a74</anchor>
      <arglist>(uint8_t *pLenBuf, const char *pStr, uint16_t strLen, IoVecState_t *pVecState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>appendWillVectors</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>aab2e2ea3b8f020b77e340ad96a2c4b88</anchor>
      <arglist>(const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pWillPropertyBuilder, IoVecState_t *pVecState, uint8_t *pWillPropertyLength, uint8_t *pSerializedTopicLength, uint8_t *pSerializedPayloadLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Init</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ae8444f3a85535e62cdb1ae9c192677d6</anchor>
      <arglist>(MQTTContext_t *pContext, const TransportInterface_t *pTransportInterface, MQTTGetCurrentTimeFunc_t getTimeFunction, MQTTEventCallback_t userCallback, const MQTTFixedBuffer_t *pNetworkBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitStatefulQoS</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a5cccbe5ebf767a6da5fc83db6ca8cbdf</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPubAckInfo_t *pOutgoingPublishRecords, size_t outgoingPublishCount, MQTTPubAckInfo_t *pIncomingPublishRecords, size_t incomingPublishCount, uint8_t *pAckPropsBuf, size_t ackPropsBufLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitRetransmits</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>af2d6f94bf234a888541919195002a13f</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTStorePacketForRetransmit storeFunction, MQTTRetrievePacketForRetransmit retrieveFunction, MQTTClearPacketForRetransmit clearFunction)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_CancelCallback</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a31b74c34cd295b98ed5f5b4c15ed4a8b</anchor>
      <arglist>(const MQTTContext_t *pContext, uint16_t packetId)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_CheckConnectStatus</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab42f3cc5a0681373e4ee973df062bbcd</anchor>
      <arglist>(const MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Connect</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a454a3ec3559a26e007a76f0efeb25c4b</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, uint32_t timeoutMs, bool *pSessionPresent, MQTTPropBuilder_t *pPropertyBuilder, const MQTTPropBuilder_t *pWillPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Subscribe</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ac038c272d04880c46e75406ace568b77</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Publish</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>af495ad1fb62be67b92be65c0b9ed0050</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTPublishInfo_t *pPublishInfo, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Ping</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a66eced0c62ace790354ae3de40fc0959</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Unsubscribe</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a815ff2a751eb8ac3bb4f7ea4f8420154</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Disconnect</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a8c15c0b8701e253183e24b8d0d2ca104</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTPropBuilder_t *pPropertyBuilder, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ProcessLoop</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab95d3d6b3eed98a6184fb2018c5b55d7</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ReceiveLoop</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>aeb7c37284fcf6f68eb577427a6763fc6</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>MQTT_GetPacketId</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a00e1a3eba2c21899a6b4312c7d65d090</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_MatchTopic</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a5dc1ea25687e43b3228e8cf3c7a8e917</anchor>
      <arglist>(const char *pTopicName, const size_t topicNameLength, const char *pTopicFilter, const size_t topicFilterLength, bool *pIsMatch)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetSubAckStatusCodes</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ac43449e06856c6703cda73359c222bd2</anchor>
      <arglist>(const MQTTPacketInfo_t *pSubackPacket, uint8_t **pPayloadStart, size_t *pPayloadSize)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>MQTT_Status_strerror</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a05d9facfce89c5f9edef09ca13717f50</anchor>
      <arglist>(MQTTStatus_t status)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetBytesInMQTTVec</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a8338dd0b461b479e412f03e2986a1eb7</anchor>
      <arglist>(const MQTTVec_t *pVec, size_t *pOutput)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MQTT_SerializeMQTTVec</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>ab65e1ed5db559aa7d9158e9311048b7e</anchor>
      <arglist>(uint8_t *pAllocatedMem, const MQTTVec_t *pVec)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>MQTT_GetPacketTypeString</name>
      <anchorfile>core__mqtt_8c.html</anchorfile>
      <anchor>a371f21f1a4fbc47f708b80f7a684d3a1</anchor>
      <arglist>(uint8_t packetType)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_prop_deserializer.c</name>
    <path>source/</path>
    <filename>core__mqtt__prop__deserializer_8c.html</filename>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="core__mqtt__serializer__private_8h" name="core_mqtt_serializer_private.h" local="yes" imported="no">private/core_mqtt_serializer_private.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>checkPropBuilderParams</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a8d8e01ba1eadbaa438e1ca492ad70f71</anchor>
      <arglist>(const MQTTPropBuilder_t *mqttPropBuilder, const size_t *currentIndex)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>getPropUint8</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a0365ef1bdcf38298d1f4c96d89068200</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t propertyId, uint8_t *property)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>getPropUint16</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aa3a89742b5cba1002568aa9a814e49be</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t propertyId, uint16_t *property)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>getPropUint32</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a93e68940dcc6e676cc1b90e421e8f7bb</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t propertyId, uint32_t *property)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>getPropUtf8</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a7064c3e407225d8874949580a699eab8</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t propertyId, const char **property, size_t *propertyLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetNextPropertyType</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a31161169ebc4b2e107f24a65d0314eac</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, const size_t *currentIndex, uint8_t *property)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SkipNextProperty</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a5f21325b7eb8bb49f3b4f5922f59c10c</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_UserProp</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>ab0bc946040ace4ca8bef29712e4a8a75</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, MQTTUserProperty_t *pUserProperty)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SessionExpiry</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aa6a5c98cf0929d51b83169f6c861bfa0</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pSessionExpiry)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ReceiveMax</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>ad82a828e6d299166fc6eebcded636f3d</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pReceiveMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MaxQos</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a9999939b9df9ee3748e7536364cde470</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pMaxQos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_RetainAvailable</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a321201fd94cb0045c175a18d8aab518a</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pRetainAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MaxPacketSize</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a36d98889d757a8dab491ff735ea2c614</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pMaxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AssignedClientId</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a0ad93d838bf88d1837014d868ec78bf4</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pClientId, size_t *pClientIdLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_TopicAliasMax</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aae3f2e17652e2b46bc004ab61ac0f5d9</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pTopicAliasMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ReasonString</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aeeb3fa7a6bb8887036860cdbb7f735f2</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pReasonString, size_t *pReasonStringLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_WildcardId</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aa44abca689de5311a7b9208c97926614</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pWildcardAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SubsIdAvailable</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a03c9fabd0f7be1ef87aafc758c07e847</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pSubsIdAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SharedSubAvailable</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>aeb3c649b0b6415619c19dbe3e7c7f264</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pSharedSubAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ServerKeepAlive</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a28abec58b4f2ebb6e298fd84b7309c46</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pServerKeepAlive)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ResponseInfo</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>abc5ea886e2fb8744e239d88174cbf606</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pResponseInfo, size_t *pResponseInfoLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ServerRef</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>ab21f89b33a0d834bde04a81b46bc4920</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pServerRef, size_t *pServerRefLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AuthMethod</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a494a7f3fc59e534f8f156b2c2e11218a</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pAuthMethod, size_t *pAuthMethodLen)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AuthData</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>ad426691e883c67d25b33902374250f74</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pAuthData, size_t *pAuthDataLen)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_PayloadFormatIndicator</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>adf6cf83d3cdcc1ff897aaef2ffa59498</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pPayloadFormat)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MessageExpiryInterval</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a3b8104573b072caed2a550d1d0ca1b92</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pMessageExpiry)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_TopicAlias</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a892a6d571ba311023010322573051fe9</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pTopicAlias)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ResponseTopic</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a3282d3f1b5847fd017bd70c760040127</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pResponseTopic, size_t *pResponseTopicLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_CorrelationData</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a57d0b5be13a9062f4d4917a3d030e120</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pCorrelationData, size_t *pCorrelationDataLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SubscriptionId</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>ac66a7eed173968b2a00d7bd29e229444</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pSubscriptionId)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ContentType</name>
      <anchorfile>core__mqtt__prop__deserializer_8c.html</anchorfile>
      <anchor>a34ca9db573593410637c2cda371d1e41</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pContentType, size_t *pContentTypeLength)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_prop_serializer.c</name>
    <path>source/</path>
    <filename>core__mqtt__prop__serializer_8c.html</filename>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="core__mqtt__serializer__private_8h" name="core_mqtt_serializer_private.h" local="yes" imported="no">private/core_mqtt_serializer_private.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isValidPropertyInPacketType</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a5b4f0f2475755e2326069e6ff39a17c7</anchor>
      <arglist>(const uint8_t *mqttPacketType, uint8_t propBitLocation)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addPropUint8</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a7d45a3907a422cac511543a58322c9e3</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint8_t property, uint8_t propId, uint8_t fieldPosition, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addPropUint16</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>aa670023c00414d6fbc5e2f3c33d1fc50</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t property, uint8_t propId, uint8_t fieldPosition, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addPropUint32</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>afac990bb789c18eb35ac9a3db00267f4</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t property, uint8_t propId, uint8_t fieldPosition, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addPropUtf8</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>aad73d5f3c02f162979d2b007cd093892</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *property, size_t propertyLength, uint8_t propId, uint8_t fieldPosition, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_SubscriptionId</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a70891c3c8bba3ea43b9e86108aa803ef</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t subscriptionId, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_UserProp</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a942a44cc446e7a1aa1d177947f336aa0</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const MQTTUserProperty_t *userProperty, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_SessionExpiry</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a68a57b8974bb9123001d7048b3b542af</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t sessionExpiry, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ReceiveMax</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a8b98ad82495b9affed0d41e7b9073d2c</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t receiveMax, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_MaxPacketSize</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a2c908f6cac3ea122d30ee6e172d7a2d2</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t maxPacketSize, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_TopicAliasMax</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a621b90e12ef25b89a1241514dfd26314</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t topicAliasMax, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_RequestRespInfo</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a0df00ea718e8acacff979a23e5160db3</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool requestResponseInfo, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_RequestProbInfo</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>af08456a32706f0edc2aba52c2687f06d</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool requestProblemInfo, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_AuthMethod</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a1540b39fc3737b796d9b45420b87cd70</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *authMethod, size_t authMethodLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_AuthData</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>ad93c187864ab460b2793495d81747eda</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *authData, size_t authDataLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_PayloadFormat</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a0eacc61ec8a6cc13ff4d3669b5718672</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool payloadFormat, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_MessageExpiry</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>ae776d9ed35d16d6fc0e88ffacd36a123</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t messageExpiry, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_WillDelayInterval</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>ae951d156cff169b8379575901d8157fb</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t willDelayInterval, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_TopicAlias</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a0288cfcf7675dcc9c0f9f5aad5cf7077</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t topicAlias, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ResponseTopic</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a0013ac6c13f0c5c92af5af561b0b6a16</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *responseTopic, size_t responseTopicLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_CorrelationData</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>abe7a86739faa4d6689f89d1e49a73b5f</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const void *pCorrelationData, size_t correlationLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ContentType</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a9f00d8054888651ff36382d1f0ab09a2</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *contentType, size_t contentTypeLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ReasonString</name>
      <anchorfile>core__mqtt__prop__serializer_8c.html</anchorfile>
      <anchor>a67b568e5dd77d4bbab897d2cd61e4699</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *pReasonString, size_t reasonStringLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_serializer.c</name>
    <path>source/</path>
    <filename>core__mqtt__serializer_8c.html</filename>
    <includes id="core__mqtt_8h" name="core_mqtt.h" local="yes" imported="no">core_mqtt.h</includes>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="transport__interface_8h" name="transport_interface.h" local="yes" imported="no">transport_interface.h</includes>
    <includes id="core__mqtt__serializer__private_8h" name="core_mqtt_serializer_private.h" local="yes" imported="no">private/core_mqtt_serializer_private.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <class kind="struct">ConnackSeenFlags_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_CONNECT_HEADER_SIZE</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aa7c310cb084af0025c356ed844ae443d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_FLAG_RETAIN</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a3d04b1e1ad7ec25d18fd13726e164f06</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_FLAG_QOS1</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac23212835606fade167fb5ce25eaf103</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_FLAG_QOS2</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>afe1d2a0b7c0803f5a20ebb3c7a607d65</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_FLAG_DUP</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a57c437ecc3720de76093b08eb0d4f813</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_PINGREQ_SIZE</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a6e8a49d0d88f0b038a5379d533858103</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_CONNACK_SESSION_PRESENT_MASK</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aab69dd14c12f8086245c2371288944f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_SIMPLE_ACK_REMAINING_LENGTH</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a4c576df64bca769a91cb64d5d5d86505</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_PINGRESP_REMAINING_LENGTH</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aeab5c92e86ed98750cbf6422b8b57c06</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_CONNACK_MINIMUM_SIZE</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>af64e05d9575ed0e7b4b9c88477b36be9</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>serializePublishCommon</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a26f645e70b94585245c3545ee3c994b7</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint32_t remainingLength, uint16_t packetIdentifier, const MQTTFixedBuffer_t *pFixedBuffer, bool serializePayload)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>calculatePublishPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>af5170d368fc23fc0d8e1a5d89e370bf7</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize, uint32_t publishPropertyLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>calculateSubscriptionPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ae5a41bbfba14b60d7f76c2ae729e3f8b</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t subscribePropLen, uint32_t maxPacketSize, MQTTSubscriptionType_t subscriptionType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateSubscriptionSerializeParams</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a7ee9b526fa781c31bf90f4c6b32e0515</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>serializeConnectPacket</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ae56f35d31545ed6567429feefb00a9b5</anchor>
      <arglist>(const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pConnectProperties, const MQTTPropBuilder_t *pWillProperties, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>logConnackResponse</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a5451f2e3468faaf2bdf85220ebb95aaa</anchor>
      <arglist>(uint8_t responseCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>remainingLengthEncodedSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a65755cc070ea038350e40a61e2f50712</anchor>
      <arglist>(uint32_t length)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint32_t</type>
      <name>getRemainingLength</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a62a69d191b2f4f8b6c32cfcf90a10571</anchor>
      <arglist>(TransportRecv_t recvFunc, NetworkContext_t *pNetworkContext)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>processRemainingLength</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a8a4f72e05cd72fa57ba5a90e204569b3</anchor>
      <arglist>(const uint8_t *pBuffer, const size_t *pIndex, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>incomingPacketValid</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a03dfebbfbc1635567839f7abb7c0f8db</anchor>
      <arglist>(uint8_t packetType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>checkPublishRemainingLength</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>af85f85a24e0cec5bf6d3e2f3b8eb1214</anchor>
      <arglist>(uint32_t remainingLength, MQTTQoS_t qos, uint32_t qos0Minimum)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>processPublishFlags</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a47a044115ee5df1ac7fe02d2ee37e1e0</anchor>
      <arglist>(uint8_t publishFlags, MQTTPublishInfo_t *pPublishInfo)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializeConnack</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a973c3565984ef5b32a869939ba713ce5</anchor>
      <arglist>(MQTTConnectionProperties_t *pConnackProperties, const MQTTPacketInfo_t *pIncomingPacket, bool *pSessionPresent, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>readSubackStatus</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a65b40a24b30c8cc5685d6a145052c788</anchor>
      <arglist>(size_t statusCount, const uint8_t *pStatusStart, MQTTReasonCodeInfo_t *pReasonCodes)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializeSubUnsubAck</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a52a08e1c9e1cccb2f3f3b1ccbaede48b</anchor>
      <arglist>(const MQTTPacketInfo_t *incomingPacket, uint16_t *pPacketId, MQTTReasonCodeInfo_t *pReasonCodes, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializePublish</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aa86da9f52a89f2b4018f00ec984817bc</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, uint16_t *pPacketId, MQTTPublishInfo_t *pPublishInfo, MQTTPropBuilder_t *pPropBuffer, uint16_t topicAliasMax)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializePingresp</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>afdd9b08562ccaa6cf8dd68baa6bc7060</anchor>
      <arglist>(const MQTTPacketInfo_t *pPingresp)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateConnackParams</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a024626265eee4668fe7b6ade70c96b5b</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, bool *pSessionPresent)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializeConnackProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a9e3490888bf89a74e7538334f9b92b7d</anchor>
      <arglist>(MQTTConnectionProperties_t *pConnackProperties, uint32_t length, uint8_t *pIndex, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializeSubUnsubAckProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a04f893f71c9c749d19af5d70efa85d8a</anchor>
      <arglist>(MQTTPropBuilder_t *pPropBuffer, uint8_t *pIndex, size_t *pSubackPropertyLength, uint32_t remainingLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializePubAcks</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac71e533c40d6f02b8afd6e9c687522c5</anchor>
      <arglist>(const MQTTPacketInfo_t *pAck, uint16_t *pPacketIdentifier, MQTTReasonCodeInfo_t *pReasonCode, bool requestProblem, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>decodePubAckProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a6f25c8bb1ad34bfeec6c9407daf40a49</anchor>
      <arglist>(MQTTPropBuilder_t *pPropBuffer, uint8_t *pIndex, uint32_t remainingLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>logAckResponse</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>acb1bc90825581c4c01f99fa063aeaea8</anchor>
      <arglist>(MQTTSuccessFailReasonCode_t reasonCode, uint16_t packetIdentifier)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializePublishProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ae1b08656ec76d18e1f5dd8991ded58c5</anchor>
      <arglist>(MQTTPublishInfo_t *pPublishInfo, MQTTPropBuilder_t *pPropBuffer, uint8_t *pIndex, uint16_t topicAliasMax, uint32_t remainingLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateDisconnectResponse</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac58018d5f96db919d541234eb1fd72de</anchor>
      <arglist>(MQTTSuccessFailReasonCode_t reasonCode, bool incoming)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateReasonCodeForAck</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>adeb2c714a218aec424594f88d4b35b51</anchor>
      <arglist>(uint8_t ackPacketType, MQTTSuccessFailReasonCode_t reasonCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>isValidConnackReasonCode</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a97e5ca58bde2f5ab7d0748ad125f509d</anchor>
      <arglist>(uint8_t reasonCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateIncomingDisconnectProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a024f33ab9a07f74fc864771e99894b09</anchor>
      <arglist>(uint8_t *pIndex, uint32_t disconnectPropertyLength)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>deserializeConnackProperty</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a547aada736b1e8c487c4d3ff4feb0672</anchor>
      <arglist>(uint8_t propertyId, MQTTConnectionProperties_t *pConnackProperties, uint32_t *pPropertyLength, uint8_t **ppVariableHeader, MQTTPropBuilder_t *pPropBuffer, ConnackSeenFlags_t *pSeen)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setConnackPropBit</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a19aa3a5282920678b738fe04e92aee46</anchor>
      <arglist>(MQTTPropBuilder_t *pPropBuffer, uint8_t bitPos)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateBoolProp</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a6b6290c3e8028b71c7c2c52edcd1a495</anchor>
      <arglist>(uint8_t value, const char *pPropName)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>decodeConnackBoolProp</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aae6b8bcb166276ac328c4bc8fa75e89d</anchor>
      <arglist>(uint8_t *pDest, uint32_t *pPropertyLength, bool *pSeen, uint8_t **ppIndex, const char *pPropName, MQTTPropBuilder_t *pPropBuffer, uint8_t bitPos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublishHeaderWithoutTopic</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a66b53664d5d95f55ea5034f097f0cc12</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, uint32_t remainingLength, uint8_t *pBuffer, size_t *headerSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>updateContextWithConnectProps</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ad0d40d4e24e129696094e9a3f51c4e31</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropBuilder, MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetConnectPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a931058b61e64e33e2e96e2c97a21c598</anchor>
      <arglist>(const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pConnectProperties, const MQTTPropBuilder_t *pWillProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeConnect</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a02a046a75ac234c3825f371ec871d084</anchor>
      <arglist>(const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pConnectProperties, const MQTTPropBuilder_t *pWillProperties, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetSubscribePacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a424849a89214232309992fce73d31f86</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pSubscribeProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeSubscribe</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ada1de8a0de3d64505b330e910bb5b04f</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pSubscribeProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetUnsubscribePacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a7070e8c794a3a6e59904c3023a1b471f</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pUnsubscribeProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateUnsubscribeProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aefeb3ff88a39059bd8a598bb63483992</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeUnsubscribe</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aeaba374fa2ca036f088b29ba3e589d67</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pUnsubscribeProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetPublishPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a1252f0560efe63df1d0ae4265dc39516</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublish</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac85ffdd66fbd90b88b370f680a82edcd</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublishHeader</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a644f11fd079963d7b77cb2be0e5cf539</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer, size_t *pHeaderSize)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>serializeAckWithProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a1386a415c14c725148a710a9229b72a2</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer, uint8_t packetType, uint16_t packetId, const MQTTPropBuilder_t *pAckProperties, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>serializeAckBody</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>af26347a83a6b6833259d38fa993124ab</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer, uint8_t packetType, uint16_t packetId, const MQTTPropBuilder_t *pAckProperties, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeAck</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac5f698b6cf6b3329202b8afb2b310151</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer, uint8_t packetType, uint16_t packetId, const MQTTPropBuilder_t *pAckProperties, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetDisconnectPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a9dd03887f3a2b2c51bad03975a9a3440</anchor>
      <arglist>(const MQTTPropBuilder_t *pDisconnectProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeDisconnect</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ac9dae825ef229773a87903af5f7da04f</anchor>
      <arglist>(const MQTTPropBuilder_t *pDisconnectProperties, const MQTTSuccessFailReasonCode_t *pReasonCode, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetPingreqPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a9efe92a214584a4334571492041cc9e6</anchor>
      <arglist>(uint32_t *pPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePingreq</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>af3b3e40858fd984c871511e02a61e15d</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializePublish</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a491ba3d7ace4451bd808779779c540b5</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, uint16_t *pPacketId, MQTTPublishInfo_t *pPublishInfo, MQTTPropBuilder_t *propBuffer, uint32_t maxPacketSize, uint16_t topicAliasMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeConnAck</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a90b1c8adabd6f5109aa1164438bec324</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, bool *pSessionPresent, MQTTPropBuilder_t *pPropBuffer, MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeAck</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a70ccf788bbe591d3ea0ed09f9faf5964</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, uint16_t *pPacketId, MQTTReasonCodeInfo_t *pReasonCode, MQTTPropBuilder_t *pPropBuffer, const MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetIncomingPacketTypeAndLength</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a98cdda86f86a0a1888745a584199e930</anchor>
      <arglist>(TransportRecv_t readFunc, NetworkContext_t *pNetworkContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_UpdateDuplicatePublishFlag</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a269f0b0e193a7a10a6e392b88768ae94</anchor>
      <arglist>(uint8_t *pHeader, bool set)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ProcessIncomingPacketTypeAndLength</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a94fd3f746074b3f6e16ae6b23dad9a28</anchor>
      <arglist>(const uint8_t *pBuffer, const size_t *pIndex, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitConnect</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ad2cb962fb1564e92cb7d931ef1759d5e</anchor>
      <arglist>(MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropertyBuilder_Init</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a1f298e44147e62b8a4cbf735fa4fde57</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint8_t *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateWillProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a7e1952aacd4318c304c79d896835bd06</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>checkOnce</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a6cbb947c39634d9292c06400620d620a</anchor>
      <arglist>(uint32_t *pBitMask, uint8_t bitPos, const char *pPropName)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>validateConnectProperty</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a482f2f2d378f72225fa9ede813979340</anchor>
      <arglist>(uint8_t propertyId, uint32_t *pPropertyLength, uint8_t **ppIndex, uint32_t *pBitMask, bool *pIsRequestProblemInfoSet, uint32_t *pPacketMaxSizeValue)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateConnectProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a79b75b57edbeb5995da36c48387aca04</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, bool *isRequestProblemInfoSet, uint32_t *pPacketMaxSizeValue)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateSubscribeProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a269ce3e2203e88608a5205e5a3ddc2e3</anchor>
      <arglist>(bool isSubscriptionIdAvailable, const MQTTPropBuilder_t *propBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a162d32c54e27ce976c2a5bec5deed3dc</anchor>
      <arglist>(uint16_t serverTopicAliasMax, const MQTTPropBuilder_t *propBuilder, uint16_t *topicAlias)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishParams</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>ae349b08ac962a464a0f1491748765bd0</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, uint8_t retainAvailable, uint8_t maxQos, uint16_t topicAlias, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishAckProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>aec02145ce3dc1b8745b5ad96de5820cf</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetAckPacketSize</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a6fa0494e35cba6482aef49898ff4c6dd</anchor>
      <arglist>(uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize, size_t ackPropertyLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateDisconnectProperties</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a5bfe18401ff9848c18729640f7cef6e7</anchor>
      <arglist>(uint32_t connectSessionExpiry, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeDisconnect</name>
      <anchorfile>core__mqtt__serializer_8c.html</anchorfile>
      <anchor>a73f6ec7ed9e222865a35cdd6d03ea9ae</anchor>
      <arglist>(const MQTTPacketInfo_t *pPacket, uint32_t maxPacketSize, MQTTReasonCodeInfo_t *pDisconnectInfo, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_serializer_private.c</name>
    <path>source/</path>
    <filename>core__mqtt__serializer__private_8c.html</filename>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="core__mqtt__serializer__private_8h" name="core_mqtt_serializer_private.h" local="yes" imported="no">private/core_mqtt_serializer_private.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_VERSION_5</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>acb92be6482db7e3847e9b7a51d1246fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>variableLengthEncodedSize</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a46042dc479bce2b9601cf26430ba6385</anchor>
      <arglist>(uint32_t length)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>encodeString</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>ab7a7d37896bed9ddf38fdb9aadae0f2d</anchor>
      <arglist>(uint8_t *pDestination, const char *pSource, uint16_t sourceLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeUserProp</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>afb581c5b8bec32d9d6e632dadcafbb7e</anchor>
      <arglist>(const char **pPropertyKey, size_t *pPropertyKeyLen, const char **pPropertyValue, size_t *pPropertyValueLen, uint32_t *pPropertyLength, uint8_t **pIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeUint32t</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a2c146729480ada5ec1823ac9ef0447a6</anchor>
      <arglist>(uint32_t *pProperty, uint32_t *pPropertyLength, bool *pUsed, uint8_t **pIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeUint16t</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a2ffcd41513505b60d2ea285fbd36ff61</anchor>
      <arglist>(uint16_t *pProperty, uint32_t *pPropertyLength, bool *pUsed, uint8_t **pIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeUint8t</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a93d23b217a4a3a0e3781ac815536b7ec</anchor>
      <arglist>(uint8_t *pProperty, uint32_t *pPropertyLength, bool *pUsed, uint8_t **pIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeUtf8</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a2fea41d80c1fadecb1600af47d849643</anchor>
      <arglist>(const char **pProperty, size_t *pLength, uint32_t *pPropertyLength, bool *pUsed, uint8_t **pIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeVariableLength</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>af444451f541cea21af0f1f3599520c83</anchor>
      <arglist>(const uint8_t *pBuffer, size_t bufferLength, uint32_t *pLength)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>encodeVariableLength</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a04a8edd2e978d3578cf78b97422ad904</anchor>
      <arglist>(uint8_t *pDestination, uint32_t length)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>serializeAckFixed</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>ac8ce1855434b61aeb0e7d1f262723bb9</anchor>
      <arglist>(uint8_t *pIndex, uint8_t packetType, uint16_t packetId, uint32_t remainingLength, MQTTSuccessFailReasonCode_t reasonCode)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>serializeConnectFixedHeader</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>ae4a2ea03186dc15dc6607d332fe953d7</anchor>
      <arglist>(uint8_t *pIndex, const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, uint32_t remainingLength)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>serializeSubscribeHeader</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a740069a97f44f239d1dd39895c29f763</anchor>
      <arglist>(uint32_t remainingLength, uint8_t *pIndex, uint16_t packetId)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>serializeUnsubscribeHeader</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>a2b2098ac73eb591a6809b5d6517eb96f</anchor>
      <arglist>(uint32_t remainingLength, uint8_t *pIndex, uint16_t packetId)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t *</type>
      <name>serializeDisconnectFixed</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>ab6e65545ba4a14b42287e69a696fad10</anchor>
      <arglist>(uint8_t *pIndex, const MQTTSuccessFailReasonCode_t *pReasonCode, uint32_t remainingLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>decodeSubackPropertyLength</name>
      <anchorfile>core__mqtt__serializer__private_8c.html</anchorfile>
      <anchor>afb207053dbb9620245aa8edf2524540f</anchor>
      <arglist>(const uint8_t *pIndex, uint32_t remainingLength, uint32_t *subackPropertyLength)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_state.c</name>
    <path>source/</path>
    <filename>core__mqtt__state_8c.html</filename>
    <includes id="core__mqtt__state_8h" name="core_mqtt_state.h" local="yes" imported="no">core_mqtt_state.h</includes>
    <includes id="core__mqtt__config__defaults_8h" name="core_mqtt_config_defaults.h" local="yes" imported="no">core_mqtt_config_defaults.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_INVALID_STATE_COUNT</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a49d2236ebe2b3d27e82e54a7b9e74984</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_BITMAP_BIT_SET_AT</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a50892214c51968df798f584272f16a17</anchor>
      <arglist>(position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_SET_BIT</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>acd96521b31682b7d93de544704fd9594</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_CHECK_BIT</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a085ab1307745f304ce2e6d24bdc3f6a7</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>validateTransitionPublish</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>aad1473b9a2d46be62c3e80dd3524af9d</anchor>
      <arglist>(MQTTPublishState_t currentState, MQTTPublishState_t newState, MQTTStateOperation_t opType, MQTTQoS_t qos)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>validateTransitionAck</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>ac85ca8874163b399b7f8e5e17d3c5872</anchor>
      <arglist>(MQTTPublishState_t currentState, MQTTPublishState_t newState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isPublishOutgoing</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>aaf9d4c6e766e40189ff7b68ffea40aa0</anchor>
      <arglist>(MQTTPubAckType_t packetType, MQTTStateOperation_t opType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static size_t</type>
      <name>findInRecord</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>ac805558ac65e84ea9111ce70c873e59e</anchor>
      <arglist>(const MQTTPubAckInfo_t *records, size_t recordCount, uint16_t packetId, MQTTQoS_t *pQos, MQTTPublishState_t *pCurrentState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>compactRecords</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a6cd7b86de2ddb125fee886d58d1a5fd4</anchor>
      <arglist>(MQTTPubAckInfo_t *records, size_t recordCount)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>addRecord</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a5d0ffdfde0c38a1cc1d4e3f4750a8cc4</anchor>
      <arglist>(MQTTPubAckInfo_t *records, size_t recordCount, uint16_t packetId, MQTTQoS_t qos, MQTTPublishState_t publishState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>updateRecord</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a819c7c72087621fcf97a028bff02759e</anchor>
      <arglist>(MQTTPubAckInfo_t *records, size_t recordIndex, MQTTPublishState_t newState, bool shouldDelete)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static uint16_t</type>
      <name>stateSelect</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>adfc09b0c75d5de09cd73650f944699c0</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, uint16_t searchStates, MQTTStateCursor_t *pCursor)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>updateStateAck</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a174a91b9491a344d6fb4f0b39189392f</anchor>
      <arglist>(MQTTPubAckInfo_t *records, size_t maxRecordCount, size_t recordIndex, uint16_t packetId, MQTTPublishState_t currentState, MQTTPublishState_t newState)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static MQTTStatus_t</type>
      <name>updateStatePublish</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>aa0550584e3733da2e31c9478b9307b49</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, size_t recordIndex, uint16_t packetId, MQTTStateOperation_t opType, MQTTQoS_t qos, MQTTPublishState_t currentState, MQTTPublishState_t newState)</arglist>
    </member>
    <member kind="function">
      <type>MQTTPublishState_t</type>
      <name>MQTT_CalculateStateAck</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a0cad28e34f03b84aff43ee243ce8e2cf</anchor>
      <arglist>(MQTTPubAckType_t packetType, MQTTStateOperation_t opType, MQTTQoS_t qos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ReserveState</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a43bc5d82716e1d8b6e167ec0fe50de5d</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, uint16_t packetId, MQTTQoS_t qos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTPublishState_t</type>
      <name>MQTT_CalculateStatePublish</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>aadc4fdd8af74ac25b848a33e916bff50</anchor>
      <arglist>(MQTTStateOperation_t opType, MQTTQoS_t qos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_UpdateStatePublish</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>ad657bd67745c66bc50f0441b4cc94763</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, uint16_t packetId, MQTTStateOperation_t opType, MQTTQoS_t qos, MQTTPublishState_t *pNewState)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_RemoveStateRecord</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>aef2c13cffbbd5c71183282e69ac9d799</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, uint16_t packetId)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_UpdateStateAck</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a09a013b709085ffd51faa33c067cce1f</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, uint16_t packetId, MQTTPubAckType_t packetType, MQTTStateOperation_t opType, MQTTPublishState_t *pNewState)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>MQTT_PubrelToResend</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>ae58ade262ec01262687554b349b2fdf5</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, MQTTStateCursor_t *pCursor, MQTTPublishState_t *pState)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>MQTT_PublishToResend</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a44b3cf50dc477a9f97413a9238a961f6</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, MQTTStateCursor_t *pCursor)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>MQTT_State_strerror</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>a53d786203ca4d5d5630a9eb3dd4cddae</anchor>
      <arglist>(MQTTPublishState_t state)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const size_t</type>
      <name>ZERO_SIZE_T</name>
      <anchorfile>core__mqtt__state_8c.html</anchorfile>
      <anchor>ab93bbb754488b23d5ac75abcd385e086</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt.h</name>
    <path>source/include/</path>
    <filename>core__mqtt_8h.html</filename>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <includes id="transport__interface_8h" name="transport_interface.h" local="yes" imported="no">transport_interface.h</includes>
    <class kind="struct">MQTTPubAckInfo_t</class>
    <class kind="struct">MQTTContext_t</class>
    <class kind="struct">MQTTDeserializedInfo_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_ID_INVALID</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga9fde6503edb9eaad50ecd3392ab9992a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t(*</type>
      <name>MQTTGetCurrentTimeFunc_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>gae3bea55b0e49e5208b8c5709a5ea23aa</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="typedef">
      <type>bool(*</type>
      <name>MQTTEventCallback_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga5ff18bbacb958f5973e7bc4ab035f98b</anchor>
      <arglist>)(struct MQTTContext *pContext, struct MQTTPacketInfo *pPacketInfo, struct MQTTDeserializedInfo *pDeserializedInfo, enum MQTTSuccessFailReasonCode *pReasonCode, struct MQTTPropBuilder *pSendPropsBuffer, struct MQTTPropBuilder *pGetPropsBuffer)</arglist>
    </member>
    <member kind="typedef">
      <type>bool(*</type>
      <name>MQTTStorePacketForRetransmit</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a40084bbbc7f051fc115edd15c338fff6</anchor>
      <arglist>)(struct MQTTContext *pContext, uint32_t handle, MQTTVec_t *pMqttVec)</arglist>
    </member>
    <member kind="typedef">
      <type>bool(*</type>
      <name>MQTTRetrievePacketForRetransmit</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a34d467d70684aa62c91221ea747acbe4</anchor>
      <arglist>)(struct MQTTContext *pContext, uint32_t handle, uint8_t **pSerializedMqttVec, size_t *pSerializedMqttVecLen)</arglist>
    </member>
    <member kind="typedef">
      <type>void(*</type>
      <name>MQTTClearPacketForRetransmit</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a0ca27699ce7ac7a8f80a2eac6b28592b</anchor>
      <arglist>)(struct MQTTContext *pContext, uint32_t handle)</arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTConnectionStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga9f84d003695205cf10a7bd0bafb3dbf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNotConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a0320177ebf1f1b2e24646b44702cec69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a82c8f64d976734e5632e5257bc429ef5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTDisconnectPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a3c7e17f463f44d8f7ce50475b129ac24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTPublishState_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga0480de7552eedd739a26a23fa8e6fd94</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStateNull</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a8349567b7a9efb3913a64a8f4f6fe5c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a896b1507647b504c9208580e4cde26ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubAckSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a65f6f7b343a30fc0558e3aeeb8c97f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRecSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a11e2319a2b25b82121471743d39761e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRelSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a5d2ee2709c6dc7a1eb8b9c40f318909b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubCompSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a7d88904d550b502b4424a41aa4205e56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubAckPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94ab086c55acf106cdc8d420f90899b6803</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRecPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a1bea59454700be9b683b7eb8aaf6bb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRelPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a695431cde1dc9dc5a2dcbd10eba49df2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubCompPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a3281a28d1829d954b596f091b547b627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishDone</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94ad07733793a235ef9a6a04d16637cd7dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTPubAckType_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga8c1bee959b3ed5fab2a2688dd72bf237</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPuback</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a53d5939c680962f37c15ee87ffd63d0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubrec</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a8c98d5d1a68dda33d9039009ab4ef053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubrel</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237af2d737088a231c88e7603acfdbc4fc8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubcomp</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a910c34311ad6a2341afc04839e1c13bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSubAckStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga48dabc1579e3c0ac6058ce9068054611</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611abcc3040d7d53025baee3542c40758abb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611ab180361a6da712c8144d8c499537787d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611a877b2afbc6ec7d9ab57d4862caadf4f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckFailure</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611aeb83b20da8eda934cde6b92db225a808</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Init</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ae8444f3a85535e62cdb1ae9c192677d6</anchor>
      <arglist>(MQTTContext_t *pContext, const TransportInterface_t *pTransportInterface, MQTTGetCurrentTimeFunc_t getTimeFunction, MQTTEventCallback_t userCallback, const MQTTFixedBuffer_t *pNetworkBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitStatefulQoS</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a5cccbe5ebf767a6da5fc83db6ca8cbdf</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTPubAckInfo_t *pOutgoingPublishRecords, size_t outgoingPublishCount, MQTTPubAckInfo_t *pIncomingPublishRecords, size_t incomingPublishCount, uint8_t *pAckPropsBuf, size_t ackPropsBufLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitRetransmits</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>af2d6f94bf234a888541919195002a13f</anchor>
      <arglist>(MQTTContext_t *pContext, MQTTStorePacketForRetransmit storeFunction, MQTTRetrievePacketForRetransmit retrieveFunction, MQTTClearPacketForRetransmit clearFunction)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_CheckConnectStatus</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ab42f3cc5a0681373e4ee973df062bbcd</anchor>
      <arglist>(const MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Connect</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a454a3ec3559a26e007a76f0efeb25c4b</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, uint32_t timeoutMs, bool *pSessionPresent, MQTTPropBuilder_t *pPropertyBuilder, const MQTTPropBuilder_t *pWillPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Subscribe</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ac038c272d04880c46e75406ace568b77</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Publish</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>af495ad1fb62be67b92be65c0b9ed0050</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTPublishInfo_t *pPublishInfo, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_CancelCallback</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a31b74c34cd295b98ed5f5b4c15ed4a8b</anchor>
      <arglist>(const MQTTContext_t *pContext, uint16_t packetId)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Ping</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a66eced0c62ace790354ae3de40fc0959</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Unsubscribe</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a815ff2a751eb8ac3bb4f7ea4f8420154</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, uint16_t packetId, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_Disconnect</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a8c15c0b8701e253183e24b8d0d2ca104</anchor>
      <arglist>(MQTTContext_t *pContext, const MQTTPropBuilder_t *pPropertyBuilder, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ProcessLoop</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ab95d3d6b3eed98a6184fb2018c5b55d7</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ReceiveLoop</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>aeb7c37284fcf6f68eb577427a6763fc6</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>MQTT_GetPacketId</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a00e1a3eba2c21899a6b4312c7d65d090</anchor>
      <arglist>(MQTTContext_t *pContext)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_MatchTopic</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a5dc1ea25687e43b3228e8cf3c7a8e917</anchor>
      <arglist>(const char *pTopicName, const size_t topicNameLength, const char *pTopicFilter, const size_t topicFilterLength, bool *pIsMatch)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetSubAckStatusCodes</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ac43449e06856c6703cda73359c222bd2</anchor>
      <arglist>(const MQTTPacketInfo_t *pSubackPacket, uint8_t **pPayloadStart, size_t *pPayloadSize)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>MQTT_Status_strerror</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a05d9facfce89c5f9edef09ca13717f50</anchor>
      <arglist>(MQTTStatus_t status)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetBytesInMQTTVec</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a8338dd0b461b479e412f03e2986a1eb7</anchor>
      <arglist>(const MQTTVec_t *pVec, size_t *pOutput)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>MQTT_SerializeMQTTVec</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>ab65e1ed5db559aa7d9158e9311048b7e</anchor>
      <arglist>(uint8_t *pAllocatedMem, const MQTTVec_t *pVec)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>MQTT_GetPacketTypeString</name>
      <anchorfile>core__mqtt_8h.html</anchorfile>
      <anchor>a371f21f1a4fbc47f708b80f7a684d3a1</anchor>
      <arglist>(uint8_t packetType)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_config_defaults.h</name>
    <path>source/include/</path>
    <filename>core__mqtt__config__defaults_8h.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_DO_NOT_USE_CUSTOM_CONFIG</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>abd12bb401eecf3f6694447ea3b1c886d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUB_UNSUB_MAX_VECTORS</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga928ea0bff12ebf9cf9fa9dfe5cafebbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a8ca6c96436d5e7c2c8a7933fb28a5c87</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PINGRESP_TIMEOUT_MS</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>afa825fddb52da7df88fb56d2befcd2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PACKET_TX_TIMEOUT_MS</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a9b3c7c33badcafec477a205d916424bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PACKET_RX_TIMEOUT_MS</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a46ef647d5a8315f626dc17d62e264aed</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RECV_POLLING_TIMEOUT_MS</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a43dc9a67427d9e420a65955eea0e2671</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SEND_TIMEOUT_MS</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>ac262cab68c4c713ebc2b91a2e4ab8b19</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LogError</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a8d9dbaaa88129137a4c68ba0456a18b1</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LogWarn</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a7da92048aaf0cbfcacde9539c98a0e05</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LogInfo</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a00810b1cb9d2f25d25ce2d4d93815fba</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LogDebug</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>af60e8ffc327d136e5d0d8441ed98c98d</anchor>
      <arglist>(message)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LogTrace</name>
      <anchorfile>core__mqtt__config__defaults_8h.html</anchorfile>
      <anchor>a5746dc4ea5f49e1a35ec2c1076dd1913</anchor>
      <arglist>(message)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_serializer.h</name>
    <path>source/include/</path>
    <filename>core__mqtt__serializer_8h.html</filename>
    <includes id="transport__interface_8h" name="transport_interface.h" local="yes" imported="no">transport_interface.h</includes>
    <class kind="struct">MQTTFixedBuffer_t</class>
    <class kind="struct">MQTTConnectInfo_t</class>
    <class kind="struct">MQTTSubscribeInfo_t</class>
    <class kind="struct">MQTTPublishInfo_t</class>
    <class kind="struct">MQTTPacketInfo_t</class>
    <class kind="struct">MQTTPropBuilder_t</class>
    <class kind="struct">MQTTReasonCodeInfo_t</class>
    <class kind="struct">MQTTConnectionProperties_t</class>
    <class kind="struct">MQTTUserProperty_t</class>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_CONNECT</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga64a0515bda2ecc89e97595535e1cf0ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_CONNACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gab14f6c39c303eac1a76816edfde7feab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBLISH</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga5b2d79c61f2591c8e5772f974826d4ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga5f279d63de47a973b41b95f74f47a4f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBREC</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gafa2d8f28da39560f152076b99610e6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBREL</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gaeaa2ceecffda50e2d22ccecf046083c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBCOMP</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga478ecbc98d2ca83d4ce7db33622b5c3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_SUBSCRIBE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga80cfef333e60d967ca927b2e5e665f18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_SUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga307e0186aa17fdd0d6d181d3d2715766</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_UNSUBSCRIBE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga4a94c954cfcea31c8fc3e2adf092b228</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_UNSUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga38bc8ed0b9a1581cf85cecdede7d1a64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PINGREQ</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gacbe28c7d081275d1805c2142ff792185</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PINGRESP</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga285fc02048e2482794042fa98639e514</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_DISCONNECT</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gaed07155a3d6fa4b7624b9f36ed33ec6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_AUTH</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga14ad742747e745fea71a20ae587db704</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_ACK_PACKET_SIZE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga26994fcfacb1cff892caa45ec31ca7c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_QOS1</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a1b87c5ebb6c1e6907a59710f1c57e4ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_QOS2</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a7dae72c27b63f7ea24f54213788ba8d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_NO_LOCAL</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ab358e7c90b003c8f5c287d26db40aada</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_RETAIN_AS_PUBLISHED</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a1d32653aa8131a0aee7ec1448c80f5ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_RETAIN_HANDLING1</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa39d371721c6a63140973ac41f7c2dbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIBE_RETAIN_HANDLING2</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0f858dc57ebeb7b38fb83c1cf7717e5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SESSION_EXPIRY_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ac1e2cffe4c4abe09c23ea66a4bcd9f47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RECEIVE_MAX_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a6ae36e05851d5fd3c952391f3f6a1734</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_PACKET_SIZE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0d799c7db9a9d9bbbbd6e817be580eef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_TOPIC_ALIAS_MAX_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a63431aee8db6e48476d247c0264411ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REQUEST_RESPONSE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a6991e7e75f326c038b3beb7dd925f382</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REQUEST_PROBLEM_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aef8b0a6a96ad3c176480d441063db81e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_USER_PROPERTY_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa26c39157ae4f8634e5c5c3ddc166ca5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_AUTH_METHOD_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa05a5e6ff5bfba0078d909eb2430412a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_AUTH_DATA_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a43cc9da2928edae0cf1ec742c1fdccc4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_WILL_DELAY_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>af876de4b31b67d3117eee587471caa0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PAYLOAD_FORMAT_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad12f255cbee69a03575f0224d3590994</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MSG_EXPIRY_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a258afeb4922eca2dcbede1a279169fc0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONTENT_TYPE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a01a73ed2c10aabac0929ddd5ecdf9518</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RESPONSE_TOPIC_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a6167d14717ce6bc82dd2a61ce5da12d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CORRELATION_DATA_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>af3c47b888403ce143b688aedc833c876</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_TOPIC_ALIAS_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a95e3cc4784f4f3aa7c958c0d620d3524</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_QOS_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ab5747c9c29e3b2e46dd778814388e53a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RETAIN_AVAILABLE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a164ab1133408109f8f7e7fe3e166a73a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_ASSIGNED_CLIENT_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a00ec1a7b787f5f45195d11d4fcbda414</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REASON_STRING_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a5b1aa2c05f1c5b0a088789710f2e757b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_WILDCARD_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ae955f226e3ad14476bbb889ff3cc1817</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUB_AVAILABLE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a869b3061d024ea586ae2be19234e0ad2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SHARED_SUB_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>abd69de026a9e13953f6dab4ba8a79456</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SERVER_KEEP_ALIVE_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a2fe7996d86eac3cd268aa568a779a880</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RESPONSE_INFO_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a3a5c0c90103a9f2506a269d0268b75b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SERVER_REF_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a34fbb14daa5f401e5e33e3cc323bfa8a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIPTION_ID_ID</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa35431ea47815c0ba43bb1a4ceda4051</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gaba7ec045874a1c3432f99173367f735c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSuccess</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca484e062cb4f3fccc1858dd25cfeee056</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTBadParameter</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa39030c93b0263b2699502a074f003b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNoMemory</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735cab1be4db832a0468f024243bca151a8df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSendFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735cafd06b63fe9677fa2af06b0f4c7d4ad55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTRecvFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa14bc8aa4ad218702d782366945d43ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTBadResponse</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa5d7507e7664a14d63a8bc44b280093e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTServerRefused</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca25a3d1747e308e99daa805fe576f84b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNoDataAvailable</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca676f21c0ddf297ae3ec874bc829aa957</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTIllegalState</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca67905d7a05f98faa557a73eb5092bd8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStateCollision</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca8d05b92240dea6df08eab5a9e3799c11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTKeepAliveTimeout</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca076ca8965e836a06e707a94adb26144f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNeedMoreBytes</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa97df53014d919df5ecd54398f89f9b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTEndOfProperties</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caeb817a941b956ae02f011976fa1ce645</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa13a678a441c9ef6b88d6d8be8a64090</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusNotConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca074273b04f70b93c1f20702096cac273</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusDisconnectPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca230baa3eaabf50e6b319f792a82bb863</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishStoreFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca61a3e19715d8ee1ee330c9bbd4b2708a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishRetrieveFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca274b61e8a2aea2cf551353c6d09a6eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTEventCallbackFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca9a19fd9b3228ebcdf2a461c97e53d580</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTQoS_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gae308a5928d7f537379c29a894228093a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aaad51b23a1ae1417f96d8f343c788d1d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aa019d0b8a8cfadb6f98462b046bdacbb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aa85e04ac0465cbdef6dd69ff71b2bbfbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTRetainHandling_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga276a86010075cf64d522fe56cabee1e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainSendOnSub</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3ad87d133aa73a39124fa12597b9d7c449</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainSendOnSubIfNotPresent</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3a24a986f1b6e7f71f08756fc9e170743b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainDoNotSendonSub</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3a83c31ac46bb92684ee74579b7088a8b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSuccessFailReasonCode_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga377edfeeb661cf054a3c7c598637450e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaec27e57d19af296e4bb5d8f90341fa3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea73b7865fc7a3270a164578ddb64ad8d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9a9d5864934d095d93f67161f81595d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea82e0d55e321db001275fee139d577191</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9c8370fe6dccaac60b16231547d5b6a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8b17f877c0fdb9c9335eac60a9dbf22c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafc2774d718797370f5906e7b20539aac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea05c1966ba2278d44e83b9ddc85837643</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea932a871c8527003e79f798057761bc55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8d1f682d9f527fab38d402de4db8733d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_NO_MATCHING_SUBSCRIBERS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5230510b5f7cfd96c6ea2aa9a0b493f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea168512c03686ee4050ea35d0525ff6bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa414c4457e344718201c7123cfc5f13d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea913d3524748b2a666d3b7f76de6c9c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea1772f8237e7aae6721795c723bbac7d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea09b6d2b1f88e3c37ae65d6c18c875d73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea21b7b0bd8de50f4fe60dabc4147a3635</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9ad53a4155b45aa8caef03161da252c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREL_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea160a49e8cdf0c3057c1b62b91140da81</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea2adc444a76d72ba1d82e49d2f76898f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBCOMP_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea30200593b5428a3fde25fff1bca99ccd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBCOMP_PACKET_IDENTIFIER_NOT_FOUND</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3b21320822eb0c9523e2c6933a15a436</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa05642f2d6bcee441d2110eb02806b14</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3fcfe2653d5beddda324c84054b6f307</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_MALFORMED_PACKET</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eadfca66707d73f1f7a29873f2c560318c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PROTOCOL_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5523e76b618f2b5825a6f9903ca737c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9e2ee2a9c4cd76850c5c57ebe1b934c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea19d7ac951f3bcca5cd180a38296d72d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea2080f224b87c2c29bccc710c7db5bf82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafc3f87d6a108fd648cb7c85850fa7cf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea55c5e62fab7ed193a2a2e958828439e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_UNAVAILABLE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4af7f1f540e1a273935ad68b863049ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_BUSY</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaf5f487d1e6f66ce57d72e6d4dbfe712d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BANNED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae3b5b13fbe11aa24e05eda01e5df3f6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaa0caacb28142c9d425fd8bc3a916d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea273cba3efdf86e3fe73a2c8d977da2aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PACKET_TOO_LARGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7b20c7314226847850f87c099f5ce259</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5f6a48c02fc7a9472bdbadd2d09507e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea04ae6a169ed488fc07199e59e500aa36</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaad9d9fa043c9031989ca258ed76d92c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead0dd365babab6299360409c9b8f5ec4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_USE_ANOTHER_SERVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9603c89aa2e2043199b83ef04d21e300</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_MOVED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaae65c001ed16cde6e73c90ae51a52e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea18febf18ff0950ac7d75a1bf2e0beb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea691755049d5fc3165296ac233d907a4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaef376fb954f8b54ff99d86cb4d50c11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae2263fb8bdb2b367a55baebd4d90a1a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaba8aba7d1f6dad09ed7078e33453ec50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaff2be32d18beea90566c7fccb11db5ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea823864d4817117a7b918c759d96f53bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea59a194884e3e1abccd33c2aaa11d8378</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea5e8eedbc1c09584c2741fc5e3f086a81</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaeef90d33af2613da0a0967982ad9053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae67171661c2dd9e03c10135e70a7b2b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea41a9e6abaa661b7fbf3c6dc0617afb0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae291e9ec840e39f3b970daf3f8447b38</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea34f4d0fe2dcef18f535f172c846f06e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_NO_SUBSCRIPTION_EXISTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea533c4cff4a264052cfdaaf7b8cfce0a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaedc0af01a1ebcd3c260f44aa71d82935</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea1fd2b23ad03605ee3f8f6acf9550f0c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafdd2cd9e4261858d07236e91e67380c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3dc70aeec66055f198326693a55d1e9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7a8d20d9fe656ce1bbb2cfb3ecf08db8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea55edb84a47cdda9f150a12bfd2e38db1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaa11f277027410058166f75e0d3dbacd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea836695bd488cd356386ec63e20728a7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MALFORMED_PACKET</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea228092ce6e3a800f8bb7a5ee49297fa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PROTOCOL_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8a9d508be1ca7eba456b79b3249beff6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea26b9d9b87e78ae31feacbeec1fc68ce0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead4542bb94fa87e1daa29ca5609d671fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_BUSY</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab4cdb5cb419222a0e5ff587dcb5eb17f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab082c579f274ae698dcc3c91a896f8d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_BAD_AUTHENTICATION_METHOD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead4edae719818611fc3db6063ab4e4105</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea56f178ce8bc66bebe20b5d414f795695</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SESSION_TAKEN_OVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea562bc2b776339a525922f14024cca3fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea5e64bb43f132a39d0fe5af25a99ea420</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eabe648c943a525c291a2f4a1eee5315d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_RECEIVE_MAXIMUM_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7f7163334d5377850cd658049fd4183b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_ALIAS_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa1bbbe637efa97e740141a357507fb04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eadd81177a4addf111b27e1dfdcfc7f328</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MESSAGE_RATE_TOO_HIGH</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaef7a88988105ebb7baaf9f5924b26897</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafe62edd931be084a0a40a60039744f87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_ADMINISTRATIVE_ACTION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4d2cc1ccbb197ebe3ced7718ae002237</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea06eaa2d01ace739d436800ec4cadf437</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_RETAIN_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab490ac58760a290b109929691931674d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_QOS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa95ba7d7fc079d39632477a9776e467e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_USE_ANOTHER_SERVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea440082b1d151728ca496c1624f77ea4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_MOVED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7b4ba5aa35ca01bc22012d2b2cfc3cd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead41f26fd3217575342fe35da3ec17d99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_CONNECTION_RATE_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4beedd8b2cdb30d03f0651d852fbbbd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MAXIMUM_CONNECT_TIME</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3fdbd45bd2c7955b8ed3aa441c5e0dca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea173f54ae8b2e495b94ae0431e96590ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea944e2976e661db8f7831cec36cdd731c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_INVALID_REASON_CODE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa174c5b038a43f24722fc1bf35bb3b05</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSubscriptionType_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga92e39b92b76d439a183fc6f5e300195f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_TYPE_SUBSCRIBE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga92e39b92b76d439a183fc6f5e300195fafc8760ca08fcb6c2011be8660cf539d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_TYPE_UNSUBSCRIBE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga92e39b92b76d439a183fc6f5e300195fa91f5a06482f72290b846ffd16f0a4523</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetConnectPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a931058b61e64e33e2e96e2c97a21c598</anchor>
      <arglist>(const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pConnectProperties, const MQTTPropBuilder_t *pWillProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeConnect</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a02a046a75ac234c3825f371ec871d084</anchor>
      <arglist>(const MQTTConnectInfo_t *pConnectInfo, const MQTTPublishInfo_t *pWillInfo, const MQTTPropBuilder_t *pConnectProperties, const MQTTPropBuilder_t *pWillProperties, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetSubscribePacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a424849a89214232309992fce73d31f86</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pSubscribeProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeSubscribe</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ada1de8a0de3d64505b330e910bb5b04f</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pSubscribeProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetUnsubscribePacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a7070e8c794a3a6e59904c3023a1b471f</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pUnsubscribeProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeUnsubscribe</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aeaba374fa2ca036f088b29ba3e589d67</anchor>
      <arglist>(const MQTTSubscribeInfo_t *pSubscriptionList, size_t subscriptionCount, const MQTTPropBuilder_t *pUnsubscribeProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetPublishPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a1252f0560efe63df1d0ae4265dc39516</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublish</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ac85ffdd66fbd90b88b370f680a82edcd</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublishHeaderWithoutTopic</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a66b53664d5d95f55ea5034f097f0cc12</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, uint32_t remainingLength, uint8_t *pBuffer, size_t *headerSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePublishHeader</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a644f11fd079963d7b77cb2be0e5cf539</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, const MQTTPropBuilder_t *pPublishProperties, uint16_t packetId, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer, size_t *pHeaderSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeAck</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ac5f698b6cf6b3329202b8afb2b310151</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer, uint8_t packetType, uint16_t packetId, const MQTTPropBuilder_t *pAckProperties, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetDisconnectPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a9dd03887f3a2b2c51bad03975a9a3440</anchor>
      <arglist>(const MQTTPropBuilder_t *pDisconnectProperties, uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize, const MQTTSuccessFailReasonCode_t *pReasonCode)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializeDisconnect</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ac9dae825ef229773a87903af5f7da04f</anchor>
      <arglist>(const MQTTPropBuilder_t *pDisconnectProperties, const MQTTSuccessFailReasonCode_t *pReasonCode, uint32_t remainingLength, const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetPingreqPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a9efe92a214584a4334571492041cc9e6</anchor>
      <arglist>(uint32_t *pPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SerializePingreq</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>af3b3e40858fd984c871511e02a61e15d</anchor>
      <arglist>(const MQTTFixedBuffer_t *pFixedBuffer)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializePublish</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a491ba3d7ace4451bd808779779c540b5</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, uint16_t *pPacketId, MQTTPublishInfo_t *pPublishInfo, MQTTPropBuilder_t *propBuffer, uint32_t maxPacketSize, uint16_t topicAliasMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeAck</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a70ccf788bbe591d3ea0ed09f9faf5964</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, uint16_t *pPacketId, MQTTReasonCodeInfo_t *pReasonCode, MQTTPropBuilder_t *pPropBuffer, const MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeConnAck</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a90b1c8adabd6f5109aa1164438bec324</anchor>
      <arglist>(const MQTTPacketInfo_t *pIncomingPacket, bool *pSessionPresent, MQTTPropBuilder_t *pPropBuffer, MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetIncomingPacketTypeAndLength</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a98cdda86f86a0a1888745a584199e930</anchor>
      <arglist>(TransportRecv_t readFunc, NetworkContext_t *pNetworkContext, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ProcessIncomingPacketTypeAndLength</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a94fd3f746074b3f6e16ae6b23dad9a28</anchor>
      <arglist>(const uint8_t *pBuffer, const size_t *pIndex, MQTTPacketInfo_t *pIncomingPacket)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_UpdateDuplicatePublishFlag</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a269f0b0e193a7a10a6e392b88768ae94</anchor>
      <arglist>(uint8_t *pHeader, bool set)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_InitConnect</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad2cb962fb1564e92cb7d931ef1759d5e</anchor>
      <arglist>(MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropertyBuilder_Init</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a1f298e44147e62b8a4cbf735fa4fde57</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint8_t *buffer, size_t length)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateWillProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a7e1952aacd4318c304c79d896835bd06</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateConnectProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a79b75b57edbeb5995da36c48387aca04</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, bool *isRequestProblemInfoSet, uint32_t *pPacketMaxSizeValue)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_SubscriptionId</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a70891c3c8bba3ea43b9e86108aa803ef</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t subscriptionId, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_UserProp</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a942a44cc446e7a1aa1d177947f336aa0</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const MQTTUserProperty_t *userProperty, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_SessionExpiry</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a68a57b8974bb9123001d7048b3b542af</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t sessionExpiry, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ReceiveMax</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a8b98ad82495b9affed0d41e7b9073d2c</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t receiveMax, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_MaxPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a2c908f6cac3ea122d30ee6e172d7a2d2</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t maxPacketSize, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_TopicAliasMax</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a621b90e12ef25b89a1241514dfd26314</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t topicAliasMax, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_RequestRespInfo</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0df00ea718e8acacff979a23e5160db3</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool requestResponseInfo, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_RequestProbInfo</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>af08456a32706f0edc2aba52c2687f06d</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool requestProblemInfo, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_AuthMethod</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a1540b39fc3737b796d9b45420b87cd70</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *authMethod, size_t authMethodLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_AuthData</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad93c187864ab460b2793495d81747eda</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *authData, size_t authDataLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_PayloadFormat</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0eacc61ec8a6cc13ff4d3669b5718672</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, bool payloadFormat, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_MessageExpiry</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ae776d9ed35d16d6fc0e88ffacd36a123</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t messageExpiry, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_WillDelayInterval</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ae951d156cff169b8379575901d8157fb</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint32_t willDelayInterval, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_TopicAlias</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0288cfcf7675dcc9c0f9f5aad5cf7077</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, uint16_t topicAlias, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ResponseTopic</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0013ac6c13f0c5c92af5af561b0b6a16</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *responseTopic, size_t responseTopicLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_CorrelationData</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>abe7a86739faa4d6689f89d1e49a73b5f</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const void *pCorrelationData, size_t correlationLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ContentType</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a9f00d8054888651ff36382d1f0ab09a2</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *contentType, size_t contentTypeLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropAdd_ReasonString</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a67b568e5dd77d4bbab897d2cd61e4699</anchor>
      <arglist>(MQTTPropBuilder_t *pPropertyBuilder, const char *pReasonString, size_t reasonStringLength, const uint8_t *pOptionalMqttPacketType)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateSubscribeProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a269ce3e2203e88608a5205e5a3ddc2e3</anchor>
      <arglist>(bool isSubscriptionIdAvailable, const MQTTPropBuilder_t *propBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>updateContextWithConnectProps</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad0d40d4e24e129696094e9a3f51c4e31</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropBuilder, MQTTConnectionProperties_t *pConnectProperties)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetNextPropertyType</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a31161169ebc4b2e107f24a65d0314eac</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, const size_t *currentIndex, uint8_t *property)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_SkipNextProperty</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a5f21325b7eb8bb49f3b4f5922f59c10c</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_UserProp</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ab0bc946040ace4ca8bef29712e4a8a75</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, MQTTUserProperty_t *pUserProperty)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SessionExpiry</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa6a5c98cf0929d51b83169f6c861bfa0</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pSessionExpiry)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ReceiveMax</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad82a828e6d299166fc6eebcded636f3d</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pReceiveMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MaxQos</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a9999939b9df9ee3748e7536364cde470</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pMaxQos)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_RetainAvailable</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a321201fd94cb0045c175a18d8aab518a</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pRetainAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MaxPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a36d98889d757a8dab491ff735ea2c614</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pMaxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AssignedClientId</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a0ad93d838bf88d1837014d868ec78bf4</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pClientId, size_t *pClientIdLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_TopicAliasMax</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aae3f2e17652e2b46bc004ab61ac0f5d9</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pTopicAliasMax)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ReasonString</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aeeb3fa7a6bb8887036860cdbb7f735f2</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pReasonString, size_t *pReasonStringLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_WildcardId</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aa44abca689de5311a7b9208c97926614</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pWildcardAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SubsIdAvailable</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a03c9fabd0f7be1ef87aafc758c07e847</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pSubsIdAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SharedSubAvailable</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aeb3c649b0b6415619c19dbe3e7c7f264</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pSharedSubAvailable)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ServerKeepAlive</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a28abec58b4f2ebb6e298fd84b7309c46</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pServerKeepAlive)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ResponseInfo</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>abc5ea886e2fb8744e239d88174cbf606</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pResponseInfo, size_t *pResponseInfoLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ServerRef</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ab21f89b33a0d834bde04a81b46bc4920</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pServerRef, size_t *pServerRefLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AuthMethod</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a494a7f3fc59e534f8f156b2c2e11218a</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pAuthMethod, size_t *pAuthMethodLen)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_AuthData</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ad426691e883c67d25b33902374250f74</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pAuthData, size_t *pAuthDataLen)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_PayloadFormatIndicator</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>adf6cf83d3cdcc1ff897aaef2ffa59498</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint8_t *pPayloadFormat)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_MessageExpiryInterval</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a3b8104573b072caed2a550d1d0ca1b92</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pMessageExpiry)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_TopicAlias</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a892a6d571ba311023010322573051fe9</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint16_t *pTopicAlias)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ResponseTopic</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a3282d3f1b5847fd017bd70c760040127</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pResponseTopic, size_t *pResponseTopicLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_CorrelationData</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a57d0b5be13a9062f4d4917a3d030e120</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pCorrelationData, size_t *pCorrelationDataLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_SubscriptionId</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ac66a7eed173968b2a00d7bd29e229444</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, uint32_t *pSubscriptionId)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTTPropGet_ContentType</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a34ca9db573593410637c2cda371d1e41</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder, size_t *currentIndex, const char **pContentType, size_t *pContentTypeLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a162d32c54e27ce976c2a5bec5deed3dc</anchor>
      <arglist>(uint16_t serverTopicAliasMax, const MQTTPropBuilder_t *propBuilder, uint16_t *topicAlias)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishParams</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>ae349b08ac962a464a0f1491748765bd0</anchor>
      <arglist>(const MQTTPublishInfo_t *pPublishInfo, uint8_t retainAvailable, uint8_t maxQos, uint16_t topicAlias, uint32_t maxPacketSize)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidatePublishAckProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aec02145ce3dc1b8745b5ad96de5820cf</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateUnsubscribeProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>aefeb3ff88a39059bd8a598bb63483992</anchor>
      <arglist>(const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_GetAckPacketSize</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a6fa0494e35cba6482aef49898ff4c6dd</anchor>
      <arglist>(uint32_t *pRemainingLength, uint32_t *pPacketSize, uint32_t maxPacketSize, size_t ackPropertyLength)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_ValidateDisconnectProperties</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a5bfe18401ff9848c18729640f7cef6e7</anchor>
      <arglist>(uint32_t connectSessionExpiry, const MQTTPropBuilder_t *pPropertyBuilder)</arglist>
    </member>
    <member kind="function">
      <type>MQTTStatus_t</type>
      <name>MQTT_DeserializeDisconnect</name>
      <anchorfile>core__mqtt__serializer_8h.html</anchorfile>
      <anchor>a73f6ec7ed9e222865a35cdd6d03ea9ae</anchor>
      <arglist>(const MQTTPacketInfo_t *pPacket, uint32_t maxPacketSize, MQTTReasonCodeInfo_t *pDisconnectInfo, MQTTPropBuilder_t *pPropBuffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_state.h</name>
    <path>source/include/</path>
    <filename>core__mqtt__state_8h.html</filename>
    <includes id="core__mqtt_8h" name="core_mqtt.h" local="yes" imported="no">core_mqtt.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_STATE_CURSOR_INITIALIZER</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga666ad78e7eaaffa51f5cab96201a9476</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>size_t</type>
      <name>MQTTStateCursor_t</name>
      <anchorfile>group__mqtt__basic__types.html</anchorfile>
      <anchor>ga2ca7d486d83fe555953a8c7876ee0d6e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>MQTT_PublishToResend</name>
      <anchorfile>core__mqtt__state_8h.html</anchorfile>
      <anchor>a44b3cf50dc477a9f97413a9238a961f6</anchor>
      <arglist>(const MQTTContext_t *pMqttContext, MQTTStateCursor_t *pCursor)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_mqtt_serializer_private.h</name>
    <path>source/include/private/</path>
    <filename>core__mqtt__serializer__private_8h.html</filename>
    <includes id="core__mqtt__serializer_8h" name="core_mqtt_serializer.h" local="yes" imported="no">core_mqtt_serializer.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIPTION_ID_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a244f6f3a77139d1ff13e1fb2c737caf1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SESSION_EXPIRY_INTERVAL_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a5c39a6cbd1e010d54426c4374d9df977</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RECEIVE_MAXIMUM_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ad42fcef2928f85f7930f8fb21ff08c4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_PACKET_SIZE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a1bb69f8dabc7d924f44d8ec26b0cc8c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_TOPIC_ALIAS_MAX_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a798ffefa808fae297a73190252092f9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REQUEST_RESPONSE_INFO_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ae19b0cd820f1bf3d9ff3ba4aae8abb06</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REQUEST_PROBLEM_INFO_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a553abd40d7bd84ab3276683967368875</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_AUTHENTICATION_METHOD_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a808e34151fed55fb4190fd2a4f3decb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_AUTHENTICATION_DATA_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>acfd4605e9d19308eab0796862918ba75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PAYLOAD_FORMAT_INDICATOR_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>acb0c25196858a4d9ba920fb24a14033d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MESSAGE_EXPIRY_INTERVAL_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a59f5b4d203569d3fde99ee4438be7ab4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_TOPIC_ALIAS_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a1aacf46037c3058099e7766488c4248a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RESPONSE_TOPIC_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a0bfb8a9178a259f948c55ae747ac041b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CORRELATION_DATA_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>abd9c8a97bc07727e410ea6ea285ab889</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONTENT_TYPE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a20e4de68f9ca86b85bc87f643f622b61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REASON_STRING_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a4e62361869698201ab19e0e13f8a1552</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_WILL_DELAY_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ae93b83901589e79aedee599adb9a2437</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_ASSIGNED_CLIENT_ID_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ada2b0b4b4a9a1d22bacca31651fe8e09</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SERVER_KEEP_ALIVE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a69f75e93a32b71acdd71f2d16d95037e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RESPONSE_INFORMATION_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a20193809b884059cc380c2673ba81170</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SERVER_REFERENCE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a348463b6d2e9104fda55c1c2be2eaf55</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_QOS_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a19c48ffb00e790a787328c31d5e29f59</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_RETAIN_AVAILABLE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a5249cb513b0413466e34faa22863d197</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_WILDCARD_SUBSCRIPTION_AVAILABLE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a8381c304ca45c70f5c52364e1514e976</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUBSCRIPTION_ID_AVAILABLE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a7199564ede60d29c99dc000ed8d640bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SHARED_SUBSCRIPTION_AVAILABLE_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a1e0b27fd05f0bfe7f8dc56b2973f82c7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_USER_PROP_POS</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ab04bd6c77b1e648d5bdbc81c3c35686d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_CLEAN</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a1b131e766e003e36fe499d9f6a79fc03</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_WILL</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a04d8c55ea2b595a277cbcd4340e36d6c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_WILL_QOS1</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a2aee739b1fa7e61feb907bc92a73c3b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_WILL_QOS2</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ac750789b338a2b9be75725ab340dabce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_WILL_RETAIN</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a8ae294d4ca7960920816339fedbdc4a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_PASSWORD</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ac5f0bb47789c1182392f5029e0238a81</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_CONNECT_FLAG_USERNAME</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a8d23d14a4cf296feffb9db79728dd1d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_DECODE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a17ed1ce0de61ab9f36bbcbe66486245c</anchor>
      <arglist>(ptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WRITE_UINT32</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ab842a87bcd94fd85f394b2b687118526</anchor>
      <arglist>(addr, val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_SET_BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>af259c91b3075c24df53fa3ffe516b208</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_CLEAR_BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a549d24726d8ff1b4b32a6cb48654ad90</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_CHECK_BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a07cc5f3f934e1ebf8011a6c15a667206</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_HIGH_BYTE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a24aab781ef139dd38be534ee137ea2f9</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_LOW_BYTE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>af2ae35b27e0140a77238cd175508cb4e</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_DECODE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>acc849aa739edff3ec532219a3860a3a0</anchor>
      <arglist>(ptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_SET_BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>aa7efbea5e18e04f3ccedeb0f31342b22</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_CHECK_BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ad1efde8b09ebf3852dea527ac4a2f538</anchor>
      <arglist>(x, position)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_REMAINING_LENGTH</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a9d9ea40a1ff486557a553523a0743647</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_REMAINING_LENGTH_INVALID</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a6f6b43661df6f9e9e9e7123ab01e9eb5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_PACKET_SIZE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ae09b594688a59f1427c7e45259e039b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_MAX_UTF8_STR_LENGTH</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a039589db800a34e144426f7f5e0d08d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_VARIABLE_LENGTH_INT_VALUE</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a45496cdd6f28e9ebdfd2fd1b5e0ea813</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHECK_U32T_OVERFLOWS_SIZE_T</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>aab25f8e1aef38f0bad37a4b50aeb4fe9</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHECK_SIZE_T_OVERFLOWS_16BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a54e84d173cf1917e1db2b3ec7d6ddc84</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHECK_SIZE_T_OVERFLOWS_32BIT</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>a18de3db6e4c408ce4e235f67c8b3e6fc</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDITION_WILL_OVERFLOW_U32</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>af9dc8d18062c5d202def971e7ea7fde8</anchor>
      <arglist>(x, y)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDITION_WILL_OVERFLOW_SIZE_T</name>
      <anchorfile>core__mqtt__serializer__private_8h.html</anchorfile>
      <anchor>ae9f065fa00c12023ad2b8916fa4d5153</anchor>
      <arglist>(x, y)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>transport_interface.h</name>
    <path>source/interface/</path>
    <filename>transport__interface_8h.html</filename>
    <class kind="struct">TransportOutVector_t</class>
    <class kind="struct">TransportInterface_t</class>
    <member kind="typedef">
      <type>struct NetworkContext</type>
      <name>NetworkContext_t</name>
      <anchorfile>group__mqtt__struct__types.html</anchorfile>
      <anchor>ga7769e434e7811caed8cd6fd7f9ec26ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportRecv_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga227df31d6daf07e5d833537c12130167</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, void *pBuffer, size_t bytesToRecv)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportSend_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga2a39853ff952edd715ab07b33ab2a7c5</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, const void *pBuffer, size_t bytesToSend)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportWritev_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga47e779557b0c2db95949ef9526861dfb</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, TransportOutVector_t *pIoVec, size_t ioVecCount)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ConnackSeenFlags_t</name>
    <filename>struct_connack_seen_flags__t.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>sessionExpiry</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a958f3c794781bd4f94dd6b2e1443a67b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>receiveMax</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a20c48c5eb146dc895df6e3a838339614</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>maxQos</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a8a16d4b14748953c66b9bf91368cbe42</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>retain</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a5bbec471e46b5b0c4de8e6a0ace5797b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>maxPacket</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a2a80bf96f74faedbc2ca0a4c76a9d573</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>clientId</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>ac88943992228691d1a048d9fae049bfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>topicAlias</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a4cc5dbb078c1f8e0d225be9893df3b5a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>wildcard</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a9340a9ddd75f65fa2794960e9ef12524</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>subId</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>ab5203674bfdf2c3c17bade28c01fc3ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>sharedSub</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>ab3ac197e34eb75415c62df5aba5be083</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>keepAlive</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a5cb3cdd7433b9049c37b5aa31f98709f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>responseInfo</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a008f2307886bbeed0153bf862ce478a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>serverRef</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a61a8488cfde72f441594744425a43618</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>authMethod</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a505911de06dc97ad0e69f1926ab705d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>authData</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a327d7f2977c9ed45475200631b8e92c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>reasonString</name>
      <anchorfile>struct_connack_seen_flags__t.html</anchorfile>
      <anchor>a1217d753e8e5bfefc0572fd9a3aaf882</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>IoVecState_t</name>
    <filename>struct_io_vec_state__t.html</filename>
    <member kind="variable">
      <type>TransportOutVector_t *</type>
      <name>pIterator</name>
      <anchorfile>struct_io_vec_state__t.html</anchorfile>
      <anchor>a8cacfc6b2c90d47463c49d59241bd263</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>ioVectorLength</name>
      <anchorfile>struct_io_vec_state__t.html</anchorfile>
      <anchor>ae35f6fc09530c952d1c7db9355cdbfb8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>totalMessageLength</name>
      <anchorfile>struct_io_vec_state__t.html</anchorfile>
      <anchor>a863cb9f4a254e7301cc7c32ad9ab69d3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTConnectInfo_t</name>
    <filename>struct_m_q_t_t_connect_info__t.html</filename>
    <member kind="variable">
      <type>bool</type>
      <name>cleanSession</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a606e7765c4f2215fb2bf630f6eb9ff6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>keepAliveSeconds</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a7d05d53261732ebdfbb9ee665a347591</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pClientIdentifier</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a010f8f6993cbf8899648d5c515ff7884</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>clientIdentifierLength</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a0c68759c2b85bb4d2815c61476196631</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pUserName</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a1118d7d3251a11445318557280db53b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>userNameLength</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>a7ce1cf073a36e29748c79b712de5e5bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pPassword</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>acec6c79a11d2f0f130802393f34d2b5e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>passwordLength</name>
      <anchorfile>struct_m_q_t_t_connect_info__t.html</anchorfile>
      <anchor>adf9b33cb9fe6479338813578673cf605</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTConnectionProperties_t</name>
    <filename>struct_m_q_t_t_connection_properties__t.html</filename>
    <member kind="variable">
      <type>uint32_t</type>
      <name>sessionExpiry</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>ac7c25d5c5a9538184437879bb05281a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>receiveMax</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a88d862e8132699a3520cfcda2e77c1c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>maxPacketSize</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>ad647e802a7aecda1394634f0c8e45e7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>topicAliasMax</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>ac199a8efa5d695c3b8913d448f4998d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>requestResponseInfo</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a0f66bbb49942cbaf8bf0ee52384467d8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>requestProblemInfo</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a2c8472adb537d9b6857da6c7803b0ef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>serverReceiveMax</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>add994c9e0f9c3993960bd35496486e04</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>serverMaxQos</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a3123b62b6ebae1b8183f592c041a0b4a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>retainAvailable</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a94a9fdef74b690f76f889966fcfed8c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>serverMaxPacketSize</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>aae7d346ecae1a5dd809784d2d34aebbc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>serverTopicAliasMax</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a28b3da2cb05b3b9116204ded988831e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isWildcardAvailable</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>ad726fc175701ac8b1d6f2c9c67f648b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isSubscriptionIdAvailable</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a2c8483c67a129e1d662a845191e5d624</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>isSharedAvailable</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>a2b5401c1665c3973de6afeb2c22dcb1c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>serverKeepAlive</name>
      <anchorfile>struct_m_q_t_t_connection_properties__t.html</anchorfile>
      <anchor>aa9567f22d2dc102b99893d619d7bebcc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTContext_t</name>
    <filename>struct_m_q_t_t_context__t.html</filename>
    <member kind="variable">
      <type>MQTTPubAckInfo_t *</type>
      <name>outgoingPublishRecords</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a4ea1e37e0e81f010fbf84365ac2ef6de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTPubAckInfo_t *</type>
      <name>incomingPublishRecords</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>afc147663a5933de81212fa77057f0a4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>outgoingPublishRecordMaxCount</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a2851073e252d1e744596272ef13dd14a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>incomingPublishRecordMaxCount</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>aa33ed2e10380a854629f1386d0323ea8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TransportInterface_t</type>
      <name>transportInterface</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a87ab9d61e7711325c2c85ce3ce63386a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTFixedBuffer_t</type>
      <name>networkBuffer</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a231c5576a6ce389317a3f00f95628276</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTPropBuilder_t</type>
      <name>ackPropsBuffer</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>acc6db180acbc7cc7b1ef7c2f5f553631</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>nextPacketId</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>af47ed55ad7e9bb112324f5f209b70534</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTConnectionStatus_t</type>
      <name>connectStatus</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a4e38c4dc77e7751a0ad8730a41bee47f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTGetCurrentTimeFunc_t</type>
      <name>getTime</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>aabe1d302a16771292151013e8e30c582</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTEventCallback_t</type>
      <name>appCallback</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a73bd9259db9c3a9b84518cbf928ed91f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>lastPacketTxTime</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a01acf90953e830ba3e7f44182cb1d482</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>lastPacketRxTime</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a7111ef16e4a4e75a72861f6f3ea8a7c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>controlPacketSent</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>af9724f2426132e3ce96a03892902ef89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>index</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a41b7735cd0746563483b72e17cf103aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>keepAliveIntervalSec</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>afd6071827ef48b230212a5725c2075be</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>pingReqSendTimeMs</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>acca3efa4146d85f7e874c7c326e23556</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>waitingForPingResp</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>ac7073f43645f7b7c0c5b7763980004bb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTConnectionProperties_t</type>
      <name>connectionProperties</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a9914b8259063a3ee3b77b90de0ad6507</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTStorePacketForRetransmit</type>
      <name>storeFunction</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>ac205fc33078b0cfa0dfc62807bd9574f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTRetrievePacketForRetransmit</type>
      <name>retrieveFunction</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a270e9e595ecd60948c960d059f8fe718</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTClearPacketForRetransmit</type>
      <name>clearFunction</name>
      <anchorfile>struct_m_q_t_t_context__t.html</anchorfile>
      <anchor>a36e3d500e6efa050d6e666aefa3ff577</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTDeserializedInfo_t</name>
    <filename>struct_m_q_t_t_deserialized_info__t.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>packetIdentifier</name>
      <anchorfile>struct_m_q_t_t_deserialized_info__t.html</anchorfile>
      <anchor>af4df2a9926a4a68059195daa712d9b84</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTPublishInfo_t *</type>
      <name>pPublishInfo</name>
      <anchorfile>struct_m_q_t_t_deserialized_info__t.html</anchorfile>
      <anchor>ac347273fae1e92b9cbeda1714066c1de</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTStatus_t</type>
      <name>deserializationResult</name>
      <anchorfile>struct_m_q_t_t_deserialized_info__t.html</anchorfile>
      <anchor>a7df1b7b60404c9f1604fec0081d2625d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTReasonCodeInfo_t *</type>
      <name>pReasonCode</name>
      <anchorfile>struct_m_q_t_t_deserialized_info__t.html</anchorfile>
      <anchor>aa6e6ca63ae46b306dd3d304f6f564c29</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTFixedBuffer_t</name>
    <filename>struct_m_q_t_t_fixed_buffer__t.html</filename>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>pBuffer</name>
      <anchorfile>struct_m_q_t_t_fixed_buffer__t.html</anchorfile>
      <anchor>acea147448e044870fb36b7fa2347dbd6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>size</name>
      <anchorfile>struct_m_q_t_t_fixed_buffer__t.html</anchorfile>
      <anchor>a0b0b6a93cc62751ebeb03095d5431636</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTPacketInfo_t</name>
    <filename>struct_m_q_t_t_packet_info__t.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>type</name>
      <anchorfile>struct_m_q_t_t_packet_info__t.html</anchorfile>
      <anchor>a7fef40548c1aa0f0e7f812a6a7243758</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>pRemainingData</name>
      <anchorfile>struct_m_q_t_t_packet_info__t.html</anchorfile>
      <anchor>ac66cedff052bc844ec9b296387df60bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>remainingLength</name>
      <anchorfile>struct_m_q_t_t_packet_info__t.html</anchorfile>
      <anchor>a566b295792625e8cf04944d34bdeb538</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>headerLength</name>
      <anchorfile>struct_m_q_t_t_packet_info__t.html</anchorfile>
      <anchor>aa7de1631ed8e08410942d36a72db558a</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTPropBuilder_t</name>
    <filename>struct_m_q_t_t_prop_builder__t.html</filename>
    <member kind="variable">
      <type>uint8_t *</type>
      <name>pBuffer</name>
      <anchorfile>struct_m_q_t_t_prop_builder__t.html</anchorfile>
      <anchor>a7b2fa2be2e3aa4e99fbdc4a797891a34</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>bufferLength</name>
      <anchorfile>struct_m_q_t_t_prop_builder__t.html</anchorfile>
      <anchor>a93627d10e651b2d47a1ea18a9c510a16</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>currentIndex</name>
      <anchorfile>struct_m_q_t_t_prop_builder__t.html</anchorfile>
      <anchor>a5f271ebc88dcae13add5df2ec78a2e0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>fieldSet</name>
      <anchorfile>struct_m_q_t_t_prop_builder__t.html</anchorfile>
      <anchor>a6cc81cd8ebc111cdfbb2c736482d278f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTPubAckInfo_t</name>
    <filename>struct_m_q_t_t_pub_ack_info__t.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>packetId</name>
      <anchorfile>struct_m_q_t_t_pub_ack_info__t.html</anchorfile>
      <anchor>a66cef7b43af5d7fdd33b5d2dc766b2d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTQoS_t</type>
      <name>qos</name>
      <anchorfile>struct_m_q_t_t_pub_ack_info__t.html</anchorfile>
      <anchor>a086fcd48ef0b787697526a95c861e8a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTPublishState_t</type>
      <name>publishState</name>
      <anchorfile>struct_m_q_t_t_pub_ack_info__t.html</anchorfile>
      <anchor>a61314203ef87a231c6489c68b579de34</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTPublishInfo_t</name>
    <filename>struct_m_q_t_t_publish_info__t.html</filename>
    <member kind="variable">
      <type>MQTTQoS_t</type>
      <name>qos</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>a178224d02b4acdec7e08e88de0e4b399</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>retain</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>a343b0af89c46a900db4aa5c775a0975a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>dup</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>aa1c8954e83bfa678d1ff5429679d4e89</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pTopicName</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>aa80e8ca282d01630f878ad0afe81d7a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>topicNameLength</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>a865573b203c848cc28cdf955b7028629</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const void *</type>
      <name>pPayload</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>afc28299f4f625f5e674bb61b42f03380</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>payloadLength</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>a7997964e11571f35f0c3b729db0f760f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>propertyLength</name>
      <anchorfile>struct_m_q_t_t_publish_info__t.html</anchorfile>
      <anchor>aaf01d089ad9db1b041015715071942f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTReasonCodeInfo_t</name>
    <filename>struct_m_q_t_t_reason_code_info__t.html</filename>
    <member kind="variable">
      <type>const uint8_t *</type>
      <name>reasonCode</name>
      <anchorfile>struct_m_q_t_t_reason_code_info__t.html</anchorfile>
      <anchor>a2b61cf1ee4caa1865a54d47da2e76477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>reasonCodeLength</name>
      <anchorfile>struct_m_q_t_t_reason_code_info__t.html</anchorfile>
      <anchor>a1476b796abb5bd5acf218a50c3ff53ac</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTSubscribeInfo_t</name>
    <filename>struct_m_q_t_t_subscribe_info__t.html</filename>
    <member kind="variable">
      <type>MQTTQoS_t</type>
      <name>qos</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>a64cf2e423f60cfec122eeaef80c0fd86</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pTopicFilter</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>adb0b28240fdcd82a85f11cf2f8b5bbf0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>topicFilterLength</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>a5849d6d2b87a8bcef894cb92bddc9d14</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>noLocalOption</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>a9a417e8150a741cfe92e7176209d326c</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>retainAsPublishedOption</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>a4ea94e01ac69e4744710bb50fdebe503</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MQTTRetainHandling_t</type>
      <name>retainHandlingOption</name>
      <anchorfile>struct_m_q_t_t_subscribe_info__t.html</anchorfile>
      <anchor>a65c895ef9d14fb6202d07d4e8e5d1073</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTUserProperty_t</name>
    <filename>struct_m_q_t_t_user_property__t.html</filename>
    <member kind="variable">
      <type>const char *</type>
      <name>pKey</name>
      <anchorfile>struct_m_q_t_t_user_property__t.html</anchorfile>
      <anchor>aaaea6defd6ebdde0e7f92f957077e0ee</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>keyLength</name>
      <anchorfile>struct_m_q_t_t_user_property__t.html</anchorfile>
      <anchor>a5d5212b909aa8e0776f8eb7fab92127f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const char *</type>
      <name>pValue</name>
      <anchorfile>struct_m_q_t_t_user_property__t.html</anchorfile>
      <anchor>a42f2cccdff31cac6a881b6efd0a88efb</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>valueLength</name>
      <anchorfile>struct_m_q_t_t_user_property__t.html</anchorfile>
      <anchor>ac00547315a3542befae9ee06c68047e9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>MQTTVec</name>
    <filename>struct_m_q_t_t_vec.html</filename>
    <member kind="variable">
      <type>TransportOutVector_t *</type>
      <name>pVector</name>
      <anchorfile>struct_m_q_t_t_vec.html</anchorfile>
      <anchor>a35ba714afe80db56d4346949ce635a5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>vectorLen</name>
      <anchorfile>struct_m_q_t_t_vec.html</anchorfile>
      <anchor>a53b13c310696095b7f1a5254e75552fb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TransportInterface_t</name>
    <filename>struct_transport_interface__t.html</filename>
    <member kind="variable">
      <type>TransportRecv_t</type>
      <name>recv</name>
      <anchorfile>struct_transport_interface__t.html</anchorfile>
      <anchor>a7c34e9b865e2a509306f09c7dfa3699e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TransportSend_t</type>
      <name>send</name>
      <anchorfile>struct_transport_interface__t.html</anchorfile>
      <anchor>a01cd9935e9a5266ca196243a0054d489</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>TransportWritev_t</type>
      <name>writev</name>
      <anchorfile>struct_transport_interface__t.html</anchorfile>
      <anchor>a8cf677fbeee53d270daa6dacfa138b79</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>NetworkContext_t *</type>
      <name>pNetworkContext</name>
      <anchorfile>struct_transport_interface__t.html</anchorfile>
      <anchor>aaf4702050bef8d62714a4d3900e95087</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>TransportOutVector_t</name>
    <filename>struct_transport_out_vector__t.html</filename>
    <member kind="variable">
      <type>const void *</type>
      <name>iov_base</name>
      <anchorfile>struct_transport_out_vector__t.html</anchorfile>
      <anchor>a0ffa5c06bf00006cbafa8e244951038d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>iov_len</name>
      <anchorfile>struct_transport_out_vector__t.html</anchorfile>
      <anchor>ada73dafb2d34311f33fefad38603b35c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>mqtt_enum_types</name>
    <title>Enumerated Types</title>
    <filename>group__mqtt__enum__types.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>MQTTConnectionStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga9f84d003695205cf10a7bd0bafb3dbf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNotConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a0320177ebf1f1b2e24646b44702cec69</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a82c8f64d976734e5632e5257bc429ef5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTDisconnectPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga9f84d003695205cf10a7bd0bafb3dbf6a3c7e17f463f44d8f7ce50475b129ac24</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTPublishState_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga0480de7552eedd739a26a23fa8e6fd94</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStateNull</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a8349567b7a9efb3913a64a8f4f6fe5c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a896b1507647b504c9208580e4cde26ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubAckSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a65f6f7b343a30fc0558e3aeeb8c97f35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRecSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a11e2319a2b25b82121471743d39761e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRelSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a5d2ee2709c6dc7a1eb8b9c40f318909b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubCompSend</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a7d88904d550b502b4424a41aa4205e56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubAckPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94ab086c55acf106cdc8d420f90899b6803</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRecPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a1bea59454700be9b683b7eb8aaf6bb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubRelPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a695431cde1dc9dc5a2dcbd10eba49df2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubCompPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94a3281a28d1829d954b596f091b547b627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishDone</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga0480de7552eedd739a26a23fa8e6fd94ad07733793a235ef9a6a04d16637cd7dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTPubAckType_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga8c1bee959b3ed5fab2a2688dd72bf237</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPuback</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a53d5939c680962f37c15ee87ffd63d0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubrec</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a8c98d5d1a68dda33d9039009ab4ef053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubrel</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237af2d737088a231c88e7603acfdbc4fc8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPubcomp</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga8c1bee959b3ed5fab2a2688dd72bf237a910c34311ad6a2341afc04839e1c13bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSubAckStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga48dabc1579e3c0ac6058ce9068054611</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611abcc3040d7d53025baee3542c40758abb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611ab180361a6da712c8144d8c499537787d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckSuccessQos2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611a877b2afbc6ec7d9ab57d4862caadf4f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSubAckFailure</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga48dabc1579e3c0ac6058ce9068054611aeb83b20da8eda934cde6b92db225a808</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTStatus_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gaba7ec045874a1c3432f99173367f735c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSuccess</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca484e062cb4f3fccc1858dd25cfeee056</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTBadParameter</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa39030c93b0263b2699502a074f003b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNoMemory</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735cab1be4db832a0468f024243bca151a8df</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTSendFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735cafd06b63fe9677fa2af06b0f4c7d4ad55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTRecvFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa14bc8aa4ad218702d782366945d43ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTBadResponse</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa5d7507e7664a14d63a8bc44b280093e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTServerRefused</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca25a3d1747e308e99daa805fe576f84b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNoDataAvailable</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca676f21c0ddf297ae3ec874bc829aa957</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTIllegalState</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca67905d7a05f98faa557a73eb5092bd8f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStateCollision</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca8d05b92240dea6df08eab5a9e3799c11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTKeepAliveTimeout</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca076ca8965e836a06e707a94adb26144f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTNeedMoreBytes</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa97df53014d919df5ecd54398f89f9b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTEndOfProperties</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caeb817a941b956ae02f011976fa1ce645</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735caa13a678a441c9ef6b88d6d8be8a64090</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusNotConnected</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca074273b04f70b93c1f20702096cac273</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTStatusDisconnectPending</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca230baa3eaabf50e6b319f792a82bb863</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishStoreFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca61a3e19715d8ee1ee330c9bbd4b2708a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTPublishRetrieveFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca274b61e8a2aea2cf551353c6d09a6eb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTEventCallbackFailed</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggaba7ec045874a1c3432f99173367f735ca9a19fd9b3228ebcdf2a461c97e53d580</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTQoS_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gae308a5928d7f537379c29a894228093a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aaad51b23a1ae1417f96d8f343c788d1d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aa019d0b8a8cfadb6f98462b046bdacbb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTTQoS2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ggae308a5928d7f537379c29a894228093aa85e04ac0465cbdef6dd69ff71b2bbfbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTRetainHandling_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga276a86010075cf64d522fe56cabee1e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainSendOnSub</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3ad87d133aa73a39124fa12597b9d7c449</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainSendOnSubIfNotPresent</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3a24a986f1b6e7f71f08756fc9e170743b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>retainDoNotSendonSub</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga276a86010075cf64d522fe56cabee1e3a83c31ac46bb92684ee74579b7088a8b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSuccessFailReasonCode_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga377edfeeb661cf054a3c7c598637450e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaec27e57d19af296e4bb5d8f90341fa3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_NO_MATCHING_SUBSCRIBERS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea73b7865fc7a3270a164578ddb64ad8d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9a9d5864934d095d93f67161f81595d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea82e0d55e321db001275fee139d577191</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9c8370fe6dccaac60b16231547d5b6a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8b17f877c0fdb9c9335eac60a9dbf22c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafc2774d718797370f5906e7b20539aac</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea05c1966ba2278d44e83b9ddc85837643</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBACK_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea932a871c8527003e79f798057761bc55</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8d1f682d9f527fab38d402de4db8733d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_NO_MATCHING_SUBSCRIBERS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5230510b5f7cfd96c6ea2aa9a0b493f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea168512c03686ee4050ea35d0525ff6bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa414c4457e344718201c7123cfc5f13d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea913d3524748b2a666d3b7f76de6c9c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea1772f8237e7aae6721795c723bbac7d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea09b6d2b1f88e3c37ae65d6c18c875d73</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea21b7b0bd8de50f4fe60dabc4147a3635</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREC_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9ad53a4155b45aa8caef03161da252c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREL_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea160a49e8cdf0c3057c1b62b91140da81</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBREL_PACKET_IDENTIFIER_NOT_FOUND</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea2adc444a76d72ba1d82e49d2f76898f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBCOMP_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea30200593b5428a3fde25fff1bca99ccd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_PUBCOMP_PACKET_IDENTIFIER_NOT_FOUND</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3b21320822eb0c9523e2c6933a15a436</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa05642f2d6bcee441d2110eb02806b14</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3fcfe2653d5beddda324c84054b6f307</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_MALFORMED_PACKET</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eadfca66707d73f1f7a29873f2c560318c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PROTOCOL_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5523e76b618f2b5825a6f9903ca737c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9e2ee2a9c4cd76850c5c57ebe1b934c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_UNSUPPORTED_PROTOCOL_VERSION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea19d7ac951f3bcca5cd180a38296d72d0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_CLIENT_IDENTIFIER_NOT_VALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea2080f224b87c2c29bccc710c7db5bf82</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BAD_USER_NAME_OR_PASSWORD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafc3f87d6a108fd648cb7c85850fa7cf5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea55c5e62fab7ed193a2a2e958828439e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_UNAVAILABLE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4af7f1f540e1a273935ad68b863049ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_BUSY</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaf5f487d1e6f66ce57d72e6d4dbfe712d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BANNED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae3b5b13fbe11aa24e05eda01e5df3f6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_BAD_AUTHENTICATION_METHOD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaa0caacb28142c9d425fd8bc3a916d97</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea273cba3efdf86e3fe73a2c8d977da2aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PACKET_TOO_LARGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7b20c7314226847850f87c099f5ce259</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab5f6a48c02fc7a9472bdbadd2d09507e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea04ae6a169ed488fc07199e59e500aa36</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_RETAIN_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaad9d9fa043c9031989ca258ed76d92c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_QOS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead0dd365babab6299360409c9b8f5ec4b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_USE_ANOTHER_SERVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea9603c89aa2e2043199b83ef04d21e300</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_SERVER_MOVED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaae65c001ed16cde6e73c90ae51a52e31</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_CONNACK_CONNECTION_RATE_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea18febf18ff0950ac7d75a1bf2e0beb4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS0</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea691755049d5fc3165296ac233d907a4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS1</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaef376fb954f8b54ff99d86cb4d50c11</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_GRANTED_QOS2</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae2263fb8bdb2b367a55baebd4d90a1a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaba8aba7d1f6dad09ed7078e33453ec50</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaff2be32d18beea90566c7fccb11db5ff</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea823864d4817117a7b918c759d96f53bd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea59a194884e3e1abccd33c2aaa11d8378</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea5e8eedbc1c09584c2741fc5e3f086a81</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaeef90d33af2613da0a0967982ad9053</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae67171661c2dd9e03c10135e70a7b2b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea41a9e6abaa661b7fbf3c6dc0617afb0a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_SUBACK_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eae291e9ec840e39f3b970daf3f8447b38</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_SUCCESS</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea34f4d0fe2dcef18f535f172c846f06e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_NO_SUBSCRIPTION_EXISTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea533c4cff4a264052cfdaaf7b8cfce0a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaedc0af01a1ebcd3c260f44aa71d82935</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea1fd2b23ad03605ee3f8f6acf9550f0c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafdd2cd9e4261858d07236e91e67380c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3dc70aeec66055f198326693a55d1e9b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_UNSUBACK_PACKET_IDENTIFIER_IN_USE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7a8d20d9fe656ce1bbb2cfb3ecf08db8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_NORMAL_DISCONNECTION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea55edb84a47cdda9f150a12bfd2e38db1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_DISCONNECT_WITH_WILL_MESSAGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaaa11f277027410058166f75e0d3dbacd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_UNSPECIFIED_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea836695bd488cd356386ec63e20728a7e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MALFORMED_PACKET</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea228092ce6e3a800f8bb7a5ee49297fa0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PROTOCOL_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea8a9d508be1ca7eba456b79b3249beff6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_IMPLEMENTATION_SPECIFIC_ERROR</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea26b9d9b87e78ae31feacbeec1fc68ce0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_NOT_AUTHORIZED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead4542bb94fa87e1daa29ca5609d671fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_BUSY</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab4cdb5cb419222a0e5ff587dcb5eb17f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_SHUTTING_DOWN</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab082c579f274ae698dcc3c91a896f8d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_BAD_AUTHENTICATION_METHOD</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead4edae719818611fc3db6063ab4e4105</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_KEEP_ALIVE_TIMEOUT</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea56f178ce8bc66bebe20b5d414f795695</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SESSION_TAKEN_OVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea562bc2b776339a525922f14024cca3fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_FILTER_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea5e64bb43f132a39d0fe5af25a99ea420</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_NAME_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eabe648c943a525c291a2f4a1eee5315d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_RECEIVE_MAXIMUM_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7f7163334d5377850cd658049fd4183b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_TOPIC_ALIAS_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa1bbbe637efa97e740141a357507fb04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PACKET_TOO_LARGE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eadd81177a4addf111b27e1dfdcfc7f328</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MESSAGE_RATE_TOO_HIGH</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaef7a88988105ebb7baaf9f5924b26897</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_QUOTA_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eafe62edd931be084a0a40a60039744f87</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_ADMINISTRATIVE_ACTION</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4d2cc1ccbb197ebe3ced7718ae002237</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_PAYLOAD_FORMAT_INVALID</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea06eaa2d01ace739d436800ec4cadf437</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_RETAIN_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eab490ac58760a290b109929691931674d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_QOS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa95ba7d7fc079d39632477a9776e467e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_USE_ANOTHER_SERVER</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea440082b1d151728ca496c1624f77ea4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SERVER_MOVED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea7b4ba5aa35ca01bc22012d2b2cfc3cd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SHARED_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ead41f26fd3217575342fe35da3ec17d99</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_CONNECTION_RATE_EXCEEDED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea4beedd8b2cdb30d03f0651d852fbbbd9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_MAXIMUM_CONNECT_TIME</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea3fdbd45bd2c7955b8ed3aa441c5e0dca</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_SUBSCRIPTION_IDENTIFIERS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea173f54ae8b2e495b94ae0431e96590ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_REASON_DISCONNECT_WILDCARD_SUBSCRIPTIONS_NOT_SUPPORTED</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450ea944e2976e661db8f7831cec36cdd731c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_INVALID_REASON_CODE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga377edfeeb661cf054a3c7c598637450eaa174c5b038a43f24722fc1bf35bb3b05</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>MQTTSubscriptionType_t</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>ga92e39b92b76d439a183fc6f5e300195f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_TYPE_SUBSCRIBE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga92e39b92b76d439a183fc6f5e300195fafc8760ca08fcb6c2011be8660cf539d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>MQTT_TYPE_UNSUBSCRIBE</name>
      <anchorfile>group__mqtt__enum__types.html</anchorfile>
      <anchor>gga92e39b92b76d439a183fc6f5e300195fa91f5a06482f72290b846ffd16f0a4523</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>mqtt_callback_types</name>
    <title>Callback Types</title>
    <filename>group__mqtt__callback__types.html</filename>
    <member kind="typedef">
      <type>uint32_t(*</type>
      <name>MQTTGetCurrentTimeFunc_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>gae3bea55b0e49e5208b8c5709a5ea23aa</anchor>
      <arglist>)(void)</arglist>
    </member>
    <member kind="typedef">
      <type>bool(*</type>
      <name>MQTTEventCallback_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga5ff18bbacb958f5973e7bc4ab035f98b</anchor>
      <arglist>)(struct MQTTContext *pContext, struct MQTTPacketInfo *pPacketInfo, struct MQTTDeserializedInfo *pDeserializedInfo, enum MQTTSuccessFailReasonCode *pReasonCode, struct MQTTPropBuilder *pSendPropsBuffer, struct MQTTPropBuilder *pGetPropsBuffer)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportRecv_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga227df31d6daf07e5d833537c12130167</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, void *pBuffer, size_t bytesToRecv)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportSend_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga2a39853ff952edd715ab07b33ab2a7c5</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, const void *pBuffer, size_t bytesToSend)</arglist>
    </member>
    <member kind="typedef">
      <type>int32_t(*</type>
      <name>TransportWritev_t</name>
      <anchorfile>group__mqtt__callback__types.html</anchorfile>
      <anchor>ga47e779557b0c2db95949ef9526861dfb</anchor>
      <arglist>)(NetworkContext_t *pNetworkContext, TransportOutVector_t *pIoVec, size_t ioVecCount)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>mqtt_struct_types</name>
    <title>Parameter Structures</title>
    <filename>group__mqtt__struct__types.html</filename>
    <class kind="struct">MQTTPubAckInfo_t</class>
    <class kind="struct">MQTTContext_t</class>
    <class kind="struct">MQTTDeserializedInfo_t</class>
    <class kind="struct">MQTTFixedBuffer_t</class>
    <class kind="struct">MQTTConnectInfo_t</class>
    <class kind="struct">MQTTSubscribeInfo_t</class>
    <class kind="struct">MQTTPublishInfo_t</class>
    <class kind="struct">MQTTPacketInfo_t</class>
    <class kind="struct">MQTTPropBuilder_t</class>
    <class kind="struct">MQTTReasonCodeInfo_t</class>
    <class kind="struct">MQTTConnectionProperties_t</class>
    <class kind="struct">MQTTUserProperty_t</class>
    <class kind="struct">TransportInterface_t</class>
    <member kind="typedef">
      <type>struct NetworkContext</type>
      <name>NetworkContext_t</name>
      <anchorfile>group__mqtt__struct__types.html</anchorfile>
      <anchor>ga7769e434e7811caed8cd6fd7f9ec26ec</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>mqtt_basic_types</name>
    <title>Basic Types</title>
    <filename>group__mqtt__basic__types.html</filename>
    <member kind="typedef">
      <type>size_t</type>
      <name>MQTTStateCursor_t</name>
      <anchorfile>group__mqtt__basic__types.html</anchorfile>
      <anchor>ga2ca7d486d83fe555953a8c7876ee0d6e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>mqtt_constants</name>
    <title>Constants</title>
    <filename>group__mqtt__constants.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_ID_INVALID</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga9fde6503edb9eaad50ecd3392ab9992a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_SUB_UNSUB_MAX_VECTORS</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga928ea0bff12ebf9cf9fa9dfe5cafebbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_CONNECT</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga64a0515bda2ecc89e97595535e1cf0ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_CONNACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gab14f6c39c303eac1a76816edfde7feab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBLISH</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga5b2d79c61f2591c8e5772f974826d4ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga5f279d63de47a973b41b95f74f47a4f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBREC</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gafa2d8f28da39560f152076b99610e6a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBREL</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gaeaa2ceecffda50e2d22ccecf046083c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PUBCOMP</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga478ecbc98d2ca83d4ce7db33622b5c3b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_SUBSCRIBE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga80cfef333e60d967ca927b2e5e665f18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_SUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga307e0186aa17fdd0d6d181d3d2715766</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_UNSUBSCRIBE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga4a94c954cfcea31c8fc3e2adf092b228</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_UNSUBACK</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga38bc8ed0b9a1581cf85cecdede7d1a64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PINGREQ</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gacbe28c7d081275d1805c2142ff792185</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_PINGRESP</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga285fc02048e2482794042fa98639e514</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_DISCONNECT</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>gaed07155a3d6fa4b7624b9f36ed33ec6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PACKET_TYPE_AUTH</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga14ad742747e745fea71a20ae587db704</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_PUBLISH_ACK_PACKET_SIZE</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga26994fcfacb1cff892caa45ec31ca7c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MQTT_STATE_CURSOR_INITIALIZER</name>
      <anchorfile>group__mqtt__constants.html</anchorfile>
      <anchor>ga666ad78e7eaaffa51f5cab96201a9476</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>mqtt_design</name>
    <title>Design</title>
    <filename>mqtt_design.html</filename>
    <docanchor file="mqtt_design.html" title="Interfaces and Callbacks">mqtt_interfaces</docanchor>
    <docanchor file="mqtt_design.html" title="Serializers and Deserializers">mqtt_serializers</docanchor>
    <docanchor file="mqtt_design.html" title="Sessions and State">mqtt_sessions</docanchor>
    <docanchor file="mqtt_design.html" title="Packet Reception">mqtt_receivepackets</docanchor>
    <docanchor file="mqtt_design.html" title="Runtime Timeouts passed to MQTT library">mqtt_receivetimeout</docanchor>
    <docanchor file="mqtt_design.html" title="Keep-Alive">mqtt_keepalive</docanchor>
  </compound>
  <compound kind="page">
    <name>core_mqtt_config</name>
    <title>Configurations</title>
    <filename>core_mqtt_config.html</filename>
    <docanchor file="core_mqtt_config.html">MQTT_DO_NOT_USE_CUSTOM_CONFIG</docanchor>
    <docanchor file="core_mqtt_config.html">MQTT_PINGRESP_TIMEOUT_MS</docanchor>
    <docanchor file="core_mqtt_config.html">MQTT_RECV_POLLING_TIMEOUT_MS</docanchor>
    <docanchor file="core_mqtt_config.html">MQTT_SEND_TIMEOUT_MS</docanchor>
    <docanchor file="core_mqtt_config.html">MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT</docanchor>
    <docanchor file="core_mqtt_config.html" title="LogError">mqtt_logerror</docanchor>
    <docanchor file="core_mqtt_config.html" title="LogWarn">mqtt_logwarn</docanchor>
    <docanchor file="core_mqtt_config.html" title="LogInfo">mqtt_loginfo</docanchor>
    <docanchor file="core_mqtt_config.html" title="LogDebug">mqtt_logdebug</docanchor>
  </compound>
  <compound kind="page">
    <name>mqtt_functions</name>
    <title>Functions</title>
    <filename>mqtt_functions.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_primaryfunctions</name>
    <title>Primary functions</title>
    <filename>mqtt_primaryfunctions.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializerfunctions</name>
    <title>Serializer functions</title>
    <filename>mqtt_serializerfunctions.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_propertyaddfunctions</name>
    <title>Property Add functions</title>
    <filename>mqtt_propertyaddfunctions.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_propertygetfunctions</name>
    <title>Property Get functions</title>
    <filename>mqtt_propertygetfunctions.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_init_function</name>
    <title>MQTT_Init</title>
    <filename>mqtt_init_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_initstatefulqos_function</name>
    <title>MQTT_InitStatefulQoS</title>
    <filename>mqtt_initstatefulqos_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_initretransmits_function</name>
    <title>MQTT_InitRetransmits</title>
    <filename>mqtt_initretransmits_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_connect_function</name>
    <title>MQTT_Connect</title>
    <filename>mqtt_connect_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_subscribe_function</name>
    <title>MQTT_Subscribe</title>
    <filename>mqtt_subscribe_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_publish_function</name>
    <title>MQTT_Publish</title>
    <filename>mqtt_publish_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_ping_function</name>
    <title>MQTT_Ping</title>
    <filename>mqtt_ping_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_unsubscribe_function</name>
    <title>MQTT_Unsubscribe</title>
    <filename>mqtt_unsubscribe_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_disconnect_function</name>
    <title>MQTT_Disconnect</title>
    <filename>mqtt_disconnect_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_processloop_function</name>
    <title>MQTT_ProcessLoop</title>
    <filename>mqtt_processloop_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_receiveloop_function</name>
    <title>MQTT_ReceiveLoop</title>
    <filename>mqtt_receiveloop_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getpacketid_function</name>
    <title>MQTT_GetPacketId</title>
    <filename>mqtt_getpacketid_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getsubackstatuscodes_function</name>
    <title>MQTT_GetSubAckStatusCodes</title>
    <filename>mqtt_getsubackstatuscodes_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_status_strerror_function</name>
    <title>MQTT_Status_strerror</title>
    <filename>mqtt_status_strerror_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_publishtoresend_function</name>
    <title>MQTT_PublishToResend</title>
    <filename>mqtt_publishtoresend_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropertybuilder_init_function</name>
    <title>MQTTPropertyBuilder_Init</title>
    <filename>mqttpropertybuilder_init_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getconnectpacketsize_function</name>
    <title>MQTT_GetConnectPacketSize</title>
    <filename>mqtt_getconnectpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializeconnect_function</name>
    <title>MQTT_SerializeConnect</title>
    <filename>mqtt_serializeconnect_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getsubscribepacketsize_function</name>
    <title>MQTT_GetSubscribePacketSize</title>
    <filename>mqtt_getsubscribepacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializesubscribe_function</name>
    <title>MQTT_SerializeSubscribe</title>
    <filename>mqtt_serializesubscribe_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getunsubscribepacketsize_function</name>
    <title>MQTT_GetUnsubscribePacketSize</title>
    <filename>mqtt_getunsubscribepacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializeunsubscribe_function</name>
    <title>MQTT_SerializeUnsubscribe</title>
    <filename>mqtt_serializeunsubscribe_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_validatepublishparams_function</name>
    <title>MQTT_ValidatePublishParams</title>
    <filename>mqtt_validatepublishparams_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getpublishpacketsize_function</name>
    <title>MQTT_GetPublishPacketSize</title>
    <filename>mqtt_getpublishpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializepublish_function</name>
    <title>MQTT_SerializePublish</title>
    <filename>mqtt_serializepublish_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_validatepublishproperties_function</name>
    <title>MQTT_ValidatePublishProperties</title>
    <filename>mqtt_validatepublishproperties_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializepublishheader_function</name>
    <title>MQTT_SerializePublishHeader</title>
    <filename>mqtt_serializepublishheader_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializepublishheaderwithouttopic_function</name>
    <title>MQTT_SerializePublishHeaderWithoutTopic</title>
    <filename>mqtt_serializepublishheaderwithouttopic_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializeack_function</name>
    <title>MQTT_SerializeAck</title>
    <filename>mqtt_serializeack_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getackpacketsize_function</name>
    <title>MQTT_GetAckPacketSize</title>
    <filename>mqtt_getackpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getdisconnectpacketsize_function</name>
    <title>MQTT_GetDisconnectPacketSize</title>
    <filename>mqtt_getdisconnectpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getpingreqpacketsize_function</name>
    <title>MQTT_GetPingreqPacketSize</title>
    <filename>mqtt_getpingreqpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_serializepingreq_function</name>
    <title>MQTT_SerializePingreq</title>
    <filename>mqtt_serializepingreq_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_deserializepublish_function</name>
    <title>MQTT_DeserializePublish</title>
    <filename>mqtt_deserializepublish_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_deserializeack_function</name>
    <title>MQTT_DeserializeAck</title>
    <filename>mqtt_deserializeack_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_getincomingpackettypeandlength_function</name>
    <title>MQTT_GetIncomingPacketTypeAndLength</title>
    <filename>mqtt_getincomingpackettypeandlength_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_subscriptionid_function</name>
    <title>MQTTPropAdd_SubscriptionId</title>
    <filename>mqttpropadd_subscriptionid_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_userprop_function</name>
    <title>MQTTPropAdd_UserProp</title>
    <filename>mqttpropadd_userprop_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_sessionexpiry_function</name>
    <title>MQTTPropAdd_SessionExpiry</title>
    <filename>mqttpropadd_sessionexpiry_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_willdelayinterval_function</name>
    <title>MQTTPropAdd_WillDelayInterval</title>
    <filename>mqttpropadd_willdelayinterval_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connreceivemax_function</name>
    <title>MQTTPropAdd_ReceiveMax</title>
    <filename>mqttpropadd_connreceivemax_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connmaxpacketsize_function</name>
    <title>MQTTPropAdd_MaxPacketSize</title>
    <filename>mqttpropadd_connmaxpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_conntopicaliasmax_function</name>
    <title>MQTTPropAdd_TopicAliasMax</title>
    <filename>mqttpropadd_conntopicaliasmax_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connrequestrespinfo_function</name>
    <title>MQTTPropAdd_RequestRespInfo</title>
    <filename>mqttpropadd_connrequestrespinfo_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connrequestprobinfo_function</name>
    <title>MQTTPropAdd_RequestProbInfo</title>
    <filename>mqttpropadd_connrequestprobinfo_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connauthmethod_function</name>
    <title>MQTTPropAdd_AuthMethod</title>
    <filename>mqttpropadd_connauthmethod_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_connauthdata_function</name>
    <title>MQTTPropAdd_AuthData</title>
    <filename>mqttpropadd_connauthdata_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubpayloadformat_function</name>
    <title>MQTTPropAdd_PayloadFormat</title>
    <filename>mqttpropadd_pubpayloadformat_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubmessageexpiry_function</name>
    <title>MQTTPropAdd_MessageExpiry</title>
    <filename>mqttpropadd_pubmessageexpiry_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubtopicalias_function</name>
    <title>MQTTPropAdd_TopicAlias</title>
    <filename>mqttpropadd_pubtopicalias_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubresponsetopic_function</name>
    <title>MQTTPropAdd_ResponseTopic</title>
    <filename>mqttpropadd_pubresponsetopic_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubcorrelationdata_function</name>
    <title>MQTTPropAdd_CorrelationData</title>
    <filename>mqttpropadd_pubcorrelationdata_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_pubcontenttype_function</name>
    <title>MQTTPropAdd_ContentType</title>
    <filename>mqttpropadd_pubcontenttype_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropadd_reasonstring_function</name>
    <title>MQTTPropAdd_ReasonString</title>
    <filename>mqttpropadd_reasonstring_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_validatesubscribeproperties_function</name>
    <title>MQTT_ValidateSubscribeProperties</title>
    <filename>mqtt_validatesubscribeproperties_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_payloadformat_function</name>
    <title>MQTTPropGet_PayloadFormatIndicator</title>
    <filename>_m_q_t_t_prop_get_payloadformat_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_responsetopic_function</name>
    <title>MQTTPropGet_ResponseTopic</title>
    <filename>_m_q_t_t_prop_get_responsetopic_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_correlationdata_function</name>
    <title>MQTTPropGet_CorrelationData</title>
    <filename>_m_q_t_t_prop_get_correlationdata_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_messageexpiryinterval_function</name>
    <title>MQTTPropGet_MessageExpiryInterval</title>
    <filename>_m_q_t_t_prop_get_messageexpiryinterval_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_contenttype_function</name>
    <title>MQTTPropGet_ContentType</title>
    <filename>_m_q_t_t_prop_get_contenttype_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_subscriptionid_function</name>
    <title>MQTTPropGet_SubscriptionId</title>
    <filename>_m_q_t_t_prop_get_subscriptionid_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropget_userprop_function</name>
    <title>MQTTPropGet_UserProp</title>
    <filename>mqttpropget_userprop_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropget_reasonstring_function</name>
    <title>MQTTPropGet_ReasonString</title>
    <filename>mqttpropget_reasonstring_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropget_disconnectserverref_function</name>
    <title>MQTTPropGet_ServerRef</title>
    <filename>mqttpropget_disconnectserverref_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqttpropget_sessionexpiry_function</name>
    <title>MQTTPropGet_SessionExpiry</title>
    <filename>mqttpropget_sessionexpiry_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_topicaliasmax_function</name>
    <title>MQTTPropGet_TopicAliasMax</title>
    <filename>_m_q_t_t_prop_get_topicaliasmax_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_receivemax_function</name>
    <title>MQTTPropGet_ReceiveMax</title>
    <filename>_m_q_t_t_prop_get_receivemax_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_maxqos_function</name>
    <title>MQTTPropGet_MaxQos</title>
    <filename>_m_q_t_t_prop_get_maxqos_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_retainavailable_function</name>
    <title>MQTTPropGet_RetainAvailable</title>
    <filename>_m_q_t_t_prop_get_retainavailable_function.html</filename>
  </compound>
  <compound kind="page">
    <name>MQTTPropGet_maxpacketsize_function</name>
    <title>MQTTPropGet_MaxPacketSize</title>
    <filename>_m_q_t_t_prop_get_maxpacketsize_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_initconnect_function</name>
    <title>MQTT_InitConnect</title>
    <filename>mqtt_initconnect_function.html</filename>
  </compound>
  <compound kind="page">
    <name>mqtt_porting</name>
    <title>Porting Guide</title>
    <filename>mqtt_porting.html</filename>
    <docanchor file="mqtt_porting.html" title="Configuration Macros">mqtt_porting_config</docanchor>
    <docanchor file="mqtt_porting.html" title="Transport Interface">mqtt_porting_transport</docanchor>
    <docanchor file="mqtt_porting.html" title="Time Function">mqtt_porting_time</docanchor>
  </compound>
  <compound kind="page">
    <name>mqtt_timeouts</name>
    <title>Timeouts in coreMQTT library</title>
    <filename>mqtt_timeouts.html</filename>
    <docanchor file="mqtt_timeouts.html" title="Transport Send and Receive timeouts">mqtt_timeouts_transport_send_receive</docanchor>
    <docanchor file="mqtt_timeouts.html" title="MQTT Keep Alive interval">mqtt_timeouts_keep_alive</docanchor>
    <docanchor file="mqtt_timeouts.html" title="MQTT Ping Response timeout">mqtt_timeouts_ping_response</docanchor>
    <docanchor file="mqtt_timeouts.html" title="MQTT Receive Polling timeout">mqtt_timeouts_receive_polling</docanchor>
    <docanchor file="mqtt_timeouts.html" title="MQTT Send timeout">mqtt_timeouts_send</docanchor>
    <docanchor file="mqtt_timeouts.html" title="Timeouts for MQTT_ProcessLoop and MQTT_ReceiveLoop APIs">mqtt_timeouts_process_receive_loop</docanchor>
    <docanchor file="mqtt_timeouts.html" title="Timeout for MQTT_Connect">mqtt_timeouts_connect</docanchor>
  </compound>
  <compound kind="page">
    <name>mqtt_transport_interface</name>
    <title>Transport Interface</title>
    <filename>mqtt_transport_interface.html</filename>
    <docanchor file="mqtt_transport_interface.html" title="Transport Interface Overview">mqtt_transport_interface_overview</docanchor>
    <docanchor file="mqtt_transport_interface.html" title="Implementing the Transport Interface">mqtt_transport_interface_implementation</docanchor>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Overview</title>
    <filename>index.html</filename>
    <docanchor file="index.html">mqtt</docanchor>
    <docanchor file="index.html" title="Memory Requirements">mqtt_memory_requirements</docanchor>
  </compound>
</tagfile>
