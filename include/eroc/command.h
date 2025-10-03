/**
 * \file eroc/command.h
 *
 * \brief Command parsing and invocation.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <eroc/buffer.h>
#include <stdbool.h>
#include <stddef.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A command consists of an optional start location, optional end
 * location, command function, buffer, line, and optional parameters.
 */
typedef struct eroc_command eroc_command;

/**
 * \brief A command function takes a command structure as an argument.
 */
typedef int (*eroc_command_fn)(eroc_command* cmd);

struct eroc_command
{
    size_t start;
    size_t end;
    bool start_provided;
    bool end_provided;
    eroc_buffer* buffer;
    eroc_buffer_line* line;
    const char* parameters;
    eroc_command_fn command_fn;
};

/**
 * \brief Given a command string and a buffer, parse the string into a command.
 *
 * \param command           Pointer to the command pointer to set with this
 *                          command on success.
 * \param buffer            The buffer on which this command is executed.
 * \param input             The input string to parse.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_parse(
    eroc_command** command, eroc_buffer* buffer, const char* input);

/**
 * \brief Release a command.
 *
 * \param command           The command to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_release(eroc_command* command);

/**
 * \brief Run a command.
 *
 * \param command           The command to run.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_run(eroc_command* command);

/**
 * \brief Print command function.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_print(eroc_command* command);

/**
 * \brief Advance the cursor by one, printing this new line.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_advance(eroc_command* command);

/**
 * \brief Move the cursor to the given position denoted in start.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_move(eroc_command* command);

/**
 * \brief Attempt to quit this session.
 *
 * \param command           The command instance.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_function_quit(eroc_command* command);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
