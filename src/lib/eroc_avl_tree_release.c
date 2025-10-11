/**
 * \file lib/eroc_avl_tree_release.c
 *
 * \brief Release an AVL tree instance.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>
#include <stdlib.h>

/**
 * \brief Release an \ref eroc_avl_tree instance, calling the user release
 * function on any elements in the tree.
 *
 * \param tree          The tree instance to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_release(eroc_avl_tree* tree)
{
    int retval = eroc_avl_tree_clear(tree);

    free(tree);

    return retval;
}
