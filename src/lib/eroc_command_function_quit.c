/**
 * \file lib/eroc_command_function_quit.c
 *
 * \brief Run the quit command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Attempt to quit this session.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_quit(eroc_command* command)
{
    /* set the quit request flag, and let the top level decide what to do. */
    command->buffer->flags |= EROC_BUFFER_FLAG_QUIT_REQUESTED;

    return 0;
}
