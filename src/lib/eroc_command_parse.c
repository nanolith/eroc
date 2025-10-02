/**
 * \file lib/eroc_command_parse.c
 *
 * \brief Parse a command.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <ctype.h>
#include <eroc/command.h>
#include <stdlib.h>
#include <string.h>

enum parse_token
{
    TOK_EOF = -1,
    TOK_UNKNOWN = 0,
    TOK_COMMAND_PRINT,
    TOK_COMMAND_QUIT,
};

static int command_token_read(const char** input);
static int command_set(eroc_command* command, int tok);

/**
 * \brief Given a command string, a buffer, and the current line, parse the
 * string into a command.
 *
 * \param command           Pointer to the command pointer to set with this
 *                          command on success.
 * \param buffer            The buffer on which this command is executed.
 * \param line              The current line.
 * \param input             The input string to parse.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_command_parse(
    eroc_command** command, eroc_buffer* buffer, eroc_buffer_line* line,
    const char* input)
{
    int retval, release_retval, tok;
    eroc_command* tmp;

    /* create the command to populate. */
    tmp = (eroc_command*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        retval = 1;
        goto done;
    }

    /* Set initial fields. */
    memset(tmp, 0, sizeof(*tmp));
    tmp->buffer = buffer;
    tmp->line = line;

    do {
        /* read the first command token. */
        tok = command_token_read(&input);
        switch (tok)
        {
            case TOK_COMMAND_PRINT:
            case TOK_COMMAND_QUIT:
                retval = command_set(tmp, tok);
                if (0 != retval)
                {
                    goto done;
                }
                goto success;

            case TOK_EOF:
            case TOK_UNKNOWN:
                retval = 2;
                goto done;
        }
    } while (TOK_EOF != tok);

success:
    retval = 0;
    tmp->parameters = input;
    *command = tmp;
    tmp = NULL;

done:
    if (NULL != tmp)
    {
        release_retval = eroc_command_release(tmp);
        if (0 != release_retval)
        {
            retval = release_retval;
        }
    }

    return retval;
}

/**
 * \brief Read the next token from the stream.
 *
 * \param input             The input stream, which is updated on read.
 *
 * \returns the next token read.
 */
static int command_token_read(const char** input)
{
    const char* inp = *input;

    /* skip any space before the next token. */
    while (*inp && isspace(*inp))
        ++inp;

    switch (*inp)
    {
        case 0:
            *input = inp;
            return TOK_EOF;

        case 'p':
            *input = inp + 1;
            return TOK_COMMAND_PRINT;

        case 'q':
            *input = inp + 1;
            return TOK_COMMAND_QUIT;

        default:
            *input = inp;
            return TOK_UNKNOWN;
    }
}

/**
 * \brief Set the command based on the token.
 *
 * \param command               The command structure for this operation.
 * \param tok                   The token to map to a command function.
 *
 * \returns 0 on success and non-zero on error.
 */
static int command_set(eroc_command* command, int tok)
{
    switch (tok)
    {
        case TOK_COMMAND_PRINT:
            command->command_fn = &eroc_command_function_print;
            return 0;

        case TOK_COMMAND_QUIT:
            command->command_fn = &eroc_command_function_quit;
            return 0;

        default:
            return 3;
    }
}
