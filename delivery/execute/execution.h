/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:49:18 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:57:57 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../libft/libft.h"
# include "../btree/btree.h"
# include <stdbool.h>

typedef struct s_pipex
{
	char		**paths;
	char		**envp;
	t_btnode	*cmds;
	int			ncmds;
	int			fd1;
	int			fd2;
	bool		append_fd2;
	pid_t		last_pid;
}	t_pipex;

void	free_join(char **args);
bool	create_cmd_path(char *cmd, char **envp, char **path);
int		process(t_pipex *pipex);
int		execute(t_btnode *tree, char *envp[]);
char	**convert_list_to_vector(t_list *list);

# ifndef TEST

void	debug_btree_print(t_btnode *node);
# endif

#endif // EXECUTION_H
