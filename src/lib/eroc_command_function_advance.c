/**
 * \file lib/eroc_command_function_advance.c
 *
 * \brief Advance the cursor by one.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Advance the cursor by one, printing this new line.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_advance(eroc_command* command)
{
    int retval;

    /* advance the cursor. */
    retval = eroc_buffer_cursor_advance(command->buffer);
    if (0 != retval)
    {
        return retval;
    }

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
