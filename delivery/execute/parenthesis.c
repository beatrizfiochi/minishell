/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parenthesis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 19:20:56 by djunho            #+#    #+#             */
/*   Updated: 2025/07/23 21:02:23 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../minishell.h"
#include "../cmd.h"
#include "../btree/btree.h"
#include "execution.h"
#include <stdio.h>			// p_error
#include <sys/wait.h>		// waitpid
#include <unistd.h>			// fork

int	prepare_parenthesis(t_btnode *node, bool *should_continue)
{
	t_content_node	*content;
	pid_t			pid;
	int				wstatus;
	int				tmp;

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
		return (EXIT_SUCCESS);
	*should_continue = false;
	tmp = waitpid(pid, &wstatus, 0);
	if (tmp > 0)
	{
		while ((tmp > 0))
			tmp = wait(NULL);
		return (get_fork_return(wstatus));
	}
	return (EXIT_FAILURE);
}

int	process_parenthesis(t_shell *shell, t_btnode *node, int ret,
							bool *should_continue)
{
	t_content_node	*content;

	(void)shell;
	(void)node;
	(void)should_continue;
	content = (t_content_node *)node->content;
	if (content->cmd.is_parentheses)
	{
		clear_minishell(shell);
		exit(ret);
	}
	return (EXIT_SUCCESS);
}
