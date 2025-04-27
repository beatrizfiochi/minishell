/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:40:35 by djunho            #+#    #+#             */
/*   Updated: 2025/04/21 19:03:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BTREE_H
# define BTREE_H

# include "stdbool.h"

typedef struct s_btnode
{
	void			*content;
	struct s_btnode	*right;
	struct s_btnode	*left;
}	t_btnode;

// Create a new btree node
t_btnode	*btree_new(void *content);
void		btree_delete(t_btnode *node, void (*content_free)(void *));
void		btree_clear(t_btnode *node, void (*content_free)(void *));

typedef int	(*t_foreach_node_cb)(t_btnode *node, int ret_left);
typedef int	(*t_foreach_leaf_cb)(t_btnode *node);

// Runs a DFS approach
// https://www.geeksforgeeks.org/dfs-traversal-of-a-tree-using-recursion/
// Implements the In-order Traversal (left, root, right)
int			btree_foreach_dfs(t_btnode *node,
				t_foreach_node_cb cb_node, t_foreach_leaf_cb cb_leaf);

#endif // BTREE_H
