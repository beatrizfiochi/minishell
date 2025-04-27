/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:52:57 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/27 15:26:46 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdbool.h>					//bool


static char	*go_next_char(char *line, char c)
{
	while (*line != c && *line != '\0')
		line++;
	return (line);
}

bool	search_op(char *line)
{
	if (line == NULL)
		return (false);
	while (*line != '\0')
	{
		if (*line == '\'' || *line == '"')
			line = go_next_char(line + 1, *line);
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
