/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:13:47 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 22:55:18 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>				// perror
#include <unistd.h>				// getcwd
#include "../../minishell.h"

int	pwd(int argc, char *argv[], t_shell *shell)
{
	char	*pwd;

	(void)argc;
	(void)argv;
	(void)shell;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		perror("bash: pwd: getcwd failed");
		return (EXIT_FAILURE);
	}
	ft_printf("%s\n", pwd);
	free(pwd);
	return (EXIT_SUCCESS);
}
