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

/**
 * \brief We can parse a capture group holding a concat.
 */
TEST(parse_capture_of_concat)
{
    eroc_regex_ast_node* outer;
    const char* INPUT = "(..)";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&outer, INPUT));

    /* outer is not NULL. */
    TEST_ASSERT(nullptr != outer);
    /* outer does not have a next pointer. */
    TEST_EXPECT(nullptr == outer->next);
    /* outer's type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == outer->type);
    /* outer's group index is 0. */
    TEST_EXPECT(0 == outer->data.capture.group_index);

    /* inner is not NULL. */
    auto inner = outer->data.capture.child;
    TEST_ASSERT(nullptr != inner);
    /* inner does not have a next pointer. */
    TEST_EXPECT(nullptr == inner->next);
    /* inner's type is CONCAT. */
    TEST_EXPECT(EROC_REGEX_AST_CONCAT == inner->type);

    /* inner_left is not NULL. */
    auto inner_left = inner->data.binary.left;
    TEST_ASSERT(nullptr != inner_left);
    /* inner_left does not have a next pointer. */
    TEST_EXPECT(nullptr == inner_left->next);
    /* inner_left's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == inner_left->type);

    /* inner_right is not NULL. */
    auto inner_right = inner->data.binary.right;
    TEST_ASSERT(nullptr != inner_right);
    /* inner_right does not have a next pointer. */
    TEST_EXPECT(nullptr == inner_right->next);
    /* inner_right's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == inner_right->type);

    /* clean up. */
    eroc_regex_ast_node_release(outer);
}

/**
 * \brief We can parse a capture group holding an alternate.
 */
TEST(parse_capture_of_alternate)
{
    eroc_regex_ast_node* outer;
    const char* INPUT = "(.|.)";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&outer, INPUT));

    /* outer is not NULL. */
    TEST_ASSERT(nullptr != outer);
    /* outer does not have a next pointer. */
    TEST_EXPECT(nullptr == outer->next);
    /* outer's type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == outer->type);
    /* outer's group index is 0. */
    TEST_EXPECT(0 == outer->data.capture.group_index);

    /* inner is not NULL. */
    auto inner = outer->data.capture.child;
    TEST_ASSERT(nullptr != inner);
    /* inner does not have a next pointer. */
    TEST_EXPECT(nullptr == inner->next);
    /* inner's type is ALTERNATE. */
    TEST_EXPECT(EROC_REGEX_AST_ALTERNATE == inner->type);

    /* inner_left is not NULL. */
    auto inner_left = inner->data.binary.left;
    TEST_ASSERT(nullptr != inner_left);
    /* inner_left does not have a next pointer. */
    TEST_EXPECT(nullptr == inner_left->next);
    /* inner_left's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == inner_left->type);

    /* inner_right is not NULL. */
    auto inner_right = inner->data.binary.right;
    TEST_ASSERT(nullptr != inner_right);
    /* inner_right does not have a next pointer. */
    TEST_EXPECT(nullptr == inner_right->next);
    /* inner_right's type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == inner_right->type);

    /* clean up. */
    eroc_regex_ast_node_release(outer);
}

/**
 * \brief A start char class without an end is an error.
 */
TEST(parse_start_char_class_no_end_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief A char class with no members is an error.
 */
TEST(parse_char_class_no_members_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[]";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief An inverse char class with no members is an error.
 */
TEST(parse_inverse_char_class_no_members_failure)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[^]";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief We can parse an inverse caret character class.
 */
TEST(parse_inverse_caret_char_class)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[^^]";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CHAR_CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == ast->type);
    /* this is an inverse char class. */
    TEST_EXPECT(ast->data.char_class.inverse);
    /* caret is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '^'));

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief We can parse a character class with multiple characters.
 */
TEST(parse_char_class_multi)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[abc]";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CHAR_CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == ast->type);
    /* this is NOT an inverse char class. */
    TEST_EXPECT(!ast->data.char_class.inverse);
    /* 'a' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'a'));
    /* 'b' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'b'));
    /* 'c' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'c'));

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief We can parse an inverse character class with multiple characters.
 */
TEST(parse_inverse_char_class_multi)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[^abc]";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CHAR_CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == ast->type);
    /* this is an inverse char class. */
    TEST_EXPECT(ast->data.char_class.inverse);
    /* 'a' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'a'));
    /* 'b' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'b'));
    /* 'c' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, 'c'));

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief We can parse a ranged character class.
 */
TEST(parse_char_class_range)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[0-9]";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CHAR_CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == ast->type);
    /* this is NOT an inverse char class. */
    TEST_EXPECT(!ast->data.char_class.inverse);
    /* '0' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '0'));
    /* '1' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '1'));
    /* '2' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '2'));
    /* '3' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '3'));
    /* '4' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '4'));
    /* '5' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '5'));
    /* '6' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '6'));
    /* '7' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '7'));
    /* '8' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '8'));
    /* '9' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '9'));

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}

/**
 * \brief A reversed range is an error.
 */
TEST(parse_reversed_range_error)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[9-0]";

    TEST_ASSERT(0 != eroc_regex_compiler_parse(&ast, INPUT));
}

/**
 * \brief We can parse the +,- ranged character class.
 */
TEST(parse_char_class_range_plus_comma_minus)
{
    eroc_regex_ast_node* ast;
    const char* INPUT = "[+--]";

    TEST_ASSERT(0 == eroc_regex_compiler_parse(&ast, INPUT));

    /* ast is not NULL. */
    TEST_ASSERT(nullptr != ast);
    /* ast does not have a next pointer. */
    TEST_EXPECT(nullptr == ast->next);
    /* ast's type is CHAR_CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == ast->type);
    /* this is NOT an inverse char class. */
    TEST_EXPECT(!ast->data.char_class.inverse);
    /* '+' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '+'));
    /* ',' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, ','));
    /* '-' is a member of this character class. */
    TEST_EXPECT(eroc_regex_ast_char_class_member_check(ast, '-'));

    /* clean up. */
    eroc_regex_ast_node_release(ast);
}
