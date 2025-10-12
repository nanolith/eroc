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

/**
 * Test that we can insert a node to the right.
 */
TEST(right_node_insert)
{
    eroc_avl_tree* tree;

    test_node* root = test_node_create(7, "seven");
    test_node* right = test_node_create(10, "ten");

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* insert the root node. */
    eroc_avl_tree_insert(tree, &root->hdr);

    /* insert a node to the right. */
    eroc_avl_tree_insert(tree, &right->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr == tree->root->left);
    TEST_ASSERT(nullptr != tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&root->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the right node. */
    TEST_ASSERT(&right->hdr == tree->root->right);
    TEST_ASSERT(nullptr == right->hdr.left);
    TEST_ASSERT(nullptr == right->hdr.right);
    TEST_ASSERT(&root->hdr == right->hdr.parent);
    TEST_ASSERT(1 == right->hdr.height);

    /* verify the count. */
    TEST_ASSERT(2 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}

/**
 * Test that we can insert nodes to the left and right.
 */
TEST(left_right_node_insert)
{
    eroc_avl_tree* tree;

    test_node* root = test_node_create(7, "seven");
    test_node* left = test_node_create(3, "three");
    test_node* right = test_node_create(10, "ten");

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

    /* insert a node to the right. */
    eroc_avl_tree_insert(tree, &right->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr != tree->root->left);
    TEST_ASSERT(nullptr != tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&root->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the left node. */
    TEST_ASSERT(&left->hdr == tree->root->left);
    TEST_ASSERT(nullptr == left->hdr.left);
    TEST_ASSERT(nullptr == left->hdr.right);
    TEST_ASSERT(&root->hdr == left->hdr.parent);
    TEST_ASSERT(1 == left->hdr.height);

    /* verify the right node. */
    TEST_ASSERT(&right->hdr == tree->root->right);
    TEST_ASSERT(nullptr == right->hdr.left);
    TEST_ASSERT(nullptr == right->hdr.right);
    TEST_ASSERT(&root->hdr == right->hdr.parent);
    TEST_ASSERT(1 == right->hdr.height);

    /* verify the count. */
    TEST_ASSERT(3 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}

/**
 * A left-left insertion should balance the tree by rotating to the right.
 */
TEST(left_left_node_insert)
{
    eroc_avl_tree* tree;

    test_node* root = test_node_create(7, "seven");
    test_node* left = test_node_create(3, "three");
    test_node* ll = test_node_create(1, "one");

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

    /* insert a second node to the left. */
    eroc_avl_tree_insert(tree, &ll->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr != tree->root->left);
    TEST_ASSERT(nullptr != tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&left->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the left node. */
    TEST_ASSERT(&ll->hdr == tree->root->left);
    TEST_ASSERT(nullptr == ll->hdr.left);
    TEST_ASSERT(nullptr == ll->hdr.right);
    TEST_ASSERT(&left->hdr == ll->hdr.parent);
    TEST_ASSERT(1 == ll->hdr.height);

    /* verify the right node. */
    TEST_ASSERT(&root->hdr == tree->root->right);
    TEST_ASSERT(nullptr == root->hdr.left);
    TEST_ASSERT(nullptr == root->hdr.right);
    TEST_ASSERT(&left->hdr == root->hdr.parent);
    TEST_ASSERT(1 == root->hdr.height);

    /* verify the count. */
    TEST_ASSERT(3 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}

/**
 * A right-right insertion should balance the tree by rotating to the left.
 */
TEST(right_right_node_insert)
{
    eroc_avl_tree* tree;

    test_node* seven = test_node_create(7, "seven");
    test_node* ten = test_node_create(10, "ten");
    test_node* fourteen = test_node_create(14, "fourteen");

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* insert the seven node. */
    eroc_avl_tree_insert(tree, &seven->hdr);

    /* insert the ten node. */
    eroc_avl_tree_insert(tree, &ten->hdr);

    /* insert the fourteen node. */
    eroc_avl_tree_insert(tree, &fourteen->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr != tree->root->left);
    TEST_ASSERT(nullptr != tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&ten->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the seven node. */
    TEST_ASSERT(&seven->hdr == tree->root->left);
    TEST_ASSERT(nullptr == seven->hdr.left);
    TEST_ASSERT(nullptr == seven->hdr.right);
    TEST_ASSERT(&ten->hdr == seven->hdr.parent);
    TEST_ASSERT(1 == seven->hdr.height);

    /* verify the fourteen node. */
    TEST_ASSERT(&fourteen->hdr == tree->root->right);
    TEST_ASSERT(nullptr == fourteen->hdr.left);
    TEST_ASSERT(nullptr == fourteen->hdr.right);
    TEST_ASSERT(&ten->hdr == fourteen->hdr.parent);
    TEST_ASSERT(1 == fourteen->hdr.height);

    /* verify the count. */
    TEST_ASSERT(3 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}

/**
 * A little-left-big-left insertion should balance the tree by doing a
 * left-right double rotation.
 */
TEST(little_left_big_left_node_insert)
{
    eroc_avl_tree* tree;

    test_node* seven = test_node_create(7, "seven");
    test_node* one = test_node_create(1, "one");
    test_node* three = test_node_create(3, "three");

    /* we can create the tree. */
    TEST_ASSERT(
        0
            == eroc_avl_tree_create(
                    &tree, (eroc_avl_tree_compare_fn)&test_compare,
                    (eroc_avl_tree_key_fn)&test_key,
                    (eroc_avl_tree_release_fn)&test_node_release, NULL));

    /* insert the seven node. */
    eroc_avl_tree_insert(tree, &seven->hdr);

    /* insert the one node. */
    eroc_avl_tree_insert(tree, &one->hdr);

    /* insert the three node. */
    eroc_avl_tree_insert(tree, &three->hdr);

    /* verify the root node. */
    TEST_ASSERT(nullptr != tree->root);
    TEST_ASSERT(nullptr != tree->root->left);
    TEST_ASSERT(nullptr != tree->root->right);
    TEST_ASSERT(nullptr == tree->root->parent);
    TEST_ASSERT(&three->hdr == tree->root);
    TEST_ASSERT(2 == tree->root->height);

    /* verify the one node. */
    TEST_ASSERT(&one->hdr == tree->root->left);
    TEST_ASSERT(nullptr == one->hdr.left);
    TEST_ASSERT(nullptr == one->hdr.right);
    TEST_ASSERT(&three->hdr == one->hdr.parent);
    TEST_ASSERT(1 == one->hdr.height);

    /* verify the seven node. */
    TEST_ASSERT(&seven->hdr == tree->root->right);
    TEST_ASSERT(nullptr == seven->hdr.left);
    TEST_ASSERT(nullptr == seven->hdr.right);
    TEST_ASSERT(&three->hdr == seven->hdr.parent);
    TEST_ASSERT(1 == seven->hdr.height);

    /* verify the count. */
    TEST_ASSERT(3 == tree->count);

    /* clean up. */
    TEST_ASSERT(0 == eroc_avl_tree_release(tree));
}
