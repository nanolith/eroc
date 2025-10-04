/**
 * \file lib/eroc_command_function_write.c
 *
 * \brief Run the write command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <ctype.h>
#include <eroc/command.h>
#include <stdio.h>

/**
 * \brief Write the buffer to the named file.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_write(eroc_command* command)
{
    int retval;
    size_t write_size = 0U;
    const char* name = command->parameters;

    /* skip past whitespace in name. */
    while (*name && isspace(*name))
        ++name;

    /* Default to the buffer name if this name is invalid. */
    if (0 == *name)
    {
        name = command->buffer->name;

        if (NULL == name)
        {
            return 1;
        }
    }

    /* If the buffer was previously unnamed, then set it to this name. */
    if (NULL == command->buffer->name)
    {
        retval = eroc_buffer_name_set(command->buffer, name);
        if (0 != retval)
        {
            return retval;
        }

        /* use the buffer name. */
        name = command->buffer->name;
    }

    /* attempt to write this buffer. */
    retval = eroc_buffer_save(command->buffer, &write_size, name);
    if (0 != retval)
    {
        return retval;
    }

    /* output the size. */
    printf("%zu\n", write_size);

    /* If this is the buffer name, then the buffer is no longer modified. */
    if (name == command->buffer->name)
    {
        command->buffer->flags &= ~EROC_BUFFER_FLAG_MODIFIED;
    }

    return 0;
}
