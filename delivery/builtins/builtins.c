/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 13:30:38 by djunho            #+#    #+#             */
/*   Updated: 2025/06/22 15:28:38 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "builtins.h"

int	run_builtin(int argc, char *argv[], int *ret)
{
	size_t					i;
	const struct s_builtins	cmds[] = {
	{"echo", echo,},
	};

	*ret = 127;
	i = 0;
	while ((i < (sizeof(cmds) / sizeof(cmds[0])))
		&& (ft_strncmp(cmds[i].cmd, (const char*)argv[0],
				ft_strlen(cmds[i].cmd)) != 0))
		i++;
	if (i < (sizeof(cmds) / sizeof(cmds[0])))
		*ret = (cmds[i].func(argc, argv));
	return (*ret);
}
