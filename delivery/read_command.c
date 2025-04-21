/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/21 17:35:31 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>				//printf
#include <readline/readline.h>
#include <stdlib.h>				//free
#include <unistd.h>				//write


// 0 -> success
// -1 -> error
static int	read_command(void)
{
	char	*line;
	char	*clean_line;

	line = readline(PROMPT);
	clean_line = clean_string(line);
	if (clean_line == NULL)
		return (-1);
	printf("COMMAND RECEIVED: %s\n", clean_line);
	are_closed_single_quotes(clean_line);
	are_closed_db_quotes(clean_line);
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
		printf("EXECUTANDO COMANDOS\n");
		ret = read_command();
	}
	if (ret != 0)
		printf_error("EOF\n");
	return (ret);
}
