/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:33:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/25 00:09:29 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include <stdbool.h>
#include "aux.h"
#include "../variables/expand_variables.h"

t_list	*create_token(const char *line, int len)
{
	char	*content;

	content = ft_strndup(line, len);
	if (content == NULL)
		return (NULL);
	return (ft_lstnew(content));
}

bool	clean_string_quotes(char **content_ptr)
{
	char	*new_str;
	char	*aux;

	new_str = *content_ptr;
	while (*new_str != '\0')
	{
		if (*new_str == QUOTE_MARK)
		{
			aux = go_to_end_quote(new_str);
			ft_memmove(aux - 1, aux, ft_strlen(aux) + 1);
			ft_memmove(new_str, new_str + 1, ft_strlen(new_str + 1) + 1);
			new_str = aux - 2;
		}
		else
			new_str++;
	}
	return (true);
}

bool	clean_token_quotes(t_list *token_list)
{
	while (token_list != NULL)
	{
		if (clean_string_quotes((char **)&token_list->content) == false)
			return (false);
		token_list = token_list->next;
	}
	return (true);
}

bool	remove_if_empty(t_list **token_list, t_list *current, t_list *prev)
{
	char	*content;

	content = (char *)current->content;
	if (content != NULL && *content == '\0')
	{
		if (prev == NULL)
			*token_list = current->next;
		else
			prev->next = current->next;
		ft_lstdelone(current, free);
		return (true);
	}
	return (false);
}
