/**
 * \file lib/eroc_list_create.c
 *
 * \brief Create an empty list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Create an \ref eroc_list instance, using the given release method.
 *
 * \param list          Pointer to the list pointer to receive the created list
 *                      on success.
 * \param node_release  Method to release a node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_create(eroc_list** list, int (*node_release)(eroc_list_node*))
{
    eroc_list *tmp = (eroc_list*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }

    memset(tmp, 0, sizeof(*tmp));
    tmp->eroc_list_node_release = node_release;

    *list = tmp;
    return 0;
}
