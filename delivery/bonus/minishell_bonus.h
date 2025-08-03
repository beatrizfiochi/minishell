/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:37:40 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/08/03 11:23:51 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>		// bool
# include <sys/types.h>		// pid_t
# include "../libft/libft.h"
# include "btree/btree_bonus.h"
# include "cmd_bonus.h"

# define EXIT_INCORRECT_USAGE	2
# define EXIT_CMD_CANNOT_EXEC	126
# define EXIT_CMD_NOT_FOUND		127
# define EXIT_SIGNAL_BASE		128

typedef struct s_shell
{
	char		**paths;
	t_btnode	*cmds;
	t_cmd		*last_cmd;
	int			ncmds;
	int			nlines;				// Number of lines read (readline calls)
	pid_t		last_pid;
	int			last_exit_status;	// 0 - 255
									// 0 - success	(EXIT_SUCCESS)
									// 1 - generic error (EXIT_FAILURE)
									// 2 - incorrect command usage
									// 126 - command invoked cannot execute
									// 127 - command not found
									// 128 + n - fatal error signal "n"
	t_list		*variable_list;		// Variable list
	t_list		*tmp_var_list;		// Temporary variable list
	bool		is_running_redirect;// Flag to indicate that the redirect is
									//  running (pipe, redirect in/out)
	bool		is_last_redirect;	// Flag to indicate if this is the last
									//  redirect in the chain. So it should
									//  print at the stdout or the redirect
									//  output file
	int			in_fd;
	int			out_fd;
	struct s_pipe
	{
		int		pipe[2];			// pipe file descriptors
		int		carry_over_fd;		// file descriptor to carry over the pipe
	} pipe;
}	t_shell;

int		read_command(t_shell *shell);
void	free_btree_node(void *content);
void	clear_minishell(t_shell *shell);
void	reset_minishell(t_shell *shell, int ret);
char	*sh_read_line(t_shell *shell, const char *prompt);
void	sh_add_history(t_shell *shell, const char *line);
void	sh_clean_history(void);
bool	is_directory(const char *path);
bool	file_exist(const char *path);

int		p_error(const char *msg, const int ret);

#endif
