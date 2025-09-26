
/**
 * \file lib/eroc_list_node_delete.c
 *
 * \brief Delete a node from the list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Delete a node from the list.
 *
 * \param list          The list to use for deleting.
 * \param node          The node to delete.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_node_delete(eroc_list* list, eroc_list_node* node)
{
    if (node == list->head)
    {
        list->head = node->next;
        if (NULL != list->head)
        {
            list->head->prev = NULL;
        }
        else
        {
            list->tail = NULL;
        }
    }
    else if (node == list->tail)
    {
        list->tail = node->prev;
        if (NULL != list->tail)
        {
            list->tail->next = NULL;
        }
        else
        {
            list->head = NULL;
        }
    }
    else
    {
        if (NULL != node->prev)
        {
            node->prev->next = node->next;
        }
        if (NULL != node->next)
        {
            node->next->prev = node->prev;
        }
    }

    list->count -= 1;

    return list->eroc_list_node_release(node);
}
