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
#include <limits.h>

enum parse_token
{
    TOK_EOF = -1,
    TOK_UNKNOWN = 0,
    TOK_COMMAND_ADVANCE,
    TOK_COMMAND_APPEND,
    TOK_COMMAND_CHANGE,
    TOK_COMMAND_DELETE,
    TOK_COMMAND_DISPLAY_LINE_NUMBER,
    TOK_COMMAND_INSERT,
    TOK_COMMAND_MOVE,
    TOK_COMMAND_PRINT,
    TOK_COMMAND_QUIT,
    TOK_COMMAND_WRITE,
    TOK_ADDRESS,
};

typedef struct parse_address parse_address;
struct parse_address
{
    bool sign_set;
    long value;
};

typedef union parse_value parse_value;
union parse_value
{
    parse_address address;
};

static int command_token_read(
    parse_value* val, const eroc_buffer* eroc_buffer, const char** input);
static int command_set(eroc_command* command, int tok);
static int parse_numeric_address(
    parse_address* addr, const char* start, const char* end);
static int translate_relative_address(
    unsigned long* abs, const parse_address* rel, const eroc_buffer* buffer);

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
    eroc_command** command, eroc_buffer* buffer, const char* input)
{
    int retval, release_retval, tok;
    eroc_command* tmp;
    parse_value val;

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
    tmp->line = buffer->cursor;

    do {
        /* read the first command token. */
        tok = command_token_read(&val, buffer, &input);
        switch (tok)
        {
            case TOK_COMMAND_APPEND:
            case TOK_COMMAND_CHANGE:
            case TOK_COMMAND_DELETE:
            case TOK_COMMAND_DISPLAY_LINE_NUMBER:
            case TOK_COMMAND_INSERT:
            case TOK_COMMAND_PRINT:
            case TOK_COMMAND_QUIT:
            case TOK_COMMAND_WRITE:
                retval = command_set(tmp, tok);
                if (0 != retval)
                {
                    goto done;
                }
                goto success;

            case TOK_ADDRESS:
                /* TODO - add support for ranged commands. */
                retval =
                    translate_relative_address(
                        &tmp->start, &val.address, buffer);
                if (0 != retval)
                {
                    goto done;
                }
                tmp->start_provided = true;
                retval = command_set(tmp, TOK_COMMAND_MOVE);
                if (0 != retval)
                {
                    goto done;
                }
                goto success;

            case TOK_EOF:
                /* advance the cursor by 1. */
                retval = command_set(tmp, TOK_COMMAND_ADVANCE);
                if (0 != retval)
                {
                    goto done;
                }
                goto success;

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
 * \param val               The parsed value if general.
 * \param buffer            The buffer for this token.
 * \param input             The input stream, which is updated on read.
 *
 * \returns the next token read.
 */
static int command_token_read(
    parse_value* val, const eroc_buffer* buffer, const char** input)
{
    int retval;
    const char* inp = *input;
    const char* end;

    /* skip any space before the next token. */
    while (*inp && isspace(*inp))
        ++inp;

    switch (*inp)
    {
        case 0:
            *input = inp;
            return TOK_EOF;

        case '=':
            *input = inp + 1;
            return TOK_COMMAND_DISPLAY_LINE_NUMBER;

        case 'a':
            *input = inp + 1;
            return TOK_COMMAND_APPEND;

        case 'c':
            *input = inp + 1;
            return TOK_COMMAND_CHANGE;

        case 'd':
            *input = inp + 1;
            return TOK_COMMAND_DELETE;

        case 'i':
            *input = inp + 1;
            return TOK_COMMAND_INSERT;

        case 'p':
            *input = inp + 1;
            return TOK_COMMAND_PRINT;

        case 'q':
            *input = inp + 1;
            return TOK_COMMAND_QUIT;

        case 'w':
            *input = inp + 1;
            return TOK_COMMAND_WRITE;

        case '-': case '+':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            end = inp + 1;
            while (*end && isnumber(*end))
                ++end;
            retval = parse_numeric_address(&val->address, inp, end);
            if (0 != retval)
            {
                return TOK_UNKNOWN;
            }
            *input = end;
            return TOK_ADDRESS;

        case '$':
            *input = inp + 1;
            val->address.sign_set = false;
            val->address.value = buffer->lines->count;
            return TOK_ADDRESS;

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
        case TOK_COMMAND_ADVANCE:
            command->command_fn = &eroc_command_function_advance;
            return 0;

        case TOK_COMMAND_APPEND:
            command->command_fn = &eroc_command_function_append;
            return 0;

        case TOK_COMMAND_CHANGE:
            command->command_fn = &eroc_command_function_change;
            return 0;

        case TOK_COMMAND_DELETE:
            command->command_fn = &eroc_command_function_delete;
            return 0;

        case TOK_COMMAND_DISPLAY_LINE_NUMBER:
            command->command_fn = &eroc_command_function_display_line_number;
            return 0;

        case TOK_COMMAND_INSERT:
            command->command_fn = &eroc_command_function_insert;
            return 0;

        case TOK_COMMAND_MOVE:
            command->command_fn = &eroc_command_function_move;
            return 0;

        case TOK_COMMAND_PRINT:
            command->command_fn = &eroc_command_function_print;
            return 0;

        case TOK_COMMAND_QUIT:
            command->command_fn = &eroc_command_function_quit;
            return 0;

        case TOK_COMMAND_WRITE:
            command->command_fn = &eroc_command_function_write;
            return 0;

        default:
            return 3;
    }
}

/**
 * \brief Parse the address at [start,end).
 *
 * \param addr                  Pointer to the variable to hold the address on
 *                              success.
 * \param start                 The start of the address to parse.
 * \param end                   One past the end of the address.
 *
 * \returns 0 on success and non-zero on error.
 */
static int parse_numeric_address(
    parse_address* addr, const char* start, const char* end)
{
    const long max_shift_value = LONG_MAX / 10;
    long value = 0U;
    bool sign_set = false;
    bool negative = false;

    /* is the first character a sign? */
    switch (*start)
    {
        case '-':
            negative = true;
            /* fall-through. */
        case '+':
            sign_set = true;
            ++start;
            break;
    }

    /* loop until the end of input */
    for (; start != end; ++start)
    {
        int digitval = *start - '0';

        /* the value is too big to shift. */
        if (value > max_shift_value)
        {
            return 1;
        }

        /* shift in the next digit. */
        value *= 10;
        value += digitval;
    }

    /* if the sign is set and the value is 0, set it to 1. */
    if (sign_set && 0 == value)
        value = 1;

    /* negate the value if needed. */
    if (negative) value *= -1;

    /* save the address. */
    addr->value = value;
    addr->sign_set = sign_set;

    return 0;
}

/**
 * \brief Translate a relative address into an absolute address.
 *
 * \param abs               Output parameter for absolute address.
 * \param rel               The relative address to translate.
 * \param buffer            The buffer to which this relative address relates.
 */
static int translate_relative_address(
    unsigned long* abs, const parse_address* rel, const eroc_buffer* buffer)
{
    if (rel->sign_set)
    {
        long addr = buffer->lineno + rel->value;
        if (addr < 0 || addr >= (long)buffer->lines->count)
        {
            return 1;
        }

        *abs = addr;
        return 0;
    }
    else
    {
        if (rel->value < 1 || rel->value > (long)buffer->lines->count)
        {
            return 2;
        }

        *abs = rel->value - 1;
        return 0;
    }
}
