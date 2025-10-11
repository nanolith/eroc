/**
 * \file lib/eroc_avl_tree_find.c
 *
 * \brief Find a node in the AVL tree matching the given user-defined key.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Find a node in the AVL tree matching the given user defined key.
 *
 * \param node          Pointer to the node pointer to set to the found element
 *                      if found.
 * \param tree          The tree instance for this find operation.
 * \param key           The user-defined key for this find operation.
 *
 * \returns true if this element was found and false otherwise.
 */
bool eroc_avl_tree_find(
    eroc_avl_tree_node** node, eroc_avl_tree* tree, const void* key)
{
    eroc_avl_tree_node* x = tree->root;

    while (x != NULL)
    {
        const void* x_key = tree->key_fn(tree->context, x);

        int compare_result = tree->compare_fn(tree->context, key, x_key);
        if (0 == compare_result)
        {
            *node = x;
            return true;
        }
        else if (compare_result < 0)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    return false;
}
