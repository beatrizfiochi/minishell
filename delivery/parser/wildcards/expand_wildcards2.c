/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 21:57:54 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "../../parser/aux.h"
#include "../../execute/env_utils.h"

int	ft_strcmp_insensitive(char *s1, char *s2)
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

static int	check_dir_file(struct dirent *entry, char **file, char *path,
								char *pattern)
{
	char	*filename;

	if (entry == NULL)
		return (EXIT_FAILURE);
	if (!match_pattern(entry->d_name, pattern, false))
		return (EXIT_FAILURE);
	if (ft_strncmp(path, ".", 2) == 0)
		filename = ft_strdup(entry->d_name);
	else
		filename = ft_strjoin(path, entry->d_name);
	if (filename == NULL)
	{
		ft_fprintf(STDERR_FILENO, "%s: error on malloc\n", __FUNCTION__);
		return (EXIT_FAILURE);
	}
	(*file) = filename;
	return (EXIT_SUCCESS);
}

int	get_number_files(char *path, char *pattern)
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
		if (match_pattern(entry->d_name, pattern, false))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

char	**read_dir(char **files, int size, char *path, char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				i;

	if (size == 0)
		return (free_envp(files));
	dir = opendir(path);
	if (dir == NULL)
		return (free_envp(files));
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
