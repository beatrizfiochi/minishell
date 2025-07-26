/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:19:10 by djunho            #+#    #+#             */
/*   Updated: 2025/07/26 00:25:19 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../btree/btree.h"
#include "../cmd.h"

static t_node_op	get_next_operation_above(t_btnode *node)
{
	t_btnode		*aux;

	if (node->parent == NULL)
		return (OP_INVALID);
	aux = node;
	while (aux != NULL)
	{
		if (node_cnt(aux)->cmd.is_parentheses)
			return (OP_INVALID);
		if ((aux->parent != NULL)
			&& (node_cnt(aux->parent)->cmd.is_parentheses == false)
			&& (aux->parent->right != aux))
			break ;
		aux = aux->parent;
	}
	if (aux == NULL)
		return (OP_INVALID);
	if (node_cnt(aux->parent)->cmd.is_parentheses)
		return (OP_INVALID);
	return (node_cnt(aux->parent)->op);
}

t_node_op	get_next_operation(t_btnode *node)
{
	t_btnode		*aux;

	if (node == NULL || node->content == NULL)
		return (OP_INVALID);
	if (node_cnt(node->right)->op == OP_CMD)
		return (get_next_operation_above(node));
	aux = node->right;
	if (node_cnt(aux)->op == OP_CMD)
		return (OP_INVALID);
	while ((aux->left != NULL)
		&& (node_cnt(aux->parent)->cmd.is_parentheses == false))
	{
		aux = aux->left;
	}
	if (node_cnt(aux->parent)->cmd.is_parentheses == true)
		return (OP_INVALID);
	return (node_cnt(aux->parent)->op);
}

bool	is_redirect_file_op(t_node_op op)
{
	return (op == OP_RD_OUTPUT || op == OP_APPEND_RD_OUTPUT
		|| op == OP_RD_INPUT || op == OP_HEREDOC);
}

t_content_node	*get_next_cmd(t_btnode *node)
{
	while (node != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_CMD)
			return ((t_content_node *)node->content);
		if (((t_content_node *)node->content)->cmd.is_parentheses)
			return ((t_content_node *)node->content);
		if (is_redirect_file_op(((t_content_node *)node->content)->op))
			node = node->left;
		else
			node = node->right;
	}
	return (NULL);
}

t_content_node	*get_first_cmd(t_btnode *node)
{
	while (node != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_CMD)
			return ((t_content_node *)node->content);
		node = node->left;
	}
	return (NULL);
}
