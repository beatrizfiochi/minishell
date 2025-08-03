/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/31 11:12:36 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "../btree/btree_bonus.h"
#include "../minishell_bonus.h"
#include "../cmd_bonus.h"
#include "../heredoc/heredoc_bonus.h"
#include "aux_bonus.h"
#include "parser_bonus.h"
#include "tokens_bonus.h"
#include "tree_creator_bonus.h"

static t_btnode	*create_node(t_list *token_list, t_btnode *parent,
								t_btnode *left, t_btnode *right)
{
	t_content_node	*content;
	t_btnode		*tree_node;

	if (token_list == NULL)
		return (NULL);
	content = malloc(sizeof(t_content_node));
	if (content == NULL)
		return (NULL);
	content->op = op((char *)(token_list->content));
	content->cmd.tokens = token_list;
	content->cmd.is_builtin = false;
	content->cmd.finished = false;
	content->cmd.is_parentheses = false;
	content->cmd.redir.fd_in = -1;
	content->cmd.redir.fd_out = -1;
	content->cmd.pipe_fd = -1;
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

static t_btnode	*create_first(t_list **tokens, t_btnode *parent,
								enum e_expand_type *expand)
{
	t_list						*op_node;
	t_btnode					*tree;
	struct s_split_token_list	split;

	if ((*expand == EXP_PAREN) && (op_list(*tokens) == OP_PAREN_OPEN))
		if (check_enclosure(tokens, expand) == false)
			*expand = EXP_NORMAL;
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
	if (split_tokens(tokens, &split, false, expand) == EXIT_FAILURE)
		return (NULL);
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
								enum e_expand_type expand)
{
	t_btnode					*tree;
	t_btnode					*old_tree;
	struct s_split_token_list	split;

	tree = create_first(token_list, parent, &expand);
	if (tree == NULL)
		return (NULL);
	split.remain = *token_list;
	while ((split.remain != NULL) && !((expand == EXP_PAREN)
			&& (split.remain->next == NULL)
			&& (op_list(*token_list) == OP_PAREN_CLOSE)))
	{
		old_tree = tree;
		split = (struct s_split_token_list){
			.left = split.remain, .op = NULL, .right = NULL, .remain = NULL
		};
		if (split_tokens(token_list, &split, true, &expand) == EXIT_FAILURE)
			return (NULL);
		if (split.left != NULL)
			return (abort_tree_lst(tree, &split));
		tree = create_node(split.op, parent, old_tree, NULL);
		tree->right = create_node(split.right, tree, NULL, NULL);
	}
	*token_list = split.remain;
	return (tree);
}

int	create_tree(t_shell *shell, t_btnode **tree, t_list **token_list,
					t_btnode *parent)
{
	bool	expanded;
	int		ret;

	if (token_list == NULL || *token_list == NULL)
		return (EXIT_INCORRECT_USAGE);
	*tree = create_basic_tree(token_list, parent, false);
	if (*tree == NULL)
		return (EXIT_INCORRECT_USAGE);
	expanded = true;
	while (expanded == true)
	{
		expanded = false;
		*tree = expand_tree_by_type(*tree, EXP_PIPE, &expanded);
		*tree = expand_tree_by_type(*tree, EXP_REDIR, &expanded);
		*tree = expand_tree_by_type(*tree, EXP_PAREN, &expanded);
		if (*tree == NULL)
			return (EXIT_INCORRECT_USAGE);
	}
	*tree = check_tree_syntax(*tree);
	if (*tree == NULL)
		return (EXIT_INCORRECT_USAGE);
	ret = check_tree_for_heredoc(shell, *tree);
	if (ret != EXIT_SUCCESS)
		btree_delete(tree, free_btree_node);
	return (ret);
}
