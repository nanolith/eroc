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
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
