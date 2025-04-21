/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:00:26 by djunho            #+#    #+#             */
/*   Updated: 2025/04/21 18:19:04 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "btree.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef struct s_content
{
	char	*str;
	int		ret;
}	t_content;

static int	printf_string_node(t_btnode *node, int ret)
{
	(void)ret;
	printf("Node> %s\n", ((t_content *)node->content)->str);
	return (((t_content *)node->content)->ret);
}

static int	printf_string_leaf(t_btnode *node)
{
	printf("Leaf> %s\n", ((t_content *)node->content)->str);
	return (((t_content *)node->content)->ret);
}

bool	test1(void)
{
	bool	ret;

	printf(CYAN"Running test 1\n"RESET);
	t_content c_right = {
		.str = "\tright",
		ret = 0,
	};
	t_btnode right = btree_new(&c_right);
	t_content c_left = {
		.str = "\tleft",
		ret = 0,
	};
	t_btnode left = btree_new(&c_left);
	t_content c_parent = {
		.str = "\tparent",
		ret = 0,
	};
	t_btnode parent = btree_new(&c_parent);
	parent.left = &left;
	parent.right = &right;
	printf(MAGENTA);
	btree_foreach_dfs(&parent, printf_string_node, printf_string_leaf);
	printf(RESET);
	char *answer = readline("Do you see \"left, parent, right\" order? Y/n:");
	if ((strcmp(answer, "y") == 0) || (strcmp(answer, "Y") == 0) || (strcmp(answer, "") == 0))
		ret = true;
	else
		ret = false;
	free(answer);
	return (ret);
}
