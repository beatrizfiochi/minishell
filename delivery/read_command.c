/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/22 18:03:13 by bfiochi-         ###   ########.fr       */
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
#include "signals/signals.h"

#define PROMPT "My shell > "

int	g_signal;

__attribute__((weak)) void	debug_print_tree(t_btnode *tree)
{
	(void)tree;
}

__attribute__((weak)) void	debug_print_read_command(t_list *node, char *line)
{
	(void)node;
	(void)line;
}

void	free_btree_node(void *content)
{
	t_content_node	*node_content;

	node_content = (t_content_node *)content;
	if (node_content->op == OP_CMD)
	{
		ft_lstclear(&(node_content->cmd.tokens), free);
	}
	else if (node_content->op == OP_PIPE)
	{
		if (node_content->pipe.pipe[0] != -1)
			close(node_content->pipe.pipe[0]);
		if (node_content->pipe.pipe[1] != -1)
			close(node_content->pipe.pipe[1]);
	}
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
int	read_command(t_shell *shell, char *envp[])
{
	char		*line;
	t_list		*token_list;
	int			ret;

	init_signals();
	line = read_line();
	if (line == NULL)
		return (-1);
	if (line != NULL && *line != '\0')
		add_history (line);
	token_list = tokenization(line);
	debug_print_read_command(token_list, line);
	search_and_expand(token_list, shell->variable_list);
	clean_token_quotes(token_list);
	if (token_list != NULL)
	{
		shell->cmds = create_tree(&token_list, NULL);
		if (shell->cmds == NULL)
			printf_error("Error to parse\n");
		debug_print_tree(shell->cmds);
		ret = execute(shell, envp);
		(void)ret;
		btree_clear(&shell->cmds, free_btree_node);
	}
	else
		ft_lstclear(&token_list, free);
	free(line);
	return (0);
}
