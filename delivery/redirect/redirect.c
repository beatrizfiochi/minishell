/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:42 by djunho            #+#    #+#             */
/*   Updated: 2025/07/25 23:56:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>			// EXIT_FAILURE
#include <fcntl.h>			// open()
#include <unistd.h>
#include "../minishell.h"
#include "../execute/execution.h"
#include "../execute/exec_utils.h"
#include "redirect_aux.h"

static void	common_redirect_prepare(t_shell *shell)
{
	shell->is_running_redirect = true;
	shell->is_last_redirect = true;
}

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

	common_redirect_prepare(shell);
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
	if ((previous_cmd == NULL) || (previous_cmd->cmd.redir.fd_out != -1))
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	previous_cmd->cmd.redir.fd_out = fd;
	((t_content_node *)op->content)->cmd.redir.fd_out = fd;
	return (EXIT_SUCCESS);
}

int	prepare_redirect_in(t_shell *shell, t_btnode *op)
{
	t_content_node	*previous_cmd;
	int				fd;
	char			*name;

	common_redirect_prepare(shell);
	name = get_redir_filename(shell, op->right);
	if (name == NULL)
		return (EXIT_FAILURE);
	fd = open(name, O_RDONLY);
	if (fd < 0)
		return (p_error(name, EXIT_FAILURE));
	previous_cmd = get_first_cmd(op->left);
	if ((previous_cmd == NULL) || (previous_cmd->cmd.redir.fd_in != -1))
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	previous_cmd->cmd.redir.fd_in = fd;
	((t_content_node *)op->content)->cmd.redir.fd_in = fd;
	return (EXIT_SUCCESS);
}

int	process_redirect(t_shell *shell, int ret, t_btnode *node,
						bool *should_continue)
{
	(void)node;
	*should_continue = false;
	if ((shell->last_cmd != NULL)
		&& (!shell->last_cmd->is_builtin && !shell->last_cmd->finished))
		ret = wait_previous_process(shell);
	return (ret);
}

void	configure_redir(t_shell *shell, const t_cmd *cmd)
{
	bool	is_pipe;

	is_pipe = (shell->pipe.pipe[0] != -1);
	if (cmd->redir.fd_in > 0)
	{
		dup2(cmd->redir.fd_in, STDIN_FILENO);
		close(cmd->redir.fd_in);
	}
	else if (shell->pipe.carry_over_fd != -1)
		dup2(shell->pipe.carry_over_fd, STDIN_FILENO);
	if (cmd->redir.fd_out > 0)
	{
		dup2(cmd->redir.fd_out, STDOUT_FILENO);
		close(cmd->redir.fd_out);
	}
	else if (is_pipe && !shell->is_last_redirect)
		dup2(shell->pipe.pipe[1], STDOUT_FILENO);
	else if (shell->out_fd != STDOUT_FILENO)
		dup2(shell->out_fd, STDOUT_FILENO);
	close_any_possible_fd(shell);
}
