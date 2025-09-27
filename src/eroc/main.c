/**
 * \file eroc/main.c
 *
 * \brief Main entry point for eroc.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdio.h>

eroc_buffer* global;

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
}
