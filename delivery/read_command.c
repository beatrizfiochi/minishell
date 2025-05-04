/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/04 15:42:44 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>				//printf
#include <readline/readline.h>
#include <stdlib.h>				//free
#include <unistd.h>				//write
#include "parser/parser.h"
#include "cmd.h"

#define PROMPT "My shell > "

//APAGAR
#ifndef TEST

static void	debug_print_read_command(t_list *node, char *line)
{
	(void)node;
	(void)line;
}
#else

static void	debug_print_read_command(t_list *node, char *line)
{
	(void)node;
	(void)line;
	printf("Command received: %s\n", line);
	// printf("Found operator: %s\n", search_op(line)  ? "true" : "false");
	while (node != NULL)
	{
		printf("Nodes: %s\n", (char *)node->content);
		node = node->next;
	}
}
#endif

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

// 0 -> success
// -1 -> error
static int	read_command(void)
{
	char		*line;
	char		*clean_line;
	t_list		*token_list;
	char		*prompt;
	t_btnode	*btree;

	if (isatty(STDIN_FILENO))
		prompt = PROMPT;
	else
		prompt = "";
	line = readline(prompt);
	clean_line = clean_string(line);
	if (clean_line == NULL)
		return (-1);
	token_list = tokenization(clean_line);
	debug_print_read_command(token_list, clean_line);
	btree = create_tree(token_list);
	btree_foreach_dfs(btree, printf_string_node, printf_string_leaf);
	btree_clear(btree, free);
	ft_lstclear(&token_list, free);
	free(line);
	free(clean_line);
	return (0);
}

int	run_minishell(void)
{
	int	ret;

	ret = read_command();
	while (ret == 0)
	{
		ret = read_command();
	}
	return (ret);
}
