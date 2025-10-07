/**
 * \file lib/eroc_regex_compiler_instance_create.c
 *
 * \brief Create the initial compiler instance backed by the given input string.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create a compiler instance backed by the given input string.
 *
 * \note The input string remains owned by the caller.
 *
 * \param inst          Pointer to the instance pointer to be populated with
 *                      this created instance on success.
 * \param input         The input string that this compiler transforms.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_compiler_instance_create(
    eroc_regex_compiler_instance** inst, const char* input)
{
    eroc_regex_compiler_instance* tmp;

    /* allocate memory for this instance. */
    tmp = (eroc_regex_compiler_instance*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }

    /* clear instance memory. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize instance. */
    tmp->state = EROC_REGEX_COMPILER_STATE_INIT;
    tmp->input = input;

    /* success. */
    *inst = tmp;
    return 0;
}
