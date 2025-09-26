/**
 * \file lib/eroc_list_append.c
 *
 * \brief Append a node at the end of a list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>

/**
 * \brief Append a node at the end of a list.
 *
 * \param list          The list to use for appending.
 * \param node          The node to append.
 */
void eroc_list_append(eroc_list* list, eroc_list_node* node)
{
    eroc_list_append_after(list, NULL, node);
}
