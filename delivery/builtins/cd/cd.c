/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 18:02:44 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 18:45:57 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>	// free
#include <unistd.h>
#include "../../libft/libft.h"
#include "../../execute/env_utils.h"
#include "../../minishell.h"
#include "../../variables/variables.h"

static void	p_error(const char *first, const char *second)
{
	char	*final;

	final = ft_strjoin(first, second);
	if (final == NULL)
		return ;
	perror(final);
	free(final);
}

static char	*get_home(t_shell *shell)
{
	char		*new_path;

	new_path = (char *)get_env("HOME", shell->variable_list);
	if (new_path != NULL)
		return (new_path);
	ft_fprintf(STDERR_FILENO, "cd: HOME not set\n");
	return (NULL);
}

static char	*concat_path(const char *path, const char *new_path)
{
	char	*new_pwd;
	char	*new_pwd2;

	if ((path == NULL) || (new_path == NULL))
		return (NULL);
	if (new_path[0] == '/')
	{
		new_pwd = ft_strdup(new_path);
		if (new_pwd == NULL)
			perror("cd: malloc failed");
		return (new_pwd);
	}
	new_pwd = NULL;
	new_pwd2 = ft_strjoin(path, "/");
	if (new_pwd2 != NULL)
		new_pwd = ft_strjoin(new_pwd2, new_path);
	if ((new_pwd == NULL) || (new_pwd2 == NULL))
		perror("cd: malloc failed");
	if (new_pwd2 != NULL)
		free(new_pwd2);
	return (new_pwd);
}

static void	update_variables(t_shell *shell, const char *path)
{
	char			*new_path;
	char			*var_pwd;

	var_pwd = (char *)get_env("PWD", shell->variable_list);
	if (var_pwd != NULL)
		check_and_replace_var(shell->variable_list, "OLDPWD", var_pwd, NULL);
	else
		check_and_replace_var(shell->variable_list, "OLDPWD", "", NULL);
	new_path = getcwd(NULL, 0);
	if (new_path == NULL)
	{
		perror("cd: getcwd failed");
		new_path = concat_path(var_pwd, path);
	}
	if (new_path != NULL)
	{
		check_and_replace_var(shell->variable_list, "PWD", new_path, NULL);
		free(new_path);
	}
}

int	cd(int argc, char *argv[], t_shell *shell)
{
	char		*new_path;
	int			ret;

	if (argc == 1)
		new_path = get_home(shell);
	else if (argc > 2)
	{
		ft_fprintf(STDERR_FILENO, "minishell: cd: too many arguments\n");
		return (EXIT_FAILURE);
	}
	else
		new_path = argv[1];
	if (new_path == NULL)
		return (1);
	ret = chdir(new_path);
	if (ret != 0)
		p_error("minishell: cd: ", new_path);
	else
		update_variables(shell, new_path);
	return (ret != 0);
}
