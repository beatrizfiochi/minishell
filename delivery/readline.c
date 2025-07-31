/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 20:27:08 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 20:55:28 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"
#include "signals/signals.h"

char	*sh_read_line(t_shell *shell, const char *prompt)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		line = readline(prompt);
	else
		line = readline(NULL);
	if (line == NULL)
		return (NULL);
	shell->nlines++;
	return (line);
}

void	sh_add_history(t_shell *shell, const char *line)
{
	(void)shell;
	if (line != NULL && line[0] != '\0')
	{
		if (isatty(STDIN_FILENO))
			add_history(line);
	}
}
