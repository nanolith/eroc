/**
 * \file lib/eroc_list_node_at.c
 *
 * \brief Get the node at a given index.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Attempt to get the node at the given 0-based index.
 *
 * \param node          Pointer to the node pointer to be updated on success.
 * \param list          The list for this operation.
 * \param index         The index to find.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_node_at(
    eroc_list_node** node, eroc_list* list, unsigned long index)
{
    eroc_list_node* tmp = list->head;

    /* index out of bounds. */
    if (index >= list->count)
    {
        return 1;
    }

    /* Scan to the requested line. */
    while (index--)
        tmp = tmp->next;

    /* Success. Return the node. */
    *node = tmp;
    return 0;
}
