/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:26:54 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/30 10:58:43 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>      //write
#include <stdlib.h>
#include "../libft/libft.h"
#include "parser.h"
#include "aux.h"

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

static void	search_token(char *line, int *len)
{
	int		op_len;
	char	c;

	*len = 0;
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
	search_token_utils(line, &c, len);
}

t_list	*exit_tokenization(t_list *prev_token, t_list *head_token)
{
	if (prev_token != NULL)
		ft_fprintf(STDERR_FILENO, "syntax error near unexpected token `%s'\n",
			(char *)prev_token->content);
	else
		ft_fprintf(STDERR_FILENO, "syntax error near unexpected token\n");
	ft_lstclear(&head_token, free);
	return (NULL);
}

// Tokenize the input line into a linked list of tokens.
// Note: This function handles spaces, quotes, and operators.
// When finding a space after an assign operator, it creates an empty token.
t_list	*tokenization(char *line)
{
	t_list	*head_token;
	t_list	*new_token;
	t_list	*prev_token;
	int		len;

	head_token = NULL;
	prev_token = NULL;
	if (line == NULL)
		return (NULL);
	while (*line != '\0')
	{
		while (*line == ' ')
			line++;
		if (*line == '\0')
			break ;
		search_token(line, &len);
		if (len == -1)
			return (exit_tokenization(prev_token, head_token));
		new_token = create_token(line, len);
		ft_lstadd_back(&head_token, new_token);
		if (check_token(prev_token, new_token) == EXIT_FAILURE)
			return (exit_tokenization(new_token, head_token));
		prev_token = new_token;
		line += len;
	}
	if (check_token(prev_token, NULL) == EXIT_FAILURE)
			return (exit_tokenization(new_token, head_token));
	return (head_token);
}
