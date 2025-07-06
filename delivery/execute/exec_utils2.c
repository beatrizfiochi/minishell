/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:02:00 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 17:05:22 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "execution.h"
#include "../minishell.h"

int	wait_previous_process(t_shell *shell)
{
	int	wstatus;
	int	tmp;

	tmp = waitpid(shell->last_pid, &wstatus, 0);
	if (tmp > 0)
	{
		shell->last_cmd->finished = true;
		if (shell->redirect.fd_out > 0)
		{
			close(shell->redirect.fd_out);
			shell->redirect.fd_out = -1;
		}
		while ((tmp > 0))
			tmp = wait(NULL);
		shell->last_pid = -1;
		return (get_fork_return(wstatus));
	}
	return (EXIT_FAILURE);
}
