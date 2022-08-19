#include <stdlib.h>

void * memmove( void * destination,
                const void * source,
                size_t num )
{
    __CPROVER_assert( destination, "memmove destination is nonnull" );
    __CPROVER_assert( source, "memmove source is nonnull" );
    __CPROVER_havoc_object( destination );
    return destination;
}
