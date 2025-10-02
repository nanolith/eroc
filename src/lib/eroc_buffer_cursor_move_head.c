/**
 * \file lib/eroc_buffer_move_head.c
 *
 * \brief Move the cursor to the head of the buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Move the cursor to the head of the buffer.
 *
 * \param buffer            The buffer for this operation.
 */
void eroc_buffer_cursor_move_head(eroc_buffer* buffer)
{
    buffer->cursor = (eroc_buffer_line*)buffer->lines->head;
    buffer->lineno = 0;
}
