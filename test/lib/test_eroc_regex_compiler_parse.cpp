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
