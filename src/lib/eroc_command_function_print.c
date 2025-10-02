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
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_print(eroc_command* command)
{
    /* TODO - we haven't added code to go to a line in the buffer yet. */
    printf("%s\n", command->line->line);

    return 0;
}
