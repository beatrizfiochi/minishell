/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:56:03 by djunho            #+#    #+#             */
/*   Updated: 2025/05/25 20:15:56 by djunho           ###   ########.fr       */
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
		.parent = NULL,
		.left = NULL,
		.right = NULL,
	};
	return (node);
}

void	btree_delete(t_btnode **node, void (*content_free)(void *))
{
	if ((node == NULL) || (*node == NULL))
		return ;
	if (content_free != NULL)
		content_free((*node)->content);
	free(*node);
	*node = NULL;
}

void	btree_clear(t_btnode **node, void (*content_free)(void *))
{
	if ((node == NULL) || (*node == NULL))
		return ;
	if ((*node)->left != NULL)
		btree_clear(&(*node)->left, content_free);
	if ((*node)->right != NULL)
		btree_clear(&(*node)->right, content_free);
	btree_delete(node, content_free);
}

bool	btree_is_leaf(t_btnode *node)
{
	if (node == NULL)
		return (false);
	return ((node->left == NULL) && (node->right == NULL));
}
