/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/11 09:40:46 by djunho           ###   ########.fr       */
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
	int			ret;

	init_signals();
	line = sh_read_line(shell, COLOR_GREEN PROMPT COLOR_RESET);
	if (line == NULL)
		return (-1);
	sh_add_history(shell, line);
	token_list = tokenization(line);
	debug_print_read_command(token_list, line);
	while (token_list != NULL)
	{
		ret = create_tree(shell, &shell->cmds, &token_list, NULL);
		if (ret != EXIT_SUCCESS)
			break ;
		debug_print_tree(shell->cmds);
		ret = execute(shell);
		handle_signal_output(ret);
		btree_clear(&shell->cmds, free_btree_node);
		break ;
	}
	shell->last_exit_status = ret;
	free(line);
	return (shell->last_exit_status);
}
