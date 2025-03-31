/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstremove.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:57:18 by djunho            #+#    #+#             */
/*   Updated: 2025/03/02 18:35:01 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstremove(t_list **lst, t_list *node)
{
	t_list	*prev;
	t_list	*cur;

	if ((lst == NULL) || (node == NULL))
		return ;
	prev = *lst;
	if (prev == node)
	{
		*lst = prev->next;
		return ;
	}
	cur = prev->next;
	while ((cur != NULL) && (cur != node))
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur == node)
	{
		prev->next = cur->next;
	}
}
