/**
 * \file lib/eroc_buffer_append.c
 *
 * \brief Append a line after the given line in a buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Append the given line to the given buffer, after the given line.
 *
 * \param buffer            The buffer for this append operation.
 * \param after             The line after this line should be appended, or NULL
 *                          if this line should be appended at the end of the
 *                          buffer.
 * \param line              The line to append.
 */
void eroc_buffer_append(
    eroc_buffer* buffer, eroc_buffer_line* after, eroc_buffer_line* line)
{
    eroc_list_append_after(buffer->lines, &after->hdr, &line->hdr);
}
