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

static test_node* test_node_create(int key, const char* value)
{
    test_node* retval = (test_node*)malloc(sizeof(test_node));
    memset(retval, 0, sizeof(*retval));
    retval->key = key;
    retval->value = strdup(value);

    return retval;
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

/**
 * Test that we can insert a root node.
 */
TEST(root_node_insert)
{
    eroc_avl_tree* tree;

    test_node* node = test_node_create(7, "seven");

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* insert the root node. */
    eroc_avl_tree_insert(tree, &node->hdr);

    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr == tree->root->left);
    TEST_ASSERT(nullptr == tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(1 == tree->root->height);
    TEST_ASSERT(1 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}

/**
 * Test that we can insert a node to the left.
 */
TEST(left_node_insert)
{
    eroc_avl_tree* tree;

    test_node* root = test_node_create(7, "seven");
    test_node* left = test_node_create(3, "three");

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* insert the root node. */
    eroc_avl_tree_insert(tree, &root->hdr);

    /* insert a node to the left. */
    eroc_avl_tree_insert(tree, &left->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr != tree->root->left);
    TEST_ASSERT(nullptr == tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&root->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the left node. */
    TEST_ASSERT(&left->hdr == tree->root->left);
    TEST_ASSERT(nullptr == left->hdr.left);
    TEST_ASSERT(nullptr == left->hdr.right);
    TEST_ASSERT(&root->hdr == left->hdr.parent);
    TEST_ASSERT(1 == left->hdr.height);

    /* verify the count. */
    TEST_ASSERT(2 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}
