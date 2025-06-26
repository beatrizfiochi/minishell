/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:16:24 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 14:02:17 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>			// NULL
#include <stdio.h>
#include <stdlib.h>			// free()
#include <stdbool.h>		// bool
#include <unistd.h>			// access
#include "../libft/libft.h"	// ft_.*()
#include "env_utils.h"		// get_env()

bool	free_join(char **args)
{
	int	j;

	j = 0;
	while (args[j] != NULL)
	{
		free(args[j++]);
	}
	free(args);
	return (true);
}

static bool	check_cmd(char *cmd, char **path)
{
	*path = NULL;
	if (access(cmd, X_OK) == 0)
	{
		*path = ft_strdup(cmd);
		return (true);
	}
	return (false);
}

static char	*build_path(char *base, char *cmd)
{
	char	*tmp_path;
	char	*path;

	tmp_path = ft_strjoin(base, "/");
	path = ft_strjoin(tmp_path, cmd);
	free(tmp_path);
	return (path);
}

bool	create_cmd_path(char *cmd, char **envp, char **path)
{
	int			i;
	char		**tmp;
	const char	*path_value;

	if (check_cmd(cmd, path))
		return (true);
	path_value = get_env("PATH=", envp);
	if (path_value == NULL)
		return (false);
	tmp = ft_split(path_value, ':');
	i = -1;
	while (tmp[++i] != NULL)
	{
		if (*path != NULL)
			free(*path);
		*path = build_path(tmp[i], cmd);
		if (access(*path, X_OK) != 0)
			continue ;
		return (free_join(tmp));
	}
	free(*path);
	return (!free_join(tmp));
}

char	**convert_list_to_vector(t_list *list, int *size)
{
	int		i;
	char	**vector;
	t_list	*tmp;

	if (list == NULL)
		return (NULL);
	vector = malloc(sizeof(char *) * (ft_lstsize(list) + 1));
	if (vector == NULL)
		return (NULL);
	i = 0;
	tmp = list;
	while (tmp != NULL)
	{
		vector[i++] = tmp->content;
		tmp = tmp->next;
	}
	vector[i] = NULL;
	if (size != NULL)
		*size = i;
	return (vector);
}
