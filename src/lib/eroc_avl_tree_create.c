/**
 * \file lib/eroc_avl_tree_create.c
 *
 * \brief Create a new \ref eroc_avl_tree instance.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create a new \ref eroc_avl_tree instance.
 *
 * \param tree          Pointer to the \ref eroc_avl_tree pointer to set to the
 *                      created instance on success.
 * \param compare_fn    The comparison function to use to compare keys.
 * \param key_fn        Function to get a key for a given tree node.
 * \param release_fn    Function to release a tree node.
 * \param context       The user context for this tree.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_avl_tree_create(
    eroc_avl_tree** tree, eroc_avl_tree_compare_fn compare_fn,
    eroc_avl_tree_key_fn key_fn, eroc_avl_tree_release_fn release_fn,
    void* context)
{
    eroc_avl_tree* tmp;

    /* allocate memory for this instance. */
    tmp = (eroc_avl_tree*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }

    /* populate values. */
    memset(tmp, 0, sizeof(*tmp));
    tmp->compare_fn = compare_fn;
    tmp->key_fn = key_fn;
    tmp->release_fn = release_fn;
    tmp->context = context;

    /* success. */
    *tree = tmp;
    return 0;
}
