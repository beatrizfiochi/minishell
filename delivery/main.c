/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:59:31 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 12:28:29 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>				// isatty
#include <stdio.h>				// printf
#include "libft/libft.h"
#include "minishell.h"
#include "parser/parser.h"
#include "variables/variables.h"
#include "signals/signals.h"
#include "execute/execute_debug.h"		// debug_execution_pos();

void	clear_minishell(t_shell *shell)
{
	btree_clear(&shell->cmds, free_btree_node);
	if (shell->variable_list != NULL)
		ft_lstclear(&shell->variable_list, free_var_content);
}

static void	init_var_list(t_shell *shell, char *envp[])
{
	#ifdef TEST
	char	*name_1 = "oi";
	char	*value_1 = "hi";
	char	*name_2 = "tchau";
	char	*value_2 = "bye";
	char	*name_3 = "oooi";
	char	*value_3 = "hhhi";
	char	*name_4 = "oiii";
	char	*value_4 = "hiii";

	shell->variable_list = create_var_node(name_1, value_1);
	shell->variable_list->next = create_var_node(name_2, value_2);
	shell->variable_list->next->next = create_var_node(name_3, value_3);
	shell->variable_list->next->next->next = create_var_node(name_4, value_4);
	shell->variable_list->next->next->next->next = NULL;

	#endif // TEST

	envp_list(shell, envp);
}
static int	run_minishell(char *envp[])
{
	int		ret;
	t_shell	shell;

	shell.variable_list = NULL;
	shell.last_exit_status = EXIT_SUCCESS;
	shell.cmds = NULL;
	init_var_list(&shell, envp);
	ret = read_command(&shell, envp);
	while (ret >= 0)
	{
		debug_execution_pos(&shell);
		#ifdef TEST
		t_list	*variable_iter;
		variable_iter = shell.variable_list;
		while (variable_iter != NULL)
		{
			printf("var_name = %s, var_value = %s\n",
				((t_content_var *)(variable_iter->content))->var_name,
				((t_content_var *)(variable_iter->content))->var_value);
			variable_iter = variable_iter->next;
		}
		#endif // TEST
		ret = read_command(&shell, envp);
	}
	clear_minishell(&shell);
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "exit\n", 5);
	return (shell.last_exit_status);
}

int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	(void)envp;
	if (isatty(STDIN_FILENO))
	{
		ft_printf("Hello, World of minishell!\n");
	}
	return (run_minishell(envp));
}
