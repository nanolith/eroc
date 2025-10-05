/**
 * \file eroc/regex.h
 *
 * \brief Simple and opinionated DFA regular expression parser, compiler, and
 * runtime.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Types of AST nodes supported by the regular expression parser.
 */
enum eroc_regex_ast_node_type
{
    EROC_REGEX_AST_EMPTY,
    EROC_REGEX_AST_LITERAL,
    EROC_REGEX_AST_CONCAT,
    EROC_REGEX_AST_ALTERNATE,
    EROC_REGEX_AST_CHAR_CLASS,
    EROC_REGEX_AST_STAR,
    EROC_REGEX_AST_PLUS,
    EROC_REGEX_AST_OPTIONAL,
    EROC_REGEX_AST_CAPTURE
};

/**
 * \brief The regular expression AST node is used by the parser to represent a
 * regular expression operation.
 */
typedef struct eroc_regex_ast_node eroc_regex_ast_node;

struct eroc_regex_ast_node
{
    int type;
    union
    {
        char literal;
        struct
        {
            eroc_regex_ast_node* left;
            eroc_regex_ast_node* right;
        } binary;
    } data;
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
