/**
 * \file test/lib/test_eroc_list.cpp
 *
 * \brief Unit tests for eroc_list.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/list.h>
#include <minunit/minunit.h>
#include <stdlib.h>

TEST_SUITE(eroc_list);

static int test_node_release(eroc_list_node* node)
{
    free(node);
    return 0;
}

/**
 * \brief We should be able to create and release a linked list.
 */
TEST(create_release)
{
    eroc_list* list;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}
