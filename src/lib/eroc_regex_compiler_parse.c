/**
 * \file lib/eroc_regex_compiler_parse.c
 *
 * \brief Given an input string, create an AST for further processing.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>

/**
 * \brief Given an input string, create an AST for further processing.
 *
 * \param ast           Pointer to the AST pointer to be populated with the AST
 *                      on success.
 * \param input         The input string to parse.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_regex_compiler_parse(eroc_regex_ast_node** ast, const char* input)
{
    (void)ast;
    (void)input;

    return 1;
}
