/**
 * \file lib/eroc_command_function_print.c
 *
 * \brief Run the print command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Print command function.
 *
 * \param command           The command instance.
 * \param lineno            The current line number.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_print(eroc_command* command, size_t* lineno)
{
    size_t start = *lineno;
    size_t count = 1;
    eroc_buffer_line* line = command->line;

    /* is start provided? */
    if (command->start_provided)
    {
        start = command->start;
        if (
            0 !=
                eroc_list_node_at(
                    (eroc_list_node**)&line, command->buffer->lines, start))
        {
            return 1;
        }
    }

    /* is end provided? */
    if (command->end_provided)
    {
        /* end must occur after start. */
        if (command->end < start)
        {
            return 2;
        }

        /* end can't exceed count. */
        if (command->end >= command->buffer->lines->count)
        {
            return 3;
        }

        /* calculate count. */
        count = command->end - start + 1;
    }

    while (count-- && NULL != line)
    {
        printf("%s\n", line->line);
        line = (eroc_buffer_line*)line->hdr.next;
    }

    return 0;
}
