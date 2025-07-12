/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:13:21 by djunho            #+#    #+#             */
/*   Updated: 2025/07/12 14:25:23 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../cmd.h"
# include "../libft/libft.h"
# include "../minishell.h"

struct	s_builtins
{
	const char	*cmd;
	int			(*func)(int argc, char *argv[], t_shell *shell);
};

int		execute_builtin(t_cmd *cmd, t_shell *shell);

int		echo(int argc, char *argv[], t_shell *shell);
int		cd(int argc, char *argv[], t_shell *shell);
int		env(int argc, char *argv[], t_shell *shell);
int		export(int argc, char *argv[], t_shell *shell);
int		pwd(int argc, char *argv[], t_shell *shell);
int		sh_exit(int argc, char *argv[], t_shell *shell);
int		unset(int argc, char *argv[], t_shell *shell);

#endif // BUILTINS_H
