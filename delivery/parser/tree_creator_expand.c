/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/27 12:29:27 by djunho           ###   ########.fr       */
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

static bool	is_expansion_needed(t_node_op op)
{
	if ((op == OP_CMD) || (op == OP_PAREN_OPEN) || (op == OP_RD_INPUT)
		|| (op == OP_RD_OUTPUT) || (op == OP_HEREDOC)
		|| (op == OP_APPEND_RD_OUTPUT))
		return (true);
	return (false);
}

static	void	update_node_properties(bool *had_expand, t_btnode *new_node,
						t_btnode *old_node, enum e_expand_type expand)
{
	t_content_node	*new_cnt;
	t_content_node	*old_cnt;

	new_cnt = (t_content_node *)new_node->content;
	if (is_btnode_different(old_node, new_node))
	{
		*had_expand = true;
		if (expand == EXP_PAREN)
			new_cnt->cmd.is_parentheses = true;
	}
	old_cnt = (t_content_node *)old_node->content;
	if (old_cnt->cmd.is_parentheses)
		new_cnt->cmd.is_parentheses = old_cnt->cmd.is_parentheses;
}

static t_btnode	*expand_btree_node(t_btnode *node,
						enum e_expand_type expand,
						bool *had_expand)
{
	t_content_node	*cnt;
	t_btnode		*tree;

	if (btree_is_leaf(node))
	{
		cnt = (t_content_node *)node->content;
		if (is_expansion_needed(cnt->op))
		{
			tree = create_basic_tree(&cnt->cmd.tokens, node->parent, expand);
			if (tree != NULL)
				update_node_properties(had_expand, tree, node, expand);
			btree_delete(&node, free_btree_node);
			return (tree);
		}
		else
			return (node);
	}
	if (node->left != NULL)
		node->left = expand_btree_node(node->left, expand, had_expand);
	if (node->right != NULL)
		node->right = expand_btree_node(node->right, expand, had_expand);
	return (node);
}

t_btnode	*expand_tree_by_type(t_btnode *tree, enum e_expand_type type,
							bool *expanded)
{
	bool	i_expanded;

	if (tree == NULL)
		return (NULL);
	i_expanded = true;
	while (i_expanded == true)
	{
		i_expanded = false;
		tree = expand_btree_node(tree, type, &i_expanded);
		if (tree == NULL)
			return (NULL);
		*expanded = *expanded || i_expanded;
	}
	return (tree);
}
