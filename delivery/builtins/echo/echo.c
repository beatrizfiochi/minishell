/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:48:18 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 14:14:07 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../../libft/libft.h"

int	echo(int argc, char *argv[], char *envp[])
{
	int		i;
	bool	no_nl;

	(void)envp;
	i = 1;
	no_nl = false;
	if ((i < argc) && (ft_strncmp(argv[i], "-n", 2) == 0))
	{
		no_nl = true;
		i++;
	}
	if (i < argc)
		ft_printf("%s", argv[i++]);
	while (i < argc)
	{
		ft_printf(" %s", argv[i++]);
	}
	if (!no_nl)
		ft_printf("\n");
	return (0);
}
