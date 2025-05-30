/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/06/01 20:33:26 by bfiochi-         ###   ########.fr       */
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
#include "../signals/signals.h"
#include "execution.h"
#include "../variables/variables.h"

static int	btree_operator_before_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_content_node	*content;
	t_content_node	*parent_content;

	(void)ret;
	(void)_shell;
	content = (t_content_node *)node->content;
	ignore_signals();
	*should_continue = true;
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
				int ret, bool *should_continue, void *_shell)
{
	t_shell			*shell;

	shell = _shell;
	if (ret != 0)
		return (0);
	*should_continue = true;
	if (node->content != NULL)
	{
		ignore_signals();
		if (((t_content_node *)node->content)->op == OP_AND)
		{
			return (process_and(shell, should_continue));
		}
		else if (((t_content_node *)node->content)->op == OP_OR)
		{
			return (process_or(shell, should_continue));
		}
		else if (((t_content_node *)node->content)->op == OP_PIPE)
		{
			return (process_pipe(node));
		}
		else if (((t_content_node *)node->content)->op == OP_VAR_ASSIGN)
		{
			return (process_var_assign(node, shell));
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
	t_node_op		operator;

	shell = (t_shell *)_shell;
	parent_content = NULL;
	operator = OP_INVALID;
	if (node->parent != NULL)
	{
		parent_content = (t_content_node *)node->parent->content;
		operator = parent_content->op;
	}
	if (node->content == NULL)
		return (0);
	debug_btree_print(node);
	if (operator == OP_VAR_ASSIGN)
		return (0);
	shell->last_pid = fork();
	if (shell->last_pid < 0)
		return (1);
	if (shell->last_pid == 0)
	{
		if (operator == OP_PIPE)
			configure_pipe(parent_content->pipe.pipe,
				parent_content->pipe.carry_over_fd,
				parent_content->pipe.is_last_pipe);
		exit(run_child(&((t_content_node *)node->content)->cmd, shell));
	}
	return (0);
}

int	process(t_shell *shell)
{
	int								tmp;
	int								i;
	int								ret;
	const t_btree_foreach_dfs_cb	cfg = {
		.cb_node_before = btree_operator_before_callback,
		.cb_node_between = btree_operator_between_callback,
		.cb_leaf = btree_cmd_callback,
		.ctx = shell
	};

	i = -1;
	shell->last_exit_status = EXIT_SUCCESS;
	ret = btree_foreach_before_and_between_dfs(shell->cmds, &cfg);
	if (ret != 0)
		return (ret);
	tmp = waitpid(shell->last_pid, &i, 0);
	if (tmp > 0)
	{
		while ((tmp > 0))
			tmp = wait(NULL);
		return (get_fork_return(i));
	}
	return (ret);
}
