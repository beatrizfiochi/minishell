/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:26:54 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/27 16:51:26 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>       //printf
#include <stdlib.h>
#include "../libft/libft.h"

t_list	*create_token(const char *start, int len)
{
	char	*content;

	content = ft_strndup(start, len);
	if (content == NULL)
		return (NULL);
	return (ft_lstnew(content));
}

void	free_tokens(t_list *list)
{
	t_list	*tmp;

	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		free(tmp->content);
		free(tmp);
	}
}

static void	search_token(char *line, char *c, int *len)
{
	if (line == NULL)
		return ;
	while (line[*len] != '\0' && (line[*len] != ' ' || *len == 0))
	{
		if (line[*len] == '\'' || line[*len] == '"')
		{
			*c = line[*len];
			(*len)++;
			*len = go_next_char(&line[*len], *c) - line;
			if (line[*len] == *c)
				(*len)++;
			return ;
		}
		else
			(*len)++;
	}
}

t_list	*tokenization(char *line)
{
	t_list	*head_token;
	t_list	*new_token;
	char	quote;
	int		len;

	head_token = NULL;
	quote = 0;
	while (*line != '\0')
	{
		while (*line == ' ')
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
