/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:13:21 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 14:15:17 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../cmd.h"

struct	s_builtins
{
	const char	*cmd;
	int			(*func)(int argc, char *argv[], char *envp[]);
};

int	execute_builtin(t_cmd *cmd, char *envp[]);

int	echo(int argc, char *argv[], char *envp[]);
int	cd(int argc, char *argv[], char *envp[]);

#endif // BUILTINS_H
