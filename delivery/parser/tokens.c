/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:05:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/20 23:37:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../libft/libft.h"
#include "../redirect/redirect_aux.h"	// is_a_redirect_file_op()
#include "tokens.h"
#include "aux.h"

static int	fix_split_simple(struct s_split_token_list *split)
{
	if (split->remain == NULL)
	{
		split->left = split->right->next;
		split->right->next = NULL;
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static int	fix_split_complex(struct s_split_token_list *split,
								enum e_expand_type *expand)
{
	t_list	*cmd;
	t_list	*after_cmd;
	t_list	*before_cmd;
	t_list	*last_remain;

	cmd = split->remain;
	while ((cmd != NULL) && (op_list(cmd) == OP_RD_INPUT))
		cmd = cmd->next->next;
	if (cmd == NULL)
		return (EXIT_FAILURE);
	split->left = cmd;
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
	return (EXIT_SUCCESS);
}

// This function tries to detect and fix the inverted order of OP_RD_INPUT and
// OP_HEREDOC operators in the split token list.
// It "converts" from:
// < file cmd
// to:
// cmd < file
static bool	fix_split(struct s_split_token_list *split,
								enum e_expand_type *expand)
{
	if ((op_list(split->op) == OP_RD_INPUT) && (split->left == NULL))
	{
		if (fix_split_simple(split) == EXIT_SUCCESS)
			return (true);
		return (fix_split_complex(split, expand) == EXIT_SUCCESS);
	}
	return (true);
}

// Warning: This function assumes that the split->left is not NULL
bool	split_token_list(struct s_split_token_list *split,
							enum e_expand_type expand_type)
{
	t_list	*aux;
	t_list	*aux2;

	while (1)
	{
		aux = search_op(split->left, expand_type);
		if ((aux == NULL) || (aux->next == NULL))
			break ;
		split->op = aux;
		aux2 = prev_list_item(split->left, split->op);
		if (aux2 != NULL)
			aux2->next = NULL;
		else
			split->left = NULL;
		split->right = split->op->next;
		split->op->next = NULL;
		aux = search_op(split->right, expand_type);
		if (aux == split->right)
			break ;
		split->remain = aux;
		if (split->remain != NULL)
			prev_list_item(split->right, split->remain)->next = NULL;
		return (true);
	}
	return (abort_tree_lst(NULL, split) != NULL);
}

int	split_tokens(t_list **tokens, struct s_split_token_list *split,
					enum e_expand_type *expand)
{
	if (split_token_list(split, *expand) == false)
	{
		*tokens = NULL;
		return (EXIT_FAILURE);
	}
	if ((!fix_split(split, expand))
		|| ((split->left == NULL) && !is_redirect_file_op(op_list(split->op))))
	{
		abort_tree_lst(NULL, split);
		*tokens = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
