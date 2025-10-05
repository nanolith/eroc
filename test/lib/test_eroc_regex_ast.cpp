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
