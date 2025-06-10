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
 * @file core_mqtt_utils.h
 * @brief utility function used in other files.
 */
#ifndef CORE_MQTT_UTILS_H
#define CORE_MQTT_UTILS_H

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

/**
 * @brief Set a bit in an 8-bit unsigned integer.
 */
#define UINT8_SET_BIT( x, position )      ( ( x ) = ( uint8_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )

/**
 * @brief Macro for checking if a bit is set in a 1-byte unsigned int.
 *
 * @param[in] x The unsigned int to check.
 * @param[in] position Which bit to check.
 */
#define UINT8_CHECK_BIT( x, position )    ( ( ( x ) &( 0x01U << ( position ) ) ) == ( 0x01U << ( position ) ) )

/**
 * @brief Set a bit in an 32-bit unsigned integer.
 */
#define UINT32_SET_BIT( x, position ) \
    ( ( x ) = ( uint32_t ) ( ( x ) | ( ( uint32_t ) 0x01U << ( position ) ) ) )

/**
 * @brief Macro for checking if a bit is set in a 4-byte unsigned int.
 *
 * @param[in] x The unsigned int to check.
 * @param[in] position Which bit to check.
 */
#define UINT32_CHECK_BIT( x, position ) \
    ( ( ( uint32_t ) ( x ) &( ( uint32_t ) 0x01U << ( position ) ) ) == ( ( uint32_t ) 0x01U << ( position ) ) )

/**
 * @brief Get the high byte of a 16-bit unsigned integer.
 */
#define UINT16_HIGH_BYTE( x )    ( ( uint8_t ) ( ( x ) >> 8 ) )

/**
 * @brief Get the low byte of a 16-bit unsigned integer.
 */
#define UINT16_LOW_BYTE( x )     ( ( uint8_t ) ( ( x ) & 0x00ffU ) )

/**
 * @brief Macro for decoding a 2-byte unsigned int from a sequence of bytes.
 *
 * @param[in] ptr A uint8_t* that points to the high byte.
 */
#define UINT16_DECODE( ptr )                            \
    ( uint16_t ) ( ( ( ( uint16_t ) ptr[ 0 ] ) << 8 ) | \
                   ( ( uint16_t ) ptr[ 1 ] ) )

/**
 * @brief Macro for decoding a 4-byte unsigned int from a sequence of bytes.
 *
 * @param[in] ptr A uint8_t* that points to the high byte.
 */
#define UINT32_DECODE( ptr )                             \
    ( uint32_t ) ( ( ( ( uint32_t ) ptr[ 0 ] ) << 24 ) | \
                   ( ( ( uint32_t ) ptr[ 1 ] ) << 16 ) | \
                   ( ( ( uint32_t ) ptr[ 2 ] ) << 8 ) |  \
                   ( ( uint32_t ) ptr[ 3 ] ) )

/**
 * @brief Macro to extract a specific byte from a 32-bit unsigned integer.
 *
 * @param[in] x The 32-bit unsigned integer to extract from.
 * @param[in] byteNumber The byte position to extract (0-3).
 */
#define UINT32_GET_BYTE( x, byteNumber ) \
    ( ( uint8_t ) ( ( ( uint32_t ) ( x ) >> ( ( uint8_t ) ( byteNumber ) * 8U ) ) & 0xFFU ) )

/**
 * @brief Clear a bit in an 8-bit unsigned integer.
 */
#define UINT8_CLEAR_BIT( x, position )    ( ( x ) = ( uint8_t ) ( ( x ) &( ~( 0x01U << ( position ) ) ) ) )

/**
 * @brief Encodes the remaining length of the packet using the variable length
 * encoding scheme provided in the MQTT 5.0 specification.
 *
 * @param[out] pDestination The destination buffer to store the encoded remaining
 * length.
 * @param[in] length The remaining length to encode.
 *
 * @return The location of the byte following the encoded value.
 */
uint8_t * encodeVariableLength( uint8_t * pDestination,
                                size_t length );

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* ifndef CORE_MQTT_UTILS_H*/
