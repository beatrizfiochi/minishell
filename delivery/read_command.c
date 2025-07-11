/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/11 08:51:04 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>				//free
#include <stdio.h>				//printf
#include <unistd.h>				//write
#include "libft/libft.h"
#include "minishell.h"
#include "parser/parser.h"
#include "cmd.h"
#include "debug.h"
#include "execute/execution.h"
#include "signals/signals.h"
#include "color.h"

#define PROMPT COLOR_GREEN"My shell > "COLOR_RESET

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

// 0 -> success
// -1 -> error
int	read_command(t_shell *shell)
{
	char		*line;
	t_list		*token_list;

	init_signals();
	line = sh_read_line(shell, PROMPT);
	if (line == NULL)
		return (-1);
	sh_add_history(shell, line);
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
