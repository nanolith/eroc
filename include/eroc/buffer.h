/**
 * \file eroc/buffer.h
 *
 * \brief Simple buffer, based on a linked list of lines.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <eroc/list.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A buffer line is a linked list node with a string.
 */
typedef struct eroc_buffer_line eroc_buffer_line;

struct eroc_buffer_line
{
    eroc_list_node hdr;
    char* line;
};

/**
 * \brief A buffer is a linked list of buffer lines.
 */
typedef struct eroc_buffer eroc_buffer;

struct eroc_buffer
{
    eroc_list* lines;
};

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
int eroc_buffer_line_create(eroc_buffer_line** line, char* linestr);

/**
 * \brief Release a buffer line.
 *
 * \param line              The buffer line to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_line_release(eroc_buffer_line* line);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
