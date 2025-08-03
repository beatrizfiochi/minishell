/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 18:48:52 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/25 00:07:29 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../libft/libft.h"
#include "tokens.h"

// 0 -> error | 1 -> success
static int	is_a_possible_op(char c)
{
	if (c == '|' || c == '&' || c == '<' || c == '>' || c == '('
		|| c == ')')
		return (1);
	else
		return (0);
}

// 0 -> invalid, 1 ou 2 -> size of valid op
// '&' valid as '&&'
// '|' valid as '|' or '||'
// '<' and '>' valid as single or double
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
	if ((c == '(') || (c == ')'))
		return (1);
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
	if (ft_strncmp(token, "&&", 3) == 0
		|| ft_strncmp(token, "||", 3) == 0
		|| ft_strncmp(token, "<<", 3) == 0
		|| ft_strncmp(token, ">>", 3) == 0
		|| ft_strncmp(token, "|", 2) == 0
		|| ft_strncmp(token, "&", 2) == 0
		|| ft_strncmp(token, "<", 2) == 0
		|| ft_strncmp(token, ">", 2) == 0)
		return (1);
	return (0);
}

void	remove_empty_token(t_list **token_list)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;

	current = *token_list;
	prev = NULL;
	while (current != NULL)
	{
		next = current->next;
		if (remove_if_empty(token_list, current, prev) == false)
			prev = current;
		current = next;
	}
}
