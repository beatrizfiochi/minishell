/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/02 19:56:27 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>				// printf
#include <stdlib.h>
#include "../btree/btree.h"
#include "../minishell.h"
#include "../cmd.h"
#include "aux.h"
#include "parser.h"
#include "tree_creator.h"

static t_btnode	*create_node(t_list *token_list, t_btnode *parent,
								t_btnode *left, t_btnode *right)
{
	t_content_node	*content;
	t_btnode		*tree_node;

	content = malloc(sizeof(t_content_node));
	if (content == NULL)
		return (NULL);
	content->op = op((char *)(token_list->content));
	content->cmd.tokens = token_list;
	content->cmd.is_builtin = false;
	content->cmd.finished = false;
	content->cmd.redir.fd_in = -1;
	content->cmd.redir.fd_out = -1;
	tree_node = btree_new(content);
	tree_node->parent = parent;
	tree_node->left = left;
	if (left != NULL)
		left->parent = tree_node;
	tree_node->right = right;
	if (right != NULL)
		right->parent = tree_node;
	return (tree_node);
}

// This function tries to detect and fix the inverted order of OP_RD_INPUT and
// OP_HEREDOC operators in the split token list.
// It "converts" from:
// < file cmd
// to:
// cmd < file
static int	fix_split(struct s_split_token_list *split,
								enum e_expand_type *expand)
{
	t_list *cmd;
	t_list *after_cmd;
	t_list *before_cmd;
	t_list *last_remain;

	if ((op_list(split->op) == OP_RD_INPUT) && (split->left == split->op))
	{
		if (split->remain == NULL)
		{
			split->left = split->right->next;
			split->right->next = NULL;
			return (EXIT_SUCCESS);
		}
		cmd = split->remain;
		while ((cmd != NULL) && (op_list(cmd) == OP_RD_INPUT))
		{
			cmd = cmd->next->next;
		}
		if (cmd == NULL)
			return (EXIT_FAILURE);
		split->left = cmd;
		// Get the previous of the command found
		before_cmd = prev_list_item(split->remain, split->left);
		if (before_cmd != split->remain)
			before_cmd->next = NULL;
		else
			split->remain = NULL;
		after_cmd = search_op(split->left, *expand);
		if (after_cmd == NULL)
			return (EXIT_SUCCESS);
		last_remain = ft_lstlast(split->remain);
		if (last_remain == NULL)
			split->remain = after_cmd;
		else
			last_remain->next = after_cmd;
	}
	return (EXIT_SUCCESS);
}

static t_btnode	*create_first(t_list **tokens, t_btnode *parent,
								enum e_expand_type *expand)
{
	t_list						*op_node;
	t_btnode					*tree;
	struct s_split_token_list	split;

	if ((*expand == EXP_PAREN) && (op_list(*tokens) == OP_PAREN_OPEN)
		&& (check_enclosure(tokens, expand) == false))
		return (NULL);
	op_node = search_op(*tokens, *expand);
	if ((op_node == NULL) || ((op_node->next == NULL)
			&& (op_list(op_node) == OP_PAREN_CLOSE)))
	{
		tree = create_node(*tokens, parent, NULL, NULL);
		*tokens = op_node;
		return (tree);
	}
	split = (struct s_split_token_list){
		.left = *tokens, .op = NULL, .right = NULL, .remain = NULL
	};
	if (split_token_list(&split, *expand) == false)
	{
		abort_tree_lst(NULL, &split, "Error: spliting tokens from list\n");
		*tokens = NULL;
		return (NULL);
	}
	if ((fix_split(&split, expand) == EXIT_FAILURE) || (split.left == split.op))
	{
		abort_tree_lst(NULL, &split, "Error: spliting tokens from list\n");
		*tokens = NULL;
		return (NULL);
	}
	tree = create_node(split.op, parent, NULL, NULL);
	tree->left = create_node(split.left, tree, NULL, NULL);
	tree->right = create_node(split.right, tree, NULL, NULL);
	*tokens = split.remain;
	return (tree);
}

// Search for and operator
// Every operator must be placed between 2 commands
//
t_btnode	*create_basic_tree(t_list **token_list, t_btnode *parent,
								enum e_expand_type expand_type)
{
	t_btnode					*tree;
	t_btnode					*old_tree;
	struct s_split_token_list	split;

	tree = create_first(token_list, parent, &expand_type);
	if (tree == NULL)
		return (NULL);
	split = (struct s_split_token_list){
		.left = NULL, .op = NULL, .right = NULL, .remain = *token_list
	};
	while ((split.remain != NULL) && !((expand_type == EXP_PAREN)
			&& (split.remain->next == NULL)
			&& (op_list(*token_list) == OP_PAREN_CLOSE)))
	{
		old_tree = tree;
		split = (struct s_split_token_list){
			.left = split.remain, .op = NULL, .right = NULL, .remain = NULL
		};
		if (split_token_list(&split, expand_type) == false)
			return (abort_tree_lst(tree, &split, NULL));
		if (split.left != split.op)
			return (abort_tree_lst(tree, &split, NULL));
		tree = create_node(split.op, parent, old_tree, NULL);
		tree->right = create_node(split.right, tree, NULL, NULL);
	}
	*token_list = split.remain;
	return (tree);
}

t_btnode	*create_tree(t_list **token_list, t_btnode *parent)
{
	t_btnode	*tree;
	bool		expanded;

	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	tree = create_basic_tree(token_list, parent, false);
	if (tree == NULL)
		return (NULL);
	expanded = true;
	while (expanded == true)
	{
		expanded = false;
		tree = expand_tree_pipe(tree, &expanded);
		if (tree == NULL)
			return (NULL);
		tree = expand_tree_parenthesis(tree, &expanded);
		if (tree == NULL)
			return (NULL);
	}
	return (check_tree_syntax(tree));
}
