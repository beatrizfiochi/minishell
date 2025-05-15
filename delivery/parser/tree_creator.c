/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/14 12:09:27 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (OP_EQUAL);
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
			|| operator == OP_EQUAL || operator == OP_HEREDOC
			|| operator == OP_RD_INPUT || operator == OP_APPEND_RD_OUTPUT
			|| operator == OP_RD_OUTPUT)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

static t_btnode	*create_node(t_list *token_list)
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

static void	delete_btree_node(t_btnode *node, t_list *left,
								t_list *cur, t_list *right)
{
	ft_lstclear(&left, free);
	ft_lstclear(&cur, free);
	ft_lstclear(&right, free);
	btree_delete(node->left, free);
	btree_delete(node->right, free);
	btree_delete(node, free);
}

// Search for and operator
// Every operator must be placed between 2 commands
t_btnode	*create_tree(t_list **token_list)
{
	t_list		*op_node;
	t_list		*aux;
	t_btnode	*tree;

	op_node = search_op(*token_list);
	if (op_node == NULL)
		return (create_node(*token_list));
	aux = prev_list_item(*token_list, op_node);
	if ((op_node->next == NULL) || (aux == NULL))
	{
		ft_lstclear(token_list, free);
		return (NULL);
	}
	aux->next = NULL;
	aux = op_node->next;
	op_node->next = NULL;
	tree = create_node(op_node);
	tree->left = create_tree(token_list);
	tree->right = create_tree(&aux);
	if ((tree->left == NULL) || (tree->right == NULL))
	{
		delete_btree_node(tree, *token_list, op_node, aux);
		tree = NULL;
	}
	return (tree);
}
