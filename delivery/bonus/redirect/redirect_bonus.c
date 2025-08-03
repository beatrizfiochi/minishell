/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:56:42 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 11:08:37 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>			// EXIT_FAILURE
#include <fcntl.h>			// open()
#include <unistd.h>
#include "../minishell_bonus.h"
#include "../execute/exec_utils_bonus.h"
#include "redirect_aux_bonus.h"

static void	common_redirect_prepare(t_shell *shell, bool is_output)
{
	shell->is_running_redirect = true;
	if (is_output)
		shell->is_last_redirect = true;
}

// Go to the previous command (or the last command to be executed before
//  returning to the op_node) and mark its redirection
//  To make the exactly same behavior of bash from the command example above
// ls > oi1 > oi2
// we need to create the file oi1, but not write anything to it
// The file oi2 will be created and written to, as expected
static int	prepare_redirect_out(t_shell *shell, t_btnode *op, t_cmd *cmd)
{
	int				fd;
	int				flag;
	char			*name;

	common_redirect_prepare(shell, true);
	flag = O_CREAT | O_APPEND | O_WRONLY;
	if (((t_content_node *)op->content)->op == OP_RD_OUTPUT)
		flag = O_CREAT | O_TRUNC | O_WRONLY;
	name = get_redir_filename(shell, op->right);
	if (name == NULL)
		return (EXIT_FAILURE);
	fd = open(name, flag, 0644);
	if (fd < 0)
		return (p_error(name, EXIT_FAILURE));
	if (cmd == NULL)
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	cmd->redir.fd_out = fd;
	node_cnt(op)->cmd.redir.fd_out = fd;
	return (EXIT_SUCCESS);
}

static int	prepare_redirect_in(t_shell *shell, t_btnode *op, t_cmd *cmd)
{
	int				fd;
	char			*name;

	common_redirect_prepare(shell, false);
	name = get_redir_filename(shell, op->right);
	if (name == NULL)
		return (EXIT_FAILURE);
	fd = open(name, O_RDONLY);
	if (fd < 0)
		return (p_error(name, EXIT_FAILURE));
	if (cmd == NULL)
	{
		close(fd);
		return (EXIT_SUCCESS);
	}
	cmd->redir.fd_in = fd;
	node_cnt(op)->cmd.redir.fd_in = fd;
	return (EXIT_SUCCESS);
}

int	prepare_redirect(t_shell *shell, t_btnode *node, t_cmd *cmd)
{
	t_btnode	*aux;
	int			ret;

	aux = node;
	ret = EXIT_SUCCESS;
	while (aux != NULL)
	{
		if (node_cnt(aux) == NULL)
			return (EXIT_SUCCESS);
		if ((node_cnt(aux)->op == OP_RD_OUTPUT)
			|| (node_cnt(aux)->op == OP_APPEND_RD_OUTPUT))
			ret = prepare_redirect_out(shell, aux, cmd);
		else if (node_cnt(aux)->op == OP_RD_INPUT)
			ret = prepare_redirect_in(shell, aux, cmd);
		if (ret != EXIT_SUCCESS)
			return (ret);
		if (node_cnt(aux)->cmd.is_parentheses)
			return (EXIT_SUCCESS);
		if ((node_cnt(aux)->op == OP_AND) || (node_cnt(aux)->op == OP_OR))
			return (EXIT_SUCCESS);
		aux = aux->parent;
	}
	return (EXIT_SUCCESS);
}

int	process_redirect(t_shell *shell, int ret, t_btnode *node,
						bool *should_continue)
{
	(void)node;
	*should_continue = false;
	if ((shell->last_cmd != NULL) && (shell->last_pid != -1)
		&& (!shell->last_cmd->is_builtin && !shell->last_cmd->finished))
		ret = wait_previous_process(shell);
	return (ret);
}
