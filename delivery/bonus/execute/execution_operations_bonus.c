/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:59:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 18:42:10 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>		// bool
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../minishell_bonus.h"
#include "../cmd_bonus.h"
#include "../signals/signals_bonus.h"
#include "../builtins/builtins_bonus.h"
#include "execution_bonus.h"
#include "exec_utils_bonus.h"
#include "env_utils_bonus.h"
#include "../minishell_bonus.h"

static int	execute_command(t_cmd *cmd, t_shell *shell)
{
	int		ret;
	char	**args;
	char	**envp;
	int		argc;

	subprocess_signals();
	ret = execute_builtin(cmd, shell, false);
	if (ret != EXIT_CMD_NOT_FOUND)
		return (ret);
	ret = EXIT_FAILURE;
	args = convert_list_to_vector(cmd->tokens, &argc);
	if ((args == NULL) || (args[0] == NULL))
		return (EXIT_FAILURE);
	envp = convert_list_to_envp(shell->variable_list, shell->tmp_var_list);
	if (envp != NULL)
		ret = exec_cmd(shell, args, envp);
	if (args != NULL)
		free(args);
	if (envp != NULL)
		free_envp(envp);
	return (ret);
}

int	run_child(t_cmd *cmd, t_shell *shell)
{
	int		ret;

	ret = execute_command(cmd, shell);
	clear_minishell(shell);
	return (ret);
}

void	close_possible_pipe(t_shell *shell)
{
	if (shell->pipe.pipe[0] != -1)
	{
		close(shell->pipe.pipe[0]);
		shell->pipe.pipe[0] = -1;
	}
	if (shell->pipe.pipe[1] != -1)
	{
		close(shell->pipe.pipe[1]);
		shell->pipe.pipe[1] = -1;
	}
}

int	process_and(t_shell *shell, int ret, bool *should_continue)
{
	*should_continue = true;
	if (shell->last_pid > 0)
		ret = wait_previous_process(shell);
	if (ret != EXIT_SUCCESS)
		*should_continue = false;
	close_any_possible_fd(shell);
	return (ret);
}

int	process_or(t_shell *shell, int ret, bool *should_continue)
{
	*should_continue = true;
	if (shell->last_pid > 0)
		ret = wait_previous_process(shell);
	if (ret == EXIT_SUCCESS)
		*should_continue = false;
	close_any_possible_fd(shell);
	return (ret);
}
