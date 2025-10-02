/**
 * \file lib/eroc_command_run.c
 *
 * \brief Run a command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>

/**
 * \brief Run a command.
 *
 * \param command           The command to run.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_run(eroc_command* command)
{
    return command->command_fn(command, &command->buffer->lineno);
}
