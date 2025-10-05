/**
 * \file lib/eroc_regex_ast_node_release.c
 *
 * \brief Release a regular expression AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <stdlib.h>

/**
 * \brief Release an AST node.
 *
 * \note This release mechanism is recursive; it also releases all child nodes
 * it owns.
 *
 * \param node          The node to release.
 */
void eroc_regex_ast_node_release(eroc_regex_ast_node* node)
{
    switch (node->type)
    {
        case EROC_REGEX_AST_EMPTY:
        case EROC_REGEX_AST_LITERAL:
        case EROC_REGEX_AST_CHAR_CLASS:
            /* no sub-nodes. */
            break;

        case EROC_REGEX_AST_CONCAT:
        case EROC_REGEX_AST_ALTERNATE:
            eroc_regex_ast_node_release(node->data.binary.left);
            eroc_regex_ast_node_release(node->data.binary.right);
            break;

        case EROC_REGEX_AST_STAR:
        case EROC_REGEX_AST_PLUS:
        case EROC_REGEX_AST_OPTIONAL:
            eroc_regex_ast_node_release(node->data.unary.child);
            break;

        case EROC_REGEX_AST_CAPTURE:
            eroc_regex_ast_node_release(node->data.capture.child);
            break;
    }

    free(node);
}
