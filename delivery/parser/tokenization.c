/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:26:54 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/24 17:21:02 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>       //printf
#include <stdlib.h>
#include "../libft/libft.h"

t_tokens	*create_token(const char *start, int len)
{
    t_tokens	*new;

	new = malloc(sizeof(t_tokens));
    if (new == NULL)
        return NULL;
    new->content = ft_strndup(start, len);
    new->next = NULL;
    return (new);
}
int	append_token(t_tokens **head, t_tokens **tail, t_tokens *new)
{
	if (new == NULL)
		return (0);
	if (*head == NULL)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
	return (1);
}

void	free_tokens(t_tokens *list)
{
	t_tokens	*tmp;
	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		free(tmp->content);
		free(tmp);
	}
}

void	search_quotes(char *line, char *c, int *len)
{
	if (line == NULL)
		return ;
	while (line[*len] != '\0' && (line[*len] != ' ' || *len == 0))
	{
		if (line[*len] == '\'' || line[*len] == '"')
			{
				*c = line[*len];
				(*len)++;
				while (line[*len] != '\0'&& line[*len] != *c)
					(*len)++;
				if (line[*len] == *c)
					(*len)++;
				return ;
			}
			else
				(*len)++;
	}
}

t_tokens	*tokenization(char *line)
{
	t_tokens	*head_token;
	t_tokens	*tail_token;
	t_tokens	*new_token;
	char		*start;
	char		quote;
	int			len;

	head_token = NULL;
	tail_token = NULL;
	quote = 0;
    while (*line != '\0')
	{
        while (*line == ' ')
            line++;
        if (*line == '\0')
            break;
        start = line;
		len = 0;
		search_quotes(line, &quote, &len);
		new_token = create_token(start, len);
		if (append_token(&head_token, &tail_token, new_token) == 1)
			line+= len;
    }
    return (head_token);
}
