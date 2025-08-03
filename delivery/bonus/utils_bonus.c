/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:51:22 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/27 18:06:00 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>			// perror
#include <unistd.h>           //write
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "btree/btree_bonus.h"
#include "cmd_bonus.h"

int	p_error(const char *msg, const int ret)
{
	perror(msg);
	return (ret);
}

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

t_content_node	*node_cnt(const t_btnode *node)
{
	if (node == NULL || node->content == NULL)
		return (NULL);
	return ((t_content_node *)node->content);
}
