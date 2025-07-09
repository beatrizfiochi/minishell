/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/02 19:22:44 by djunho           ###   ########.fr       */
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
	ft_lstclear(&(node_content->cmd.tokens), free);
	free(node_content);
}

static char	*read_line(void)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = readline(PROMPT);
	else
		line = readline(NULL);
	if (line == NULL)
		return (NULL);
	if (line[0] != '\0')
		add_history (line);
	return (line);
}

// 0 -> success
// -1 -> error
int	read_command(t_shell *shell)
{
	char		*line;
	t_list		*token_list;

	init_signals();
	line = read_line();
	if (line == NULL)
		return (-1);
	token_list = tokenization(line);
	debug_print_read_command(token_list, line);
	while (token_list != NULL)
	{
		shell->cmds = create_tree(&token_list, NULL);
		if (shell->cmds == NULL)
		{
			shell->last_exit_status = EXIT_INCORRECT_USAGE;
			break ;
		}
		debug_print_tree(shell->cmds);
		shell->last_exit_status = execute(shell);
		handle_signal_output(shell);
		btree_clear(&shell->cmds, free_btree_node);
		break ;
	}
	free(line);
	return (shell->last_exit_status);
}
