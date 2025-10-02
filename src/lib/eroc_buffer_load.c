/**
 * \file lib/eroc_buffer_load.c
 *
 * \brief Load a file at the given path into a new buffer.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \brief Attempt to load a text file with the given path into a buffer.
 *
 * \param buffer            Pointer to the buffer pointer to be set with this
 *                          loaded file on success.
 * \param size              Set to the number of bytes read on success.
 * \param path              Path to the file to load.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_load(eroc_buffer** buffer, size_t* size, const char* path)
{
    int retval, release_retval;
    size_t tmpsize = 0U;
    eroc_buffer* tmp;
    FILE* fp;

    /* attempt to open the file for reading. */
    fp = fopen(path, "r");
    if (NULL == fp)
    {
        retval = 1;
        goto done;
    }

    /* create a buffer. */
    retval = eroc_buffer_create(&tmp);
    if (0 != retval)
    {
        retval = 2;
        goto cleanup_fp;
    }

    /* while there are lines to read, read them. */
    for (;;)
    {
        size_t linecap = 0;
        char* line = NULL;
        ssize_t read_bytes = getline(&line, &linecap, fp);
        if (read_bytes < 0)
        {
            free(line);
            break;
        }
        else if (read_bytes > 0)
        {
            if ('\n' == line[read_bytes - 1])
            {
                line[read_bytes - 1] = 0;
            }
        }

        eroc_buffer_line* bufline;
        retval = eroc_buffer_line_create(&bufline, line);
        if (0 != retval)
        {
            retval = 4;
            goto cleanup_tmp;
        }

        eroc_buffer_append(tmp, NULL, bufline);
        tmpsize += read_bytes + 1;
    }

    /* success. */
    *buffer = tmp;
    *size = tmpsize;
    retval = 0;
    goto cleanup_fp;

cleanup_tmp:
    release_retval = eroc_buffer_release(tmp);
    if (0 != release_retval)
    {
        retval = release_retval;
    }

cleanup_fp:
    release_retval = fclose(fp);
    if (0 != release_retval)
    {
        /* if the file can't be closed, this is an error. */
        if (0 == retval)
        {
            eroc_buffer_release(tmp);
            *buffer = NULL;
            *size = 0;
        }

        retval = 3;
    }

done:
    return retval;
}
