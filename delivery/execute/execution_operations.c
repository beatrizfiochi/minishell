/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_operations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 11:59:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/22 19:52:18 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>		// bool
#include <sys/types.h>		// pid_t
#include <unistd.h>			// execve, fork, pipe, dup2, close
#include <sys/wait.h>		// wait
#include "../minishell.h"
#include "../cmd.h"
#include "../signals/signals.h"
#include "../builtins/builtins.h"
#include "execution.h"
#include "exec_utils.h"
#include "env_utils.h"
#include "../minishell.h"

static int	execute_command(char **args, char **envp, t_cmd *cmd,
								t_shell *shell)
{
	int		ret;
	char	*path;

	if ((args == NULL) || (args[0] == NULL))
		return (EXIT_FAILURE);
	reset_signals();
	ret = execute_builtin(cmd, shell);
	if (ret != EXIT_CMD_NOT_FOUND)
		return (ret);
	if (is_directory(args[0]))
	{
		ft_fprintf(STDERR_FILENO, "%s: Is a directory\n", args[0]);
		return (EXIT_CMD_CANNOT_EXEC);
	}
	if (check_file_exists(args, envp) == EXIT_CMD_NOT_FOUND)
		return (EXIT_CMD_NOT_FOUND);
	else if (check_file_exists(args, envp) == EXIT_CMD_CANNOT_EXEC)
		return (EXIT_CMD_CANNOT_EXEC);
	if (create_cmd_path(args[0], shell->variable_list, &path))
	{
		execve(path, args, envp);
		free(path);
	}
	ft_fprintf(STDERR_FILENO, "%s: command not found\n", args[0]);
	return (EXIT_CMD_NOT_FOUND);
}

int	run_child(t_cmd *cmd, t_shell *shell)
{
	char	**args;
	char	**envp;
	int		ret;
	int		argc;

	args = convert_list_to_vector(cmd->tokens, &argc);
	envp = convert_list_to_envp(shell->variable_list, shell->tmp_var_list);
	ret = execute_command(args, envp, cmd, shell);
	clear_minishell(shell);
	if (args != NULL)
		free(args);
	if (envp != NULL)
		free_envp(envp);
	return (ret);
}

static void	close_possible_pipe(t_shell *shell)
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
	if (shell->pipe.carry_over_fd != -1)
	{
		close(shell->pipe.carry_over_fd);
		shell->pipe.carry_over_fd = -1;
	}
}

int	process_and(t_shell *shell, int ret, bool *should_continue)
{
	*should_continue = true;
	if ((shell->last_cmd != NULL) && (ret == EXIT_SUCCESS))
	{
		if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
			ret = wait_previous_process(shell);
		if (ret != EXIT_SUCCESS)
			*should_continue = false;
	}
	else
	{
		*should_continue = (ret == EXIT_SUCCESS);
	}
	close_possible_pipe(shell);
	return (ret);
}

int	process_or(t_shell *shell, int ret, bool *should_continue)
{
	*should_continue = true;
	if ((shell->last_cmd != NULL) && (ret == EXIT_SUCCESS))
	{
		if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
			ret = wait_previous_process(shell);
		if (ret == EXIT_SUCCESS)
			*should_continue = false;
	}
	else
	{
		*should_continue = (ret != EXIT_SUCCESS);
	}
	close_possible_pipe(shell);
	return (ret);
}
