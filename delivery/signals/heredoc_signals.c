/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 08:56:59 by djunho            #+#    #+#             */
/*   Updated: 2025/07/11 22:53:36 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include "signals.h"

void	handle_sigint_on_heredoc(int signal)
{
	g_signal = signal;
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(STDOUT_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

static void	reopen_stdin(void)
{
	int	fd;

	close(STDIN_FILENO);
	fd = open("/dev/tty", O_RDONLY | O_CLOEXEC);
	if (fd < 0)
	{
		perror("Error reopening stdin");
		exit(EXIT_FAILURE);
	}
	if (fd != STDIN_FILENO)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			perror("Error duplicating stdin");
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

void	heredoc_signals(void)
{
	g_signal = 0;
	signal(SIGINT, handle_sigint_on_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_ignore_signals(void)
{
	if (g_signal == SIGINT)
		reopen_stdin();
	ignore_signals();
}
