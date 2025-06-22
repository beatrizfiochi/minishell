/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:13:21 by djunho            #+#    #+#             */
/*   Updated: 2025/06/22 19:52:44 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../cmd.h"

struct	s_builtins
{
	const char	*cmd;
	int			(*func)(int argc, char *argv[]);
};

int	execute_builtin(t_cmd *cmd);

int	echo(int argc, char *argv[]);

#endif // BUILTINS_H
