var group__mqtt__struct__types =
[
    [ "MQTTPubAckInfo_t", "struct_m_q_t_t_pub_ack_info__t.html", [
      [ "packetId", "struct_m_q_t_t_pub_ack_info__t.html#a66cef7b43af5d7fdd33b5d2dc766b2d0", null ],
      [ "qos", "struct_m_q_t_t_pub_ack_info__t.html#a086fcd48ef0b787697526a95c861e8a0", null ],
      [ "publishState", "struct_m_q_t_t_pub_ack_info__t.html#a61314203ef87a231c6489c68b579de34", null ]
    ] ],
    [ "MQTTContext_t", "struct_m_q_t_t_context__t.html", [
      [ "outgoingPublishRecords", "struct_m_q_t_t_context__t.html#a4ea1e37e0e81f010fbf84365ac2ef6de", null ],
      [ "incomingPublishRecords", "struct_m_q_t_t_context__t.html#afc147663a5933de81212fa77057f0a4d", null ],
      [ "outgoingPublishRecordMaxCount", "struct_m_q_t_t_context__t.html#a2851073e252d1e744596272ef13dd14a", null ],
      [ "incomingPublishRecordMaxCount", "struct_m_q_t_t_context__t.html#aa33ed2e10380a854629f1386d0323ea8", null ],
      [ "transportInterface", "struct_m_q_t_t_context__t.html#a87ab9d61e7711325c2c85ce3ce63386a", null ],
      [ "networkBuffer", "struct_m_q_t_t_context__t.html#a231c5576a6ce389317a3f00f95628276", null ],
      [ "ackPropsBuffer", "struct_m_q_t_t_context__t.html#acc6db180acbc7cc7b1ef7c2f5f553631", null ],
      [ "nextPacketId", "struct_m_q_t_t_context__t.html#af47ed55ad7e9bb112324f5f209b70534", null ],
      [ "connectStatus", "struct_m_q_t_t_context__t.html#a4e38c4dc77e7751a0ad8730a41bee47f", null ],
      [ "getTime", "struct_m_q_t_t_context__t.html#aabe1d302a16771292151013e8e30c582", null ],
      [ "appCallback", "struct_m_q_t_t_context__t.html#a73bd9259db9c3a9b84518cbf928ed91f", null ],
      [ "lastPacketTxTime", "struct_m_q_t_t_context__t.html#a01acf90953e830ba3e7f44182cb1d482", null ],
      [ "lastPacketRxTime", "struct_m_q_t_t_context__t.html#a7111ef16e4a4e75a72861f6f3ea8a7c3", null ],
      [ "controlPacketSent", "struct_m_q_t_t_context__t.html#af9724f2426132e3ce96a03892902ef89", null ],
      [ "index", "struct_m_q_t_t_context__t.html#a41b7735cd0746563483b72e17cf103aa", null ],
      [ "keepAliveIntervalSec", "struct_m_q_t_t_context__t.html#afd6071827ef48b230212a5725c2075be", null ],
      [ "pingReqSendTimeMs", "struct_m_q_t_t_context__t.html#acca3efa4146d85f7e874c7c326e23556", null ],
      [ "waitingForPingResp", "struct_m_q_t_t_context__t.html#ac7073f43645f7b7c0c5b7763980004bb", null ],
      [ "connectionProperties", "struct_m_q_t_t_context__t.html#a9914b8259063a3ee3b77b90de0ad6507", null ],
      [ "storeFunction", "struct_m_q_t_t_context__t.html#ac205fc33078b0cfa0dfc62807bd9574f", null ],
      [ "retrieveFunction", "struct_m_q_t_t_context__t.html#a270e9e595ecd60948c960d059f8fe718", null ],
      [ "clearFunction", "struct_m_q_t_t_context__t.html#a36e3d500e6efa050d6e666aefa3ff577", null ]
    ] ],
    [ "MQTTDeserializedInfo_t", "struct_m_q_t_t_deserialized_info__t.html", [
      [ "packetIdentifier", "struct_m_q_t_t_deserialized_info__t.html#af4df2a9926a4a68059195daa712d9b84", null ],
      [ "pPublishInfo", "struct_m_q_t_t_deserialized_info__t.html#ac347273fae1e92b9cbeda1714066c1de", null ],
      [ "deserializationResult", "struct_m_q_t_t_deserialized_info__t.html#a7df1b7b60404c9f1604fec0081d2625d", null ],
      [ "pReasonCode", "struct_m_q_t_t_deserialized_info__t.html#aa6e6ca63ae46b306dd3d304f6f564c29", null ]
    ] ],
    [ "MQTTFixedBuffer_t", "struct_m_q_t_t_fixed_buffer__t.html", [
      [ "pBuffer", "struct_m_q_t_t_fixed_buffer__t.html#acea147448e044870fb36b7fa2347dbd6", null ],
      [ "size", "struct_m_q_t_t_fixed_buffer__t.html#a0b0b6a93cc62751ebeb03095d5431636", null ]
    ] ],
    [ "MQTTConnectInfo_t", "struct_m_q_t_t_connect_info__t.html", [
      [ "cleanSession", "struct_m_q_t_t_connect_info__t.html#a606e7765c4f2215fb2bf630f6eb9ff6b", null ],
      [ "keepAliveSeconds", "struct_m_q_t_t_connect_info__t.html#a7d05d53261732ebdfbb9ee665a347591", null ],
      [ "pClientIdentifier", "struct_m_q_t_t_connect_info__t.html#a010f8f6993cbf8899648d5c515ff7884", null ],
      [ "clientIdentifierLength", "struct_m_q_t_t_connect_info__t.html#a0c68759c2b85bb4d2815c61476196631", null ],
      [ "pUserName", "struct_m_q_t_t_connect_info__t.html#a1118d7d3251a11445318557280db53b4", null ],
      [ "userNameLength", "struct_m_q_t_t_connect_info__t.html#a7ce1cf073a36e29748c79b712de5e5bd", null ],
      [ "pPassword", "struct_m_q_t_t_connect_info__t.html#acec6c79a11d2f0f130802393f34d2b5e", null ],
      [ "passwordLength", "struct_m_q_t_t_connect_info__t.html#adf9b33cb9fe6479338813578673cf605", null ]
    ] ],
    [ "MQTTSubscribeInfo_t", "struct_m_q_t_t_subscribe_info__t.html", [
      [ "qos", "struct_m_q_t_t_subscribe_info__t.html#a64cf2e423f60cfec122eeaef80c0fd86", null ],
      [ "pTopicFilter", "struct_m_q_t_t_subscribe_info__t.html#adb0b28240fdcd82a85f11cf2f8b5bbf0", null ],
      [ "topicFilterLength", "struct_m_q_t_t_subscribe_info__t.html#a5849d6d2b87a8bcef894cb92bddc9d14", null ],
      [ "noLocalOption", "struct_m_q_t_t_subscribe_info__t.html#a9a417e8150a741cfe92e7176209d326c", null ],
      [ "retainAsPublishedOption", "struct_m_q_t_t_subscribe_info__t.html#a4ea94e01ac69e4744710bb50fdebe503", null ],
      [ "retainHandlingOption", "struct_m_q_t_t_subscribe_info__t.html#a65c895ef9d14fb6202d07d4e8e5d1073", null ]
    ] ],
    [ "MQTTPublishInfo_t", "struct_m_q_t_t_publish_info__t.html", [
      [ "qos", "struct_m_q_t_t_publish_info__t.html#a178224d02b4acdec7e08e88de0e4b399", null ],
      [ "retain", "struct_m_q_t_t_publish_info__t.html#a343b0af89c46a900db4aa5c775a0975a", null ],
      [ "dup", "struct_m_q_t_t_publish_info__t.html#aa1c8954e83bfa678d1ff5429679d4e89", null ],
      [ "pTopicName", "struct_m_q_t_t_publish_info__t.html#aa80e8ca282d01630f878ad0afe81d7a4", null ],
      [ "topicNameLength", "struct_m_q_t_t_publish_info__t.html#a865573b203c848cc28cdf955b7028629", null ],
      [ "pPayload", "struct_m_q_t_t_publish_info__t.html#afc28299f4f625f5e674bb61b42f03380", null ],
      [ "payloadLength", "struct_m_q_t_t_publish_info__t.html#a7997964e11571f35f0c3b729db0f760f", null ],
      [ "propertyLength", "struct_m_q_t_t_publish_info__t.html#aaf01d089ad9db1b041015715071942f9", null ]
    ] ],
    [ "MQTTPacketInfo_t", "struct_m_q_t_t_packet_info__t.html", [
      [ "type", "struct_m_q_t_t_packet_info__t.html#a7fef40548c1aa0f0e7f812a6a7243758", null ],
      [ "pRemainingData", "struct_m_q_t_t_packet_info__t.html#ac66cedff052bc844ec9b296387df60bc", null ],
      [ "remainingLength", "struct_m_q_t_t_packet_info__t.html#a566b295792625e8cf04944d34bdeb538", null ],
      [ "headerLength", "struct_m_q_t_t_packet_info__t.html#aa7de1631ed8e08410942d36a72db558a", null ]
    ] ],
    [ "MQTTPropBuilder_t", "struct_m_q_t_t_prop_builder__t.html", [
      [ "pBuffer", "struct_m_q_t_t_prop_builder__t.html#a7b2fa2be2e3aa4e99fbdc4a797891a34", null ],
      [ "bufferLength", "struct_m_q_t_t_prop_builder__t.html#a93627d10e651b2d47a1ea18a9c510a16", null ],
      [ "currentIndex", "struct_m_q_t_t_prop_builder__t.html#a5f271ebc88dcae13add5df2ec78a2e0e", null ],
      [ "fieldSet", "struct_m_q_t_t_prop_builder__t.html#a6cc81cd8ebc111cdfbb2c736482d278f", null ]
    ] ],
    [ "MQTTReasonCodeInfo_t", "struct_m_q_t_t_reason_code_info__t.html", [
      [ "reasonCode", "struct_m_q_t_t_reason_code_info__t.html#a2b61cf1ee4caa1865a54d47da2e76477", null ],
      [ "reasonCodeLength", "struct_m_q_t_t_reason_code_info__t.html#a1476b796abb5bd5acf218a50c3ff53ac", null ]
    ] ],
    [ "MQTTConnectionProperties_t", "struct_m_q_t_t_connection_properties__t.html", [
      [ "sessionExpiry", "struct_m_q_t_t_connection_properties__t.html#ac7c25d5c5a9538184437879bb05281a2", null ],
      [ "receiveMax", "struct_m_q_t_t_connection_properties__t.html#a88d862e8132699a3520cfcda2e77c1c6", null ],
      [ "maxPacketSize", "struct_m_q_t_t_connection_properties__t.html#ad647e802a7aecda1394634f0c8e45e7e", null ],
      [ "topicAliasMax", "struct_m_q_t_t_connection_properties__t.html#ac199a8efa5d695c3b8913d448f4998d0", null ],
      [ "requestResponseInfo", "struct_m_q_t_t_connection_properties__t.html#a0f66bbb49942cbaf8bf0ee52384467d8", null ],
      [ "requestProblemInfo", "struct_m_q_t_t_connection_properties__t.html#a2c8472adb537d9b6857da6c7803b0ef1", null ],
      [ "serverReceiveMax", "struct_m_q_t_t_connection_properties__t.html#add994c9e0f9c3993960bd35496486e04", null ],
      [ "serverMaxQos", "struct_m_q_t_t_connection_properties__t.html#a3123b62b6ebae1b8183f592c041a0b4a", null ],
      [ "retainAvailable", "struct_m_q_t_t_connection_properties__t.html#a94a9fdef74b690f76f889966fcfed8c2", null ],
      [ "serverMaxPacketSize", "struct_m_q_t_t_connection_properties__t.html#aae7d346ecae1a5dd809784d2d34aebbc", null ],
      [ "serverTopicAliasMax", "struct_m_q_t_t_connection_properties__t.html#a28b3da2cb05b3b9116204ded988831e2", null ],
      [ "isWildcardAvailable", "struct_m_q_t_t_connection_properties__t.html#ad726fc175701ac8b1d6f2c9c67f648b5", null ],
      [ "isSubscriptionIdAvailable", "struct_m_q_t_t_connection_properties__t.html#a2c8483c67a129e1d662a845191e5d624", null ],
      [ "isSharedAvailable", "struct_m_q_t_t_connection_properties__t.html#a2b5401c1665c3973de6afeb2c22dcb1c", null ],
      [ "serverKeepAlive", "struct_m_q_t_t_connection_properties__t.html#aa9567f22d2dc102b99893d619d7bebcc", null ]
    ] ],
    [ "MQTTUserProperty_t", "struct_m_q_t_t_user_property__t.html", [
      [ "pKey", "struct_m_q_t_t_user_property__t.html#aaaea6defd6ebdde0e7f92f957077e0ee", null ],
      [ "keyLength", "struct_m_q_t_t_user_property__t.html#a5d5212b909aa8e0776f8eb7fab92127f", null ],
      [ "pValue", "struct_m_q_t_t_user_property__t.html#a42f2cccdff31cac6a881b6efd0a88efb", null ],
      [ "valueLength", "struct_m_q_t_t_user_property__t.html#ac00547315a3542befae9ee06c68047e9", null ]
    ] ],
    [ "TransportInterface_t", "struct_transport_interface__t.html", [
      [ "recv", "struct_transport_interface__t.html#a7c34e9b865e2a509306f09c7dfa3699e", null ],
      [ "send", "struct_transport_interface__t.html#a01cd9935e9a5266ca196243a0054d489", null ],
      [ "writev", "struct_transport_interface__t.html#a8cf677fbeee53d270daa6dacfa138b79", null ],
      [ "pNetworkContext", "struct_transport_interface__t.html#aaf4702050bef8d62714a4d3900e95087", null ]
    ] ],
    [ "NetworkContext_t", "group__mqtt__struct__types.html#ga7769e434e7811caed8cd6fd7f9ec26ec", null ]
];