/**
 * \file lib/eroc_command_release.c
 *
 * \brief Release a command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <stdlib.h>

/**
 * \brief Release a command.
 *
 * \param command           The command to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_release(eroc_command* command)
{
    free(command);

    return 0;
}
