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

enum eroc_regex_ast_node_type
{
    EROC_REGEX_AST_EMPTY,
    EROC_REGEX_AST_LITERAL,
    EROC_REGEX_AST_CONCAT,
    EROC_REGEX_AST_ALTERNATE,
    EROC_REGEX_AST_STAR,
    EROC_REGEX_AST_PLUS,
    EROC_REGEX_AST_OPTIONAL,
    EROC_REGEX_AST_CAPTURE
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
