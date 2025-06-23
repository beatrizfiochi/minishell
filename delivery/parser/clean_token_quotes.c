/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_token_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 11:33:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/23 18:30:58 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "../libft/libft.h"
#include <stdbool.h>
#include "aux.h"

static int	copy_inside_quotes(const char *src, char *new_str, int i, int *j)
{
	int		k;
	char	quote;

	quote = src[i];
	k = i + 1;
	while ((src[k] != '\0') && (src[k] != quote))
		k++;
	if (src[k] == quote)
	{
		i++;
		while (i < k)
			new_str[(*j)++] = src[i++];
		return (k + 1);
	}
	else
	{
		new_str[(*j)++] = src[i++];
		return (i);
	}
}

static void	copy_without_matched_quotes(const char *src, char *new_str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (src[i] != '\0')
	{
		if (is_quote(src[i]) == true)
			i = copy_inside_quotes(src, new_str, i, &j);
		else
			new_str[j++] = src[i++];
	}
	new_str[j] = '\0';
}

static char	*remove_quotes(const char *src)
{
	char	*new_str;
	int		len;

	len = ft_strlen(src);
	new_str = malloc(len + 1);
	if (new_str == NULL)
		return (NULL);
	copy_without_matched_quotes(src, new_str);
	return (new_str);
}

static bool	clean_single_token_quotes(char **content_ptr)
{
	char	*new_str;

	new_str = remove_quotes((const char *)*content_ptr);
	if (new_str == NULL)
		return (false);
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
