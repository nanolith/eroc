/**
 * \file lib/eroc_buffer_name_set.c
 *
 * \brief Set the name of the buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Set the buffer's name.
 *
 * \param buffer            The buffer for this operation.
 * \param name              The name of the buffer.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_name_set(eroc_buffer* buffer, const char* name)
{
    /* duplicate the name. */
    char* tmp = strdup(name);
    if (NULL == tmp)
    {
        return 1;
    }

    /* free the old name if set. */
    if (NULL != buffer->name)
    {
        free(buffer->name);
    }

    /* save the new name. */
    buffer->name = tmp;

    return 0;
}
