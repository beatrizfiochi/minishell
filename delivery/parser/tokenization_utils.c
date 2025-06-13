/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:48:52 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/13 15:35:36 by bfiochi-         ###   ########.fr       */
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

// 0 -> error | 1 -> success
static int	is_a_possible_op(char c)
{
	if (c == '|' || c == '&' || c == '=' || c == '<' || c == '>')
		return (1);
	else
		return (0);
}

// 0 -> invalid, 1 ou 2 -> size of valid op
// '&' valid as '&&'
// '|' valid as '|' or '||'
// '<' and '>' valid as single or double
// '=' valid as single
static int	check_sequence(char *line, char c)
{
	int	i;

	i = 0;
	while (line[i] == c)
		i++;
	if (c == '&' && i != 2)
		return (0);
	if (c == '|' && (i != 1 && i != 2))
		return (0);
	if ((c == '<' || c == '>') && (i != 1 && i != 2))
		return (0);
	if (c == '=' && i != 1)
		return (0);
	return (i);
}

// 0 -> invalid, 1 ou 2 -> size of valid op
int	is_op(char *line)
{
	int	len;

	if (*line == '\0')
		return (0);
	if (is_a_possible_op(*line) == 1)
	{
		len = check_sequence(line, *line);
		return (len);
	}
	return (0);
}

int	is_token_operator(char *token)
{
	if (token == NULL)
		return (0);
	if (ft_strncmp(token, "&&", ft_strlen(token)) == 0
		|| ft_strncmp(token, "||", ft_strlen(token)) == 0
		|| ft_strncmp(token, "|", ft_strlen(token)) == 0
		|| ft_strncmp(token, "&", ft_strlen(token)) == 0
		|| ft_strncmp(token, "<", ft_strlen(token)) == 0
		|| ft_strncmp(token, "<<", ft_strlen(token)) == 0
		|| ft_strncmp(token, ">", ft_strlen(token)) == 0
		|| ft_strncmp(token, ">>", ft_strlen(token)) == 0
		|| ft_strncmp(token, "=", ft_strlen(token)) == 0)
		return (1);
	return (0);
}
