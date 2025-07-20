/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:42 by djunho            #+#    #+#             */
/*   Updated: 2025/07/20 22:58:10 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>			// EXIT_FAILURE
#include <fcntl.h>			// open()
#include <unistd.h>
#include "../minishell.h"
#include "../execute/execution.h"
#include "../execute/exec_utils.h"
#include "redirect_aux.h"

// Go to the previous command (or the last command to be executed before
//  returning to the op_node) and mark its redirection
//  To make the exactly same behavior of bash from the command example above
// ls > oi1 > oi2
// we need to create the file oi1, but not write anything to it
// The file oi2 will be created and written to, as expected
int	prepare_redirect_out(t_shell *shell, t_btnode *op)
{
	t_content_node	*previous_cmd;
	int				fd;
	int				flag;
	char			*name;

	shell->is_running_redirect = true;
	flag = O_CREAT | O_APPEND | O_WRONLY;
	if (((t_content_node *)op->content)->op == OP_RD_OUTPUT)
		flag = O_CREAT | O_TRUNC | O_WRONLY;
	name = get_redir_filename(shell, op->right);
	if (name == NULL)
		return (EXIT_FAILURE);
	fd = open(name, flag, 0644);
	if (fd < 0)
		return (p_error(name, EXIT_FAILURE));
	previous_cmd = get_next_cmd(op->left);
	if (previous_cmd == NULL)
		return (EXIT_FAILURE);
	if (previous_cmd->cmd.redir.fd_out != -1)
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	previous_cmd->cmd.redir.fd_out = fd;
	return (EXIT_SUCCESS);
}

int	prepare_redirect_in(t_shell *shell, t_btnode *op)
{
	t_content_node	*previous_cmd;
	int				fd;
	char			*name;

	shell->is_running_redirect = true;
	name = get_redir_filename(shell, op->right);
	if (name == NULL)
		return (EXIT_FAILURE);
	fd = open(name, O_RDONLY);
	if (fd < 0)
		return (p_error(name, EXIT_FAILURE));
	previous_cmd = get_first_cmd(op->left);
	if (previous_cmd == NULL)
		return (EXIT_FAILURE);
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

void	configure_redir(t_shell *shell, const t_cmd *cmd)
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
