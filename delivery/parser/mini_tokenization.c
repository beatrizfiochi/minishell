/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_tokenization.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:35:10 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/14 12:02:20 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"

static void	free_str_array(char **arr)
{
	int i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

t_list	*mini_tokenization(t_list **token_list)
{
	t_list	*current;
	t_list	*mini_token_list;
	t_list	*new_node;
	char	**split_tokens;
	int		i;

	current = *token_list;
	mini_token_list = NULL;
	while (current != NULL)
	{
		split_tokens = ft_split((char *)current->content, ' ');
		if (split_tokens == NULL)
			return (NULL);
		i = -1;
		while (split_tokens[++i] != NULL)
		{
			new_node = ft_lstnew(ft_strdup(split_tokens[i]));
			if (new_node == NULL)
				return (NULL);
			ft_lstadd_back(&mini_token_list, new_node);
		}
		free_str_array(split_tokens);
		current = current ->next;
	}
	return (mini_token_list);
}
