/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:51:22 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/21 22:50:03 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>           //write
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

char	*go_next_char(char *line, char c)
{
	while (*line != c && *line != '\0')
		line++;
	if (*line != c)
		return (NULL);
	return (line);
}

bool	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (false);
	return (S_ISDIR(path_stat.st_mode));
}

bool	file_exist(const char *path)
{
	return (access(path, F_OK) == 0);
}
