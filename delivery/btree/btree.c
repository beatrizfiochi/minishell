/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:56:03 by djunho            #+#    #+#             */
/*   Updated: 2025/04/21 17:41:27 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include "btree.h"

t_btnode	btree_new(void *content)
{
	t_btnode node = {
		.content = content,
		.left = NULL,
		.right = NULL,
	};
	return (node);
}

bool	btree_is_leaf(t_btnode *node)
{
	if (node == NULL)
		return (false);
	return ((node->left == NULL) && (node->right == NULL));
}

int	btree_foreach_dfs(t_btnode *node, t_foreach_node_cb cb_node, t_foreach_leaf_cb cb_leaf)
{
	int	ret;

	if (node == NULL)
		return (-1);
	if (btree_is_leaf(node) && (cb_leaf != NULL))
		return cb_leaf(node);
	ret = 0;
	if (node->left != NULL)
		ret = btree_foreach_dfs(node->left, cb_node, cb_leaf);
	if (cb_node != NULL)
		ret = cb_node(node, ret);
	if ((ret == 0) && (node->right != NULL))
		ret = btree_foreach_dfs(node->right, cb_node, cb_leaf);
	return (ret);
}
