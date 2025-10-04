/**
 * \file lib/eroc_command_function_change.c
 *
 * \brief Change lines to a new set of lines.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * \brief Change lines.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_change(eroc_command* command)
{
    int retval;
    bool last_line = false;

    /* is this the last line in the buffer? */
    if (NULL == command->buffer->cursor->hdr.next)
    {
        last_line = true;
    }

    /* first, delete all lines in this range. */
    retval = eroc_command_function_delete(command);
    if (0 != retval)
    {
        return retval;
    }

    /* at this point, we are only using the cursor for the point, so the start
     * and end lines no longer matter. */
    command->start_provided = false;
    command->end_provided = false;

    /* if this was the last line, make sure to append instead of insert. */
    if (last_line)
    {
        return eroc_command_function_append(command);
    }
    /* the cursor should be placed AFTER this range, so now insert new lines. */
    {
        return eroc_command_function_insert(command);
    }
}
