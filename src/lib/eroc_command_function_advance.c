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
    /* can't advance past the last line. */
    if (NULL == command->buffer->cursor->hdr.next)
    {
        return 1;
    }

    /* advance the cursor. */
    /* TODO - replace with buffer function. */
    command->buffer->cursor =
        (eroc_buffer_line*)command->buffer->cursor->hdr.next;
    ++command->buffer->lineno;

    printf("%s\n", command->buffer->cursor->line);

    return 0;
}
