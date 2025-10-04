/**
 * \file lib/eroc_buffer_create.c
 *
 * \brief Create an empty buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create an empty buffer.
 *
 * \param buffer            Pointer to the buffer pointer to set with this
 *                          buffer on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_create(eroc_buffer** buffer)
{
    int retval;
    eroc_buffer* tmp;

    tmp = (eroc_buffer*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        retval = 1;
        goto done;
    }

    /* clear buffer memory. */
    memset(tmp, 0, sizeof(*tmp));

    retval =
        eroc_list_create(
            &tmp->lines, (int (*)(eroc_list_node*))&eroc_buffer_line_release);
    if (0 != retval)
    {
        goto cleanup_tmp;
    }

    *buffer = tmp;
    retval = 0;
    goto done;

cleanup_tmp:
    free(tmp);

done:
    return retval;
}
