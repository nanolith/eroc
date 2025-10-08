/**
 * \file test/lib/test_eroc_regex_compiler_parse.cpp
 *
 * \brief Unit tests for regular expression compiler parser.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(eroc_regex_compiler_parse);

/**
 * \brief Parsing an empty string returns an error.
 */
TEST(parse_empty_string_error)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief We can parse an any instruction.
 */
TEST(parse_single_any)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = ".";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == ast->type);

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}
