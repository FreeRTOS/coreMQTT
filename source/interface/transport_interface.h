/*
 * coreMQTT v1.0.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * @file transport_interface.h
 * @brief Transport interface definitions to send and receive data over the
 * network.
 */
#ifndef TRANSPORT_INTERFACE_H_
#define TRANSPORT_INTERFACE_H_

#include <stdint.h>
#include <stddef.h>

/**
 * @transportpage
 * @brief The transport interface definition.
 *
 * @transportsectionoverview
 *
 * The transport interface is a set of APIs that must be implemented using an
 * external transport layer protocol. The transport interface is defined in
 * @ref transport_interface.h. This interface allows protocols like MQTT and
 * HTTP to send and receive data over the transport layer. This
 * interface does not handle connection and disconnection to the server of
 * interest. The connection, disconnection, and other transport settings, like
 * timeout and TLS setup, must be handled in the user application.
 * <br>
 *
 * The functions that must be implemented are:<br>
 * - [Transport Receive](@ref TransportRecv_t)
 * - [Transport Send](@ref TransportSend_t)
 *
 * Each of the functions above take in an opaque context @ref NetworkContext_t.
 * The functions above and the context are also grouped together in the
 * @ref TransportInterface_t structure:<br><br>
 * @snippet this define_transportinterface
 * <br>
 *
 * @transportsectionimplementation
 *
 * The following steps give guidance on implementing the transport interface:
 *
 * -# Implementing @ref NetworkContext_t<br><br>
 * @snippet this define_networkcontext
 * <br>
 * The @ref NetworkContext is an incomplete type. An implementation of this
 * interface must define `struct NetworkContext` for the system's requirements.
 * For example, a plain-text implemention of the NetworkContext type might
 * include a socket, and a TLS implementation might add a TLS context. This
 * context is passed into the @ref TransportRecv_t and the @ref TransportSend_t
 * functions.<br><br>
 * <b>Example code:</b>
 * @code{c}
 * struct NetworkContext
 * {
 *     struct MyTCPSocketContext tcpSocketContext;
 *     struct MyTLSContext tlsContext;
 * };
 * @endcode
 * <br>
 * -# Implementing @ref TransportRecv_t<br><br>
 * @snippet this define_transportrecv
 * <br>
 * This function is expected to populate a buffer, with bytes received from the
 * transport, and return the number of bytes placed in the buffer. However, if an
 * error occurs while trying to receive the payload, then a negative error
 * code will be returned. The specific error code depends on the implementation
 * of the function used. It is also possible to have a return value that is less
 * than the size requested. As such, it is recommended to check the return value
 * and repeatedly invoke this function in a loop until all bytes are received.
 * In the case of TLS over TCP, @ref TransportRecv_t is typically implemented by
 * calling the TLS layer function to receive data. In case of plaintext TCP
 * without TLS, it is typically implemented by calling the TCP layer receive
 * function. @ref TransportRecv_t may be invoked multiple times by the protocol
 * library, if fewer bytes than were requested to receive are returned.
 * <br><br>
 * <b>Example code:</b>
 * @code{c}
 * int32_t myNetworkRecvImplementation( NetworkContext_t * pNetworkContext,
 *                                      void * pBuffer,
 *                                      size_t bytesToRecv )
 * {
 *     int32_t bytesReceived = 0;
 *     bytesReceived = TLSRecv( pNetworkContext->tlsContext,
 *                              pBuffer,
 *                              bytesToRecv,
 *                              MY_SOCKET_TIMEOUT );
 *     if( bytesReceived < 0 )
 *     {
 *         // Handle socket error.
 *     }
 *     // Handle other cases.
 *
 *     return bytesReceived;
 * }
 * @endcode
 * <br>
 * -# Implementing @ref TransportSend_t<br><br>
 * @snippet this define_transportsend
 * <br>
 * This function is expected to send the bytes, in the given buffer over the
 * transport, and return the number of bytes sent. However, if an error
 * occurs while trying to send the payload, then a negative error code will be
 * returned. The specific error code depends on the implementation of the function
 * used. It is also possible to have a return value that is less than the size
 * of the size requested. As such, it is recommended to check the return value
 * and repeatedly invoke this function in a loop until all bytes are sent.
 * In the case of TLS over TCP, @ref TransportSend_t is typically implemented by
 * calling the TLS layer function to send data. In case of plaintext TCP
 * without TLS, it is typically implemented by calling the TCP layer send
 * function. @ref TransportSend_t may be invoked multiple times by the protocol
 * library, if fewer bytes than were requested to send are returned.
 * <br><br>
 * <b>Example code:</b>
 * @code{c}
 * int32_t myNetworkSendImplementation( NetworkContext_t * pNetworkContext,
 *                                      const void * pBuffer,
 *                                      size_t bytesToSend )
 * {
 *     int32_t bytesSent = 0;
 *     bytesSent = TLSSend( pNetworkContext->tlsContext,
 *                          pBuffer,
 *                          bytesToSend,
 *                          MY_SOCKET_TIMEOUT );
 *     if( bytesSent < 0 )
 *     {
 *         // Handle socket error.
 *     }
 *     // Handle other cases.
 *
 *     return bytesSent;
 * }
 * @endcode
 */

/**
 * @transportstruct
 * @typedef NetworkContext_t
 * @brief The NetworkContext is an incomplete type. An implementation of this
 * interface must define struct NetworkContext for the system requirements.
 * This context is passed into the network interface functions.
 */
/* @[define_networkcontext] */
struct NetworkContext;
typedef struct NetworkContext NetworkContext_t;
/* @[define_networkcontext] */

/**
 * @transportcallback
 * @brief Transport interface for receiving data on the network.
 *
 * @param[in] pNetworkContext Implementation-defined network context.
 * @param[in] pBuffer Buffer to receive the data into.
 * @param[in] bytesToRecv Number of bytes requested from the network.
 *
 * @return The number of bytes received or a negative error code.
 */
/* @[define_transportrecv] */
typedef int32_t ( * TransportRecv_t )( NetworkContext_t * pNetworkContext,
                                       void * pBuffer,
                                       size_t bytesToRecv );
/* @[define_transportrecv] */

/**
 * @transportcallback
 * @brief Transport interface for sending data over the network.
 *
 * @param[in] pNetworkContext Implementation-defined network context.
 * @param[in] pBuffer Buffer containing the bytes to send over the network stack.
 * @param[in] bytesToSend Number of bytes to send over the network.
 *
 * @return The number of bytes sent or a negative error code.
 */
/* @[define_transportsend] */
typedef int32_t ( * TransportSend_t )( NetworkContext_t * pNetworkContext,
                                       const void * pBuffer,
                                       size_t bytesToSend );
/* @[define_transportsend] */

/**
 * @transportstruct
 * @brief The transport layer interface.
 */
/* @[define_transportinterface] */
typedef struct TransportInterface
{
    TransportRecv_t recv;               /**< Transport receive interface. */
    TransportSend_t send;               /**< Transport send interface. */
    NetworkContext_t * pNetworkContext; /**< Implementation-defined network context. */
} TransportInterface_t;
/* @[define_transportinterface] */

#endif /* ifndef TRANSPORT_INTERFACE_H_ */
