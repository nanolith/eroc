/**
 * \file lib/eroc_list_release.c
 *
 * \brief Release a list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>
#include <stdlib.h>
#include <string.h>

/**
 * \brief Release an \ref eroc_list instance.
 *
 * \param list          The list to release.
 *
 * \returns 0 on success and non-zero on failure.
 */
int eroc_list_release(eroc_list* list)
{
    int release_retval, retval = 0;
    eroc_list_node* tmp;

    while (NULL != list->head)
    {
        tmp = list->head->next;

        release_retval = list->eroc_list_node_release(list->head);
        if (0 != release_retval)
        {
            retval = release_retval;
        }

        list->head = tmp;
    }

    free(list);

    return retval;
}
