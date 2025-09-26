/**
 * \file lib/eroc_list_node_unlink.c
 *
 * \brief Unlink a node from the list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Unlink a node from a list.
 *
 * \param list          The list to use for unlinking.
 * \param node          The node to unlink.
 *
 * \note After this call, the caller owns the node.
 *
 * \returns 0 on success and non-zero on failure.
 */
void eroc_list_node_unlink(eroc_list* list, eroc_list_node* node)
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
}
