/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 21:58:22 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "../../parser/aux.h"
#include "../../parser/parser.h"
#include "../../execute/env_utils.h"
#include "expand_wildcards.h"

static bool	split_path(char *ori, char **path, char **pattern)
{
	int	i;

	i = ft_strlen(ori);
	while ((i > 0) && (ori[i] != '/'))
		i--;
	if (i != 0)
		*path = ft_strndup(ori, i + 1);
	else
		*path = ft_strdup(".");
	if (*path == NULL)
		return (false);
	if (i != 0)
		*pattern = ft_strdup(&ori[i + 1]);
	else
		*pattern = ft_strdup(ori);
	if (*pattern == NULL)
	{
		free(*path);
		*path = NULL;
		return (false);
	}
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
	{
		perror("wildcards");
		return (token->next);
	}
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

	while (*str != '\0')
	{
		if (*str == '*')
			return (true);
		else if (is_quote(*str) && (quote == false))
			str = go_to_end_quote(str);
		else if (is_quote(*str))
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
