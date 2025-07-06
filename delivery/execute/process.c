/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:55:45 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 11:37:04 by bfiochi-         ###   ########.fr       */
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
	if (((t_content_node *)node->left->content)->op == OP_PIPE)
		return (0);
	((t_shell *)_shell)->pipe.will_run_a_pipe = true;
	((t_shell *)_shell)->pipe.is_last_pipe = false;
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
	if (ret != 0)
		return (0);
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
	}
	shell->last_exit_status = ret;
	return (ret);
}

// This function processes variable assignment commands into the
//  shell->tmp_var_list.
// It assumes that the var name is well formed
static int	handle_var_assign(t_shell *shell, t_btnode *node)
{
	t_content_node	*content;

	content = (t_content_node *)node->content;
	if (content->cmd.tokens == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error: Invalid variable assignment\n");
		return EXIT_FAILURE;
	}
	while (content->cmd.tokens != NULL)
	{
		t_list *op_node = search_op(content->cmd.tokens, EXP_ASSIGN);
		if (op_node == NULL)
			break ;
		t_list *name = prev_list_item(content->cmd.tokens, op_node);
		t_list *value = op_node->next;
		if (process_var_assign(name, op_node, value, shell) != 0)
		{
			ft_fprintf(STDERR_FILENO, "Error: Invalid variable assignment\n");
			return (EXIT_FAILURE);
		}
		content->cmd.tokens = value->next;
		value->next = NULL;
		ft_lstclear(&name, free);
	}
	return (EXIT_SUCCESS);
}

static void	join_shell_variable_lists(t_shell *shell)
{
	t_list *last;

	last = ft_lstlast(shell->variable_list);
	if (last == NULL)
		shell->variable_list = shell->tmp_var_list;
	else
		last->next = shell->tmp_var_list;
	shell->tmp_var_list = NULL;
}

static int	run_cmd(t_shell *shell, t_btnode *node, t_node_op parent_op)
{
	t_content_node	*content;
	int				ret;

	content = (t_content_node *)node->content;
	shell->last_cmd = &content->cmd;
	search_and_expand(content->cmd.tokens, shell->variable_list, shell);
	clean_token_quotes(content->cmd.tokens);
	ret = handle_var_assign(shell, node);
	if (ret != EXIT_SUCCESS)
		return (ret);
	if (content->cmd.tokens == NULL)
	{
		content->cmd.is_builtin = true;
		join_shell_variable_lists(shell);
		return (EXIT_SUCCESS);
	}
	ret = EXIT_CMD_NOT_FOUND;
	if (parent_op != OP_PIPE)
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
	shell->last_cmd = NULL;
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
