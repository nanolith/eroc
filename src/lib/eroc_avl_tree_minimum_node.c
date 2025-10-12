/**
 * \file lib/eroc_avl_tree_minimum_node.c
 *
 * \brief Get the minimum node of a given subtree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Given a subtree node, return the minimum node in this subtree.
 *
 * \param tree          The AVL tree for this operation.
 * \param x             The node from which this search should start. Must not
 *                      be NULL.
 *
 * \returns the minimum node in this subtree.
 */
eroc_avl_tree_node* eroc_avl_tree_minimum_node(
    eroc_avl_tree* tree, eroc_avl_tree_node* x)
{
    (void)tree;

    while (NULL != x->left)
    {
        x = x->left;
    }

    return x;
}
