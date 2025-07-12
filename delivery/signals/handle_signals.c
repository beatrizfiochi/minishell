/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 19:54:13 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/11 22:51:55 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include "signals.h"
#include "../minishell.h"

int	g_signal;

void	handle_sigint(int signal)
{
	g_signal = signal;
	if (signal == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		write(STDOUT_FILENO, "\n", 1);
		rl_redisplay();
	}
}

void	init_signals(void)
{
	g_signal = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}

// Reset SIGINT and SIGQUIT to Default
void	reset_signals(void)
{
	g_signal = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Ignore SIGINT and SIGQUIT
//		used to ignore signals in the parent process before forking
void	ignore_signals(void)
{
	g_signal = 0;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_signal_output(int ret)
{
	if (ret == (EXIT_SIGNAL_BASE + SIGINT))
		printf("\n");
	else if (ret == (EXIT_SIGNAL_BASE + SIGQUIT))
		printf("Quit (core dumped)\n");
}
