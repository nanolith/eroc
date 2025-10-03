/**
 * \file lib/eroc_command_function_display_line_number.c
 *
 * \brief Move the cursor to the given position denoted in start.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Display the line number.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_display_line_number(eroc_command* command)
{
    unsigned long lineno;

    /* if the end address is provided, use it for the line number. */
    if (command->end_provided)
    {
        lineno = command->end;
    }
    /* if the start line is provided, use it. */
    else if (command->start_provided)
    {
        lineno = command->start;
    }
    /* otherwise, use the current line number. */
    else
    {
        lineno = command->buffer->lineno;
    }

    /* it's an error if this line number is out of range. */
    if (lineno > command->buffer->lines->count)
    {
        return 1;
    }

    printf("%lu\n", lineno + 1);

    return 0;
}
