/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:33:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/14 12:00:27 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"

void	clean_token_quotes(t_list *token_list)
{
	char	*content;
	char	*new_content;
	int		len;

	while (token_list != NULL)
	{
		content = (char *)(token_list->content);
		len = ft_strlen(content);
		if (len >= 2 && ((content[0] == '\'' && content[len - 1] == '\'') ||
			 (content[0] == '"'  && content[len - 1] == '"')))
		{
			new_content = malloc(len - 1);
			if (new_content == NULL)
				return ;
			ft_strlcpy(new_content, content + 1, len - 1);
			new_content[len - 2] = '\0';
			free(token_list->content);
			token_list->content = new_content;
		}
		token_list = token_list->next;
	}
}

