/**
 * \file test/lib/test_eroc_regex_ast.cpp
 *
 * \brief Unit tests for eroc_regex_ast*.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/regex.h>
#include <minunit/minunit.h>
#include <string.h>

TEST_SUITE(eroc_regex_ast);

/**
 * \brief We should be able to create and release an empty AST node.
 */
TEST(create_release_empty)
{
    eroc_regex_ast_node* node = nullptr;

    /* we can create an empty node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_empty_create(&node));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is EMPTY. */
    TEST_EXPECT(EROC_REGEX_AST_EMPTY == node->type);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release an any AST node.
 */
TEST(create_release_any)
{
    eroc_regex_ast_node* node = nullptr;

    /* we can create an any node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_any_create(&node));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is ANY. */
    TEST_EXPECT(EROC_REGEX_AST_ANY == node->type);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a char literal AST node.
 */
TEST(create_release_literal)
{
    eroc_regex_ast_node* node = nullptr;
    const char LITERAL = 'a';

    /* we can create a char literal node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&node, LITERAL));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is LITERAL. */
    TEST_EXPECT(EROC_REGEX_AST_LITERAL == node->type);
    TEST_EXPECT(LITERAL == node->data.literal);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a concat AST node.
 */
TEST(create_release_concat)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* left = nullptr;
    eroc_regex_ast_node* right = nullptr;
    const char LEFT = 'a';
    const char RIGHT = 'b';

    /* create the left char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&left, LEFT));

    /* create the right char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&right, RIGHT));

    /* create the concat node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_concat_create(&node, left, right));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is CONCAT. */
    TEST_EXPECT(EROC_REGEX_AST_CONCAT == node->type);
    TEST_EXPECT(left == node->data.binary.left);
    TEST_EXPECT(right == node->data.binary.right);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release an alternate AST node.
 */
TEST(create_release_alternate)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* left = nullptr;
    eroc_regex_ast_node* right = nullptr;
    const char LEFT = 'a';
    const char RIGHT = 'b';

    /* create the left char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&left, LEFT));

    /* create the right char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&right, RIGHT));

    /* create the alternate node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_alternate_create(&node, left, right));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is ALTERNATE. */
    TEST_EXPECT(EROC_REGEX_AST_ALTERNATE == node->type);
    TEST_EXPECT(left == node->data.binary.left);
    TEST_EXPECT(right == node->data.binary.right);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a character class AST node.
 */
TEST(create_release_char_class)
{
    eroc_regex_ast_node* node = nullptr;
    const uint32_t members[8] = {0, 1, 0, 0, 0, 0, 0, 0 };

    /* create the char class node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_char_class_create(&node, members));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is CHAR CLASS. */
    TEST_EXPECT(EROC_REGEX_AST_CHAR_CLASS == node->type);
    TEST_EXPECT(
        0 == memcmp(members, node->data.char_class.members, sizeof(members)));

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a star AST node.
 */
TEST(create_release_star)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* child = nullptr;
    const char CHILD = 'a';

    /* create the child char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&child, CHILD));

    /* create the star node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_star_create(&node, child));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is STAR. */
    TEST_EXPECT(EROC_REGEX_AST_STAR == node->type);
    TEST_EXPECT(child == node->data.unary.child);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a plus AST node.
 */
TEST(create_release_plus)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* child = nullptr;
    const char CHILD = 'a';

    /* create the child char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&child, CHILD));

    /* create the plus node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_plus_create(&node, child));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is PLUS. */
    TEST_EXPECT(EROC_REGEX_AST_PLUS == node->type);
    TEST_EXPECT(child == node->data.unary.child);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release an optional AST node.
 */
TEST(create_release_optional)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* child = nullptr;
    const char CHILD = 'a';

    /* create the child char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&child, CHILD));

    /* create the optional node. */
    TEST_ASSERT(0 == eroc_regex_ast_node_optional_create(&node, child));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is OPTIONAL. */
    TEST_EXPECT(EROC_REGEX_AST_OPTIONAL == node->type);
    TEST_EXPECT(child == node->data.unary.child);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}

/**
 * \brief We should be able to create and release a capture AST node.
 */
TEST(create_release_capture)
{
    eroc_regex_ast_node* node = nullptr;
    eroc_regex_ast_node* child = nullptr;
    const char CHILD = 'a';
    const int GROUP_INDEX = 2;

    /* create the child char literal. */
    TEST_ASSERT(0 == eroc_regex_ast_node_literal_create(&child, CHILD));

    /* create the capture node. */
    TEST_ASSERT(
        0 == eroc_regex_ast_node_capture_create(&node, child, GROUP_INDEX));

    /* the node is not NULL. */
    TEST_ASSERT(nullptr != node);

    /* the node type is CAPTURE. */
    TEST_EXPECT(EROC_REGEX_AST_CAPTURE == node->type);
    TEST_EXPECT(child == node->data.capture.child);
    TEST_EXPECT(GROUP_INDEX == node->data.capture.group_index);

    /* we can release this node. */
    eroc_regex_ast_node_release(node);
}
