/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/05/26 12:04:34 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>			// pipe
#include <sys/wait.h>		// wait
#include "../libft/libft.h"
#include "../cmd.h"
#include "../debug.h"
#include "../minishell.h"
#include "execution.h"

static int	btree_operator_before_callback(t_btnode *node,
				int ret, void *_shell)
{
	t_content_node	*content;
	t_content_node	*parent_content;

	(void)ret;
	(void)_shell;
	content = (t_content_node *)node->content;
	if (content->op != OP_PIPE)
		return (0);
	if ((node->left == NULL) || (node->left->content == NULL))
		return (1);
	if (((t_content_node *)node->left->content)->op != OP_CMD)
		return (0);
	content->pipe.is_last_pipe = false;
	content->pipe.carry_over_fd = -1;
	if (node->parent != NULL)
	{
		parent_content = (t_content_node *)node->parent->content;
		if ((parent_content != NULL) && (parent_content->op == OP_PIPE))
			content->pipe.carry_over_fd = parent_content->pipe.carry_over_fd;
	}
	if (pipe(content->pipe.pipe) < 0)
		return (1);
	return (0);
}

static int	btree_operator_between_callback(t_btnode *node,
				int ret, void *_shell)
{
	t_shell			*shell;

	shell = _shell;
	if (ret != 0)
		return (0);
	if (node->content != NULL)
	{
		if (((t_content_node *)node->content)->op == OP_AND)
		{
			return (process_and(shell));
		}
		else if (((t_content_node *)node->content)->op == OP_OR)
		{
			return (process_or(shell));
		}
		else if (((t_content_node *)node->content)->op == OP_PIPE)
		{
			return (process_pipe(node));
		}
	}
	return (-1);
}

__attribute__((weak)) void	debug_btree_print(t_btnode *node)
{
	(void)node;
}

static int	btree_cmd_callback(t_btnode *node, void *_shell)
{
	t_shell			*shell;
	t_content_node	*parent_content;

	shell = (t_shell *)_shell;
	if (node->content == NULL)
		return (0);
	debug_btree_print(node);
	shell->last_pid = fork();
	if (shell->last_pid < 0)
		return (1);
	if (shell->last_pid == 0)
	{
		if ((node->parent != NULL) && (node->parent->content != NULL)
			&& (((t_content_node *)node->parent->content)->op == OP_PIPE))
		{
			parent_content = (t_content_node *)node->parent->content;
			configure_pipe(parent_content->pipe.pipe,
				parent_content->pipe.carry_over_fd,
				parent_content->pipe.is_last_pipe);
		}
		exit(run_child(&((t_content_node *)node->content)->cmd, shell));
	}
	return (0);
}

int	process(t_shell *shell)
{
	int								tmp;
	int								i;
	const t_btree_foreach_dfs_cb	cfg = {
		.cb_node_before = btree_operator_before_callback,
		.cb_node_between = btree_operator_between_callback,
		.cb_leaf = btree_cmd_callback,
		.ctx = shell
	};

	i = -1;
	btree_foreach_before_and_between_dfs(shell->cmds, &cfg);
	tmp = waitpid(shell->last_pid, &i, 0);
	while ((tmp > 0))
	{
		tmp = wait(NULL);
	}
	if (WIFEXITED(i))
		return (WEXITSTATUS(i));
	return (0);
}
