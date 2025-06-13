/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 20:28:58 by djunho            #+#    #+#             */
/*   Updated: 2025/06/03 10:23:20 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stddef.h>
#include "btree.h"

int	btree_count_leaves(t_btnode *node)
{
	int	size;

	if (node == NULL)
		return (0);
	size = 0;
	if (btree_is_leaf(node))
		return (1);
	size = 0;
	if (node->left != NULL)
		size = btree_count_leaves(node->left);
	if (node->right != NULL)
		size += btree_count_leaves(node->right);
	return (size);
}

int	btree_count_nodes(t_btnode *node)
{
	int	size;

	if (node == NULL)
		return (0);
	size = 0;
	if (btree_is_leaf(node))
		return (1);
	size = 0;
	if (node->left != NULL)
		size = btree_count_nodes(node->left);
	if (node->right != NULL)
		size += btree_count_nodes(node->right);
	return (size + 1);
}
