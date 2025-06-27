/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:48:18 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 16:09:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../../libft/libft.h"

int	echo(int argc, char *argv[], t_list *var_list)
{
	int		i;
	bool	no_nl;

	(void)var_list;
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
