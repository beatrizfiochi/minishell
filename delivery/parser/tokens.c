/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:05:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/30 15:45:09 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../libft/libft.h"
#include "../redirect/redirect_aux.h"	// is_a_redirect_file_op()
#include "tokens.h"
#include "aux.h"
#include "parser.h"

// When the split is done in a redirect operatior, the operator can be always
//  anywhere in the token list.
// For example:
//    < file cmd -arg
//    cmd < file -arg
//    cmd -arg < file
// The goal of this function is to fix this type of situations
// TODO: Describe the special case of the has_left below
static bool	fix_split(struct s_split_token_list *split, bool has_left,
								enum e_expand_type *expand)
{
	t_list	*aux;

	(void)expand;
	if (is_redirect_file_op(op_list(split->op)))
	{
		fix_split_redirect(split);
		if (is_redirect_file_op(op_list(split->op)))
		{
			if ((ft_lstsize(split->right) > 1)
				|| (has_left && (split->left != NULL)))
			{
				aux = split->op;
				split->op = create_token("&&", 2);
				aux->next = split->right;
				if (split->left != NULL)
				{
					ft_lstlast(split->left)->next = aux;
					aux = split->left;
					split->left = NULL;
				}
				split->right = aux;
			}
		}
	}
	return (true);
}

static bool	find_and_split_operator(struct s_split_token_list *split,
								enum e_expand_type expand_type)
{
	t_list	*aux;
	t_list	*prev;

	aux = search_op(split->left, expand_type);
	if ((aux == NULL) || (aux->next == NULL))
		return (false);
	split->op = aux;
	prev = prev_list_item(split->left, split->op);
	if (prev != NULL)
		prev->next = NULL;
	else
		split->left = NULL;
	split->right = split->op->next;
	split->op->next = NULL;
	return (true);
}

static void	find_remain_part(struct s_split_token_list *split,
						enum e_expand_type expand_type)
{
	t_list	*aux;
	t_list	*prev;

	aux = search_op(split->right, expand_type);
	if ((aux == split->right) && (!is_redirect_file_op(op_list(aux))))
	{
		split->remain = NULL;
		return ;
	}
	if (aux == split->right)
		aux = search_op(split->right->next, expand_type);
	split->remain = aux;
	if (split->remain != NULL)
	{
		prev = prev_list_item(split->right, split->remain);
		if (prev != NULL)
			prev->next = NULL;
	}
}

// Warning: This function assumes that the split->left is not NULL
// In this function there are 2 checks for the right operator. with and without
// the is_redirect_file_op. It is possible to have a redir together with
// another.
// For example: <infile grep i >grep | <grep wc
bool	split_token_list(struct s_split_token_list *split,
						enum e_expand_type expand_type)
{
	if (!find_and_split_operator(split, expand_type))
		return (abort_tree_lst(NULL, split) != NULL);
	find_remain_part(split, expand_type);
	return (true);
}

int	split_tokens(t_list **tokens, struct s_split_token_list *split,
					bool has_left, enum e_expand_type *expand)
{
	if (split_token_list(split, *expand) == false)
	{
		*tokens = NULL;
		return (EXIT_FAILURE);
	}
	if (!fix_split(split, has_left, expand))
	{
		abort_tree_lst(NULL, split);
		*tokens = NULL;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
