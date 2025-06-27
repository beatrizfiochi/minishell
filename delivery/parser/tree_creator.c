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
	if ((content->op == OP_CMD) || (content->op == OP_PARENTHESIS_OPEN))
	{
		content->cmd.tokens = token_list;
		content->cmd.is_builtin = false;
		content->cmd.finished = false;
	}
	else
		ft_lstclear(&token_list, free);
	tree_node = btree_new(content);
	tree_node->parent = parent;
	return (tree_node);
}

static t_btnode	*create_first(t_list **token_list, t_btnode *parent,
								enum e_expand_type *expand_type)
{
	t_list						*op_node;
	t_btnode					*tree;
	struct s_split_token_list	split;

	if ((*expand_type == EXPAND_PARENTHESIS) && (op((char *)(*token_list)->content) == OP_PARENTHESIS_OPEN))
	{
		t_list *last = ft_lstlast(*token_list);
		if (op((char *)last->content) != OP_PARENTHESIS_CLOSE)
		{
			printf("Error: Missing closing parenthesis!\n");
			return (NULL);
		}
		t_list *first = *token_list;
		t_list *prev = prev_list_item(*token_list, last);
		prev->next = NULL;
		*token_list = (*token_list)->next;
		ft_lstdelone(last, free);
		ft_lstdelone(first, free);
		*expand_type = EXPAND_NORMAL;
	}
	op_node = search_op(*token_list, *expand_type);
	if ((op_node == NULL) || ((op_node->next == NULL) && (op((char*)op_node->content) == OP_PARENTHESIS_CLOSE)))
	{
		tree = create_node(*token_list, parent);
		*token_list = op_node;
		return (tree);
	}
	split = (struct s_split_token_list){
		.left = *token_list, .op = NULL,
		.right = NULL, .remain = NULL
	};
	if (split_token_list(&split, true, *expand_type) == false)
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
								enum e_expand_type expand_type)
{
	t_btnode					*tree;
	t_btnode					*old_tree;
	struct s_split_token_list	split;

	tree = create_first(token_list, parent, &expand_type);
	if (tree == NULL)
		return (NULL);
	split = (struct s_split_token_list){
		.left = NULL, .op = NULL,
		.right = NULL, .remain = *token_list
	};
	while ((split.remain != NULL) && !((expand_type == EXPAND_PARENTHESIS)
		&& (split.remain->next == NULL) && (op((char *)(*token_list)->content) == OP_PARENTHESIS_CLOSE)))
	{
		old_tree = tree;
		split = (struct s_split_token_list){
			.left = split.remain, .op = NULL, .right = NULL, .remain = NULL
		};
		if (split_token_list(&split, false, expand_type) == false)
			return (abort_tree_lst(tree, &split.left, NULL));
		tree = create_node(split.op, parent);
		tree->left = old_tree;
		tree->left->parent = tree;
		tree->right = create_node(split.right, tree);
	}
	*token_list = split.remain;
	return (tree);
}

bool	is_btnode_different(t_btnode *node1, t_btnode *node2)
{
	if (node1 == NULL || node2 == NULL)
		return (true);
	if (node1->content == NULL || node2->content == NULL)
		return (true);
	if (((t_content_node *)node1->content)->op != ((t_content_node *)node2->content)->op)
		return (true);
	return (false);
}

static t_btnode	*expand_btree_node(t_btnode *node, enum e_expand_type expand_type, bool *expansion_happened)
{
	t_content_node	*content;
	t_btnode		*new_tree;

	if (btree_is_leaf(node))
	{
		content = (t_content_node *)node->content;
		if ((content->op == OP_CMD) || (content->op == OP_PARENTHESIS_OPEN))
		{
			new_tree = create_basic_tree(&content->cmd.tokens,
					node->parent, expand_type);
			if (is_btnode_different(node, new_tree))
				*expansion_happened = true;
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
		node->left = expand_btree_node(node->left, expand_type, expansion_happened);
	if (node->right != NULL)
		node->right = expand_btree_node(node->right, expand_type, expansion_happened);
	return (node);
}

t_btnode	*check_tree_syntax(t_btnode *node)
{
	t_content_node	*content;
	t_list			*operator;

	if (btree_is_leaf(node))
	{
		content = (t_content_node *)node->content;
		if (content->op != OP_CMD)
		{
			btree_delete(&node, free_btree_node);
			return (NULL);
		}
		operator = search_any_op(content->cmd.tokens);
		if (operator != NULL)
		{
			printf("Error: syntax error near unexpected token \"%s\"\n", (char *)operator->content);
			btree_delete(&node, free_btree_node);
			return (NULL);
		}
		return (node);
	}
	if (node->left != NULL)
		node->left = check_tree_syntax(node->left);
	if (node->right != NULL)
		node->right = check_tree_syntax(node->right);
	if ((node->left == NULL) || (node->right == NULL))
	{
		btree_clear(&node, free_btree_node);
		return (NULL);
	}
	return (node);
}

t_btnode	*create_tree(t_list **token_list, t_btnode *parent)
{
	t_btnode	*tree;
	bool		expansion_happened;
	bool		expansion_happened_by_type;

	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	tree = create_basic_tree(token_list, parent, false);
	if (tree == NULL)
		return (NULL);
	expansion_happened = true;
	while (expansion_happened == true)
	{
		expansion_happened = false;
		expansion_happened_by_type = true;
		while (expansion_happened_by_type == true)
		{
			expansion_happened_by_type = false;
			tree = expand_btree_node(tree, EXPAND_PIPE, &expansion_happened_by_type);
			if (tree == NULL)
			{
				printf("Error: Failed to expand pipe btree node\n");
				return (NULL);
			}
			expansion_happened = expansion_happened || expansion_happened_by_type;
		}
		expansion_happened_by_type = true;
		while (expansion_happened_by_type == true)
		{
			expansion_happened_by_type = false;
			tree = expand_btree_node(tree, EXPAND_PARENTHESIS, &expansion_happened_by_type);
			if (tree == NULL)
			{
				printf("Error: Failed to expand parenthesis btree node\n");
				return (NULL);
			}
			expansion_happened = expansion_happened || expansion_happened_by_type;
		}
	}
	return (check_tree_syntax(tree));
}
