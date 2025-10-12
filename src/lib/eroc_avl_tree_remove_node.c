/**
 * \file lib/eroc_avl_tree_remove_node.c
 *
 * \brief Remove the given node from the tree.
 *
 * \copyright 2025 Justin Handville.  Please see LICENSE.txt in this
 * distribution for the license terms under which this software is distributed.
 */

#include <eroc/avltree.h>

/* forward decls. */
static void transplant(
    eroc_avl_tree* tree, int* child_bf, eroc_avl_tree_node* n,
    eroc_avl_tree_node* child);
static inline int max(int lhs, int rhs)
{
    return lhs > rhs ? lhs : rhs;
}

/**
 * \brief Remove the given node from the tree, transplanting nodes as needed.
 *
 * \note This operation does not release the node. Ownership is transferred to
 * the caller.
 *
 * \param tree          The tree for this remove operation.
 * \param node          The node to remove.
 */
void eroc_avl_tree_remove_node(eroc_avl_tree* tree, eroc_avl_tree_node* node)
{
    eroc_avl_tree_node* parent = node->parent;
    eroc_avl_tree_node* left = node->left;
    eroc_avl_tree_node* right = node->right;
    int child_bf = 0;

    /* there are one fewer elements in the tree. */
    tree->count -= 1;

    /* if the left child is NULL, then the right child is transplanted. */
    if (NULL == node->left)
    {
        transplant(tree, &child_bf, node, right);
    }
    /* if the right child is NULL, then the left child is transplanted. */
    else if (NULL == node->right)
    {
        transplant(tree, &child_bf, node, left);
    }
    /* if both children are populated, then select the rightmost left
     * descendant. */
    else
    {
        eroc_avl_tree_node* rightmost_left =
            eroc_avl_tree_maximum_node(tree, left);
        eroc_avl_tree_node* rightmost_left_left = rightmost_left->left;
        eroc_avl_tree_node* rightmost_left_parent = rightmost_left->parent;

        /* transplant the rightmost_left left onto the rightmost_left parent
         * right. */
        rightmost_left_parent->right = rightmost_left_left;
        if (NULL != rightmost_left_left)
        {
            rightmost_left_left->parent = rightmost_left_parent;
        }

        /* transplant this node into our node's place. */
        if (rightmost_left != left)
        {
            rightmost_left->left = left;    left->parent = rightmost_left;
        }
        else
        {
            rightmost_left->left = NULL;
        }
        rightmost_left->right = right;      right->parent = rightmost_left;
        rightmost_left->parent = parent;

        /* fix up parent. */
        if (NULL != parent)
        {
            if (node == parent->left)
            {
                parent->left = rightmost_left;
            }
            else
            {
                parent->right = rightmost_left;
            }
        }
        /* fix up tree root. */
        else
        {
            tree->root = rightmost_left;
        }

        if (NULL != rightmost_left_parent && node != rightmost_left_parent)
        {
            /* start the tree balancing below from rightmost left's former
             * parent. */
            parent = rightmost_left_parent;
        }
        else
        {
            parent = left;
        }

        child_bf = 0;
    }

    /* balance the tere after a node prune. */
    while (NULL != parent)
    {
        /* compute the parent's balance factor. */
        int parent_left_height = parent->left ? parent->left->height : 0;
        int parent_right_height = parent->right ? parent->right->height : 0;
        parent->height = max(parent_left_height, parent_right_height) + 1;
        int parent_bf = parent_left_height - parent_right_height;

        /* is the tree left-heavy? */
        if (parent_bf >= 2)
        {
            if (child_bf < 0)
            {
                eroc_avl_tree_rotate_left(&parent->left);
                eroc_avl_tree_rotate_right(&parent);
            }
            else
            {
                eroc_avl_tree_rotate_right(&parent);
            }
        }
        /* is the tree right-heavy? */
        else if (parent_bf <= -2)
        {
            if (child_bf > 0)
            {
                eroc_avl_tree_rotate_right(&parent->right);
                eroc_avl_tree_rotate_left(&parent);
            }
            else
            {
                eroc_avl_tree_rotate_left(&parent);
            }
        }

        /* recalculate the balance factor. */
        parent_left_height = parent->left ? parent->left->height : 0;
        parent_right_height = parent->right ? parent->right->height : 0;
        child_bf = parent_left_height - parent_right_height;

        /* move up the tree. */
        parent = parent->parent;
    }
}

/**
 * \brief Transplant a child node into the current node's location, pruning it
 * from the tree.
 *
 * \param tree          The tree for this operation.
 * \param child_bf      Pointer to be populated with the balance factor of the
 *                      transplanted child.
 * \param node          The node to be pruned.
 * \param child         The child to replace this node.
 */
static void transplant(
    eroc_avl_tree* tree, int* child_bf, eroc_avl_tree_node* node,
    eroc_avl_tree_node* child)
{
    eroc_avl_tree_node* parent = node->parent;

    /* edge case: the child is the new root node. */
    if (NULL == parent)
    {
        if (NULL != child)
        {
            child->parent = NULL;
        }
        tree->root = child;
        return;
    }

    /* the pruned node is the parent's left node. */
    if (node == parent->left)
    {
        parent->left = child;
    }
    else
    {
        parent->right = child;
    }

    /* prune out the node. */
    node->parent = node->left = node->right = NULL;
    node->height = 1;

    if (NULL != child)
    {
        /* the child's parent node is now this node's parent. */
        child->parent = parent;

        /* calculate the child's balance factor. */
        int child_left_height = child->left ? child->left->height : 0;
        int child_right_height = child->right ? child->right->height : 0;
        *child_bf = child_left_height - child_right_height;
    }
    else
    {
        *child_bf = 0;
    }
}
