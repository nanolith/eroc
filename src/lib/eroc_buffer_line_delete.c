/**
 * \file lib/eroc_buffer_line_delete.c
 *
 * \brief Delete a line from a buffer, fixing up cursor as needed.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Delete the given line from the buffer, adjusting the cursor as
 * necessary.
 *
 * \param buffer            The buffer for this delete operation.
 * \param line              The line to delete.
 */
void eroc_buffer_line_delete(eroc_buffer* buffer, eroc_buffer_line* line)
{
    /* fix up cursor first. */
    if (buffer->cursor == line)
    {
        buffer->cursor = (eroc_buffer_line*)buffer->cursor->hdr.next;
    }

    /* we don't care about the return value, because eroc_buffer_line can be
     * trivially released. */
    (void)eroc_list_node_delete(buffer->lines, &line->hdr);

    if (NULL == buffer->cursor)
    {
        buffer->cursor = (eroc_buffer_line*)buffer->lines->tail;
        buffer->lineno = buffer->lines->count;
        if (buffer->lineno > 0)
            buffer->lineno -= 1;
    }
}
