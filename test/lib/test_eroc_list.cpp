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
#include <string.h>

TEST_SUITE(eroc_list);

static int test_node_release(eroc_list_node* node)
{
    free(node);
    return 0;
}

static int test_node_create(eroc_list_node** node)
{
    eroc_list_node* tmp;

    tmp = (eroc_list_node*)malloc(sizeof(*tmp));
    if (NULL == tmp)
    {
        return 1;
    }
    memset(tmp, 0, sizeof(*tmp));

    *node = tmp;
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

    /* the release method is set. */
    TEST_EXPECT(&test_node_release == list->eroc_list_node_release);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief By default, the list is empty and the count is 0.
 */
TEST(empty_list_invariant)
{
    eroc_list* list;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief If we insert a node into an empty list, the list is updated correctly.
 */
TEST(empty_list_insert)
{
    eroc_list* list;
    eroc_list_node* a;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and insert a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_insert(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief If we append a node to the end of an empty list, the list is updated
 * correctly.
 */
TEST(empty_list_append)
{
    eroc_list* list;
    eroc_list_node* a;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief eroc_list_insert_before NULL is a synonym for eroc_list_insert.
 */
TEST(empty_list_insert_before_NULL)
{
    eroc_list* list;
    eroc_list_node* a;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and insert a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_insert_before(list, NULL, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief eroc_list_append_after NULL is a synonym for eroc_list_append.
 */
TEST(empty_list_append_after_NULL)
{
    eroc_list* list;
    eroc_list_node* a;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append_after(list, NULL, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Repeated list insert works as expected.
 */
TEST(repeated_list_insert)
{
    eroc_list* list;
    eroc_list_node* a;
    eroc_list_node* b;
    eroc_list_node* c;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and insert a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_insert(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and insert b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_insert(list, b);

    /* Postconditions: there are two element in the list, b <--> a */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(b == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(b == a->prev);
    TEST_EXPECT(NULL == a->next);
    TEST_EXPECT(NULL == b->prev);
    TEST_EXPECT(a == b->next);

    /* Create and insert c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_insert(list, c);

    /* Postconditions: there are two element in the list, c <--> b <--> a */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(c == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(b == a->prev);
    TEST_EXPECT(NULL == a->next);
    TEST_EXPECT(c == b->prev);
    TEST_EXPECT(a == b->next);
    TEST_EXPECT(NULL == c->prev);
    TEST_EXPECT(b == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}
