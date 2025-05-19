/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/14 11:50:49 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>				//free
#include <stdio.h>				//printf
#include <unistd.h>				//write
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include "minishell.h"
#include "parser/parser.h"
#include "cmd.h"
#include "debug.h"

#define PROMPT "My shell > "

__attribute__((weak)) void	debug_print_tree(t_btnode *tree)
{
	(void)tree;
}

__attribute__((weak)) void	debug_print_read_command(t_list *node, char *line)
{
	(void)node;
	(void)line;
}

static void	free_btree_node(void *content)
{
	t_content_node	*node_content;

	node_content = (t_content_node *)content;
	ft_lstclear(&(node_content->cmd.tokens), free);
	free(node_content);
}

// 0 -> success
// -1 -> error
static int	read_command(t_list *var_list)
{
	char		*line;
	t_list		*token_list;
	t_btnode	*btree;

	if (isatty(STDIN_FILENO))
		line = readline(PROMPT);
	else
		line = readline(NULL);
	if (line == NULL)
		return (-1);
	if (line != NULL && *line != '\0')
		add_history (line);
	token_list = tokenization(line);
	debug_print_read_command(token_list, line);
	search_and_expand(token_list, var_list);
	clean_token_quotes(token_list);
	if (token_list != NULL)
	{
		btree = create_tree(&token_list);
		if (btree == NULL)
			printf_error("Error to parse\n");
		debug_print_tree(btree);
		btree_clear(btree, free_btree_node);
	}
	else
		ft_lstclear(&token_list, free);
	free(line);
	return (0);
}

int	run_minishell(void)
{
	int	ret;
	t_list	*variable_list;
	char	*name_1 = "oi";
	char	*value_1 = "hi";
	char	*name_2 = "tchau";
	char	*value_2 = "bye";
	char	*name_3 = "oooi";
	char	*value_3 = "hhhi";
	char	*name_4 = "oiii";
	char	*value_4 = "hiii";

	variable_list = create_var_node(name_1, value_1);
	variable_list->next = create_var_node(name_2, value_2);
	variable_list->next->next = create_var_node(name_3, value_3);
	variable_list->next->next->next = create_var_node(name_4, value_4);
	ret = read_command(variable_list);
	while (ret == 0)
	{
		printf("var_name = %s, var_value = %s\n", ((t_content_var *)(variable_list->content))->var_name, ((t_content_var *)(variable_list->content))->var_value);
		printf("var_name = %s, var_value = %s\n", ((t_content_var *)(variable_list->next->content))->var_name, ((t_content_var *)(variable_list->next->content))->var_value);
		printf("var_name = %s, var_value = %s\n", ((t_content_var *)(variable_list->next->next->content))->var_name, ((t_content_var *)(variable_list->next->next->content))->var_value);
		printf("var_name = %s, var_value = %s\n", ((t_content_var *)(variable_list->next->next->next->content))->var_name, ((t_content_var *)(variable_list->next->next->next->content))->var_value);
		ret = read_command(variable_list);
	}
	ft_lstclear(&variable_list, free_var_content);
	return (ret);
}
