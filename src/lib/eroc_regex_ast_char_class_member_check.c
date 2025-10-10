/**
 * \file lib/eroc_regex_ast_char_class_member_check.c
 *
 * \brief Check for membership of a character in the given char class AST node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Given a char class AST node and a character, check to see if this
 * character is part of this character class.
 *
 * \note this method does not consider inverse.
 *
 * \param ast           The character class AST node for this operation.
 * \param ch            The character to check.
 *
 * \returns true if this character is part of this class and false otherwise.
 */
bool eroc_regex_ast_char_class_member_check(
    const eroc_regex_ast_node* ast, char ch)
{
    unsigned int c = (unsigned char)ch;
    size_t index = c / 32;
    size_t bit = c % 32;

    /* verify that this is a char class. */
    if (EROC_REGEX_AST_CHAR_CLASS != ast->type)
    {
        return false;
    }

    return (ast->data.char_class.members[index] & (1 << bit));
}
