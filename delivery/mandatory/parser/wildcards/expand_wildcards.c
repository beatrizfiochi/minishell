/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 17:31:04 by bfiochi-         ###   ########.fr       */
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
	*pattern = NULL;
	*path = ft_strdup(".");
	if (*path != NULL)
		*pattern = ft_strdup(ori);
	if ((*path == NULL) || (*pattern == NULL))
	{
		if (*path != NULL)
			free(*path);
		ft_fprintf(STDERR_FILENO, "Malloc error\n");
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

static t_list	*create_new_lst_from_wildcards(char *pattern)
{
	char	**files;
	int		i;
	t_list	*new_lst;
	t_list	*aux;

	files = get_files(pattern);
	if (files == NULL)
		return (NULL);
	i = -1;
	new_lst = NULL;
	aux = NULL;
	while (files[++i] != NULL)
	{
		aux = create_token(files[i], ft_strlen(files[i]));
		if (aux == NULL)
			break ;
		ft_lstadd_back(&new_lst, aux);
	}
	free_envp(files);
	return (new_lst);
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

void	expand_wildcards_token(t_list **token_lst)
{
	t_list	*prev;
	t_list	*token;
	t_list	*new_lst;

	token = *token_lst;
	prev = NULL;
	while (token != NULL)
	{
		if (has_wildcards((char *)token->content))
		{
			new_lst = create_new_lst_from_wildcards((char *)token->content);
			if (new_lst != NULL)
				token = replace_lst_item_by_lst(token_lst, new_lst,
						prev, token);
		}
		prev = token;
		token = token->next;
	}
}
