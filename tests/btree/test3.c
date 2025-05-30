/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:00:26 by djunho            #+#    #+#             */
/*   Updated: 2025/05/13 21:48:44 by djunho           ###   ########.fr       */
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

static int	printf_string_node(t_btnode *node, int ret, void *ctx)
{
	(void)ret;
	(void)ctx;
	// printf("Node> %s\n", ((t_content *)node->content)->str);
	return (((t_content *)node->content)->ret);
}

static int	printf_string_leaf(t_btnode *node, void *ctx)
{
	(void)ctx;
	printf("Leaf> %s\n", ((t_content *)node->content)->str);
	return (((t_content *)node->content)->ret);
}

bool	test3(void)
{
	bool	ret;

	/*
	            ( )
	          /    \
	      ( )      (CMD)
	     /   \
	 (CMD)  (CMD)
	*/

	printf(CYAN"Running test 3\n"RESET);
	t_content c_right = {
		.str = "\tVALID CMD 3",
		ret = 0,
	};
	t_btnode *right = btree_new(&c_right);
	t_content c_left = {
		.str = "\tNOT A CMD IS A NODE",
		ret = 0,
	};
	t_btnode *left = btree_new(&c_left);
	t_content c_parent = {
		.str = "\tNOT A CMD IS A NODE",
		ret = 0,
	};
	t_btnode *parent = btree_new(&c_parent);
	parent->left = left;
	parent->right = right;

	t_content c_left_left = {
		.str = "\tVALID CMD 1",
		ret = 0,
	};
	t_btnode *left_left = btree_new(&c_left_left);
	t_content c_left_right = {
		.str = "\tVALID CMD 2",
		ret = 0,
	};
	t_btnode *left_right = btree_new(&c_left_right);
	left->left = left_left;
	left->right = left_right;
	printf(MAGENTA);
	btree_foreach_dfs(parent, printf_string_node, printf_string_leaf, NULL);
	printf(RESET);
	char *answer = readline("Do you see only 3 \"VALID CMD X\" messages in order (1, 2, 3)? Y/n:");
	if ((strcmp(answer, "y") == 0) || (strcmp(answer, "Y") == 0) || (strcmp(answer, "") == 0))
		ret = true;
	else
		ret = false;
	free(answer);

	if (3 != btree_count_leaves(parent))
	{
		printf(RED"Error: btree_count_leaves() != 3 (it is %d)\n"RESET, btree_count_leaves(parent));
		ret = false;
		return (ret);
	}
	else
		printf(GREEN"Success: btree_count_leaves()\n"RESET);

	btree_clear(&parent, NULL);
	return (ret);
}
