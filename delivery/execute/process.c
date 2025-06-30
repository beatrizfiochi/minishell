/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/06/30 20:32:04 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>			// printf
#include <stdbool.h>
#include <unistd.h>			// pipe
#include <sys/wait.h>		// wait
#include "../libft/libft.h"
#include "../cmd.h"
#include "../builtins/builtins.h"
#include "../debug.h"
#include "../parser/parser.h"
#include "../minishell.h"
#include "../signals/signals.h"
#include "../variables/variables.h"
#include "execute_debug.h"
#include "exec_utils.h"
#include "execution.h"
#include "../parser/parser.h"

static int	btree_operator_before_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_content_node	*content;

	content = (t_content_node *)node->content;
	(void)ret;
	(void)_shell;
	ignore_signals();
	*should_continue = true;
	if (content->op != OP_PIPE)
		return (0);
	if ((node->right == NULL) || (node->right->content == NULL))
		return (1);
	if ((node->left == NULL) || (node->left->content == NULL))
		return (1);
	if (((t_content_node *)node->right->content)->op != OP_CMD)
		return (0);
	if (((t_content_node *)node->left->content)->op != OP_CMD)
		return (0);
	content->pipe.is_last_pipe = false;
	content->pipe.carry_over_fd = -1;
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
			return (process_and(shell, ret, should_continue));
		else if (((t_content_node *)node->content)->op == OP_OR)
			return (process_or(shell, ret, should_continue));
		else if (((t_content_node *)node->content)->op == OP_PIPE)
			return (process_pipe(node));
		else if (((t_content_node *)node->content)->op == OP_VAR_ASSIGN)
			return (process_var_assign(node, shell));
	}
	return (-1);
}

static int	run_cmd(t_shell *shell, t_btnode *node,
				t_node_op parent_op, t_content_node	*parent_cnt)
{
	t_content_node	*content;
	int				ret;

	content = (t_content_node *)node->content;
	shell->last_cmd = &content->cmd;
	clean_token_quotes(content->cmd.tokens);
	if (parent_op != OP_PIPE)
	{
		ret = execute_builtin(&content->cmd,
				shell->variable_list);
		if (ret != 127)
			return (ret);
	}
	return (execute_execve(node, shell, parent_op, parent_cnt));
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
	if (parent_op == OP_VAR_ASSIGN)
		return (0);
	return (run_cmd((t_shell *)_shell, node, parent_op, parent_cnt));
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
	shell->last_cmd = NULL;
	ret = btree_foreach_before_and_between_dfs(shell->cmds, &cfg);
	if (ret != 0)
		return (ret);
	tmp = waitpid(shell->last_pid, &i, 0);
	if (tmp > 0)
	{
		while ((tmp > 0))
			tmp = wait(NULL);
		return (update_exit_status_and_print(shell, i));
	}
	return (ret);
}
