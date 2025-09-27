/**
 * \file lib/eroc_buffer_line_create.c
 *
 * \brief Create a buffer line.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create a buffer line.
 *
 * \note This method takes ownership of the line string and will reclaim this
 * string when it is released via \ref eroc_buffer_line_release.
 *
 * \param line              Pointer to the buffer line pointer to set with this
 *                          buffer line on success.
 * \param linestr           The string representation of this line, with newline
 *                          removed.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_line_create(eroc_buffer_line** line, char* linestr)
{
    eroc_buffer_line* tmp;

    tmp = (eroc_buffer_line*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }
    memset(tmp, 0, sizeof(*tmp));
    tmp->line = linestr;

    *line = tmp;
    return 0;
}
