/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:59:00 by djunho            #+#    #+#             */
/*   Updated: 2025/06/03 22:55:57 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>			// perror, printf
#include <stdbool.h>		// bool
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../minishell.h"
#include "../cmd.h"
#include "execution.h"
#include "../signals/signals.h"
#include "../parser/parser.h"

int	run_child(t_cmd *cmd, t_shell *shell)
{
	char	**envp;
	char	*path;
	char	**args;

	envp = shell->envp;
	clean_token_quotes(cmd->tokens);
	args = convert_list_to_vector(cmd->tokens);
	if (args == NULL)
		return (0);
	if (args[0] == NULL)
	{
		clear_minishell(shell);
		free(args);
		return (0);
	}
	reset_signals();
	execve(args[0], args, envp);
	if (create_cmd_path(args[0], envp, &path))
		execve(path, args, envp);
	perror(args[0]);
	clear_minishell(shell);
	free(args);
	return (127);
}

int	process_and(t_shell *shell, bool *should_continue)
{
	int	wstatus;
	int	ret;

	*should_continue = true;
	if (shell->last_pid == 0)
		return (0);
	waitpid(shell->last_pid, &wstatus, 0);
	ret = get_fork_return(wstatus);
	if (ret != EXIT_SUCCESS)
		*should_continue = false;
	return (ret);
}

int	process_or(t_shell *shell, bool *should_continue)
{
	int	wstatus;
	int	ret;

	if (shell->last_pid == 0)
		return (0);
	waitpid(shell->last_pid, &wstatus, 0);
	ret = get_fork_return(wstatus);
	if (ret == EXIT_SUCCESS)
		*should_continue = false;
	return (ret);
}

int	process_pipe(t_btnode *node)
{
	t_node_op		parent_operation;
	t_content_node	*content;
	t_content_node	*parent_content;

	content = (t_content_node *)node->content;
	parent_operation = OP_INVALID;
	if (node->parent != NULL)
	{
		parent_content = (t_content_node *)node->parent->content;
		if (parent_content != NULL)
			parent_operation = parent_content->op;
	}
	t_content_node	*left_content;
	if (node->left != NULL)
	{
		left_content = (t_content_node *)node->left->content;
		if ((left_content != NULL) && (left_content->op == OP_PIPE))
		{
			content->pipe.carry_over_fd = dup(left_content->pipe.pipe[0]);
			close(left_content->pipe.pipe[0]);
			close(left_content->pipe.pipe[1]);
			close(left_content->pipe.carry_over_fd);
		}
		else
			content->pipe.carry_over_fd = dup(content->pipe.pipe[0]);
	} 
	close(content->pipe.pipe[0]);
	close(content->pipe.pipe[1]);
	if (pipe(content->pipe.pipe) < 0)
		return (1);
	if (parent_operation != OP_PIPE)
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
	close(pipe[0]);
	if (!is_last_pipe)
		dup2(pipe[1], STDOUT_FILENO);
	close(pipe[1]);
}
