/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:22:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 15:03:22 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>
#include "../libft/libft.h"
#include "cmd_bonus.h"
#include "variables/variables_bonus.h"

#ifdef TEST

const char	*op_str(t_node_op op)
{
	const char	*op_strs[] = {
		"OP_INVALID",
		"OP_AND",
		"OP_OR",
		"OP_PIPE",
		"OP_RD_INPUT",
		"OP_RD_OUTPUT",
		"OP_HEREDOC",
		"OP_APPEND_RD_OUTPUT",
		"OP_CMD",
		"OP_PAREN_OPEN",
		"OP_PAREN_CLOSE",
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

void	debug_env_var_init(t_shell *shell)
{
	shell->variable_list = create_var_node("oi", "hi", false);
	shell->variable_list->next = create_var_node("tchau", "bye", false);
	shell->variable_list->next->next = create_var_node("oooi", "hhhi", false);
	shell->variable_list->next->next->next = create_var_node("oiii", "hiii",
			false);
	shell->variable_list->next->next->next->next = NULL;
}

#else

void	debug_env_var_init(t_shell *shell)
{
	(void)shell;
}

#endif
