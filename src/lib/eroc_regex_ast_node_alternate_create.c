/**
 * \file lib/eroc_regex_ast_node_alternate_create.c
 *
 * \brief Create an alternate AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Create an alternate AST node.
 *
 * \note On success, the created node takes ownership of the left and right
 * nodes.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param left          The left-hand side of the alternate.
 * \param right         The right-hand side of the alternate.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_alternate_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* left,
    eroc_regex_ast_node* right)
{
    int retval = eroc_regex_ast_node_empty_create(node);
    if (0 != retval)
    {
        return retval;
    }

    (*node)->type = EROC_REGEX_AST_ALTERNATE;
    (*node)->data.binary.left = left;
    (*node)->data.binary.right = right;

    return 0;
}
