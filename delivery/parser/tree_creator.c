/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/27 20:02:21 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>				// printf
#include <stdlib.h>
#include "../btree/btree.h"
#include "../minishell.h"
#include "../cmd.h"
#include "aux.h"
#include "parser.h"

static t_btnode	*create_node(t_list *token_list, t_btnode *parent)
{
	t_content_node	*content;
	t_btnode		*tree_node;

	content = malloc(sizeof(t_content_node));
	if (content == NULL)
		return (NULL);
	content->op = op((char *)(token_list->content));
	if (content->op == OP_CMD)
	{
		content->cmd.tokens = token_list;
		content->cmd.is_builtin = false;
		content->cmd.finished = false;
	}
	else
		ft_lstclear(&token_list, free);
	if (content->op == OP_PIPE)
	{
		content->pipe.pipe[0] = -1;
		content->pipe.pipe[1] = -1;
		content->pipe.carry_over_fd = -1;
	}
	tree_node = btree_new(content);
	tree_node->parent = parent;
	return (tree_node);
}

static t_btnode	*create_first(t_list **token_list, t_btnode *parent,
								bool full_expand)
{
	t_list						*op_node;
	t_btnode					*tree;
	struct s_split_token_list	split;

	op_node = search_op(*token_list, full_expand);
	if (op_node == NULL)
	{
		tree = create_node(*token_list, parent);
		*token_list = op_node;
		return (tree);
	}
	split = (struct s_split_token_list){
		.left = *token_list, .op = NULL,
		.right = NULL, .remain = NULL
	};
	if (split_token_list(&split, true, full_expand) == false)
		return (NULL);
	tree = create_node(split.op, parent);
	tree->left = create_node(split.left, tree);
	tree->right = create_node(split.right, tree);
	*token_list = split.remain;
	return (tree);
}

// Search for and operator
// Every operator must be placed between 2 commands
//
static t_btnode	*create_basic_tree(t_list **token_list, t_btnode *parent,
								bool full_expand)
{
	t_btnode					*tree;
	t_btnode					*old_tree;
	struct s_split_token_list	split;

	tree = create_first(token_list, parent, full_expand);
	if (tree == NULL)
		return (NULL);
	split = (struct s_split_token_list){
		.left = NULL, .op = NULL,
		.right = NULL, .remain = *token_list
	};
	while (split.remain != NULL)
	{
		old_tree = tree;
		split = (struct s_split_token_list){
			.left = split.remain, .op = NULL, .right = NULL, .remain = NULL
		};
		if (split_token_list(&split, false, full_expand) == false)
			return (abort_tree_lst(tree, &split.left, NULL));
		tree = create_node(split.op, parent);
		tree->left = old_tree;
		tree->left->parent = tree;
		tree->right = create_node(split.right, tree);
	}
	return (tree);
}

static t_btnode	*expand_pipe_btree_node(t_btnode *node)
{
	t_content_node	*content;
	t_btnode		*new_tree;

	if (btree_is_leaf(node))
	{
		content = (t_content_node *)node->content;
		if (content->op == OP_CMD)
		{
			new_tree = create_basic_tree(&content->cmd.tokens,
					node->parent, true);
			btree_delete(&node, free_btree_node);
			return (new_tree);
		}
		else
		{
			printf("Error: Operator without command\n");
			return (node);
		}
	}
	if (node->left != NULL)
		node->left = expand_pipe_btree_node(node->left);
	if (node->right != NULL)
		node->right = expand_pipe_btree_node(node->right);
	return (node);
}

t_btnode	*create_tree(t_list **token_list, t_btnode *parent)
{
	t_btnode	*tree;

	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	tree = create_basic_tree(token_list, parent, false);
	if (tree == NULL)
		return (NULL);
	tree = expand_pipe_btree_node(tree);
	if (tree == NULL)
	{
		printf("Error: Failed to expand pipe btree node\n");
		return (NULL);
	}
	return (tree);
}
