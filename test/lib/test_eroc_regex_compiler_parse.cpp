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

/**
 * \brief We can parse two any instructions.
 */
TEST(parse_two_anys)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "..";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CONCAT. */
    TEST_EXPECT(EROC_REGEX_AST_CONCAT == ast->type);

    /* left is not NULL. */
    auto left = ast->data.binary.left;
    TEST_ASSERT(nullptr != left);
    /* left does not have a next pointer. */
    TEST_EXPECT(nullptr == left->next);
    /* left's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == left->type);

    /* right is not NULL. */
    auto right = ast->data.binary.right;
    TEST_ASSERT(nullptr != right);
    /* right does not have a next pointer. */
    TEST_EXPECT(nullptr == right->next);
    /* right's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == right->type);

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief We can parse an alternate of two anys.
 */
TEST(parse_any_or_any)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = ".|.";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is ALTERNATE. */
    TEST_EXPECT(EROC_REGEX_AST_ALTERNATE == ast->type);

    /* left is not NULL. */
    auto left = ast->data.binary.left;
    TEST_ASSERT(nullptr != left);
    /* left does not have a next pointer. */
    TEST_EXPECT(nullptr == left->next);
    /* left's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == left->type);

    /* right is not NULL. */
    auto right = ast->data.binary.right;
    TEST_ASSERT(nullptr != right);
    /* right does not have a next pointer. */
    TEST_EXPECT(nullptr == right->next);
    /* right's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == right->type);

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief An alt must come after something.
 */
TEST(parse_left_alt_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "|.";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief Two alts is a failure.
 */
TEST(parse_alt_alt_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = ".||.";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief An alt with no resolution is an error.
 */
TEST(parse_right_alt)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = ".|";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief A start capture with no end is a failure.
 */
TEST(parse_start_capture_no_end)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "(";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief An end capture at the start is a failure.
 */
TEST(parse_end_capture_at_beginning_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = ")";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief An empty capture group is a failure
 */
TEST(parse_empty_capture_group_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "()";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief We can parse two capture groups.
 */
TEST(parse_two_capture_groups)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "(.)(.)";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CONCAT. */
    TEST_EXPECT(EROC_REGEX_AST_CONCAT == ast->type);

    /* left is not NULL. */
    auto left = ast->data.binary.left;
    TEST_ASSERT(nullptr != left);
    /* left does not have a next pointer. */
    TEST_EXPECT(nullptr == left->next);
    /* left's type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == left->type);
    /* left's group index is 0. */
    TEST_EXPECT(0 == left->data.capture.group_index);

    /* left's child is not NULL. */
    auto left_child = left->data.capture.child;
    TEST_ASSERT(nullptr != left_child);
    /* left_child does not have a next pointer. */
    TEST_EXPECT(nullptr == left_child->next);
    /* left_child's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == left_child->type);

    /* right is not NULL. */
    auto right = ast->data.binary.right;
    TEST_ASSERT(nullptr != right);
    /* right does not have a next pointer. */
    TEST_EXPECT(nullptr == right->next);
    /* right's type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == right->type);
    /* right's group index is 1. */
    TEST_EXPECT(1 == right->data.capture.group_index);

    /* right's child is not NULL. */
    auto right_child = right->data.capture.child;
    TEST_ASSERT(nullptr != right_child);
    /* right_child does not have a next pointer. */
    TEST_EXPECT(nullptr == right_child->next);
    /* right_child's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == right_child->type);

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief We can parse an alternate of two capture groups.
 */
TEST(parse_two_capture_groups_alternate)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "(.)|(.)";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CONCAT. */
    TEST_EXPECT(EROC_REGEX_AST_ALTERNATE == ast->type);

    /* left is not NULL. */
    auto left = ast->data.binary.left;
    TEST_ASSERT(nullptr != left);
    /* left does not have a next pointer. */
    TEST_EXPECT(nullptr == left->next);
    /* left's type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == left->type);
    /* left's group index is 0. */
    TEST_EXPECT(0 == left->data.capture.group_index);

    /* left's child is not NULL. */
    auto left_child = left->data.capture.child;
    TEST_ASSERT(nullptr != left_child);
    /* left_child does not have a next pointer. */
    TEST_EXPECT(nullptr == left_child->next);
    /* left_child's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == left_child->type);

    /* right is not NULL. */
    auto right = ast->data.binary.right;
    TEST_ASSERT(nullptr != right);
    /* right does not have a next pointer. */
    TEST_EXPECT(nullptr == right->next);
    /* right's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == right->type);
    /* right's group index is 1. */
    TEST_EXPECT(1 == right->data.capture.group_index);

    /* right's child is not NULL. */
    auto right_child = right->data.capture.child;
    TEST_ASSERT(nullptr != right_child);
    /* right_child does not have a next pointer. */
    TEST_EXPECT(nullptr == right_child->next);
    /* right_child's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == right_child->type);

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}
