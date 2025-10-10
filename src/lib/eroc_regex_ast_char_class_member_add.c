/**
 * \file lib/eroc_regex_ast_char_class_member_add.c
 *
 * \brief Add a character to the given character class AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Given a char class AST node and a character, add this character to the
 * character class.
 *
 * \param ast           The character class AST node for this operation.
 * \param ch            The character to add to this class.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_ast_char_class_member_add(eroc_regex_ast_node* ast, char ch)
{
    unsigned int c = (unsigned char)ch;
    size_t index = c / 32;
    size_t bit = c % 32;

    /* verify that this is a char class. */
    if (EROC_REGEX_AST_CHAR_CLASS != ast->type)
    {
        return 1;
    }

    /* set the appropriate membership bit. */
    ast->data.char_class.members[index] |= 1 << bit;

    return 0;
}
