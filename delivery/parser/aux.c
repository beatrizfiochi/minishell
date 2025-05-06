/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:09:35 by djunho            #+#    #+#             */
/*   Updated: 2025/05/05 18:13:30 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "aux.h"

t_list	*prev_list_item(t_list *first_node, t_list *current_node)
{
	if (first_node == NULL || first_node == current_node)
		return (NULL);
	while (first_node->next != NULL && first_node->next != current_node)
		first_node = first_node->next;
	return (first_node);
}
