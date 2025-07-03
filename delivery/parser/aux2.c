/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:12:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/02 19:22:14 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"
#include "aux.h"

t_node_op	op_list(t_list *list)
{
	return (op((char *)(list->content)));
}

t_node_op	op(char *token)
{
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
	if ((*token == '(') && (*(token + 1) == '\0'))
		return (OP_PAREN_OPEN);
	if ((*token == ')') && (*(token + 1) == '\0'))
		return (OP_PAREN_CLOSE);
	return (OP_CMD);
}

// Search for any op with exception of OP_CMD
t_list	*search_any_op(t_list *tokens)
{
	char		*content_token;

	content_token = NULL;
	while (tokens != NULL)
	{
		content_token = tokens->content;
		if (op(content_token) != OP_CMD)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_list	*search_op(t_list *tokens, enum e_expand_type expand_type)
{
	char		*content_token;
	t_node_op	oper;
	int			inside;

	inside = 0;
	content_token = NULL;
	while (tokens != NULL)
	{
		content_token = tokens->content;
		oper = op(content_token);
		if ((inside == 0) && ((oper == OP_AND) || (oper == OP_OR)
				|| (oper == OP_VAR_ASSIGN) || (oper == OP_HEREDOC)
				|| (oper == OP_RD_INPUT) || (oper == OP_APPEND_RD_OUTPUT)
				|| (oper == OP_RD_OUTPUT)))
			return (tokens);
		if ((expand_type == EXP_PIPE) && (inside == 0)
			&& (oper == OP_PIPE))
			return (tokens);
		if (oper == OP_PAREN_OPEN)
			inside++;
		if (oper == OP_PAREN_CLOSE)
			inside--;
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
