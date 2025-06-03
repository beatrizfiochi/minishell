/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:40:35 by djunho            #+#    #+#             */
/*   Updated: 2025/06/03 10:23:13 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BTREE_H
# define BTREE_H

# include "stdbool.h"

typedef struct s_btnode
{
	void			*content;
	struct s_btnode	*parent;
	struct s_btnode	*right;
	struct s_btnode	*left;
}	t_btnode;

// Create a new btree node
t_btnode	*btree_new(void *content);
bool		btree_is_leaf(t_btnode *node);
void		btree_delete(t_btnode **node, void (*content_free)(void *));
void		btree_clear(t_btnode **node, void (*content_free)(void *));

typedef int	(*t_foreach_node_cb)(t_btnode *node, int ret_left,
								bool *should_continue, void *ctx);
typedef int	(*t_foreach_leaf_cb)(t_btnode *node, void *ctx);

// Runs a DFS approach
// https://www.geeksforgeeks.org/dfs-traversal-of-a-tree-using-recursion/
// Implements the In-order Traversal (left, root, right)

// Runs at the following order:
// cb_leaf for the left node
// cb_node for the parent node
// cb_leaf for the right node
int			btree_foreach_dfs(t_btnode *node, t_foreach_node_cb cb_node,
				t_foreach_leaf_cb cb_leaf, void *ctx);

typedef struct s_btree_foreach_dfs_cb
{
	// Runs at the following order:
	// cb_node_before	for the parent node before calling the left node
	t_foreach_node_cb	cb_node_before;
	// cb_node_between	for the parent node between the left and the right calls
	t_foreach_node_cb	cb_node_between;
	// cb_leaf			for a leaf node
	t_foreach_leaf_cb	cb_leaf;
	// ctx is the context passed to all callbacks
	void				*ctx;
}	t_btree_foreach_dfs_cb;

int			btree_foreach_before_and_between_dfs(t_btnode *node,
				const t_btree_foreach_dfs_cb *const cb_funcs);

int			btree_count_leaves(t_btnode *node);
int			btree_count_nodes(t_btnode *node);

#endif // BTREE_H
