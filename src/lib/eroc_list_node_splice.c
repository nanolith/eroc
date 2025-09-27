/**
 * \file lib/eroc_list_node_splice.c
 *
 * \brief Splice the new node in place of the old node.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Splice the new node in place of the old node.
 *
 * \param list          The list to use for this splice operation.
 * \param oldnode       The node to unlink.
 * \param newnode       The node to which replaces this node.
 *
 * \note After this call, the caller owns oldnode.
 */
void eroc_list_node_splice(
    eroc_list* list, eroc_list_node* oldnode, eroc_list_node* newnode)
{
    newnode->prev = oldnode->prev;
    newnode->next = oldnode->next;

    /* patch the previous node's next. */
    if (NULL != newnode->prev)
    {
        newnode->prev->next = newnode;
    }
    /* if there is no previous node, then patch list->head. */
    else
    {
        list->head = newnode;
    }

    /* patch the next node's prev. */
    if (NULL != newnode->next)
    {
        newnode->next->prev = newnode;
    }
    /* if there is no next node, then patch list->tail. */
    else
    {
        list->tail = newnode;
    }
}
