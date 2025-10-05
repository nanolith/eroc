/**
 * \file lib/eroc_regex_ast_node_literal_create.c
 *
 * \brief Create a char literal AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Create a char literal AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param c             The character literal.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_literal_create(eroc_regex_ast_node** node, char c)
{
    int retval = eroc_regex_ast_node_empty_create(node);
    if (0 != retval)
    {
        return retval;
    }

    (*node)->type = EROC_REGEX_AST_LITERAL;
    (*node)->data.literal = c;

    return 0;
}
