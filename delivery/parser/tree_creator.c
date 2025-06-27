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
	if ((content->op == OP_CMD) || (content->op == OP_PARENTESIS_OPEN))
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
								bool *full_expand)
{
	t_list						*op_node;
	t_btnode					*tree;
	struct s_split_token_list	split;

	if ((*full_expand) && (op((char *)(*token_list)->content) == OP_PARENTESIS_OPEN))
	{
		t_list *first = *token_list;
		*token_list = (*token_list)->next;
		t_list *last = ft_lstlast(*token_list);
		t_list *prev = prev_list_item(*token_list, last);
		prev->next = NULL;
		if (op((char *)last->content) != OP_PARENTESIS_CLOSE)
			printf("Error: Missing closing parenthesis!\n");
		ft_lstdelone(last, free);
		ft_lstdelone(first, free);
		*full_expand = false;
	}
	op_node = search_op(*token_list, *full_expand);
	if ((op_node == NULL) || ((op_node->next == NULL) && (op((char*)op_node->content) == OP_PARENTESIS_CLOSE)))
	{
		tree = create_node(*token_list, parent);
		*token_list = op_node;
		return (tree);
	}
	split = (struct s_split_token_list){
		.left = *token_list, .op = NULL,
		.right = NULL, .remain = NULL
	};
	if (split_token_list(&split, true, *full_expand) == false)
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

	tree = create_first(token_list, parent, &full_expand);
	if (tree == NULL)
		return (NULL);
	split = (struct s_split_token_list){
		.left = NULL, .op = NULL,
		.right = NULL, .remain = *token_list
	};
	while ((split.remain != NULL) && !((full_expand) && (split.remain->next == NULL) && (op((char *)(*token_list)->content) == OP_PARENTESIS_CLOSE)))
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

static t_btnode	*expand_btree_node(t_btnode *node, bool full_expand, bool *expansion_happened)
{
	t_content_node	*content;
	t_btnode		*new_tree;

	if (btree_is_leaf(node))
	{
		content = (t_content_node *)node->content;
		if ((content->op == OP_CMD) || (content->op == OP_PARENTESIS_OPEN))
		{
			new_tree = create_basic_tree(&content->cmd.tokens,
					node->parent, full_expand);
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
		node->left = expand_btree_node(node->left, full_expand, expansion_happened);
	if (node->right != NULL)
		node->right = expand_btree_node(node->right, full_expand, expansion_happened);
	return (node);
}

t_btnode	*check_tree_syntax(t_btnode *node)
{
	t_content_node	*content;
	t_list			*op;

	if (btree_is_leaf(node))
	{
		content = (t_content_node *)node->content;
		if (content->op != OP_CMD)
		{
			btree_delete(&node, free_btree_node);
			return (NULL);
		}
		op = search_any_op(content->cmd.tokens);
		if (op != NULL)
		{
			printf("Error: syntax error near unexpected token \"%s\"\n", (char *)op->content);
			btree_delete(&node, free_btree_node);
			return (NULL);
		}
		return (node);
	}
	if (node->left != NULL)
		node->left = check_tree_syntax(node->left);
	if (node->right != NULL)
		node->right = check_tree_syntax(node->right);
	if (node->left == NULL || node->right == NULL)
	{
		btree_delete(&node, free_btree_node);
		return (NULL);
	}
	return (node);
}

t_btnode	*create_tree(t_list **token_list, t_btnode *parent)
{
	t_btnode	*tree;
	bool		expansion_happened;

	if (token_list == NULL || *token_list == NULL)
		return (NULL);
	tree = create_basic_tree(token_list, parent, false);
	if (tree == NULL)
		return (NULL);
	expansion_happened = true;
	while (expansion_happened == true)
	{
		expansion_happened = false;
		tree = expand_btree_node(tree, true, &expansion_happened);
		if (tree == NULL)
		{
			printf("Error: Failed to expand parentesis btree node\n");
			return (NULL);
		}
	}
	tree = expand_btree_node(tree, true, NULL);
	if (tree == NULL)
	{
		printf("Error: Failed to expand pipe btree node\n");
		return (NULL);
	}
	return (check_tree_syntax(tree));
}
