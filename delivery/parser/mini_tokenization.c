/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tokenization.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:35:10 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/13 22:00:00 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

t_list	*mini_tokenization(t_list **token_list)
{
	t_list	*current;
	char	*content;

	current = *token_list;
	while (current != NULL)
	{
		content = (char *)current->content;
		while (*content != '\0')
		{
			while (*content == ' ')
				content++;
			if (*content == '\0')
				break ;
			
		}
	}
}
