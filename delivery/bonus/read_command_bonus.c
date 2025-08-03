/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/31 20:56:30 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>				//free
#include <stdio.h>				//printf
#include <unistd.h>				//write
#include "../libft/libft.h"
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

static int	execute_token(t_list **tokens, t_shell *shell)
{
	int	ret;

	ret = create_tree(shell, &shell->cmds, tokens, NULL);
	if (ret == EXIT_INCORRECT_USAGE)
		ft_fprintf(STDERR_FILENO, "syntax error near unexpected token\n");
	if (ret != EXIT_SUCCESS)
		return (ret);
	debug_print_tree(shell->cmds);
	ret = execute(shell);
	handle_signal_output(ret);
	btree_clear(&shell->cmds, free_btree_node);
	return (ret);
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
	handle_ctrl_c(shell);
	sh_add_history(shell, line);
	token_list = tokenization(line);
	if (token_list == NULL)
		shell->last_exit_status = EXIT_INCORRECT_USAGE;
	debug_print_read_command(token_list, line);
	ret = shell->last_exit_status;
	while (token_list != NULL)
	{
		ret = execute_token(&token_list, shell);
		if (ret != EXIT_SUCCESS)
			break ;
	}
	reset_minishell(shell, ret);
	free(line);
	return (shell->last_exit_status);
}
