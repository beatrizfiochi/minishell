/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 21:23:49 by djunho            #+#    #+#             */
/*   Updated: 2025/06/13 16:43:23 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../libft/libft.h"
#include "btree.h"
#include "../cmd.h"
#include "../debug.h"

#ifdef TEST

static void	each_list_node(void *content)
{
	printf("%s ", (char *)content);
}

static int	printf_string_node(t_btnode *node, int ret,
							bool *should_continue, void *ctx)
{
	(void)ret;
	(void)ctx;
	(void)should_continue;
	printf("Btree: Operator node> %s\n",
		op_str(((t_content_node *)node->content)->op));
	return (0);
}

static int	printf_string_leaf(t_btnode *node, void *ctx)
{
	t_content_node	*content;

	(void)ctx;
	content = (t_content_node *)node->content;
	printf("Btree: Operator leaf> %s\n",
		op_str(content->op));
	if (content->op != OP_CMD)
	{
		printf("Btree: Leaf is not a command (Error)!\n");
		return (0);
	}
	printf("Btree: Leaf content: ");
	ft_lstiter(content->cmd.tokens, each_list_node);
	printf("\n");
	return (0);
}

void	debug_print_tree(t_btnode *tree)
{
	btree_foreach_dfs(tree, printf_string_node, printf_string_leaf, NULL);
}
#endif
