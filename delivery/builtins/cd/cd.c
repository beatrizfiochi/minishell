/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:02:44 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 16:13:13 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>	// free
#include <unistd.h>
#include "../../libft/libft.h"
#include "../../execute/env_utils.h"

static void	p_error(const char *first, const char *second)
{
	char	*final;

	final = ft_strjoin(first, second);
	if (final == NULL)
		return ;
	perror(final);
	free(final);
}

static char	*build_path(char *path)
{
	char	*aux;
	char	*cwd;
	char	*new_path;

	if (path[0] == '/')
		return (ft_strdup(path));
	cwd = getcwd(NULL, 0);
	aux = ft_strjoin(cwd, "/");
	free(cwd);
	new_path = ft_strjoin(aux, path);
	free(aux);
	return (new_path);
}

int	cd(int argc, char *argv[], char *envp[])
{
	char		*new_path;
	const char	*arg;
	int			ret;

	arg = argv[1];
	if (argc == 1)
	{
		arg = (char *)get_env("HOME=", envp);
		if (arg == NULL)
		{
			printf("bash: cd: HOME not set");
			return (EXIT_FAILURE);
		}
		new_path = (char *)arg;
	}
	else
		new_path = build_path(argv[1]);
	ret = chdir(new_path);
	if (ret != 0)
		p_error("minishell: cd: ", arg);
	if (argc != 1)
		free(new_path);
	return (ret != 0);
}
