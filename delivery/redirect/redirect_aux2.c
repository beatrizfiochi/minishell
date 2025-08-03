/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 08:14:24 by djunho            #+#    #+#             */
/*   Updated: 2025/08/03 11:26:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>                    // close
#include "../minishell.h"
#include "../parser/parser.h"
#include "../execute/execution.h"
#include "../execute/exec_utils.h"
#include "redirect_aux.h"

void	close_redirects(t_shell *shell)
{
	shell->is_running_redirect = false;
	shell->is_last_redirect = false;
	close_possible_pipe(shell);
}

char	*get_redir_filename(t_shell *shell, t_btnode *file_node)
{
	t_list	*token;
	int		size;

	token = ((t_content_node *)(file_node->content))->cmd.tokens;
	expand_variable_token(&token, shell->variable_list, shell);
	expand_wildcards_token(&token);
	remove_empty_token(&token);
	clean_token_quotes(token);
	((t_content_node *)(file_node->content))->cmd.tokens = token;
	size = ft_lstsize(token);
	if (size != 1)
	{
		ft_fprintf(STDERR_FILENO, "Ambiguous redirect\n");
		return (NULL);
	}
	return (((t_content_node *)(file_node->content))->cmd.tokens->content);
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
	else if ((shell->pipe.carry_over_fd != -1) && (is_pipe))
		dup2(shell->pipe.carry_over_fd, STDIN_FILENO);
	else if (is_pipe)
		close(shell->pipe.pipe[0]);
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

/**
 * Checks whether the given binary tree node represents a command branch.
 *
 * This function traverses the binary tree starting from the given node and
 * determines if the node or any of its relevant children represent a command
 * operation (OP_CMD). It also handles cases where the node contains redirection
 * file operations.
 *
 * @param node A pointer to the binary tree node to check. The node is expected
 *             to be of type `t_btnode` and may contain operation and content
 *             data.
 * @return `true` if the node or its relevant children represent a command
 *             branch, `false` otherwise.
 */
bool	is_cmd_branch(t_btnode *node)
{
	t_btnode		*aux;

	if (node == NULL || node->content == NULL)
		return (false);
	aux = node;
	if (node_cnt(aux)->op == OP_CMD)
		return (true);
	while (aux != NULL)
	{
		if (is_redirect_file_op(node_cnt(aux)->op))
			aux = aux->left;
		else if (node_cnt(aux)->op == OP_CMD)
			return (true);
		else
			return (false);
	}
	return (false);
}
