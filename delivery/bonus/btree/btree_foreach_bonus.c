/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_foreach.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 20:35:44 by djunho            #+#    #+#             */
/*   Updated: 2025/07/25 22:04:08 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "btree_bonus.h"

static int	process_leaf(t_foreach_leaf_cb func, t_btnode *node, void *ctx)
{
	if (func != NULL)
		return (func(node, ctx));
	return (0);
}

// This function auxiliates the btree_foreach_before_and_between_dfs function
//  to interact with the node calling the callbacks.
static int	internal_cfg_walk(t_btnode *node,
						const t_btree_foreach_dfs_cb *const cfg,
						int ret, bool *should_continue)
{
	while (1)
	{
		if (node == NULL)
			break ;
		if (cfg->cb_node_before != NULL)
			ret = cfg->cb_node_before(node, 0, should_continue, cfg->ctx);
		if (!*should_continue)
			break ;
		if (btree_is_leaf(node))
		{
			ret = process_leaf(cfg->cb_leaf, node, cfg->ctx);
			break ;
		}
		if (node->left != NULL)
			ret = btree_foreach_before_and_between_dfs(node->left, cfg);
		if (cfg->cb_node_between != NULL)
			ret = cfg->cb_node_between(node, ret, should_continue, cfg->ctx);
		if (!*should_continue)
			break ;
		if (node->right != NULL)
			ret = btree_foreach_before_and_between_dfs(node->right, cfg);
		break ;
	}
	return (ret);
}

int	btree_foreach_before_and_between_dfs(t_btnode *node,
		const t_btree_foreach_dfs_cb *const cfg)
{
	int		ret;
	bool	should_continue;

	ret = 0;
	should_continue = true;
	ret = internal_cfg_walk(node, cfg, ret, &should_continue);
	if ((node != NULL) && (cfg->cb_node_after != NULL))
		ret = cfg->cb_node_after(node, ret, &should_continue, cfg->ctx);
	return (ret);
}

int	btree_foreach_dfs(t_btnode *node,
			t_foreach_node_cb cb_node, t_foreach_leaf_cb cb_leaf, void *ctx)
{
	int		ret;
	bool	should_continue;

	should_continue = true;
	if (node == NULL)
		return (-1);
	if (btree_is_leaf(node))
		return (process_leaf(cb_leaf, node, ctx));
	ret = 0;
	if (node->left != NULL)
		ret = btree_foreach_dfs(node->left, cb_node, cb_leaf, ctx);
	if (cb_node != NULL)
		ret = cb_node(node, ret, &should_continue, ctx);
	if (!should_continue)
		return (ret);
	if (node->right != NULL)
		ret = btree_foreach_dfs(node->right, cb_node, cb_leaf, ctx);
	return (ret);
}
