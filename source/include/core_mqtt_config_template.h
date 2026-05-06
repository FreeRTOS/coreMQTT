/*
 * coreMQTT
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * @file core_mqtt_config_template.h
 * @brief Template configuration file for the coreMQTT library.
 *
 * This file is a starting point for the `core_mqtt_config.h` that coreMQTT
 * requires when the preprocessor macro `MQTT_DO_NOT_USE_CUSTOM_CONFIG` is
 * not defined.
 *
 * To use it:
 *   1. Copy this file into your application as `core_mqtt_config.h`.
 *   2. Make sure the directory that contains your copy is on the compiler's
 *      include path when building coreMQTT.
 *   3. Uncomment and change the values of any macros below whose defaults
 *      (defined in `core_mqtt_config_defaults.h`) do not match your
 *      application's needs.
 *
 * Any macro left commented-out here will pick up the default value from
 * `core_mqtt_config_defaults.h`. You do not need to define every macro.
 *
 * See `core_mqtt_config_defaults.h` for the authoritative list of tunables
 * and their documented ranges.
 */

#ifndef CORE_MQTT_CONFIG_H_
#define CORE_MQTT_CONFIG_H_

/*-------------------- Protocol / buffer sizing --------------------*/

/**
 * @brief Maximum number of vectors used when sending SUBSCRIBE / UNSUBSCRIBE
 * packets via the scatter-gather transport writev interface.
 *
 * Default: 4
 */
/* #define MQTT_SUB_UNSUB_MAX_VECTORS    ( 4U ) */

/*-------------------- Connect handshake --------------------*/

/**
 * @brief Number of retry attempts for receiving CONNACK when MQTT_Connect is
 * called with timeoutMs == 0. Ignored when timeoutMs > 0.
 *
 * Default: 5
 */
/* #define MQTT_MAX_CONNACK_RECEIVE_RETRY_COUNT    ( 5U ) */

/*-------------------- Keep-alive and idle timing --------------------*/

/**
 * @brief Maximum milliseconds to wait for a PINGRESP after sending a PINGREQ.
 * If exceeded, MQTT_ProcessLoop returns MQTTKeepAliveTimeout.
 *
 * Recommended: <= keepAliveSeconds / 2. Default: 5000.
 */
/* #define MQTT_PINGRESP_TIMEOUT_MS    ( 5000U ) */

/**
 * @brief Maximum milliseconds of transmit idleness before the library sends
 * a PINGREQ on its own. Capped by keepAliveSeconds if that is smaller.
 *
 * Default: 30000.
 */
/* #define PACKET_TX_TIMEOUT_MS    ( 30000U ) */

/**
 * @brief Maximum milliseconds of receive idleness before the library sends
 * a PINGREQ to verify the connection is alive.
 *
 * Default: 30000.
 */
/* #define PACKET_RX_TIMEOUT_MS    ( 30000U ) */

/*-------------------- Transport I/O timing --------------------*/

/**
 * @note All three of the transport-I/O timeouts below MUST be set to 0 if
 * the MQTTGetCurrentTimeFunc_t passed to MQTT_Init is a dummy that always
 * returns 0. See the note on MQTT_Connect's timeoutMs parameter.
 */

/**
 * @brief Maximum milliseconds to wait between successful partial reads when
 * assembling an incoming MQTT packet. Exceeding this returns MQTTRecvFailed.
 *
 * Default: 10.
 */
/* #define MQTT_RECV_POLLING_TIMEOUT_MS    ( 10U ) */

/**
 * @brief Maximum milliseconds to spend sending a single MQTT packet across
 * potentially multiple transport send / writev calls. Exceeding this returns
 * MQTTSendFailed.
 *
 * Default: 20000.
 */
/* #define MQTT_SEND_TIMEOUT_MS    ( 20000U ) */

/*-------------------- Logging --------------------*/

/**
 * @brief Logging macros.
 *
 * Map these to your application's logging implementation. All five are
 * stripped to no-ops by default, so the library produces no log output
 * unless you define them here.
 *
 * The library invokes each macro with its argument wrapped in double
 * parentheses so it remains ISO C89/C90 compliant, for example:
 *
 *     LogInfo( ( "Connected with keep alive = %u", keepAlive ) );
 *
 * A simple printf-based mapping:
 *
 *     #include <stdio.h>
 *     #define LogError( msg )    do { printf( "[ERR]  " ); printf msg; printf( "\n" ); } while( 0 )
 *     #define LogWarn(  msg )    do { printf( "[WARN] " ); printf msg; printf( "\n" ); } while( 0 )
 *     #define LogInfo(  msg )    do { printf( "[INFO] " ); printf msg; printf( "\n" ); } while( 0 )
 *     #define LogDebug( msg )    do { } while( 0 )
 *     #define LogTrace( msg )    do { } while( 0 )
 */
/* #define LogError( message ) */
/* #define LogWarn(  message ) */
/* #define LogInfo(  message ) */
/* #define LogDebug( message ) */
/* #define LogTrace( message ) */

/*-------------------- Thread-safety hooks --------------------*/

/**
 * @brief Thread-safety hooks.
 *
 * coreMQTT is single-threaded by default. If multiple threads share one
 * MQTTContext_t, define these to acquire and release a mutex that protects
 * the context. The library pairs every PRE call with a matching POST call
 * and does not re-enter a hook-protected region from inside another, so a
 * non-recursive mutex is sufficient.
 *
 * See the "Thread Safety" section of README.md for a worked FreeRTOS
 * example.
 */
/* #define MQTT_PRE_STATE_UPDATE_HOOK(  pContext ) */
/* #define MQTT_POST_STATE_UPDATE_HOOK( pContext ) */

#endif /* ifndef CORE_MQTT_CONFIG_H_ */
