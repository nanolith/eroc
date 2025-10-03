/**
 * \file lib/eroc_command_function_append.c
 *
 * \brief Append lines terminated by . to the buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int read_line(char** input_line);

/**
 * \brief Append lines terminated by . to the buffer.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_append(eroc_command* command)
{
    int retval;
    char* input_line = NULL;
    eroc_buffer_line* buffer_line = NULL;

    /* is there a start address? */
    if (command->start_provided)
    {
        /* move to this start address. */
        retval = eroc_buffer_cursor_move(command->buffer, command->start);
        if (0 != retval)
        {
            return retval;
        }
    }

    for (;;)
    {
        /* read an append line from standard input. */
        retval = read_line(&input_line);
        if (retval < 0)
        {
            clearerr(stdin);
            return 1;
        }

        /* a dot terminates this append operation. */
        if (!strcmp(input_line, "."))
        {
            free(input_line);
            return 0;
        }

        /* create a buffer line. */
        retval = eroc_buffer_line_create(&buffer_line, input_line);
        if (0 != retval)
        {
            free(input_line);
            return 2;
        }

        /* append this buffer line after the cursor. */
        eroc_buffer_append(
            command->buffer, command->buffer->cursor, buffer_line);
        command->buffer->lineno += 1;
        command->buffer->cursor = buffer_line;

        /* the buffer has been modified. */
        command->buffer->flags |= EROC_BUFFER_FLAG_MODIFIED;
    }
}

/**
 * \brief Read a line from standard input.
 *
 * \param input_line            Pointer to the character pointer to store this
 *                              line on success.
 *
 * \returns 0 on success and -1 on failure.
 */
static int read_line(char** input_line)
{
    size_t linecap = 0;
    char* line = NULL;

    ssize_t read_bytes = getline(&line, &linecap, stdin);
    if (read_bytes < 0)
    {
        free(line);
        return -1;
    }
    else if (read_bytes > 0)
    {
        if ('\n' == line[read_bytes - 1])
        {
            line[read_bytes - 1] = 0;
        }
    }

    *input_line = line;
    return 0;
}
