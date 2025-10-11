/**
 * \file lib/eroc_avl_tree_insert.c
 *
 * \brief Insert a node into the AVL tree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/* forward decls. */
static void insert_tree_node(
    int* bf, eroc_avl_tree* tree, eroc_avl_tree_node* root, const void* key,
    eroc_avl_tree_node* node);
static inline int max(int lhs, int rhs)
{
    return lhs > rhs ? lhs : rhs;
}

/**
 * \brief Insert a node into the AVL tree instance.
 *
 * \note The AVL tree takes ownership of this node.
 *
 * \param tree          The tree instance for this insert operation.
 * \param node          The node to insert.
 */
void eroc_avl_tree_insert(eroc_avl_tree* tree, eroc_avl_tree_node* node)
{
    /* make sure the node starts off as an orphan. */
    node->left = node->right = node->parent = NULL;

    /* edge case: insert this node into root if root is NULL. */
    if (NULL == tree->root)
    {
        node->height = 1;
        tree->root = node;
    }
    else
    {
        /* get the node key. */
        const void* key = tree->key_fn(tree->context, node);
        int unused_bf = 0;

        /* do a recursive insertion. */
        insert_tree_node(&unused_bf, tree, tree->root, key, node);
    }

    tree->count += 1;
}

/**
 * \brief Insert a node in the subtree that starts with root.
 *
 * \param bf            Set to the balance factor of the subtree on success.
 * \param tree          The tree for this operation.
 * \param root          The root of this subtree.
 * \param key           The key for the insertion comparison.
 * \param node          The node to insert.
 */
static void insert_tree_node(
    int* bf, eroc_avl_tree* tree, eroc_avl_tree_node* root, const void* key,
    eroc_avl_tree_node* node)
{
    const void* root_key = tree->key_fn(tree->context, root);
    int cmp = tree->compare_fn(tree->context, key, root_key);
    int child_bf = 0;

    bool is_tree_root = (root == tree->root);

    if (cmp < 0)
    {
        if (NULL == root->left)
        {
            root->left = node;
            node->parent = root;
            node->height = 1;
        }
        else
        {
            insert_tree_node(&child_bf, tree, root->left, key, node);
        }
    }
    else
    {
        if (NULL == root->right)
        {
            root->right = node;
            node->parent = root;
            node->height = 1;
        }
        else
        {
            insert_tree_node(&child_bf, tree, root->right, key, node);
        }
    }

    /* adjust the heights. */
    int left_height = root->left ? root->left->height : 0;
    int right_height = root->right ? root->right->height : 0;
    root->height = max(left_height, right_height) + 1;

    /* compute the balance factor. */
    *bf = left_height - right_height;

    /* is the tree left-heavy? */
    if (*bf >= 2)
    {
        if (child_bf < 0)
        {
            eroc_avl_tree_rotate_left(&root->left);
            eroc_avl_tree_rotate_right(&root);
        }
        else
        {
            eroc_avl_tree_rotate_right(&root);
        }
    }
    /* is the tree right-heavy? */
    else if (*bf <= -2)
    {
        if (child_bf > 0)
        {
            eroc_avl_tree_rotate_right(&root->right);
            eroc_avl_tree_rotate_left(&root);
        }
        else
        {
            eroc_avl_tree_rotate_left(&root);
        }
    }

    /* fix up root after rotations. */
    if (is_tree_root)
    {
        if (root != tree->root)
        {
            tree->root = root;
        }
    }
}
