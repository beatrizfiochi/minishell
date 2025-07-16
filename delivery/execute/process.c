/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 20:09:02 by bfiochi-         ###   ########.fr       */
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
#include "../parser/aux.h"
#include "../redirect/redirect.h"

static int	btree_operator_before_callback(t_btnode *node,
				int ret, bool *should_continue, void *_shell)
{
	t_content_node	*content;

	content = (t_content_node *)node->content;
	ignore_signals();
	*should_continue = false;
	if ((content->op == OP_RD_OUTPUT) || (content->op == OP_APPEND_RD_OUTPUT))
		ret = prepare_redirect_out((t_shell *)_shell, node);
	else if (content->op == OP_RD_INPUT)
		ret = prepare_redirect_in((t_shell *)_shell, node);
	if (ret != EXIT_SUCCESS)
		return (ret);
	*should_continue = true;
	if (!is_op_redirect_type(content->op))
		return (0);
	if ((node->right == NULL) || (node->left == NULL))
		return (1);
	((t_shell *)_shell)->is_running_redirect = true;
	((t_shell *)_shell)->is_last_redirect = false;
	if (((t_content_node *)node->left->content)->op == OP_PIPE)
		return (0);
	((t_shell *)_shell)->pipe.carry_over_fd = -1;
	if (pipe(((t_shell *)_shell)->pipe.pipe) < 0)
		return (1);
	return (0);
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
	}
	shell->last_exit_status = ret;
	return (ret);
}

static int	run_cmd(t_shell *shell, t_btnode *node, t_node_op parent_op)
{
	t_content_node	*content;
	int				ret;

	content = (t_content_node *)node->content;
	shell->last_cmd = &content->cmd;
	if (content->cmd.redir.fd_out > 0)
		shell->is_last_redirect = true;
	search_and_expand(content->cmd.tokens, shell->variable_list, shell);
	remove_empty_token(&content->cmd.tokens);
	handle_var_assign(shell, node);
	if (content->cmd.tokens == NULL)
	{
		content->cmd.is_builtin = true;
		join_shell_variable_lists(shell);
		return (EXIT_SUCCESS);
	}
	ret = EXIT_CMD_NOT_FOUND;
	if (!is_op_redirect_type(parent_op))
		ret = execute_builtin(&content->cmd, shell);
	if (ret == EXIT_CMD_NOT_FOUND)
		ret = execute_execve(node, shell);
	ft_lstclear(&shell->tmp_var_list, free_var_content);
	return (ret);
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
		.cb_leaf = btree_cmd_callback,
		.ctx = shell
	};

	shell->last_cmd = NULL;
	ret = btree_foreach_before_and_between_dfs(shell->cmds, &cfg);
	if (ret != 0)
		return (ret);
	if (shell->last_pid > 0)
		return (wait_previous_process(shell));
	else
		return (ret);
}
