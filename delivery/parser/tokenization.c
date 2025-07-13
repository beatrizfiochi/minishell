/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:26:54 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/13 13:47:08 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>       //printf
#include <unistd.h>      //write
#include <stdlib.h>
#include "../libft/libft.h"
#include "../minishell.h"
#include "parser.h"
#include "../cmd.h"
#include "aux.h"

t_list	*create_token(const char *line, int len)
{
	char	*content;

	content = ft_strndup(line, len);
	if (content == NULL)
		return (NULL);
	return (ft_lstnew(content));
}

static int	scan_until_op_or_error(char *l, char *c, int *len)
{
	char	*next_char;

	while (l[*len] != '\0' && l[*len] != ' ')
	{
		if (is_op(&l[*len]) > 0)
			break ;
		if (is_op(&l[*len]) == 0 && (l[*len] == '|' || l[*len] == '&'))
			return (-1);
		if ((l[*len] == '\'' || l[*len] == '"') && l[*len] != '\0')
		{
			*c = l[*len];
			next_char = go_next_char(&l[*len + 1], *c);
			if (next_char == NULL)
				return (-1);
			*len = next_char - l;
			if (l[*len] == *c)
				(*len)++;
		}
		else
			(*len)++;
	}
	return (0);
}

static void	search_token_utils(char *l, char *c, int *len)
{
	bool	valid_op;

	if ((*l == ' ') && (*len == 1))
		return ;
	valid_op = false;
	while (valid_op != true)
	{
		valid_op = true;
		if (scan_until_op_or_error(l, c, len) == -1)
		{
			*len = -1;
			return ;
		}
	}
}

static void	search_token(char *line, char *c, int *len)
{
	int		op_len;

	if (line == NULL)
		return ;
	op_len = is_op(&line[*len]);
	if (op_len == 0 && ((line[*len] == '&') || (line[*len] == '|')
			|| (line[*len] == '<') || (line[*len] == '>')))
	{
		*len = -1;
		return ;
	}
	if (op_len > 0)
	{
		*len += op_len;
		return ;
	}
	if (line[*len] == ' ' && *len == 0)
		(*len)++;
	search_token_utils(line, c, len);
}

// Tokenize the input line into a linked list of tokens.
// Note: This function handles spaces, quotes, and operators.
// When finding a space after an assign operator, it creates an empty token.
t_list	*tokenization(char *line)
{
	t_list	*head_token;
	t_list	*new_token;
	t_list	*prev_token;
	char	quote;
	int		len;

	head_token = NULL;
	prev_token = NULL;
	quote = 0;
	if (line == NULL)
		return (NULL);
	while (*line != '\0')
	{
		while (*line == ' ')
			line++;
		if (*line == '\0')
			break ;
		len = 0;
		search_token(line, &quote, &len);
		if (len == -1)
		{
			printf_error("syntax error near unexpected token\n");
			ft_lstclear(&head_token, free);
			return (NULL);
		}
		if (*line == ' ')
			new_token = create_token("", 0);
		else
			new_token = create_token(line, len);
		if ((prev_token != NULL)
			&& (is_token_operator(prev_token->content) == 1)
			&& (is_token_operator(new_token->content) == 1))
		{
			if ((op_list(new_token) != OP_RD_INPUT && op_list(new_token) != OP_HEREDOC))
			{
				ft_fprintf(STDERR_FILENO, "syntax error near unexpected token \"%s\"\n", (char *)(new_token->content));
				ft_lstdelone(new_token, free);
				ft_lstclear(&head_token, free);
				return (NULL);
			}
		}
		ft_lstadd_back(&head_token, new_token);
		prev_token = new_token;
		line += len;
	}
	return (head_token);
}
