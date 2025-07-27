/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:02:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 18:13:02 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "execution.h"
#include "exec_utils.h"
#include "../minishell.h"

void	close_any_possible_fd(t_shell *shell)
{
	if (shell->pipe.pipe[0] > 0)
		close(shell->pipe.pipe[0]);
	if (shell->pipe.pipe[1] > 0)
		close(shell->pipe.pipe[1]);
	if (shell->pipe.carry_over_fd > 0)
		close(shell->pipe.carry_over_fd);
	shell->pipe.pipe[0] = -1;
	shell->pipe.pipe[1] = -1;
	shell->pipe.carry_over_fd = -1;
}

void	close_all_pipes(t_btnode *node)
{
	if (node_cnt(node)->op == OP_PIPE)
	{
		if (node_cnt(node)->cmd.pipe_fd > 0)
			close(node_cnt(node)->cmd.pipe_fd);
	}
	if (node->left != NULL)
		close_all_pipes(node->left);
	if (node->right != NULL)
		close_all_pipes(node->right);
	return ;
}

int	wait_previous_process(t_shell *shell)
{
	int	wstatus;
	int	tmp;

	tmp = waitpid(shell->last_pid, &wstatus, 0);
	if (tmp > 0)
	{
		if (shell->last_cmd != NULL)
		{
			shell->last_cmd->finished = true;
		}
		while ((tmp > 0))
			tmp = wait(NULL);
		shell->last_pid = -1;
		return (get_fork_return(wstatus));
	}
	return (EXIT_FAILURE);
}

int	exec_cmd(t_shell *shell, char **args, char **envp)
{
	char	*path;
	char	*msg;

	path = NULL;
	msg = NULL;
	if ((args[0][0] == '.') || (args[0][0] == '/'))
		execve(args[0], args, envp);
	else
	{
		if (create_cmd_path(args[0], shell->variable_list, &path))
			execve(path, args, envp);
		else
			msg = "command not found";
	}
	if (msg == NULL)
		msg = strerror(errno);
	ft_fprintf(STDERR_FILENO, "%s: %s\n", args[0], msg);
	if (path != NULL)
		free(path);
	if (errno == EACCES)
		return (EXIT_CMD_CANNOT_EXEC);
	else if (errno == ENOENT)
		return (EXIT_CMD_NOT_FOUND);
	return (EXIT_CMD_NOT_FOUND);
}
