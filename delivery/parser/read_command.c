/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:00:20 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/24 15:45:32 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>				//printf
#include <readline/readline.h>
#include <stdlib.h>				//free
#include <unistd.h>				//write


//APAGAR
void printList(t_tokens* head)
{
    while (head != NULL) {
        printf("Nó: %s\n", head->content);
        head = head->next;
    }
}

// 0 -> success
// -1 -> error
static int	read_command(void)
{
	char		*line;
	char		*clean_line;
	t_tokens	*token_list;  //APAGAR

	line = readline(PROMPT);
	clean_line = clean_string(line);
	if (clean_line == NULL)
		return (-1);
	printf("COMMAND RECEIVED: %s\n", clean_line);
	token_list = tokenization(clean_line);
	printList(token_list);         //APAGAR
	free_tokens(token_list);
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
