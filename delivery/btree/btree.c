/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:56:03 by djunho            #+#    #+#             */
/*   Updated: 2025/05/04 18:55:30 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include <stdlib.h>		// malloc
#include "btree.h"

t_btnode	*btree_new(void *content)
{
	t_btnode	*node;

	node = malloc(sizeof(t_btnode));
	*node = (t_btnode){
		.content = content,
		.left = NULL,
		.right = NULL,
	};
	return (node);
}

void	btree_delete(t_btnode *node, void (*content_free)(void *))
{
	if (node == NULL)
		return ;
	if (content_free != NULL)
		content_free(node->content);
	free(node);
}

void	btree_clear(t_btnode *node, void (*content_free)(void *))
{
	if (node == NULL)
		return ;
	if (node->left != NULL)
		btree_clear(node->left, content_free);
	if (node->right != NULL)
		btree_clear(node->right, content_free);
	btree_delete(node, content_free);
	return ;
}

bool	btree_is_leaf(t_btnode *node)
{
	if (node == NULL)
		return (false);
	return ((node->left == NULL) && (node->right == NULL));
}

int	btree_foreach_dfs(t_btnode *node,
			t_foreach_node_cb cb_node, t_foreach_leaf_cb cb_leaf)
{
	int	ret;

	if (node == NULL)
		return (-1);
	if (btree_is_leaf(node) && (cb_leaf != NULL))
		return (cb_leaf(node));
	ret = 0;
	if (node->left != NULL)
		ret = btree_foreach_dfs(node->left, cb_node, cb_leaf);
	if (cb_node != NULL)
		ret = cb_node(node, ret);
	if ((ret == 0) && (node->right != NULL))
		ret = btree_foreach_dfs(node->right, cb_node, cb_leaf);
	return (ret);
}
