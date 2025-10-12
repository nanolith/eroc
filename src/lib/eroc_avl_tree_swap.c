/**
 * \file lib/eroc_avl_tree_swap.c
 *
 * \brief Swap the contents of two AVL tree instances.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>
#include <string.h>

/**
 * \brief Swap the contents of two AVL tree instances.
 *
 * \param left          The left AVL tree for the swap.
 * \param right         The right AVL tree for the swap.
 */
void eroc_avl_tree_swap(eroc_avl_tree* left, eroc_avl_tree* right)
{
    eroc_avl_tree tmp;

    memcpy(&tmp, left, sizeof(tmp));
    memcpy(left, right, sizeof(tmp));
    memcpy(right, &tmp, sizeof(tmp));
}
