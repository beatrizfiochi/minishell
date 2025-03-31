/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstfind.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 14:00:52 by djunho            #+#    #+#             */
/*   Updated: 2025/03/02 14:46:06 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "libft.h"

t_list	*ft_lstfind(t_list *lst, void *content,
					bool (*f)(void *base, void *list_content))
{
	if ((lst == NULL) || (f == NULL))
		return (NULL);
	while (lst != NULL)
	{
		if (f(content, lst->content))
			break ;
		lst = lst->next;
	}
	return (lst);
}
