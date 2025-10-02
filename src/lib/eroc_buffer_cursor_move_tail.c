/**
 * \file lib/eroc_buffer_move_tail.c
 *
 * \brief Move the cursor to the tail of the buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Move the cursor to the tail of the buffer.
 *
 * \param buffer            The buffer for this operation.
 */
void eroc_buffer_cursor_move_tail(eroc_buffer* buffer)
{
    buffer->cursor = (eroc_buffer_line*)buffer->lines->tail;

    if (buffer->lines->count > 0)
    {
        buffer->lineno = buffer->lines->count - 1;
    }
    else
    {
        buffer->lineno = 0;
    }
}
