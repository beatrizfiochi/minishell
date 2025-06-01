/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:48:52 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/01 19:07:54 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../libft/libft.h"

int	is_valid_name(char *name, int len)
{
	int	i;

	i = 0;
	if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')
		|| (name[i] == '_'))
	{
		i++;
		while (i < len)
		{
			if ((ft_isalnum(name[i]) == true) || name[i] == '_')
				i++;
			else
				return (-1);
		}
		return (0);
	}
	else
		return (-1);
}

int	is_op(char *line)
{
	if (*line == '\0')
		return (0);
	if (*line == '&' && *(line + 1) == '&')
		return (2);
	if (*line == '|' && *(line + 1) == '|')
		return (2);
	if (*line == '|')
		return (1);
	if (*line == '=')
		return (1);
	if (*line == '<' && *(line + 1) == '<')
		return (2);
	if (*line == '<')
		return (1);
	if (*line == '>' && *(line + 1) == '>')
		return (2);
	if (*line == '>')
		return (1);
	return (0);
}
