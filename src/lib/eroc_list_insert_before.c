/**
 * \file lib/eroc_list_insert_before.c
 *
 * \brief Insert a node before the given node in a list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Insert a node before the given node.
 *
 * \param list          The list to use for insertion.
 * \param before        The node before which this node is inserted.
 * \param node          The node to insert.
 */
void eroc_list_insert_before(
    eroc_list* list, eroc_list_node* before, eroc_list_node* node)
{
    node->next = before;

    /* Inserting before a node. */
    if (NULL != before)
    {
        node->prev = before->prev;
        if (NULL != before->prev)
        {
            /* patch before->prev */
            before->prev->next = node;
        }
        else
        {
            /* if before->prev is NULL, then before must be head. */
            list->head = node;
        }
        before->prev = node;
    }
    /* Inserting to head. */
    else
    {
        node->next = list->head;

        if (NULL != list->head)
        {
            list->head->prev = node;
        }

        node->prev = NULL;
        list->head = node;
    }

    /* fix up tail, if tail is NULL. */
    if (NULL == list->tail)
    {
        list->tail = node;
    }

    list->count += 1;
}
