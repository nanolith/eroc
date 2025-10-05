/**
 * \file lib/eroc_regex_ast_node_plus_create.c
 *
 * \brief Create a plus AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Create a plus AST node.
 *
 * \note On success, the created node takes ownership of the child node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param child         The child of this plus node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_plus_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* child)
{
    int retval = eroc_regex_ast_node_empty_create(node);
    if (0 != retval)
    {
        return retval;
    }

    (*node)->type = EROC_REGEX_AST_PLUS;
    (*node)->data.unary.child = child;

    return 0;
}
