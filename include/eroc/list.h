/**
 * \file eroc/list.h
 *
 * \brief Simple double linked list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#pragma once

#include <stddef.h>

/**
 * \brief Type erased linked list node.
 */
typedef struct eroc_list_node eroc_list_node;

struct eroc_list_node
{
    eroc_list_node* prev;
    eroc_list_node* next;
};

/**
 * \brief Linked list.
 */
typedef struct eroc_list eroc_list;

struct eroc_list
{
    int (*eroc_list_node_release)(eroc_list_node*);
    eroc_list_node* head;
    eroc_list_node* tail;
    size_t count;
};
