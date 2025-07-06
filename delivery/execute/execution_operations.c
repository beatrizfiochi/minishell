/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:59:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 11:37:12 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>			// perror, printf
#include <stdbool.h>		// bool
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../minishell.h"
#include "../cmd.h"
#include "../signals/signals.h"
#include "../parser/parser.h"
#include "../builtins/builtins.h"
#include "execution.h"
#include "exec_utils.h"
#include "env_utils.h"
#include "../minishell.h"

int	run_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**args;
	char	**envp;
	int		ret;
	int		argc;

	args = convert_list_to_vector(cmd->tokens, &argc);
	envp = convert_list_to_envp(shell->variable_list, shell->tmp_var_list);
	ret = 1;
	while (1)
	{
		if ((args == NULL) || (args[0] == NULL))
			break ;
		reset_signals();
		ret = execute_builtin(cmd, shell);
		if (ret != EXIT_CMD_NOT_FOUND)
			break ;
		execve(args[0], args, envp);
		if (create_cmd_path(args[0], shell->variable_list, &path))
			execve(path, args, envp);
		perror(args[0]);
		break ;
	}
	clear_minishell(shell);
	if (args != NULL)
		free(args);
	if (envp != NULL)
		free_envp(envp);
	return (ret);
}

int	process_and(t_shell *shell, int ret, bool *should_continue)
{
	int	wstatus;

	*should_continue = true;
	if ((shell->last_cmd != NULL) && (ret == EXIT_SUCCESS))
	{
		if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
		{
			waitpid(shell->last_pid, &wstatus, 0);
			shell->last_cmd->finished = true;
			ret = get_fork_return(wstatus);
		}
		if (ret != EXIT_SUCCESS)
			*should_continue = false;
	}
	else
	{
		*should_continue = (ret == EXIT_SUCCESS);
	}
	return (ret);
}

int	process_or(t_shell *shell, int ret, bool *should_continue)
{
	int	wstatus;

	*should_continue = true;
	if ((shell->last_cmd != NULL) && (ret == EXIT_SUCCESS))
	{
		if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
		{
			waitpid(shell->last_pid, &wstatus, 0);
			shell->last_cmd->finished = true;
			ret = get_fork_return(wstatus);
		}
		if (ret == EXIT_SUCCESS)
			*should_continue = false;
	}
	else
	{
		*should_continue = (ret != EXIT_SUCCESS);
	}
	return (ret);
}

t_node_op	get_next_operation(t_btnode *node)
{
	t_btnode		*aux;

	if (node == NULL || node->content == NULL)
		return (OP_INVALID);
	if (((t_content_node *)node->right->content)->op == OP_CMD)
	{
		if (node->parent == NULL)
			return (OP_INVALID);
		return (((t_content_node *)node->parent->content)->op);
	}
	aux = node->right;
	if (((t_content_node *)aux->content)->op == OP_CMD)
		return (OP_INVALID);
	while (aux->left != NULL)
	{
		aux = aux->left;
	}
	return (((t_content_node *)aux->parent->content)->op);
}

int	process_pipe(t_shell *shell, t_btnode *node)
{
	shell->pipe.will_run_a_pipe = true;
	shell->pipe.carry_over_fd = dup(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[1]);
	if (pipe(shell->pipe.pipe) < 0)
		return (1);
	if (get_next_operation(node) != OP_PIPE)
		shell->pipe.is_last_pipe = true;
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
