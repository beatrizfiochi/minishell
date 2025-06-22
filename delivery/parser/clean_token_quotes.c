/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:33:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/22 19:30:31 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include <stdbool.h>

bool	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

bool	clean_single_token_quotes(char **content_ptr)
{
	const char	*src;
	char		*new_str;
	int			len;
	int			i;
	int			j;
	char		quote;
	bool		in_quote;

	src = *content_ptr;
	len = ft_strlen(src);
	i = 0;
	j = 0;
	in_quote = false;
	new_str = malloc(len + 1);
	if (new_str == NULL)
		return (false);
	while (src[i] != '\0')
	{
		if ((in_quote == false) && (is_quote(src[i]) == true))
		{
			quote = src[i++];
			in_quote = true;
		}
		else if ((in_quote == true) && (src[i] == quote))
		{
			i++;
			in_quote = false;
		}
		else
			new_str[j++] = src[i++];
	}
	if (in_quote == true)
	{
		free(new_str);
		return (false);
	}
	new_str[j] = '\0';
	free(*content_ptr);
	*content_ptr = new_str;
	return (true);
}

bool	clean_token_quotes(t_list *token_list)
{
	while (token_list != NULL)
	{
		if (clean_single_token_quotes((char **)&token_list->content) == false)
			return (false);
		token_list = token_list->next;
	}
	return (true);
}

