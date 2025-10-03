/**
 * \file lib/eroc_buffer_cursor_move.c
 *
 * \brief Move the cursor to the given zero-indexed line number.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Move the cursor to the given zero-indexed line number.
 *
 * \param buffer            The buffer for this operation.
 * \param lineno            The line number for this buffer.
 */
int eroc_buffer_cursor_move(eroc_buffer* buffer, unsigned long lineno)
{
    int retval =
        eroc_list_node_at(
            (eroc_list_node**)&buffer->cursor, buffer->lines, lineno);
    if (0 != retval)
    {
        return retval;
    }

    buffer->lineno = lineno;
    return 0;
}
