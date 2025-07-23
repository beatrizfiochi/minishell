/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 08:05:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/23 09:28:01 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../libft/libft.h"
#include "../redirect/redirect_aux.h"	// is_a_redirect_file_op()
#include "tokens.h"
#include "aux.h"
#include "parser.h"

// Fix the situation when remain is NULL
// < file cmd
static int	fix_split_simple(struct s_split_token_list *split)
{
	if (ft_lstsize(split->right) > 1)
	{
		if (split->left != NULL)
			ft_lstlast(split->left)->next = split->right->next;
		else
			split->left = split->right->next;
		split->right->next = NULL;
	}
	return (EXIT_SUCCESS);
}

// When teh split is done in a redirect operatior, the operator can be always
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
		fix_split_simple(split);
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

// Warning: This function assumes that the split->left is not NULL
// In this function there are 2 checks for the right operator. with and without
// the is_redirect_file_op. It is possible to have a redir together with
// another.
// For example: <infile grep i >grep | <grep wc
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
		if ((aux == split->right) && !is_redirect_file_op(op_list(aux)))
			break ;
		if (aux == split->right)
			aux = search_op(split->right->next, expand_type);
		split->remain = aux;
		if (split->remain != NULL)
			prev_list_item(split->right, split->remain)->next = NULL;
		return (true);
	}
	return (abort_tree_lst(NULL, split) != NULL);
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
