/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/26 12:37:22 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include "../../../libft/libft.h"
#include "../../execute/env_utils_bonus.h"
#include "../../variables/expand_variables_bonus.h"   // QUOTE_MARK

// Recursive function to match pattern to filename. Ignore the * inside
//  QUOTE_MARK
static bool	match_pattern_internal(const char *filename, const char *pattern,
									bool quoted, bool first_call)
{
	if (first_call && filename[0] == '.' && pattern[0] != '.')
		return (false);
	if (*pattern == '\0' && *filename == '\0')
		return (true);
	if (*pattern == QUOTE_MARK)
		return (match_pattern_internal(filename, pattern + 1, !quoted, false));
	if (*pattern == '*' && !quoted)
	{
		if (match_pattern_internal(filename, pattern + 1, quoted, false))
			return (true);
		if (*filename != '\0'
			&& match_pattern_internal(filename + 1, pattern, quoted, false))
			return (true);
		return (false);
	}
	if (*filename == *pattern)
		return (match_pattern_internal(filename + 1, pattern + 1, quoted,
				false));
	return (false);
}

static bool	match_pattern(const char *filename, const char *pattern,
	bool quoted)
{
	return (match_pattern_internal(filename, pattern, quoted, true));
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
