/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:02:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/21 23:44:05 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "execution.h"
#include "../minishell.h"

int	wait_previous_process(t_shell *shell)
{
	int	wstatus;
	int	tmp;

	tmp = waitpid(shell->last_pid, &wstatus, 0);
	if (tmp > 0)
	{
		shell->last_cmd->finished = true;
		if (shell->last_cmd->redir.fd_out > 0)
			close(shell->last_cmd->redir.fd_out);
		while ((tmp > 0))
			tmp = wait(NULL);
		shell->last_pid = -1;
		return (get_fork_return(wstatus));
	}
	return (EXIT_FAILURE);
}

int	check_file_exists(char **args, char **envp)
{
	if (args[0][0] == '.')
	{
		if (file_exist(args[0]))
			execve(args[0], args, envp);
		else
		{
			ft_fprintf(STDERR_FILENO, "%s: No such file or directory\n",
				args[0]);
			return (EXIT_CMD_NOT_FOUND);
		}
	}
	return (0);
}
