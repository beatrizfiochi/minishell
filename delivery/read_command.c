/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/27 16:26:42 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>				//printf
#include <readline/readline.h>
#include <stdlib.h>				//free
#include <unistd.h>				//write

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
	printf("Found operator: %d\n", search_op(line));
	while (node != NULL)
	{
		printf("Nodes: %s\n", (char *)node->content);
		node = node->next;
	}
}
#endif

// 0 -> success
// -1 -> error
static int	read_command(void)
{
	char	*line;
	char	*clean_line;
	t_list	*token_list;
	char	*prompt;

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
