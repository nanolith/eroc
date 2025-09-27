/**
 * \file lib/eroc_buffer_insert.c
 *
 * \brief Insert a line before the given line in a buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Insert the given line into the given buffer, before the given line.
 *
 * \param buffer            The buffer for this insert operation.
 * \param before            The line before this line should be inserted, or
 *                          NULL if this line should be inserted at the
 *                          beginning of the buffer.
 * \param line              The line to insert.
 */
void eroc_buffer_insert(
    eroc_buffer* buffer, eroc_buffer_line* before, eroc_buffer_line* line)
{
    eroc_list_insert_before(buffer->lines, &before->hdr, &line->hdr);
}
