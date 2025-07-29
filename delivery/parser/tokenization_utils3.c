/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 23:17:46 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/29 23:18:58 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "aux.h"

int	check_token(t_list *prev, t_list *curr)
{
	if ((op_list(curr) == OP_PAREN_OPEN) && ((op_list(prev) == OP_RD_INPUT)
		|| (op_list(prev) == OP_RD_OUTPUT)
		|| (op_list(prev) == OP_HEREDOC)
		|| (op_list(prev) == OP_APPEND_RD_OUTPUT)
		|| (op_list(prev) == OP_CMD)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
