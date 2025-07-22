/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 15:00:45 by djunho            #+#    #+#             */
/*   Updated: 2025/07/22 19:12:03 by djunho           ###   ########.fr       */
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
