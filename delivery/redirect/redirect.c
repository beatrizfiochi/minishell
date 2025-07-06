/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:42 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 17:28:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>			// EXIT_FAILURE
#include <fcntl.h>			// open()
#include <unistd.h>
#include "../minishell.h"
#include "../execute/execution.h"
#include "../execute/exec_utils.h"

static t_node_op	get_next_operation(t_btnode *node)
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

int	prepare_redirect(t_shell *shell, t_btnode *op_node)
{
	t_content_node	*redirect_cnt;
	t_content_node	*content;
	int				flag;

	content = (t_content_node *)op_node->content;
	redirect_cnt = (t_content_node *)(op_node->right->content);
	if (content->op == OP_RD_OUTPUT)
		flag = O_CREAT | O_TRUNC | O_WRONLY;
	else if (content->op == OP_APPEND_RD_OUTPUT)
		flag = O_CREAT | O_APPEND | O_WRONLY;
	else
		return (EXIT_FAILURE);
	shell->redirect.fd_out = open((char *)(redirect_cnt->cmd.tokens->content),
			flag, 0644);
	if (shell->redirect.fd_out < 0)
		return (EXIT_FAILURE);
	return (0);
}

int	process_pipe(t_shell *shell, t_btnode *node)
{
	shell->is_running_redirect = true;
	shell->pipe.carry_over_fd = dup(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[0]);
	close(shell->pipe.pipe[1]);
	if (pipe(shell->pipe.pipe) < 0)
		return (1);
	if (!is_op_redirect_type(get_next_operation(node)))
	{
		shell->is_last_redirect = true;
	}
	return (0);
}

int	process_redirect(t_shell *shell, int ret, t_btnode *node,
						bool *should_continue)
{
	*should_continue = false;
	shell->is_running_redirect = true;
	if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
		ret = wait_previous_process(shell);
	shell->is_last_redirect = true;
	(void)node;
	return (ret);
}

void	configure_redirect(t_shell *shell)
{
	if (shell->pipe.carry_over_fd != -1)
	{
		dup2(shell->pipe.carry_over_fd, STDIN_FILENO);
		close(shell->pipe.carry_over_fd);
	}
	close(shell->pipe.pipe[0]);
	if (!shell->is_last_redirect)
		dup2(shell->pipe.pipe[1], STDOUT_FILENO);
	close(shell->pipe.pipe[1]);
	if (shell->redirect.fd_out > 0)
		dup2(shell->redirect.fd_out, STDOUT_FILENO);
}
