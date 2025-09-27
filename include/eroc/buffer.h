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

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
