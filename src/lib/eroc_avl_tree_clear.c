/**
 * \file lib/eroc_avl_tree_clear.c
 *
 * \brief Clear an AVL tree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Clear all nodes in an AVL tree, and set the count to 0.
 *
 * \param tree          The tree instance to clear.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_clear(eroc_avl_tree* tree)
{
    int retval = 0;

    /* delete all nodes in the tree if the tree is not empty. */
    if (NULL != tree->root)
    {
        retval = eroc_avl_tree_delete_nodes(tree, tree->root);
    }

    /* now the tree is empty. */
    tree->root = NULL;
    tree->count = 0;

    return retval;
}
