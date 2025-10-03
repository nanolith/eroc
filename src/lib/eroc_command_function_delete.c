/**
 * \file lib/eroc_command_function_delete.c
 *
 * \brief Delete lines.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>

/**
 * \brief Delete lines.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_delete(eroc_command* command)
{
    int retval;
    unsigned long start = command->buffer->lineno;
    unsigned long count = 1;

    /* is start provided? */
    if (command->start_provided)
    {
        start = command->start;
    }

    /* is end provided? */
    if (command->end_provided)
    {
        /* end must occur after start. */
        if (command->end < start)
        {
            return 1;
        }

        /* end can't exceed count. */
        if (command->end >= command->buffer->lines->count)
        {
            return 2;
        }

        /* calculate count. */
        count = command->end - start + 1;
    }

    /* move to the line indicated by start. */
    retval = eroc_buffer_cursor_move(command->buffer, start);
    if (0 != retval)
    {
        return retval;
    }

    while (count-- && NULL != command->buffer->cursor)
    {
        eroc_buffer_line_delete(command->buffer, command->buffer->cursor);
    }

    return 0;
}
