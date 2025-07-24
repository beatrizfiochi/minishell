/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 20:19:10 by djunho            #+#    #+#             */
/*   Updated: 2025/07/24 22:44:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../btree/btree.h"
#include "../cmd.h"
#include "../minishell.h"
#include "../parser/parser.h"

char	*get_redir_filename(t_shell *shell, t_btnode *file_node)
{
	t_list	*token;

	token = ((t_content_node *)(file_node->content))->cmd.tokens;
	expand_variable_token(&token, shell->variable_list, shell);
	clean_token_quotes(token);
	if (ft_lstsize(((t_content_node *)(file_node->content))->cmd.tokens) != 1)
	{
		ft_fprintf(STDERR_FILENO, "Ambiguous redirect\n");
		return (NULL);
	}
	return (((t_content_node *)(file_node->content))->cmd.tokens->content);
}

t_node_op	get_next_operation(t_btnode *node)
{
	t_btnode		*aux;

	if (node == NULL || node->content == NULL)
		return (OP_INVALID);
	if (((t_content_node *)node->right->content)->op == OP_CMD)
	{
		if (node->parent == NULL)
			return (OP_INVALID);
		return (((t_content_node *)node->parent->content)->op);
	}
	aux = node->right;
	if (((t_content_node *)aux->content)->op == OP_CMD)
		return (OP_INVALID);
	while (aux->left != NULL)
	{
		aux = aux->left;
	}
	return (((t_content_node *)aux->parent->content)->op);
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
