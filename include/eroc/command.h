/**
 * \file eroc/command.h
 *
 * \brief Command parsing, invocation, and simple undo.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <eroc/buffer.h>
#include <stdbool.h>
#include <stddef.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A command consists of an optional start location, optional end
 * location, command function, buffer, line, and optional parameters.
 */
typedef struct eroc_command eroc_command;

/**
 * \brief A command function takes a command structure as an argument.
 */
typedef int (*eroc_command_fn)(eroc_command* cmd);

struct eroc_command
{
    size_t start;
    size_t end;
    bool start_provided;
    bool end_provided;
    eroc_buffer* buffer;
    eroc_buffer_line* line;
    eroc_buffer_line* undo;
    char* parameters;
    eroc_command_fn command;
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
