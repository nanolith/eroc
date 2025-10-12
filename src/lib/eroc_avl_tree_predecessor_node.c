/**
 * \file lib/eroc_avl_tree_predecessor_node.c
 *
 * \brief Return the in-order predecessor node of the given node, or NULL.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Return the in-order predecessor node of the given node, or NULL.
 *
 * \param tree          The AVL tree for this operation.
 * \param x             The node from which this predecessor is found.
 *
 * \returns the predecessor node of this node, or NULL if none is found.
 */
eroc_avl_tree_node* eroc_avl_tree_predecessor_node(
    eroc_avl_tree* tree, eroc_avl_tree_node* x)
{
    if (NULL != x->left)
    {
        return eroc_avl_tree_maximum_node(tree, x->left);
    }

    eroc_avl_tree_node* y = x->parent;

    while (NULL != y && x == y->left)
    {
        x = y;
        y = y->parent;
    }

    return y;
}
