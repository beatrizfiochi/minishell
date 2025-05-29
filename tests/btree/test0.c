/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test0.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 12:00:26 by djunho            #+#    #+#             */
/*   Updated: 2025/05/13 21:51:28 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "btree.h"
#include <stdio.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef struct s_content
{
	char	*str;
	int		ret;
}	t_content;

static void my_free(void *content)
{
	printf("content is > %s\n", ((t_content *)content)->str);
	free(content);
}

void *create_content(char *str, int ret)
{
	t_content *content = malloc(sizeof(t_content));
	if (content == NULL)
		return (NULL);
	*content = (t_content){
		.str = str,
		.ret = ret,
	};
	return (content);
}

bool	test0(void)
{
	bool	ret;

	/*
	              (parent)
	              /    \
	           (left)  (right)
	           /   \
	          /    \
	 (left:left) (left:right)
	*/

	printf(CYAN"Running test 0\n"RESET);

	t_content *c_right = create_content("\tright", 0);
	t_btnode *right = btree_new(c_right);

	t_content *c_left = create_content("\tleft", 0);
	t_btnode *left = btree_new(c_left);

	t_content *c_parent = create_content("\tparent", 0);
	t_btnode *parent = btree_new(c_parent);

	parent->left = left;
	parent->right = right;

	t_content *c_left_left = create_content("\tleft: left", 1);	// WILL RETURN ERROR
	t_btnode *left_left = btree_new(c_left_left);

	t_content *c_left_right = create_content("\tleft: right", 0);
	t_btnode *left_right = btree_new(c_left_right);

	left->left = left_left;
	left->right = left_right;

	if (3 != btree_count_leaves(parent))
	{
		printf(RED"Error: btree_count_leaves() != 3 (it is %d)\n"RESET, btree_count_leaves(parent));
		ret = false;
		return (ret);
	}
	else
		printf(GREEN"Success: btree_count_leaves()\n"RESET);

	printf(MAGENTA);
	btree_clear(&parent, my_free);
	printf(RESET);
	char *answer = readline("Do you see only \"left:left, left:right, left, right, parent\" messages? Y/n:");
	if ((strcmp(answer, "y") == 0) || (strcmp(answer, "Y") == 0) || (strcmp(answer, "") == 0))
		ret = true;
	else
		ret = false;
	free(answer);

	return (ret);
}
