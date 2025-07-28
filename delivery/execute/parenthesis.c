/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:20:56 by djunho            #+#    #+#             */
/*   Updated: 2025/07/28 21:43:22 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdio.h>			// p_error
#include <sys/wait.h>		// waitpid
#include <unistd.h>			// fork
#include "../minishell.h"
#include "../cmd.h"
#include "../btree/btree.h"
#include "exec_utils.h"
#include "execution.h"

static void	handle_fd_on_parenthesis(t_shell *shell, t_content_node	*content)
{
	bool	is_pipe;
	t_cmd	*cmd;

	shell->last_pid = -1;
	shell->last_cmd = NULL;
	is_pipe = (shell->pipe.pipe[0] != -1);
	cmd = &content->cmd;
	if (cmd->redir.fd_in > 0)
	{
		dup2(cmd->redir.fd_in, STDIN_FILENO);
		close(cmd->redir.fd_in);
	}
	else if ((shell->pipe.carry_over_fd != -1) && (is_pipe))
		dup2(shell->pipe.carry_over_fd, STDIN_FILENO);
	if (cmd->redir.fd_out > 0)
	{
		dup2(cmd->redir.fd_out, STDOUT_FILENO);
		close(cmd->redir.fd_out);
		cmd->redir.fd_out = -1;
	}
	else if (is_pipe && !shell->is_last_redirect)
		dup2(shell->pipe.pipe[1], STDOUT_FILENO);
	if (shell->is_last_redirect)
		shell->out_fd = dup(STDOUT_FILENO);
	else
		shell->out_fd = dup(shell->pipe.pipe[1]);
	close_any_possible_fd(shell);
}

int	prepare_parenthesis(t_shell *shell, t_btnode *node, bool *should_continue)
{
	t_content_node	*content;
	pid_t			pid;

	content = (t_content_node *)node->content;
	if (!content->cmd.is_parentheses)
		return (EXIT_SUCCESS);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
	{
		handle_fd_on_parenthesis(shell, content);
		return (EXIT_SUCCESS);
	}
	shell->last_pid = pid;
	content->cmd.is_parentheses = false;
	*should_continue = false;
	return (EXIT_SUCCESS);
}

int	process_parenthesis(t_shell *shell, t_btnode *node, int ret,
							bool *should_continue)
{
	t_content_node	*content;

	(void)should_continue;
	content = (t_content_node *)node->content;
	if (content->cmd.is_parentheses)
	{
		close_any_possible_fd(shell);
		if (shell->last_pid > 0)
			ret = wait_previous_process(shell);
		clear_minishell(shell);
		exit(ret);
	}
	return (ret);
}
