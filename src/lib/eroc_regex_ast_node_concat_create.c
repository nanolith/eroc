/**
 * \file lib/eroc_regex_ast_node_concat_create.c
 *
 * \brief Create a concat AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Create a concat AST node.
 *
 * \note On success, the created node takes ownership of the left and right
 * nodes.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 * \param left          The left-hand side of the concat.
 * \param right         The right-hand side of the concat.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_concat_create(
    eroc_regex_ast_node** node, eroc_regex_ast_node* left,
    eroc_regex_ast_node* right)
{
    int retval = eroc_regex_ast_node_empty_create(node);
    if (0 != retval)
    {
        return retval;
    }

    (*node)->type = EROC_REGEX_AST_CONCAT;
    (*node)->data.binary.left = left;
    (*node)->data.binary.right = right;

    return 0;
}
