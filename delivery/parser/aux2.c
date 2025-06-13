/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:12:37 by djunho            #+#    #+#             */
/*   Updated: 2025/06/13 16:33:23 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"

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

t_list	*search_op(t_list *tokens, bool full_expand)
{
	char		*content_token;
	t_node_op	operator;

	content_token = NULL;
	while (tokens != NULL)
	{
		content_token = tokens->content;
		operator = op(content_token);
		if (operator == OP_AND || operator == OP_OR
			|| operator == OP_VAR_ASSIGN || operator == OP_HEREDOC
			|| operator == OP_RD_INPUT || operator == OP_APPEND_RD_OUTPUT
			|| operator == OP_RD_OUTPUT)
			return (tokens);
		if (full_expand && (operator == OP_PIPE))
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}
