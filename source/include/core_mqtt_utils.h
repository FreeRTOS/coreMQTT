/*CONNECT PROPERTIES*/

   /**
   * @brief Session expiry id.
   */
#define MQTT_SESSION_EXPIRY_ID      ( 0x11 )

   /**
   * @brief Receive maximum id.
   */
#define MQTT_RECEIVE_MAX_ID         ( 0x21 )

   /**
   * @brief Maximum packet size  id.
   */
#define MQTT_MAX_PACKET_SIZE_ID     ( 0x27 )

   /**
   * @brief Topic alias size id.
   */
#define MQTT_TOPIC_ALIAS_MAX_ID     ( 0x22 )

   /**
   * @brief Request response id.
   */
#define MQTT_REQUEST_RESPONSE_ID    ( 0x19 )

   /**
   * @brief Request problem id.
   */
#define MQTT_REQUEST_PROBLEM_ID     ( 0x17 )

   /**
   * @brief User property id.
   */
#define MQTT_USER_PROPERTY_ID       ( 0x26 )

   /**
   * @brief Authentication method id.
   */
#define MQTT_AUTH_METHOD_ID         ( 0x15 )

   /**
   * @brief  Authentication data id.
   */
#define MQTT_AUTH_DATA_ID           ( 0x16 )

   /*Publish PROPERTIES*/

   /**
   * @brief Will delay id.
   */
#define MQTT_WILL_DELAY_ID          ( 0x18 )

   /**
   * @brief Payload format id.
   */
#define MQTT_PAYLOAD_FORMAT_ID      ( 0x01 )

   /**
   * @brief Message Expiry id.
   */
#define MQTT_MSG_EXPIRY_ID          ( 0x02 )

   /**
   * @brief Content type id.
   */
#define MQTT_CONTENT_TYPE_ID        ( 0x03 )

   /**
   * @brief Response topic id.
   */
#define MQTT_RESPONSE_TOPIC_ID      ( 0x08 )

   /**
   * @brief Correlation data id.
   */
#define MQTT_CORRELATION_DATA_ID    ( 0x09 )

   /**
   * @brief Topic alias id.
   */
#define MQTT_TOPIC_ALIAS_ID         ( 0x23 )

   /*CONNACK PROPERTIES*/

   /**
   * @brief Max qos id.
   */
#define MQTT_MAX_QOS_ID              ( 0x24 )

   /**
   * @brief Retain available id.
   */
#define MQTT_RETAIN_AVAILABLE_ID     ( 0x25 )

   /**
   * @brief Assigned client identifier id.
   */
#define MQTT_ASSIGNED_CLIENT_ID      ( 0x12 )

   /**
   * @brief Reason string id.
   */
#define MQTT_REASON_STRING_ID        ( 0x1F )

   /**
   * @brief Wildcard available id.
   */
#define MQTT_WILDCARD_ID             ( 0x28 )

   /**
   * @brief Subscription available id.
   */
#define MQTT_SUB_AVAILABLE_ID        ( 0x29 )

   /**
   * @brief Shared subscription id.
   */
#define MQTT_SHARED_SUB_ID           ( 0x2A )

   /**
   * @brief Server keep alive id.
   */
#define MQTT_SERVER_KEEP_ALIVE_ID    ( 0x13 )

   /**
   * @brief Response information id.
   */

#define MQTT_RESPONSE_INFO_ID    ( 0x1A )

   /**
   * @brief Server reference  id.
   */
#define MQTT_SERVER_REF_ID       ( 0x1C )

#define MQTT_SUBSCRIPTION_ID_ID                     ( 0x0B )



uint8_t* encodeRemainingLength(uint8_t* pDestination,
    size_t length) ; 