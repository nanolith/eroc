/**
 * \file lib/eroc_buffer_save.c
 *
 * \brief Save the contents of the given buffer into the file at the given path.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/buffer.h>
#include <stdio.h>
#include <string.h>

/**
 * \brief Save the contents of the given buffer into the file at the given path.
 *
 * \param buffer            The buffer to save.
 * \param size              The number of bytes written on success.
 * \param path              The path to which the buffer is saved.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_save(const eroc_buffer* buffer, size_t* size, const char* path)
{
    int retval;
    FILE* fp;

    /* attempt to open the file for writing. */
    fp = fopen(path, "w");
    if (NULL == fp)
    {
        retval = 1;
        goto done;
    }

    /* start with 0 bytes. */
    *size = 0U;

    /* while there are lines to write, write them. */
    for (
        eroc_buffer_line* line = (eroc_buffer_line*)buffer->lines->head;
        NULL != line;
        line = (eroc_buffer_line*)line->hdr.next)
    {
        fprintf(fp, "%s\n", line->line);
        *size += strlen(line->line) + 1;
    }

    /* success. */
    retval = 0;
    goto cleanup_fp;

cleanup_fp:
    retval = fclose(fp);

done:
    return retval;
}
