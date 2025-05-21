/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/21 10:33:30 by djunho           ###   ########.fr       */
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
#include "execute/execution.h"

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

static char	*read_line(void)
{
	if (isatty(STDIN_FILENO))
		return (readline(PROMPT));
	else
		return (readline(NULL));
}

// 0 -> success
// -1 -> error
int	read_command(t_list *var_list, char *envp[])
{
	char		*line;
	t_list		*token_list;
	t_btnode	*btree;
	int			ret;

	line = read_line();
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
		ret = execute(btree, envp);
		(void)ret;
		btree_clear(btree, free_btree_node);
	}
	else
		ft_lstclear(&token_list, free);
	free(line);
	return (0);
}
