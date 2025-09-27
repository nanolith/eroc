/**
 * \file lib/eroc_buffer_line_release.c
 *
 * \brief Release a buffer line.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdlib.h>

/**
 * \brief Release a buffer line.
 *
 * \param line              The buffer line to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_line_release(eroc_buffer_line* line)
{
    free(line->line);
    free(line);

    return 0;
}
