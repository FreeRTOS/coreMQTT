#include <string.h>
#include <assert.h>

/* Include config defaults header to get default values of configs. */
#include "core_mqtt_config_defaults.h"

/**
 * @brief Set a bit in an 8-bit unsigned integer.
 */
#define UINT8_SET_BIT( x, position )      ( ( x ) = ( uint8_t ) ( ( x ) | ( 0x01U << ( position ) ) ) )


uint8_t* encodeRemainingLength(uint8_t* pDestination,
    size_t length)
{
    uint8_t lengthByte;
    uint8_t* pLengthEnd = NULL;
    size_t remainingLength = length;

    assert(pDestination != NULL);

    pLengthEnd = pDestination;

    /* This algorithm is copied from the MQTT v3.1.1 spec. */
    do
    {
        lengthByte = (uint8_t)(remainingLength % 128U);
        remainingLength = remainingLength / 128U;

        /* Set the high bit of this byte, indicating that there's more data. */
        if (remainingLength > 0U)
        {
            UINT8_SET_BIT(lengthByte, 7);
        }

        /* Output a single encoded byte. */
        *pLengthEnd = lengthByte;
        pLengthEnd++;
    } while (remainingLength > 0U);

    return pLengthEnd;
}