/**
 * \file lib/eroc_buffer_cursor_advance.c
 *
 * \brief Advance the cursor forward by one.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Advance the cursor by one.
 *
 * \param buffer            The buffer for this operation.
 */
int eroc_buffer_cursor_advance(eroc_buffer* buffer)
{
    /* can't advance past the last line. */
    if (NULL == buffer->cursor->hdr.next)
    {
        return 1;
    }

    buffer->cursor = (eroc_buffer_line*)buffer->cursor->hdr.next;
    ++buffer->lineno;

    return 0;
}
