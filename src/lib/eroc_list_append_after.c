/**
 * \file lib/eroc_list_append_after.c
 *
 * \brief Append a node after the given node in a list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Append a node after the given node.
 *
 * \param list          The list to use for appending.
 * \param after         The node after which this node is appended.
 * \param node          The node to append.
 */
void eroc_list_append_after(
    eroc_list* list, eroc_list_node* after, eroc_list_node* node)
{
    node->prev = after;

    /* Appending after a node. */
    if (NULL != after)
    {
        node->next = after->next;
        if (NULL != after->next)
        {
            /* patch after->next */
            after->next->prev = node;
        }
        else
        {
            /* if after->next is NULL, then after must be tail. */
            list->tail = node;
        }
        after->next = node;
    }
    /* Appending to tail. */
    else
    {
        node->prev = list->tail;

        if (NULL != list->tail)
        {
            list->tail->next = node;
        }

        node->next = NULL;
        list->tail = node;
    }

    /* fix up head, if head is NULL. */
    if (NULL == list->head)
    {
        list->head = node;
    }

    list->count += 1;
}
