/**
 * \file lib/eroc_buffer_release.c
 *
 * \brief Release a buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Release a buffer.
 *
 * \param buffer            The buffer to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_release(eroc_buffer* buffer)
{
    int retval;

    retval = eroc_list_release(buffer->lines);

    free(buffer);

    return retval;
}
