/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/26 12:38:38 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include "../../parser/aux.h"
#include "../../parser/parser.h"
#include "../../execute/env_utils.h"
#include "expand_wildcards.h"
#include "../../variables/expand_variables.h"   // QUOTE_MARK

static bool	split_path(char *ori, char **path, char **pattern)
{
	(void)ori;
	*path = ft_strdup(".");
	*pattern = ft_strdup(ori);
	return (true);
}

static char	**get_files(char *pattern_ori)
{
	char	**files;
	int		size;
	char	*path;
	char	*pattern;

	if (!split_path(pattern_ori, &path, &pattern))
		return (NULL);
	files = NULL;
	while (1)
	{
		size = get_number_files(path, pattern);
		if (size == -1)
			break ;
		files = ft_calloc(size + 1, sizeof(char *));
		if (files == NULL)
			break ;
		files = read_dir(files, size, path, pattern);
		sort_envp(files, ft_strcmp_insensitive);
		break ;
	}
	free(path);
	free(pattern);
	return (files);
}

static t_list	*handle_possible_wildcard(char *pattern, t_list *prev,
											t_list *token)
{
	char	**files;
	int		i;
	t_list	*new_list;
	t_list	*aux;

	files = get_files(pattern);
	if (files == NULL)
		return (token->next);
	i = -1;
	new_list = prev;
	aux = prev;
	while ((files != NULL) && (files[++i] != NULL))
	{
		aux = create_token(files[i], ft_strlen(files[i]));
		new_list->next = aux;
		new_list = aux;
	}
	aux->next = token->next;
	free_envp(files);
	ft_lstdelone(token, free);
	return (aux->next);
}

static bool	has_wildcards(char *str)
{
	bool	quote;

	quote = false;
	while (*str != '\0')
	{
		if (*str == '*')
			return (true);
		else if ((*str == QUOTE_MARK) && (quote == false))
			str = go_to_end_quote(str);
		else if (*str == QUOTE_MARK)
		{
			quote = !quote;
			str++;
		}
		else
			str++;
	}
	return (false);
}

void	expand_wildcards_token(t_list *token)
{
	char	*cnt;
	t_list	*prev;
	t_list	*aux;

	prev = NULL;
	while (token != NULL)
	{
		cnt = (char *)token->content;
		aux = token;
		if (has_wildcards(cnt))
			token = handle_possible_wildcard(cnt, prev, token);
		else
			token = token->next;
		prev = aux;
	}
}
