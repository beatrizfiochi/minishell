/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:00:45 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 18:47:50 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../cmd.h"

bool	is_op_redirect_type(t_node_op op)
{
	if ((op == OP_PIPE) || (op == OP_RD_OUTPUT) || (op == OP_RD_INPUT)
		|| (op == OP_APPEND_RD_OUTPUT))
		return (true);
	return (false);
}

bool	is_pipe(t_btnode *node)
{
	t_btnode	*aux;

	aux = node;
	while (aux != NULL)
	{
		if (node_cnt(aux) == NULL)
			return (false);
		if (node_cnt(aux)->op == OP_PIPE)
			return (true);
		if (node_cnt(aux)->cmd.is_parentheses)
			return (false);
		if ((node_cnt(aux)->op == OP_AND) || (node_cnt(aux)->op == OP_OR))
			return (false);
		aux = aux->parent;
	}
	return (false);
}
