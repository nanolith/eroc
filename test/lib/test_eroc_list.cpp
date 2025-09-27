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
 * \brief a append after NULL results in ab.
 */
TEST(a_append_after_NULL)
{
    eroc_list* list;
    eroc_list_node* a;
    eroc_list_node* b;

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

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append_after(list, NULL, b);

    /* Postconditions: there are two elements in the list. */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief b insert before NULL results in ab.
 */
TEST(b_append_after_NULL)
{
    eroc_list* list;
    eroc_list_node* a;
    eroc_list_node* b;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(b == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and insert a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_insert_before(list, NULL, a);

    /* Postconditions: there are two elements in the list. */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

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

    /* Postconditions: there are two elements in the list, b <--> a */
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

    /* Postconditions: there are three elements in the list, c <--> b <--> a */
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

/**
 * \brief Repeated list append works as expected.
 */
TEST(repeated_list_append)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can insert B between A and C.
 */
TEST(ac_insert_b)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are two element in the list, a <--> c */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(c == a->next);
    TEST_EXPECT(a == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* Create and insert b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_insert_before(list, c, b);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can append B between A and C.
 */
TEST(ac_append_b)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are two elements in the list, a <--> c */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(c == a->next);
    TEST_EXPECT(a == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append_after(list, a, b);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can insert A before B.
 */
TEST(b_insert_a)
{
    eroc_list* list;
    eroc_list_node* a;
    eroc_list_node* b;

    /* we can create the list. */
    TEST_ASSERT(0 == eroc_list_create(&list, &test_node_release));

    /* Preconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(b == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and insert a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_insert_before(list, b, a);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can append B after A.
 */
TEST(a_append_b)
{
    eroc_list* list;
    eroc_list_node* a;
    eroc_list_node* b;

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

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append_after(list, a, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can delete a from abc.
 */
TEST(abc_delete_a)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* delete a. */
    eroc_list_node_delete(list, a);

    /* Postconditions: there are two elements in the list, b <--> c */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(b == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can delete b from abc.
 */
TEST(abc_delete_b)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* delete b. */
    eroc_list_node_delete(list, b);

    /* Postconditions: there are two elements in the list, a <--> c */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(c == a->next);
    TEST_EXPECT(a == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can delete c from abc.
 */
TEST(abc_delete_c)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* delete c. */
    eroc_list_node_delete(list, c);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}

/**
 * \brief Verify that we can repeatedly delete from the end of abc.
 */
TEST(abc_delete_ends)
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

    /* Create and append a. */
    TEST_ASSERT(0 == test_node_create(&a));
    eroc_list_append(list, a);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* Create and append b. */
    TEST_ASSERT(0 == test_node_create(&b));
    eroc_list_append(list, b);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* Create and append c. */
    TEST_ASSERT(0 == test_node_create(&c));
    eroc_list_append(list, c);

    /* Postconditions: there are three elements in the list, a <--> b <--> c */
    TEST_EXPECT(3 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(c == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(c == b->next);
    TEST_EXPECT(b == c->prev);
    TEST_EXPECT(NULL == c->next);

    /* delete c. */
    eroc_list_node_delete(list, c);

    /* Postconditions: there are two elements in the list, a <--> b */
    TEST_EXPECT(2 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(b == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(b == a->next);
    TEST_EXPECT(a == b->prev);
    TEST_EXPECT(NULL == b->next);

    /* delete b. */
    eroc_list_node_delete(list, b);

    /* Postconditions: there is one element in the list. */
    TEST_EXPECT(1 == list->count);
    TEST_EXPECT(a == list->head);
    TEST_EXPECT(a == list->tail);
    TEST_EXPECT(NULL == a->prev);
    TEST_EXPECT(NULL == a->next);

    /* delete a. */
    eroc_list_node_delete(list, a);

    /* Postconditions: the list is empty. */
    TEST_EXPECT(0 == list->count);
    TEST_EXPECT(NULL == list->head);
    TEST_EXPECT(NULL == list->tail);

    /* we can release the list. */
    TEST_ASSERT(0 == eroc_list_release(list));
}
