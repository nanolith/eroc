/**
 * \file eroc/list.h
 *
 * \brief Simple double linked list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

/**
 * \brief Type erased linked list node.
 */
typedef struct eroc_list_node eroc_list_node;

struct eroc_list_node
{
    eroc_list_node* prev;
    eroc_list_node* next;
};
