/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 21:44:48 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "../../parser/aux.h"
#include "../../parser/parser.h"
#include "../../execute/env_utils.h"

static char	**get_files(char *pattern_ori);

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

// Find the far right / before a wildcards
static char	*find_folder_sep(char *path)
{
	char	*aux;

	aux = path;
	while ((path != NULL) && (*path != '\0'))
	{
		while ((*aux != '\0') && (*aux != '/') && (*aux != '*'))
			aux++;
		if ((*aux == '\0') || (*aux == '*'))
			return (NULL);
		path = aux;
		aux++;
		while ((*aux != '\0') && (*aux != '/') && (*aux != '*'))
			aux++;
		if (*aux == '*')
			return (path);
		path = aux;
	}
	return (path);
}

static bool	split_path(char *ori, char **path, char **pattern, char	**remain)
{
	char	*sep_wild;
	char	*sep_remain;

	sep_wild = find_folder_sep(ori);
	if (sep_wild == NULL)
	{
		*path = ft_strdup(".");
		sep_wild = ori + 1;
	}
	else
	{
		*path = ft_strndup(ori, sep_wild - ori);
		sep_wild++;
	}
	sep_remain = go_next_char(sep_wild + 1, '/');
	if (sep_remain == NULL)
	{
		*pattern = ft_strdup(sep_wild);
		*remain  = ft_strdup("");
	}
	else
	{
		*pattern = ft_strndup(sep_wild, sep_wild - ori);
		*remain  = ft_strdup(sep_remain + 1);
	}
	return (true);
}

static char	**get_files_files(char *path, char *pattern)
{
	char	**files;
	int		size;

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
	return (files);
}

static	char **get_files_path_pattern(char **files, int *i, char *path, char *pattern)
{
	char	*concat;
	char	**files_i;
	char	**final;

	concat = ft_strjoin(path, pattern);
	if (concat == NULL)
		return (NULL);
	files_i = get_files(concat);
	if (files_i != NULL)
	{
		free(concat);
		return (free_envp(files));
	}
	final = envp_cat(files_i, &(files[*i]));
	*i += get_envp_size(files_i);
	free(files_i);
	free(files);
	return (final);
}

static char	**get_files(char *pattern_ori)
{
	char	*path;
	char	*pattern;
	char	*remain;
	char	**files;
	int		i;

	if (!split_path(pattern_ori, &path, &pattern, &remain))
		return (NULL);
	files = get_files_files(path, pattern);
	i = 0;
	while ((files != NULL) && (remain[0] != '\0'))
	{
		files = get_files_path_pattern(files, &i, files[i], pattern);
		i++;
	}
	free(path);
	free(pattern);
	free(remain);
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

	while ((str != NULL) && (*str != '\0'))
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
