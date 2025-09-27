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

/* C++ compatibility. */
# ifdef   __cplusplus
extern "C" {
# endif /*__cplusplus*/

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

/**
 * \brief Create an \ref eroc_list instance, using the given release method.
 *
 * \param list          Pointer to the list pointer to receive the created list
 *                      on success.
 * \param node_release  Method to release a node.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_create(eroc_list** list, int (*node_release)(eroc_list_node*));

/**
 * \brief Release an \ref eroc_list instance.
 *
 * \param list          The list to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_release(eroc_list* list);

/**
 * \brief Insert a node at the beginning of a list.
 *
 * \param list          The list to use for insertion.
 * \param node          The node to insert.
 */
void eroc_list_insert(eroc_list* list, eroc_list_node* node);

/**
 * \brief Insert a node before the given node.
 *
 * \param list          The list to use for insertion.
 * \param before        The node before which this node is inserted.
 * \param node          The node to insert.
 */
void eroc_list_insert_before(
    eroc_list* list, eroc_list_node* before, eroc_list_node* node);

/**
 * \brief Append a node at the end of a list.
 *
 * \param list          The list to use for appending.
 * \param node          The node to append.
 */
void eroc_list_append(eroc_list* list, eroc_list_node* node);

/**
 * \brief Append a node after the given node.
 *
 * \param list          The list to use for appending.
 * \param after         The node after which this node is appended.
 * \param node          The node to append.
 */
void eroc_list_append_after(
    eroc_list* list, eroc_list_node* after, eroc_list_node* node);

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
void eroc_list_node_unlink(eroc_list* list, eroc_list_node* node);

/**
 * \brief Delete a node from the list.
 *
 * \param list          The list to use for deleting.
 * \param node          The node to delete.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_node_delete(eroc_list* list, eroc_list_node* node);

/**
 * \brief Splice the new node in place of the old node.
 *
 * \param list          The list to use for this splice operation.
 * \param oldnode       The node to unlink.
 * \param newnode       The node to which replaces this node.
 *
 * \note After this call, the caller owns oldnode.
 *
 * \returns 0 on success and non-zero on failure.
 */
void eroc_list_node_splice(
    eroc_list* list, eroc_list_node* oldnode, eroc_list_node* newnode);

/* C++ compatibility. */
# ifdef   __cplusplus
}
# endif /*__cplusplus*/
