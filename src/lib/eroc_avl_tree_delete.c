/**
 * \file lib/eroc_avl_tree_delete.c
 *
 * \brief Delete a node in the AVL tere matching the given user-defined key.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Delete a node in the AVL tree matching the given user-defined key.
 *
 * \param node          Pointer to the node pointer to be set to the deleted
 *                      node if found. Set to NULL to call the user release
 *                      method on this deleted node instead.
 * \param tree          The AVL tree for this delete operation.
 * \param key           The user-defined key for this delete operation.
 *
 * \returns true if an element was deleted and false otherwise.
 */
int eroc_avl_tree_delete(
    eroc_avl_tree_node** node, eroc_avl_tree* tree, const void* key)
{
    eroc_avl_tree_node* tmp = NULL;

    /* attempt to find a node matching the given key. */
    if (!eroc_avl_tree_find(&tmp, tree, key))
    {
        return 0;
    }

    /* remove the node. */
    eroc_avl_tree_remove_node(tree, tmp);

    /* if the node pointer is set, then transfer ownership. */
    if (NULL != node)
    {
        *node = tmp;
        return 0;
    }

    return tree->release_fn(tree->context, tmp);
}
