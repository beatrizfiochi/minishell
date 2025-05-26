/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:37:40 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/26 09:38:50 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>		// bool
# include <sys/types.h>		// pid_t
# include "libft/libft.h"
# include "btree/btree.h"

typedef struct s_shell
{
	char		**paths;
	char		**envp;
	t_btnode	*cmds;
	int			last_fd;
	int			ncmds;
	int			fd1;
	int			fd2;
	bool		append_fd2;
	pid_t		last_pid;
	t_list		*variable_list;
}	t_shell;

int		read_command(t_shell *shell, char *envp[]);
void	printf_error(const char *error);

#endif
