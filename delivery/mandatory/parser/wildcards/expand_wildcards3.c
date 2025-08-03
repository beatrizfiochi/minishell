/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcards3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:31:02 by djunho            #+#    #+#             */
/*   Updated: 2025/07/26 12:37:22 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../libft/libft.h"

t_list	*replace_lst_item_by_lst(t_list **token_lst, t_list *new_lst,
						t_list *prev, t_list *token)
{
	t_list	*aux;

	if (prev == NULL)
		*token_lst = new_lst;
	else
		prev->next = new_lst;
	aux = token->next;
	ft_lstdelone(token, free);
	token = ft_lstlast(new_lst);
	token->next = aux;
	return (token);
}
