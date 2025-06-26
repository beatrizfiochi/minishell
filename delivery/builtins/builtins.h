/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:13:21 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 16:10:06 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "../cmd.h"
# include "../libft/libft.h"

struct	s_builtins
{
	const char	*cmd;
	int			(*func)(int argc, char *argv[], t_list *var_list);
};

int	execute_builtin(t_cmd *cmd, t_list *var_list);

int	echo(int argc, char *argv[], t_list *var_list);
int	cd(int argc, char *argv[], t_list *var_list);

#endif // BUILTINS_H
