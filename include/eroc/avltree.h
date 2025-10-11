/**
 * \file eroc/avltree.h
 *
 * \brief AVL tree implementation for eroc.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

/**
 * \brief Type erased AVL tree node
 */
typedef struct eroc_avl_tree_node eroc_avl_tree_node;

struct eroc_avl_tree_node
{
    eroc_avl_tree_node* parent;
    eroc_avl_tree_node* left;
    eroc_avl_tree_node* right;
    int height;
};

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
