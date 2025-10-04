/**
 * \file lib/eroc_command_function_move.c
 *
 * \brief Move the cursor to the given position denoted in start.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Move the cursor to the given position denoted in start.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_move(eroc_command* command)
{
    int retval;

    /* this command requires a start address. */
    if (!command->start_provided)
    {
        return 1;
    }

    /* attempt to move to the provided address. */
    retval = eroc_buffer_cursor_move(command->buffer, command->start);
    if (0 != retval)
    {
        return retval;
    }

    /* output the line. */
    if (NULL != command->buffer->cursor)
    {
        printf("%s\n", command->buffer->cursor->line);
    }
    else
    {
        printf("\n");
    }

    return 0;
}
