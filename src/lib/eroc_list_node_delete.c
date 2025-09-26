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
    eroc_list_node_unlink(list, node);

    return list->eroc_list_node_release(node);
}
