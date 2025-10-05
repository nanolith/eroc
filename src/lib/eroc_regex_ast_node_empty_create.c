/**
 * \file lib/eroc_regex_ast_node_empty_create.c
 *
 * \brief Create an empty AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create an empty AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_empty_create(eroc_regex_ast_node** node)
{
    eroc_regex_ast_node* tmp;

    tmp = (eroc_regex_ast_node*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }

    memset(tmp, 0, sizeof(*tmp));
    tmp->type = EROC_REGEX_AST_EMPTY;

    *node = tmp;
    return 0;
}
