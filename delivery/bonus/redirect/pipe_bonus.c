/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 19:23:07 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 09:53:29 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>			// pipe
#include "../minishell_bonus.h"
#include "../execute/execution_bonus.h"
#include "../redirect/redirect_aux_bonus.h"

int	prepare_pipe(t_shell *shell, const t_btnode *node)
{
	if ((node->right == NULL) || (node->left == NULL))
		return (EXIT_FAILURE);
	shell->is_running_redirect = true;
	shell->is_last_redirect = false;
	if ((((t_content_node *)node->left->content)->op == OP_PIPE)
		&& (((t_content_node *)node->left->content)->cmd.is_parentheses
			== false))
		return (EXIT_SUCCESS);
	if (pipe(shell->pipe.pipe) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	process_pipe(t_shell *shell, t_btnode *node)
{
	shell->is_running_redirect = true;
	if (shell->pipe.pipe[0] > 0)
		shell->pipe.carry_over_fd = dup(shell->pipe.pipe[0]);
	else
		shell->pipe.carry_over_fd = -1;
	node_cnt(node)->cmd.pipe_fd = shell->pipe.carry_over_fd;
	close(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[1]);
	if (pipe(shell->pipe.pipe) < 0)
		return (1);
	if (get_next_operation(node) != OP_PIPE)
	{
		shell->is_last_redirect = true;
	}
	return (0);
}
