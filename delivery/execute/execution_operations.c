/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:59:00 by djunho            #+#    #+#             */
/*   Updated: 2025/05/29 19:23:29 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>			// perror, printf
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../minishell.h"
#include "../cmd.h"
#include "execution.h"
#include "../signals/signals.h"

int	run_child(t_cmd *cmd, t_shell *shell)
{
	char	**envp;
	char	*path;
	char	**args;

	envp = shell->envp;
	args = convert_list_to_vector(cmd->tokens);
	if (args == NULL)
		return (0);
	if (args[0] == NULL)
	{
		free_join(args);
		return (0);
	}
	reset_signals();
	execve(args[0], args, envp);
	if (create_cmd_path(args[0], envp, &path))
		execve(path, args, envp);
	perror(args[0]);
	free_join(args);
	return (127);
}

int	process_and(t_shell *shell)
{
	int		wstatus;

	if (shell->last_pid == 0)
		return (0);
	waitpid(shell->last_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else
	{
		printf("Error in child process\n");
		return (1);
	}
}

int	process_or(t_shell *shell)
{
	int		wstatus;

	if (shell->last_pid == 0)
		return (0);
	waitpid(shell->last_pid, &wstatus, 0);
	if (WIFEXITED(wstatus))
	{
		return (WEXITSTATUS(wstatus) == 0);
	}
	else
	{
		printf("Error in child process\n");
		return (1);
	}
}

int	process_pipe(t_btnode *node)
{
	t_node_op		right_operation;
	t_content_node	*content;
	t_content_node	*right_content;

	content = (t_content_node *)node->content;
	right_operation = OP_INVALID;
	if (node->right != NULL)
	{
		right_content = (t_content_node *)node->right->content;
		if (right_content != NULL)
			right_operation = right_content->op;
	}
	close(content->pipe.carry_over_fd);
	content->pipe.carry_over_fd = dup(content->pipe.pipe[0]);
	close(content->pipe.pipe[0]);
	close(content->pipe.pipe[1]);
	if (pipe(content->pipe.pipe) < 0)
		return (1);
	close(content->pipe.pipe[0]);
	content->pipe.pipe[0] = content->pipe.carry_over_fd;
	if (right_operation != OP_PIPE)
		content->pipe.is_last_pipe = true;
	return (0);
}

void	configure_pipe(int pipe[2], int carry_over_fd, bool is_last_pipe)
{
	if (carry_over_fd != -1)
	{
		dup2(carry_over_fd, STDIN_FILENO);
		close(carry_over_fd);
	}
	else
	{
		close(pipe[0]);
	}
	if (!is_last_pipe)
	{
		dup2(pipe[1], STDOUT_FILENO);
	}
	close(pipe[1]);
}
