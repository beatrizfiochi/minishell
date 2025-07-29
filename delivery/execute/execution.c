/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 13:53:41 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 18:49:03 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>			// exit()
#include <stdio.h>			// perror()
#include <stdbool.h>
#include <fcntl.h>			// open
#include <unistd.h>			// close
#include "../libft/libft.h"	// ft_printf
#include "execution.h"
#include "../parser/parser.h"
#include "../builtins/builtins.h"

static int	execute_execve(t_btnode *node, t_shell *shell)
{
	shell->last_pid = fork();
	if (shell->last_pid < 0)
	{
		perror("Error on fork");
		return (1);
	}
	if (shell->last_pid == 0)
	{
		if (shell->is_running_redirect)
			configure_redir(shell, &(((t_content_node *)node->content)->cmd));
		exit(run_child(&((t_content_node *)node->content)->cmd, shell));
	}
	if (shell->is_last_redirect)
	{
		shell->is_running_redirect = false;
		shell->is_last_redirect = false;
	}
	return (0);
}

int	run_cmd(t_shell *shell, t_btnode *node, t_node_op parent_op)
{
	t_content_node	*content;
	int				ret;

	content = (t_content_node *)node->content;
	shell->last_cmd = &content->cmd;
	if (content->cmd.redir.fd_out > 0)
		shell->is_last_redirect = true;
	search_and_expand(&content->cmd.tokens, shell->variable_list, shell);
	handle_var_assign(shell, node);
	if (content->cmd.tokens == NULL)
	{
		content->cmd.is_builtin = true;
		if (!is_op_redirect_type(parent_op))
			join_shell_variable_lists(shell);
		else
			ft_lstclear(&shell->tmp_var_list, free_var_content);
		return (EXIT_SUCCESS);
	}
	ret = EXIT_CMD_NOT_FOUND;
	if (!is_pipe(node))
		ret = execute_builtin(&content->cmd, shell, true);
	if (ret == EXIT_CMD_NOT_FOUND)
		ret = execute_execve(node, shell);
	ft_lstclear(&shell->tmp_var_list, free_var_content);
	return (ret);
}

int	get_fork_return(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
	{
		return (WTERMSIG(wstatus) + EXIT_SIGNAL_BASE);
	}
	else if (WIFSTOPPED(wstatus))
	{
		return (WSTOPSIG(wstatus) + EXIT_SIGNAL_BASE);
	}
	else
	{
		printf("Error unknown in child process\n");
		return (1);
	}
}

int	execute(t_shell *shell)
{
	shell->ncmds = btree_count_leaves(shell->cmds);
	return (process(shell));
}
