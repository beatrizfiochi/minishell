/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:22:37 by djunho            #+#    #+#             */
/*   Updated: 2025/05/21 11:00:29 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>
#include "libft/libft.h"
#include "cmd.h"

#ifdef TEST

const char	*op_str(t_node_op op)
{
	const char	*op_strs[] = {
		"OP_INVALID",
		"OP_AND",
		"OP_OR",
		"OP_PIPE",
		"OP_VAR_ASSIGN",
		"OP_RD_INPUT",
		"OP_RD_OUTPUT",
		"OP_HEREDOC",
		"OP_APPEND_RD_OUTPUT",
		"OP_CMD",
		"OP_SIZE_DO_NOT_USE",
	};

	static_assert((sizeof(op_strs) / sizeof(op_strs[0]))
		== (OP_SIZE_DO_NOT_USE + 1),
		"op_strs must be equal to the t_node_op enum");
	if (op > OP_CMD)
		return ("OP_INVALID");
	return (op_strs[op]);
}

void	debug_print_read_command(t_list *node, char *line)
{
	(void)node;
	(void)line;
	printf("Command received: %s\n", line);
	while (node != NULL)
	{
		printf("Nodes: %s\n", (char *)node->content);
		node = node->next;
	}
}
#endif
