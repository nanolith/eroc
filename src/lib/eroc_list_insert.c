/**
 * \file lib/eroc_list_insert.c
 *
 * \brief Insert a node at the beginning of a list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Insert a node at the beginning of a list.
 *
 * \param list          The list to use for insertion.
 * \param node          The node to insert.
 */
void eroc_list_insert(eroc_list* list, eroc_list_node* node)
{
    node->next = list->head;
    node->prev = NULL;

    if (NULL != list->head)
    {
        list->head->prev = node;
    }
    else
    {
        list->tail = node;
    }

    list->head = node;
    list->count += 1;
}
