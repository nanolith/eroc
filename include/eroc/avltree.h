/**
 * \file eroc/avltree.h
 *
 * \brief AVL tree implementation for eroc.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <stddef.h>

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Type erased AVL tree node.
 */
typedef struct eroc_avl_tree_node eroc_avl_tree_node;

struct eroc_avl_tree_node
{
    eroc_avl_tree_node* parent;
    eroc_avl_tree_node* left;
    eroc_avl_tree_node* right;
    int height;
};

/**
 * \brief Comparison function type for comparing two AVL tree keys.
 *
 * \param context       Context data to be passed to the comparison function.
 * \param lhs           The left-hand key of the comparison.
 * \param rhs           The right-hand key of the comparison.
 *
 * \returns an integer value representing the comparison result.
 *      - &lt; 0 if \p lhs &lt; \p rhs.
 *      - 0 if \p lhs == \p rhs.
 *      - &gt; 0 if \p lhs &gt; \p rhs.
 */
typedef int (*eroc_avl_tree_compare_fn)(
    void* context, const void* lhs, const void* rhs);

/**
 * \brief Given an AVL tree node, return the key for this node.
 *
 * \param context       Context data to be passed to the accessor function.
 * \param node          The node.
 *
 * \returns the key for this node.
 */
typedef const void* (*eroc_avl_tree_key_fn)(void* context, const void* elem);

/**
 * \brief Given an AVL tree node, release it.
 *
 * \param context       Context data to be passed to the release function.
 * \param node          Teh node to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
typedef int (*eroc_avl_tree_release_fn)(
    void* context, eroc_avl_tree_node* node);

/**
 * \brief AVL tree.
 */
typedef struct eroc_avl_tree eroc_avl_tree;

struct eroc_avl_tree
{
    eroc_avl_tree_compare_fn compare_fn;
    eroc_avl_tree_key_fn key_fn;
    eroc_avl_tree_release_fn release_fn;
    void* context;
    eroc_avl_tree_node* root;
    size_t count;
};

/**
 * \brief Create a new \ref eroc_avl_tree instance.
 *
 * \param tree          Pointer to the \ref eroc_avl_tree pointer to set to the
 *                      created instance on success.
 * \param compare_fn    The comparison function to use to compare keys.
 * \param key_fn        Function to get a key for a given tree node.
 * \param release_fn    Function to release a tree node.
 * \param context       The user context for this tree.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_create(
    eroc_avl_tree** tree, eroc_avl_tree_compare_fn compare_fn,
    eroc_avl_tree_key_fn key_fn, eroc_avl_tree_release_fn release_fn,
    void* context);

/**
 * \brief Release an \ref eroc_avl_tree instance, calling the user release
 * function on any elements in the tree.
 *
 * \param tree          The tree instance to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_release(eroc_avl_tree* tree);

/**
 * \brief Clear all nodes in an AVL tree, and set the count to 0.
 *
 * \param tree          The tree instance to clear.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_clear(eroc_avl_tree* tree);

/**
 * \brief Recursively release all nodes under a given node as well as this node
 * in a tree.
 *
 * \param tree          The tree instance for this delete operation.
 * \param node          The node to recursively delete.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_delete_nodes(eroc_avl_tree* tree, eroc_avl_tree_node* node);

/**
 * \brief Insert a node into the AVL tree instance.
 *
 * \note The AVL tree takes ownership of this node.
 *
 * \param tree          The tree instance for this insert operation.
 * \param node          The node to insert.
 */
void eroc_avl_tree_insert(eroc_avl_tree* tree, eroc_avl_tree_node* node);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
