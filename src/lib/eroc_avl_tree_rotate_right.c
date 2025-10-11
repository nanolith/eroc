/**
 * \file lib/eroc_avl_tree_rotate_right.c
 *
 * \brief Perform a right rotation on the given subtree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

static inline int max(int lhs, int rhs)
{
    return lhs > rhs ? lhs : rhs;
}

/**
 * \brief Perform a right rotation on an AVL tree.
 *
 * Root will become root->left's right-most child, and root->left will become
 * the new root of this subtree.
 *
 * \param root          The root for this rotation.
 */
void eroc_avl_tree_rotate_right(eroc_avl_tree_node** root)
{
    eroc_avl_tree_node *root_parent = NULL, *left = NULL, *right = NULL,
                       *left_left = NULL, *left_right = NULL;

    /* cache values. */
    root_parent = (*root)->parent;
    left = (*root)->left;
    right = (*root)->right;
    left_left = left->left;
    left_right = left->right;

    /* root is now to the right of left. */
    (*root)->parent = left;
    (*root)->left = left_right;
    (*root)->right = right;
    int left_right_height = left_right ? left_right->height : 0;
    int right_height = right ? right->height : 0;
    (*root)->height = max(left_right_height, right_height) + 1;

    /* left is the new parent. */
    left->parent = root_parent;
    left->left = left_left;
    left->right = *root;
    int root_height = (*root)->height;
    int left_left_height = left_left ? left_left->height : 0;
    left->height = max(root_height, left_left_height) + 1;

    /* fix up left_right parent to point to root. */
    if (NULL != left_right)
    {
        left_right->parent = *root;
    }

    /* fix up the parent to point to left. */
    if (NULL != root_parent)
    {
        if (*root == root_parent->left)
        {
            root_parent->left = left;
        }
        else
        {
            root_parent->right = left;
        }
    }

    /* let the caller know of the new root of this subtree. */
    *root = left;
}
