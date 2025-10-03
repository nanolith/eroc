/**
 * \file eroc/main.c
 *
 * \brief Main entry point for eroc.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/command.h>
#include <eroc/buffer.h>
#include <stdlib.h>
#include <stdio.h>

eroc_buffer* global;

static int repl(void);
static int read_command(eroc_command** command, char** input_line, FILE* input);

/**
 * \brief Main entry point.
 *
 * \param argc          Argument count.
 * \param argv          Arguments.
 *
 * \returns 0 on success and non-zero on error.
 */
int main(int argc, char* argv[])
{
    int retval;
    /* TODO - parse options. */

    /* if a filename is specified, open it. */
    if (argc > 1)
    {
        size_t size = 0U;
        retval = eroc_buffer_load(&global, &size, argv[1]);
        if (0 != retval)
        {
            printf("Error loading %s.\n", argv[1]);
            return 1;
        }
        else
        {
            printf("%zu\n", size);
            retval = eroc_buffer_name_set(global, argv[1]);
            if (0 != retval)
            {
                return 2;
            }
        }
    }
    else
    {
        retval = eroc_buffer_create(&global);
        if (0 != retval)
        {
            printf("Error creating buffer.\n");
            return 1;
        }
    }

    return repl();
}

/**
 * \brief Read-Eval-Print loop for eroc.
 *
 * \returns 0 on success and non-zero on error.
 */
static int repl(void)
{
    int retval;
    eroc_command* command;
    bool first_quit = false;
    char* input_line;

    do
    {
        /* read a command from standard input. */
        retval = read_command(&command, &input_line, stdin);
        if (-1 == retval)
        {
            free(input_line);
            input_line = NULL;
            clearerr(stdin);

            if ((global->flags & EROC_BUFFER_FLAG_MODIFIED) && !first_quit)
            {
                /* ensure that the quit flag is cleared. */
                global->flags &= (~EROC_BUFFER_FLAG_QUIT_REQUESTED);
                first_quit = true;
                printf("?\n");
                continue;
            }
            else
            {
                return 0;
            }
        }
        else if (0 != retval)
        {
            printf("?\n");
            goto reset;
        }

        /* evaluate the command. */
        retval = eroc_command_run(command);
        if (0 != retval)
        {
            printf("?\n");
        }
        /* has quit been requested on a modified buffer? */
        else if (
            (global->flags & EROC_BUFFER_FLAG_QUIT_REQUESTED)
         && (global->flags & EROC_BUFFER_FLAG_MODIFIED)
         && !first_quit)
        {
            /* ensure that the quit flag is cleared. */
            global->flags &= (~EROC_BUFFER_FLAG_QUIT_REQUESTED);
            first_quit = true;
            printf("?\n");
            (void)eroc_command_release(command);
            free(input_line);
            input_line = NULL;
            continue;
        }

        /* clean up. */
        (void)eroc_command_release(command);
        free(input_line);
        input_line = NULL;

    reset:
        /* reset first quit flag. */
        first_quit = false;
    } while (0 == (global->flags & EROC_BUFFER_FLAG_QUIT_REQUESTED));

    return 0;
}

/**
 * \brief Read a command from the given file handle.
 *
 * \param command               Pointer to the pointer to be set to the created
 *                              command on success.
 * \param input_line            The input line that is read.
 * \param input                 The input file from which the command is read.
 *
 * \returns 0 on success and non-zero on error.
 */
static int read_command(
    eroc_command** command, char** input_line, FILE* input)
{
    int retval;
    size_t linecap = 0;
    ssize_t read_bytes;

    *input_line = NULL;

    read_bytes = getline(input_line, &linecap, input);
    if (read_bytes < 0)
    {
        retval = -1;
        goto done;
    }
    else if (read_bytes > 0)
    {
        if ('\n' == (*input_line)[read_bytes - 1])
        {
            (*input_line)[read_bytes - 1] = 0;
        }
    }

    retval = eroc_command_parse(command, global, *input_line);
    if (0 != retval)
    {
        goto cleanup_input_line;
    }

    /* success. */
    retval = 0;
    goto done;

cleanup_input_line:
    free(*input_line);

done:
    return retval;
}
