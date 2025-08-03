/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 16:09:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 20:28:57 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../redirect/redirect_aux.h"	// is_a_redirect_file_op()
#include "tokens.h"
#include "aux.h"

static t_list	*move_item(struct s_split_token_list *split, t_list *prev,
						t_list *item)
{
	t_list	*aux;
	t_list	*last;

	last = ft_lstlast(split->left);
	if (last != NULL)
		last->next = item;
	else
		split->left = item;
	prev->next = item->next;
	aux = item->next;
	item->next = NULL;
	return (aux);
}

// Fix the situation when the redirection is in between the commadn
// cmd < file cmd -e -f -r > file2 -r -t
static int	fix_split_complex(struct s_split_token_list *split)
{
	t_list	*aux;
	t_list	*prev;

	aux = split->remain;
	while (is_redirect_file_op(op_list(aux)))
	{
		if (op_list(aux->next) != OP_CMD)
			return (EXIT_FAILURE);
		prev = aux->next;
		aux = aux->next->next;
		while ((aux != NULL) && (op_list(aux) == OP_CMD))
			aux = move_item(split, prev, aux);
	}
	return (EXIT_SUCCESS);
}

// Fix the situation when the command is after the redirect
// < file cmd
int	fix_split_redirect(struct s_split_token_list *split)
{
	if (ft_lstsize(split->right) > 1)
	{
		if (split->left != NULL)
			ft_lstlast(split->left)->next = split->right->next;
		else
			split->left = split->right->next;
		split->right->next = NULL;
	}
	return (fix_split_complex(split));
}
