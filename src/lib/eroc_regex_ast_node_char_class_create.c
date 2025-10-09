/**
 * \file lib/eroc_regex_ast_node_char_class_create.c
 *
 * \brief Create a char class AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <string.h>

/**
 * \brief Create an empty character class AST node.
 *
 * \param node          Pointer to the AST node pointer to set to the created
 *                      node on success.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_node_char_class_create(eroc_regex_ast_node** node)
{
    int retval = eroc_regex_ast_node_empty_create(node);
    if (0 != retval)
    {
        return retval;
    }

    (*node)->type = EROC_REGEX_AST_CHAR_CLASS;
    memset(
        (*node)->data.char_class.members, 0,
        sizeof((*node)->data.char_class.members));

    return 0;
}
