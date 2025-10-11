/**
 * \file lib/eroc_avl_tree_rotate_left.c
 *
 * \brief Perform a left rotation on the given subtree.
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
 * \brief Perform a left rotation on an AVL tree.
 *
 * Root will become root->right's left-most child, and root->right will become
 * the new root of this subtree.
 *
 * \param root          The root for this rotation.
 */
void eroc_avl_tree_rotate_left(eroc_avl_tree_node** root)
{
    eroc_avl_tree_node *root_parent = NULL, *left = NULL, *right = NULL,
                       *right_left = NULL, *right_right = NULL;

    /* cache values. */
    root_parent = (*root)->parent;
    left = (*root)->left;
    right = (*root)->right;
    right_left = right->left;
    right_right = right->right;

    /* root is now to the left of right. */
    (*root)->parent = right;
    (*root)->left = left;
    (*root)->right = right_left;
    int left_height = left ? left->height : 0;
    int right_left_height = right_left ? right_left->height : 0;
    (*root)->height = max(left_height, right_left_height) + 1;

    /* right is the new parent. */
    right->parent = root_parent;
    right->left = *root;
    right->right = right_right;
    int root_height = (*root)->height;
    int right_right_height = right_right ? right_right->height : 0;
    right->height = max(root_height, right_right_height) + 1;

    /* fix up right_left to point to root. */
    if (NULL != right_left)
    {
        right_left->parent = *root;
    }

    /* fix up the parent to point to right. */
    if (NULL != root_parent)
    {
        if (*root == root_parent->left)
        {
            root_parent->left = right;
        }
        else
        {
            root_parent->right = right;
        }
    }

    /* let the caller know of the new root of this subtree. */
    *root = right;
}
