/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/03 23:00:19 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>				// printf
#include "../btree/btree.h"
#include "../minishell.h"
#include "../cmd.h"
#include "aux.h"
#include "parser.h"

static t_node_op	op(char *token_str)
{
	if (*token_str == '\0')
		return (OP_INVALID);
	if (*token_str == '&' && *(token_str + 1) == '&')
		return (OP_AND);
	if (*token_str == '|' && *(token_str + 1) == '|')
		return (OP_OR);
	if (*token_str == '|')
		return (OP_PIPE);
	if (*token_str == '=')
		return (OP_VAR_ASSIGN);
	if (*token_str == '<' && *(token_str + 1) == '<')
		return (OP_HEREDOC);
	if (*token_str == '<')
		return (OP_RD_INPUT);
	if (*token_str == '>' && *(token_str + 1) == '>')
		return (OP_APPEND_RD_OUTPUT);
	if (*token_str == '>')
		return (OP_RD_OUTPUT);
	return (OP_CMD);
}

static t_list	*search_op(t_list *tokens)
{
	char		*content_token;
	t_node_op	operator;

	content_token = NULL;
	while (tokens != NULL)
	{
		content_token = tokens->content;
		operator = op(content_token);
		if (operator == OP_AND || operator == OP_OR || operator == OP_PIPE
			|| operator == OP_VAR_ASSIGN || operator == OP_HEREDOC
			|| operator == OP_RD_INPUT || operator == OP_APPEND_RD_OUTPUT
			|| operator == OP_RD_OUTPUT)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

static t_btnode	*create_node(t_list *token_list, t_btnode *parent)
{
	t_content_node	*content;
	t_btnode		*tree_node;

	content = malloc(sizeof(t_content_node));
	if (content == NULL)
		return (NULL);
	content->op = op((char *)(token_list->content));
	if (content->op == OP_CMD)
		content->cmd.tokens = token_list;
	else
		ft_lstclear(&token_list, free);
	tree_node = btree_new(content);
	tree_node->parent = parent;
	return (tree_node);
}

static void free_btree_content(void *_content)
{
	t_content_node *content;

	content = (t_content_node *)_content;
	if (content->op == OP_CMD)
	{
		if (content->cmd.tokens != NULL)
			ft_lstclear(&content->cmd.tokens, free);
	}
	free(content);
}

t_btnode	*create_first(t_list **token_list, t_btnode *parent)
{
	t_list		*op_node;
	t_list		*aux;
	t_btnode	*tree;

	op_node = search_op(*token_list);
	if (op_node == NULL)
	{
		tree = create_node(*token_list, parent);
		*token_list = op_node;
		return (tree);
	}
	aux = prev_list_item(*token_list, op_node);
	if ((op_node->next == NULL) || (aux == NULL))
	{
		ft_lstclear(token_list, free);
		return (NULL);
	}
	aux->next = NULL;
	aux = op_node->next;
	op_node->next = NULL;
	tree = create_node(op_node, parent);
	tree->left = create_node(*token_list, tree);
	tree->right = create_node(aux, tree);
	op_node = search_op(aux);
	if (op_node != NULL)
	{
		aux = prev_list_item(aux, op_node);
		if (aux == NULL)
		{
			printf("Error: Two operators in sequence\n");
			btree_clear(&tree, free_btree_content);
			ft_lstclear(&op_node, free);
			return (NULL);
		}
		aux->next = NULL;
	}
	*token_list = op_node;
	return (tree);
}

// Search for and operator
// Every operator must be placed between 2 commands
t_btnode	*create_tree(t_list **token_list, t_btnode *parent)
{
	t_list		*op_node;
	t_list		*right;
	t_list		*aux;
	t_btnode	*tree;
	t_btnode	*old_tree;

	old_tree = create_first(token_list, parent);
	if (old_tree == NULL)
		return (NULL);
	tree = old_tree;
	while (*token_list != NULL)
	{
		op_node = search_op(*token_list);
		if ((op_node == NULL) || (op_node != *token_list))
		{
			printf("Should find an operator\n");
			ft_lstclear(token_list, free);
			return (NULL);
		}
		right = op_node->next;
		op_node->next = NULL;
		tree = create_node(op_node, parent);
		tree->left = old_tree;
		tree->left->parent = tree;
		tree->right = create_node(right, tree);
		op_node = search_op(right);
		if (op_node != NULL)
		{
			aux = prev_list_item(right, op_node);
			if (aux == NULL)
			{
				printf("Error: Two operators in sequence\n");
				btree_clear(&tree, free_btree_content);
				ft_lstclear(&op_node, free);
				return (NULL);
			}
			aux->next = NULL;
		}
		*token_list = op_node;
		old_tree = tree;
	}
	return (tree);
}
