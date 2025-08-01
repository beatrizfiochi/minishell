/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:59:31 by djunho            #+#    #+#             */
/*   Updated: 2025/08/18 19:35:37 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>				// isatty
#include "banner.h"
#include "color.h"
#include "libft/libft.h"
#include "minishell.h"
#include "debug.h"
#include "variables/expand_variables.h"
#include "variables/variables.h"
#include "execute/execute_debug.h"		// debug_execution_pos();

static void	init_var_list(t_shell *shell, char *envp[])
{
	debug_env_var_init(shell);
	envp_list(shell, envp);
}

static void	init_shell(t_shell *shell)
{
	shell->variable_list = NULL;
	shell->tmp_var_list = NULL;
	shell->nlines = 0;
	shell->last_pid = -1;
	shell->cmds = NULL;
	reset_minishell(shell, EXIT_SUCCESS);
	shell->in_fd = STDIN_FILENO;
	shell->out_fd = STDOUT_FILENO;
}

static int	run_minishell(char *envp[])
{
	int		ret;
	t_shell	shell;

	init_shell(&shell);
	init_var_list(&shell, envp);
	if (isatty(STDIN_FILENO))
	{
		print_banner(&shell);
		ft_printf(COLOR_RESET"Developed by: bfiochi- and djunho\n\n");
	}
	update_shell_level(shell.variable_list);
	ret = read_command(&shell);
	while (ret >= 0)
	{
		debug_execution_pos(&shell);
		debug_print_env_var(&shell);
		ret = read_command(&shell);
	}
	clear_minishell(&shell);
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	sh_clean_history();
	return (shell.last_exit_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	return (run_minishell(envp));
}
