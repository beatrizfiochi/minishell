/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:12:37 by djunho            #+#    #+#             */
/*   Updated: 2025/06/25 16:11:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"

t_node_op	op(char *token)
{
	if (*token == '\0')
		return (OP_INVALID);
	if ((*token == '&') && (*(token + 1) == '&') && (*(token + 2) == '\0'))
		return (OP_AND);
	if ((*token == '|') && (*(token + 1) == '|') && (*(token + 2) == '\0'))
		return (OP_OR);
	if ((*token == '|') && (*(token + 1) == '\0'))
		return (OP_PIPE);
	if ((*token == '=') && (*(token + 1) == '\0'))
		return (OP_VAR_ASSIGN);
	if ((*token == '<') && (*(token + 1) == '<') && (*(token + 2) == '\0'))
		return (OP_HEREDOC);
	if ((*token == '<') && (*(token + 1) == '\0'))
		return (OP_RD_INPUT);
	if ((*token == '>') && (*(token + 1) == '>') && (*(token + 2) == '\0'))
		return (OP_APPEND_RD_OUTPUT);
	if ((*token == '>') && (*(token + 1) == '\0'))
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

char	*go_to_end_quote(char *content)
{
	char	quote;
	char	*initial_content;

	quote = *content;
	initial_content = content;
	while (*content != '\0')
	{
		content++;
		if (*content == quote)
			return (content + 1);
	}
	return (initial_content + 1);
}
