/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator_checks.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 18:57:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/30 15:15:51 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>              // printf
#include <unistd.h>
#include "../minishell.h"       // op
#include "../parser/aux.h"      // search_any_op
#include "../cmd.h"             // t_content_node
#include "../btree/btree.h"     // t_btnode
#include "../redirect/redirect_aux.h"  // is_a_redirect_file_op

bool	is_btnode_different(t_btnode *node1, t_btnode *node2)
{
	const t_content_node	*cnt_node1;
	const t_content_node	*cnt_node2;

	if (node1 == NULL || node2 == NULL)
		return (true);
	if (node1->content == NULL || node2->content == NULL)
		return (true);
	cnt_node1 = (t_content_node *)node1->content;
	cnt_node2 = (t_content_node *)node2->content;
	if (cnt_node1->op != cnt_node2->op)
		return (true);
	if (((node1->left != NULL) != (node2->left != NULL))
		|| ((node1->right != NULL) != (node2->right != NULL)))
		return (true);
	return (false);
}

static t_btnode	*check_tree_syntax_leaf(t_btnode *node)
{
	t_content_node	*content;
	const t_list	*operator;

	content = (t_content_node *)node->content;
	if (content->op != OP_CMD)
	{
		btree_delete(&node, free_btree_node);
		return (NULL);
	}
	operator = search_any_op(content->cmd.tokens);
	if (operator != NULL)
	{
		printf("syntax error near unexpected token \"%s\"\n",
			(char *)operator->content);
		btree_delete(&node, free_btree_node);
		return (NULL);
	}
	return (node);
}

// This function checks the syntax of the tree
// All nodes should have left and right (with the exception of redirect nodes
//  that may not have the left one)
t_btnode	*check_tree_syntax(t_btnode *node)
{
	if (btree_is_leaf(node))
	{
		return (check_tree_syntax_leaf(node));
	}
	if (node->left != NULL)
		node->left = check_tree_syntax(node->left);
	if (node->right != NULL)
		node->right = check_tree_syntax(node->right);
	if (((node->left == NULL) && !is_redirect_file_op(
				op_list(((t_content_node *)node->content)->cmd.tokens)))
		|| (node->right == NULL))
	{
		btree_clear(&node, free_btree_node);
		return (NULL);
	}
	return (node);
}

bool	check_enclosure(t_list **token_list, enum e_expand_type *expand_type)
{
	t_list	*last;
	t_list	*first;
	t_list	*prev;

	last = ft_lstlast(*token_list);
	if (op((char *)last->content) != OP_PAREN_CLOSE)
	{
		printf("Error: Missing closing parenthesis!\n");
		return (false);
	}
	first = *token_list;
	prev = prev_list_item(*token_list, last);
	prev->next = NULL;
	*token_list = (*token_list)->next;
	ft_lstdelone(last, free);
	ft_lstdelone(first, free);
	*expand_type = EXP_NORMAL;
	return (true);
}
