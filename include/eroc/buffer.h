/**
 * \file eroc/buffer.h
 *
 * \brief Simple buffer, based on a linked list of lines.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <eroc/list.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief A buffer line is a linked list node with a string.
 */
typedef struct eroc_buffer_line eroc_buffer_line;

struct eroc_buffer_line
{
    eroc_list_node hdr;
    char* line;
};

/**
 * \brief A buffer is a linked list of buffer lines.
 */
typedef struct eroc_buffer eroc_buffer;

struct eroc_buffer
{
    eroc_list* lines;
    char* name;
};

/**
 * \brief Create a buffer line.
 *
 * \note This method takes ownership of the line string and will reclaim this
 * string when it is released via \ref eroc_buffer_line_release.
 *
 * \param line              Pointer to the buffer line pointer to set with this
 *                          buffer line on success.
 * \param linestr           The string representation of this line, with newline
 *                          removed.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_line_create(eroc_buffer_line** line, char* linestr);

/**
 * \brief Release a buffer line.
 *
 * \param line              The buffer line to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_line_release(eroc_buffer_line* line);

/**
 * \brief Create an empty buffer.
 *
 * \param buffer            Pointer to the buffer pointer to set with this
 *                          buffer on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_create(eroc_buffer** buffer);

/**
 * \brief Release a buffer.
 *
 * \param buffer            The buffer to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_release(eroc_buffer* buffer);

/**
 * \brief Append the given line to the given buffer, after the given line.
 *
 * \param buffer            The buffer for this append operation.
 * \param after             The line after this line should be appended, or NULL
 *                          if this line should be appended at the end of the
 *                          buffer.
 * \param line              The line to append.
 */
void eroc_buffer_append(
    eroc_buffer* buffer, eroc_buffer_line* after, eroc_buffer_line* line);

/**
 * \brief Insert the given line into the given buffer, before the given line.
 *
 * \param buffer            The buffer for this insert operation.
 * \param before            The line before this line should be inserted, or
 *                          NULL if this line should be inserted at the
 *                          beginning of the buffer.
 * \param line              The line to insert.
 */
void eroc_buffer_insert(
    eroc_buffer* buffer, eroc_buffer_line* before, eroc_buffer_line* line);

/**
 * \brief Replace oldline with newline.
 *
 * \note After this operation, the caller owns oldline.
 *
 * \param buffer            The buffer for this replace operation.
 * \param oldline           The line to replace, owned by caller after the
 *                          operation.
 * \param newline           The line that \p oldline is replaced with, owned by
 *                          the buffer after the operation.
 */
void eroc_buffer_replace(
    eroc_buffer* buffer, eroc_buffer_line* oldline, eroc_buffer_line* newline);

/**
 * \brief Attempt to load a text file with the given path into a buffer.
 *
 * \param buffer            Pointer to the buffer pointer to be set with this
 *                          loaded file on success.
 * \param path              Path to the file to load.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_buffer_load(eroc_buffer** buffer, const char* path);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
