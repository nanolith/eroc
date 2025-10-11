/**
 * \file lib/eroc_avl_tree_delete_nodes.c
 *
 * \brief Recursively release all nodes under a given node as well as this node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/**
 * \brief Recursively release all nodes under a given node as well as this node
 * in a tree.
 *
 * \param tree          The tree instance for this delete operation.
 * \param node          The node to recursively delete.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_delete_nodes(eroc_avl_tree* tree, eroc_avl_tree_node* node)
{
    int retval = 0;
    int release_retval;

    /* recursively delete the left branch. */
    if (NULL != node->left)
    {
        release_retval = eroc_avl_tree_delete_nodes(tree, node->left);
        if (0 != release_retval)
        {
            retval = release_retval;
        }
    }

    /* recursively delete the right branch. */
    if (NULL != node->right)
    {
        release_retval = eroc_avl_tree_delete_nodes(tree, node->right);
        if (0 != release_retval)
        {
            retval = release_retval;
        }
    }

    /* release this node. */
    release_retval = tree->release_fn(tree->context, node);
    if (0 != release_retval)
    {
        retval = release_retval;
    }

    /* return decoded return value. */
    return retval;
}
