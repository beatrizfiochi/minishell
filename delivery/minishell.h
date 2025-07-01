/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:37:40 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/26 16:37:12 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>		// bool
# include <sys/types.h>		// pid_t
# include "libft/libft.h"
# include "btree/btree.h"
# include "cmd.h"

typedef struct s_shell
{
	char		**paths;
	t_btnode	*cmds;
	t_cmd		*last_cmd;
	int			ncmds;
	pid_t		last_pid;
	int			last_exit_status;
	t_list		*variable_list;
	struct s_pipe
	{
		int		pipe[2];			// pipe file descriptors
		int		carry_over_fd;		// file descriptor to carry over the pipe
		bool	is_last_pipe;		// Flag to indicate if this is the last
									//  pipe in the chain
		bool	will_run_a_pipe;	// Flag to indicate that the next command
									//  will run a pipe
	} pipe;
}	t_shell;

int		read_command(t_shell *shell);
void	free_btree_node(void *content);
void	clear_minishell(t_shell *shell);
void	printf_error(const char *error);

#endif
