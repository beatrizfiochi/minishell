/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:13:21 by djunho            #+#    #+#             */
/*   Updated: 2025/06/22 18:32:23 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

struct	s_builtins
{
	const char	*cmd;
	int			(*func)(int argc, char *argv[]);
};

int	run_builtin(int argc, char *argv[], int *ret);

int	echo(int argc, char *argv[]);

#endif // BUILTINS_H
