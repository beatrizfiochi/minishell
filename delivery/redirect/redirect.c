/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:42 by djunho            #+#    #+#             */
/*   Updated: 2025/07/09 19:07:05 by djunho           ###   ########.fr       */
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

static bool	is_a_redirect_file_op(t_node_op op)
{
	return (op == OP_RD_OUTPUT || op == OP_APPEND_RD_OUTPUT
		|| op == OP_RD_INPUT || op == OP_HEREDOC);
}

static t_content_node	*get_next_cmd(t_btnode *node)
{
	if (node == NULL || node->content == NULL)
		return (NULL);
	while (node != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_CMD)
			return ((t_content_node *)node->content);
		if (is_a_redirect_file_op(((t_content_node *)node->content)->op))
			node = node->left;
		else
			node = node->right;
	}
	return (NULL);
}

static t_content_node	*get_first_cmd(t_btnode *node)
{
	if (node == NULL || node->content == NULL)
		return (NULL);
	while (node != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_CMD)
			return ((t_content_node *)node->content);
		node = node->left;
	}
	return (NULL);
}

// Go to the previous command (or the last command to be executed before
//  returning to the op_node) and mark its redirection
//  To make the exactly same behavior of bash from the command example above
// ls > oi1 > oi2
// we need to create the file oi1, but not write anything to it
// The file oi2 will be created and written to, as expected
int	prepare_redirect_out(t_shell *shell, t_btnode *op_node)
{
	t_content_node	*previous_cmd;
	int             fd;
	int				flag;

	shell->is_running_redirect = true;
	if (((t_content_node *)op_node->content)->op == OP_RD_OUTPUT)
		flag = O_CREAT | O_TRUNC | O_WRONLY;
	else
		flag = O_CREAT | O_APPEND | O_WRONLY;
	fd = open(((t_content_node *)(op_node->right->content))->cmd.tokens->content,
				flag, 0644);
	if (fd < 0)
	{
		perror(((t_content_node *)(op_node->right->content))->cmd.tokens->content);
		return (EXIT_FAILURE);
	}
	previous_cmd = get_next_cmd(op_node->left);
	if (previous_cmd == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Could not find the command on left of redirect out\n");
		return (EXIT_FAILURE);
	}
	if (previous_cmd->cmd.redir.fd_out != -1)
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	previous_cmd->cmd.redir.fd_out = fd;
	return (EXIT_SUCCESS);
}

int	prepare_redirect_in(t_shell *shell, t_btnode *op_node)
{
	t_content_node	*previous_cmd;
	int             fd;

	shell->is_running_redirect = true;
	fd = open(((t_content_node *)(op_node->right->content))->cmd.tokens->content,
				O_RDONLY);
	if (fd < 0)
	{
		perror(((t_content_node *)(op_node->right->content))->cmd.tokens->content);
		return (EXIT_FAILURE);
	}
	previous_cmd = get_first_cmd(op_node->left);
	if (previous_cmd == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Could not find the command on left of redirect in\n");
		return (EXIT_FAILURE);
	}
	if (previous_cmd->cmd.redir.fd_in != -1)
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	previous_cmd->cmd.redir.fd_in = fd;
	return (EXIT_SUCCESS);
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
	if (!shell->last_cmd->is_builtin && !shell->last_cmd->finished)
		ret = wait_previous_process(shell);
	(void)node;
	return (ret);
}

void	configure_redirect(t_shell *shell, const t_cmd *cmd)
{
	bool	is_pipe;

	is_pipe = (shell->pipe.pipe[0] != -1);
	if (is_pipe && shell->pipe.carry_over_fd != -1)
	{
		dup2(shell->pipe.carry_over_fd, STDIN_FILENO);
		close(shell->pipe.carry_over_fd);
	}
	else if (cmd->redir.fd_in > 0)
	{
		dup2(cmd->redir.fd_in, STDIN_FILENO);
		close(cmd->redir.fd_in);
	}
	close(shell->pipe.pipe[0]);
	if (cmd->redir.fd_out > 0)
	{
		dup2(cmd->redir.fd_out, STDOUT_FILENO);
		close(cmd->redir.fd_out);
	}
	else if (is_pipe && !shell->is_last_redirect)
		dup2(shell->pipe.pipe[1], STDOUT_FILENO);
	close(shell->pipe.pipe[1]);
}
