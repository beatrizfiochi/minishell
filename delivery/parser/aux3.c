/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 16:12:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/21 08:24:13 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"
#include "aux.h"

bool	is_basic_op_list(t_list *list)
{
	t_node_op	oper;

	oper = op((char *)(list->content));
	return ((oper == OP_AND) || (oper == OP_OR) || (oper == OP_PAREN_OPEN)
		|| (oper == OP_RD_OUTPUT) || (oper == OP_RD_INPUT)
		|| (oper == OP_APPEND_RD_OUTPUT) || (oper == OP_PIPE)
		|| (oper == OP_HEREDOC));
}
