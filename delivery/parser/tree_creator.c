/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/04 16:17:03 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../btree/btree.h"
#include "../minishell.h"
#include "../cmd.h"
#include "parser.h"

t_list	*search_op(t_list *tokens)
{
	char		*content_token;
	t_node_op	operator;

	content_token = NULL;
	while (tokens != NULL)
	{
		content_token = tokens->content;
		operator = op(content_token);
		if (operator == OP_AND || operator == OP_OR || operator == OP_PIPE)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_node_op	op(char *token_str)
{
	if (*token_str == '\0')
		return (OP_INVALID);
	if (*token_str == '&' && *(token_str + 1) == '&')
		return (OP_AND);
	if (*token_str == '|' && *(token_str + 1) == '|')
		return (OP_OR);
	if (*token_str == '|')
		return (OP_PIPE);
	return (OP_CMD);
}

t_btnode	*create_node(t_list *token_list)
{
	t_content_node	*content;
	t_btnode		*tree_node;

	content = malloc(sizeof(t_content_node));
	if (content == NULL)
		return (NULL);
	content->op = op((char *)(token_list->content));
	content->cmd.tokens = token_list;
	tree_node = btree_new(content);
	return (tree_node);
}
t_list	*prev_token(t_list *token_list, t_list *current_token)
{
	if (token_list == NULL || token_list == current_token)
		return (NULL);
	while (token_list->next != NULL && token_list->next != current_token)
		token_list = token_list->next;
	return (token_list);
}

t_btnode	*create_tree(t_list *token_list)
{
	t_list		*op_node;
	t_list		*aux;
	t_btnode	*tree;

	op_node = search_op(token_list);
	if (op_node == NULL)
		return (create_node(token_list));
	aux = prev_token(token_list, op_node);
	aux->next = NULL;
	aux = op_node->next;
	op_node->next = NULL;
	tree = create_node(op_node);
	tree->left = create_tree(token_list);
	tree->right = create_tree(aux);
	return (tree);
}
