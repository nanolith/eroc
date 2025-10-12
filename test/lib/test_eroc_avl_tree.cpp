/**
 * \file test/lib/test_eroc_avl_tree.cpp
 *
 * \brief Unit tests for eroc_avl_tree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>
#include <minunit/minunit.h>
#include <stdlib.h>
#include <string.h>

TEST_SUITE(eroc_avl_tree);

typedef struct test_context test_context;
struct test_context
{
};

typedef struct test_node test_node;
struct test_node
{
    eroc_avl_tree_node hdr;
    int key;
    char* value;
};

static int test_compare(
    test_context* context, const int* lhs, const int* rhs)
{
    (void)context;
    return *lhs - *rhs;
}

static const void* test_key(
    test_context* context, const test_node* node)
{
    (void)context;

    return &node->key;
}

static int test_node_release(
    test_context* context, test_node* node)
{
    (void)context;

    free(node->value);
    free(node);

    return 0;
}

/**
 * Test that we can create and release an eroc_avl_tree instance.
 */
TEST(create_release)
{
    eroc_avl_tree* tree;

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* we can release the tree. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}
