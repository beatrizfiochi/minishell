/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:02:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/28 22:46:32 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "execution_bonus.h"
#include "exec_utils_bonus.h"
#include "../minishell_bonus.h"

void	close_any_possible_fd(t_shell *shell)
{
	close_possible_pipe(shell);
	if (shell->pipe.carry_over_fd > 0)
		close(shell->pipe.carry_over_fd);
	shell->pipe.carry_over_fd = -1;
}

void	close_all_pipes(t_btnode *node)
{
	if (node_cnt(node)->op == OP_PIPE)
	{
		if (node_cnt(node)->cmd.pipe_fd > 0)
		{
			close(node_cnt(node)->cmd.pipe_fd);
			node_cnt(node)->cmd.pipe_fd = -1;
		}
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

static int	check_if_directory(char **argv)
{
	if ((argv[0][0] == '.') || (argv[0][0] == '/'))
	{
		if (is_directory(argv[0]))
		{
			ft_fprintf(STDERR_FILENO, "%s: Is a directory\n", argv[0]);
			return (EXIT_CMD_CANNOT_EXEC);
		}
	}
	return (EXIT_SUCCESS);
}

int	exec_cmd(t_shell *shell, char **argv, char **envp)
{
	char	*path;
	char	*msg;

	path = NULL;
	msg = NULL;
	if (check_if_directory(argv) != EXIT_SUCCESS)
		return (EXIT_CMD_CANNOT_EXEC);
	if ((argv[0][0] == '.') || (argv[0][0] == '/'))
		execve(argv[0], argv, envp);
	else
	{
		if (create_cmd_path(argv[0], shell->variable_list, &path))
			execve(path, argv, envp);
		else
			msg = "command not found";
	}
	if (msg == NULL)
		msg = strerror(errno);
	ft_fprintf(STDERR_FILENO, "%s: %s\n", argv[0], msg);
	if (path != NULL)
		free(path);
	if (errno == EACCES)
		return (EXIT_CMD_CANNOT_EXEC);
	return (EXIT_CMD_NOT_FOUND);
}
