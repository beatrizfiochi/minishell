/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:26:54 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/01 19:29:24 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>       //printf
#include <stdlib.h>
#include "../libft/libft.h"
#include "parser.h"

t_list	*create_token(const char *line, int len)
{
	char	*content;

	content = ft_strndup(line, len);
	if (content == NULL)
		return (NULL);
	return (ft_lstnew(content));
}

static void	search_token_utils(char *l, char *c, int init, int *len)
{
	bool	valid_op;

	valid_op = false;
	while (valid_op != true)
	{
		valid_op = true;
		while (l[*len] != '\0' && l[*len] != ' ' && is_op(&l[*len]) == 0)
		{
			if ((l[*len] == '\'' || l[*len] == '"') && l[*len] != '\0')
			{
				*c = l[*len];
				*len = go_next_char(&l[*len + 1], *c) - l;
				if (l[*len] == *c)
					(*len)++;
			}
			else
				(*len)++;
		}
		if ((is_op(&l[*len]) == 1) && (l[*len] == '=')
			&& (is_valid_name(&l[init], *len - init) == -1))
		{
			valid_op = false;
			(*len)++;
		}
	}
}

static void	search_token(char *line, char *c, int *len)
{
	int		init;

	if (line == NULL)
		return ;
	if (is_op(&line[*len]) > 0)
	{
		*len += is_op(&line[*len]);
		return ;
	}
	if (line[*len] == ' ' && *len == 0)
		(*len)++;
	init = *len;
	search_token_utils(line, c, init, len);
}

t_list	*tokenization(char *line)
{
	t_list	*head_token;
	t_list	*new_token;
	char	quote;
	int		len;

	head_token = NULL;
	quote = 0;
	if (line == NULL)
		return (NULL);
	while (*line != '\0')
	{
		while (*line == ' ' && *line != '\0')
			line++;
		if (*line == '\0')
			break ;
		len = 0;
		search_token(line, &quote, &len);
		new_token = create_token(line, len);
		ft_lstadd_back(&head_token, new_token);
		line += len;
	}
	return (head_token);
}
