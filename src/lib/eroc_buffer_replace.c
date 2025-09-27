/**
 * \file lib/eroc_buffer_replace.c
 *
 * \brief Replace a line in a buffer with the given new line.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>

/**
 * \brief Replace oldline with newline.
 *
 * \note After this operation, the caller owns oldline.
 *
 * \param buffer            The buffer for this replace operation.
 * \param oldline           The line to replace, owned by caller after the
 *                          operation.
 * \param newline           The line that \p oldline is replaced with, owned by
 *                          the buffer after the operation.
 */
void eroc_buffer_replace(
    eroc_buffer* buffer, eroc_buffer_line* oldline, eroc_buffer_line* newline)
{
    eroc_list_node_splice(buffer->lines, &oldline->hdr, &newline->hdr);
}
