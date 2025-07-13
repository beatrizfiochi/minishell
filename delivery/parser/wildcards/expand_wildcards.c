/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 17:48:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "../../parser/aux.h"
#include "../../parser/parser.h"
#include "../../execute/env_utils.h"

static int	ft_strcmp_insensitive(char *s1, char *s2)
{
	int	i;
	int	c1;
	int	c2;

	i = 0;
	while (1)
	{
		c1 = s1[i];
		if (c1 >= 'A' && c1 <= 'Z')
			c1 += ('a' - 'A');
		c2 = s2[i];
		if (c2 >= 'A' && c2 <= 'Z')
			c2 += ('a' - 'A');
		if ((c1 == '\0') || (c2 == '\0') || (c1 != c2))
			break ;
		i++;
	}
	return (c1 - c2);
}

// Recursive function to match pattern to filename
static bool	match_pattern(const char *filename, const char *pattern,
							bool quoted)
{
	if (*pattern == '\0' && *filename == '\0')
		return (true);
	if (is_quote(*pattern))
		quoted = !quoted;
	if ((*pattern == '*') && !quoted)
	{
		while (*filename != '\0')
		{
			if (match_pattern(filename, pattern + 1, quoted))
				return (true);
			filename++;
		}
		return (match_pattern(filename, pattern + 1, quoted));
	}
	if (is_quote(*pattern))
		return (match_pattern(filename, pattern + 1, quoted));
	if (*filename == *pattern)
		return (match_pattern(filename + 1, pattern + 1, quoted));
	return (false);
}

static int	filename_valid(char *filename, char *pattern)
{
	return (match_pattern(filename, pattern, false));
}

static int	check_dir_file(struct dirent *entry, char **file, char *path,
								char *pattern)
{
	char	*filename;

	if (entry == NULL)
		return (EXIT_FAILURE);
	if (!filename_valid(entry->d_name, pattern))
		return (EXIT_FAILURE);
	if (ft_strncmp(path, ".", 2) == 0)
		filename = ft_strdup(entry->d_name);
	else
		filename = ft_strjoin(path, entry->d_name);
	if (filename == NULL)
		ft_fprintf(STDERR_FILENO, "%s: error on malloc\n", __FUNCTION__);
	(*file) = filename;
	return (EXIT_SUCCESS);
}

static int	get_number_files(char *path, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(path);
	if (dir == NULL)
	{
		perror("opendir:");
		return (-1);
	}
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (filename_valid(entry->d_name, pattern))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static char	**read_dir(char **files, int size, char *path, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	if (size == 0)
		return (NULL);
	dir = opendir(path);
	if (dir == NULL)
		return (NULL);
	i = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (check_dir_file(entry, &files[i], path, pattern) == EXIT_SUCCESS)
			i++;
		entry = readdir(dir);
	}
	if (++i < size)
		files = free_envp(files);
	closedir(dir);
	return (files);
}

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
		ft_printf("file: %s\n", files[i]);
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
