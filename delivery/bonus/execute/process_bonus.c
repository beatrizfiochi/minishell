/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/07/30 23:55:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>			// printf
#include <stdbool.h>
#include <unistd.h>			// pipe
#include <sys/wait.h>		// wait
#include "../../libft/libft.h"
#include "../cmd.h"
#include "../debug.h"
#include "../minishell.h"
#include "../signals/signals.h"
#include "execute_debug.h"
#include "exec_utils.h"
#include "execution.h"
#include "../redirect/redirect.h"
#include "../redirect/redirect_aux.h"	// is_redirect_file_op()

// Callback functions for the btree traversal
// Special cases:
//   A redirect (with no command) that is inside a pipe will return success
//     no matter what.
//     Example: `< a | < b` will have exit status as 0. But `< a` will have
//     exit status as 1
static int	btree_operator_before_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_content_node	*content;

	content = (t_content_node *)node->content;
	ignore_signals();
	*should_continue = true;
	ret = prepare_parenthesis((t_shell *)_shell, node, should_continue);
	if (*should_continue == false)
		return (ret);
	*should_continue = false;
	if ((node->left == NULL) && (is_redirect_file_op(content->op)))
	{
		ret = prepare_redirect((t_shell *)_shell, node, NULL);
		if (((t_shell *)_shell)->pipe.pipe[0] != -1)
			ret = 0;
	}
	else if (content->op == OP_PIPE)
		ret = prepare_pipe((t_shell *)_shell, node);
	if (ret != EXIT_SUCCESS)
		return (ret);
	*should_continue = true;
	return (ret);
}

static int	btree_operator_between_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_shell			*shell;

	shell = _shell;
	*should_continue = true;
	if (node->content != NULL)
	{
		ignore_signals();
		if (((t_content_node *)node->content)->op == OP_AND)
			ret = process_and(shell, ret, should_continue);
		else if (((t_content_node *)node->content)->op == OP_OR)
			ret = process_or(shell, ret, should_continue);
		else if (((t_content_node *)node->content)->op == OP_PIPE)
			ret = process_pipe(shell, node);
		else if ((((t_content_node *)node->content)->op == OP_APPEND_RD_OUTPUT)
			|| (((t_content_node *)node->content)->op == OP_RD_OUTPUT)
			|| (((t_content_node *)node->content)->op == OP_RD_INPUT))
			ret = process_redirect(shell, ret, node, should_continue);
		if (!is_redirect_file_op(((t_content_node *)node->content)->op)
			&& (((t_content_node *)node->content)->op != OP_PIPE))
			close_redirects(shell);
	}
	shell->last_exit_status = ret;
	return (ret);
}

static int	btree_operator_after_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_shell			*shell;

	shell = _shell;
	return (process_parenthesis(shell, node, ret, should_continue));
}

static int	btree_cmd_callback(t_btnode *node, void *_shell)
{
	t_content_node	*parent_cnt;
	t_node_op		parent_op;

	parent_cnt = NULL;
	parent_op = OP_INVALID;
	if (node->parent != NULL)
	{
		parent_cnt = (t_content_node *)node->parent->content;
		parent_op = parent_cnt->op;
	}
	if ((node->content == NULL)
		|| (((t_content_node *)node->content)->op != OP_CMD))
	{
		printf("Error: Node is not a command\n");
		return (EXIT_FAILURE);
	}
	debug_btree_print(node);
	return (run_cmd((t_shell *)_shell, node, parent_op));
}

int	process(t_shell *shell)
{
	int								ret;
	const t_btree_foreach_dfs_cb	cfg = {
		.cb_node_before = btree_operator_before_callback,
		.cb_node_between = btree_operator_between_callback,
		.cb_node_after = btree_operator_after_callback,
		.cb_leaf = btree_cmd_callback,
		.ctx = shell
	};

	shell->last_cmd = NULL;
	ret = btree_foreach_before_and_between_dfs(shell->cmds, &cfg);
	if (ret != 0)
		return (ret);
	close_any_possible_fd(shell);
	close_all_pipes(shell->cmds);
	if (shell->last_pid > 0)
		return (wait_previous_process(shell));
	else
		wait_previous_process(shell);
	return (ret);
}
