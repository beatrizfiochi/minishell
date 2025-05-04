/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_content_node.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:34:50 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/30 19:08:57 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../btree/btree.h"
#include "../minishell.h"
#include "../cmd.h"

// OP_AND -> 0
// OP_OR -> 1
// OP_PIPE -> 2
// OP_CMD -> 3
static t_node_op	op(char *line)
{
	if (*line == '\0')
		return (NULL);
	if (*line == '&' && *(line + 1) == '&')
		return (0);
	if (*line == '|' && *(line + 1) == '|')
		return (1);
	if (*line == '|')
		return (2);
	return (3);
}

t_content_node	*fill_content_node(	t_list	*token_list, char *line)
{
	t_content_node	node;
	t_node_op		op_type;

	if (search_op(line) == true)
	{
		op_type = op(line);
		if (op_type <= 2)
			node.op = op_type;
	}
}
