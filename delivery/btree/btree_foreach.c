/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_foreach.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:35:44 by djunho            #+#    #+#             */
/*   Updated: 2025/05/26 10:36:18 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "btree.h"

static int	process_leaf(t_foreach_leaf_cb func, t_btnode *node, void *ctx)
{
	if (func != NULL)
		return (func(node, ctx));
	return (0);
}

int	btree_foreach_before_and_between_dfs(t_btnode *node,
		const t_btree_foreach_dfs_cb *const cfg)
{
	int	ret;
	int	node_ret;

	if (node == NULL)
		return (-1);
	node_ret = 0;
	if (cfg->cb_node_before != NULL)
		node_ret = cfg->cb_node_before(node, 0, cfg->ctx);
	if (node_ret != 0)
		return (node_ret);
	if (btree_is_leaf(node))
		return (process_leaf(cfg->cb_leaf, node, cfg->ctx));
	ret = 0;
	if (node->left != NULL)
		ret = btree_foreach_before_and_between_dfs(node->left, cfg);
	node_ret = ret;
	if (cfg->cb_node_between != NULL)
		node_ret = cfg->cb_node_between(node, ret, cfg->ctx);
	if ((node_ret == 0) && (node->right != NULL))
		ret = btree_foreach_before_and_between_dfs(node->right, cfg);
	return (ret);
}

int	btree_foreach_dfs(t_btnode *node,
			t_foreach_node_cb cb_node, t_foreach_leaf_cb cb_leaf, void *ctx)
{
	int	ret;
	int	node_ret;

	if (node == NULL)
		return (-1);
	if (btree_is_leaf(node))
		return (process_leaf(cb_leaf, node, ctx));
	ret = 0;
	if (node->left != NULL)
		ret = btree_foreach_dfs(node->left, cb_node, cb_leaf, ctx);
	node_ret = ret;
	if (cb_node != NULL)
		node_ret = cb_node(node, ret, ctx);
	if ((node_ret == 0) && (node->right != NULL))
		ret = btree_foreach_dfs(node->right, cb_node, cb_leaf, ctx);
	return (ret);
}
