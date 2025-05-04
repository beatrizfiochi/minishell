/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:22:37 by djunho            #+#    #+#             */
/*   Updated: 2025/05/05 18:44:21 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft/libft.h"
#include "btree/btree.h"
#include "cmd.h"

#ifdef TEST

static void	each_list_node(void *content)
{
	printf("%s ", (char *)content);
}

static int	printf_string_node(t_btnode *node, int ret)
{
	(void)ret;
	ft_lstiter(((t_content_node *)node->content)->cmd.tokens, each_list_node);
	printf("\nOperator node> %d\n", ((t_content_node *)node->content)->op);
	return (0);
}

static int	printf_string_leaf(t_btnode *node)
{
	ft_lstiter(((t_content_node *)node->content)->cmd.tokens, each_list_node);
	printf("\nOperator leaf> %d\n", ((t_content_node *)node->content)->op);
	return (0);
}

void	debug_print_tree(t_btnode *tree)
{
	btree_foreach_dfs(tree, printf_string_node, printf_string_leaf);
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
