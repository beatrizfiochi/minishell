/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:52:57 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/27 14:59:07 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdbool.h>					//bool

bool	search_op(char *line)
{
	char	quote;

	quote = 0;
	if (line == NULL)
		return (false);
	while (*line != '\0')
	{
		if (*line == '\'' || *line == '"')
		{
			quote = *line;
			line++;
			while (*line != quote && *line != '\0')
				line++;
		}
		if (*line == '|')
		{
			if ((*(line + 1) != '|' && *(line + 1) != '&') && *(line + 1) != '\0')
				return (true);
			if ((*(line + 1) == '|' && (*(line + 2) != '|') && *(line + 2) != '\0'))
				return (true);
			return (false);
		}
		if (*line == '&')
		{
			if ((*(line + 1) == '&' && (*(line + 2) != '&') && *(line + 2) != '\0'))
				return (true);
			return (false);
		}
		line++;
	}
	return (false);
}
