/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 11:48:18 by djunho            #+#    #+#             */
/*   Updated: 2025/07/12 16:46:14 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "../../../libft/libft.h"
#include "../../minishell_bonus.h"

static int	check_options(int argc, char *argv[], bool *no_nl)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-')
			return (i);
		j = 1;
		while (argv[i][j] == 'n')
			j++;
		if ((j == 1) || (argv[i][j] != '\0'))
			return (i);
		*no_nl = true;
		i++;
	}
	return (i);
}

int	echo(int argc, char *argv[], t_shell *shell)
{
	int		i;
	bool	no_nl;

	(void)shell;
	no_nl = false;
	i = check_options(argc, argv, &no_nl);
	if (i < argc)
		ft_printf("%s", argv[i++]);
	while (i < argc)
	{
		ft_printf(" %s", argv[i]);
		i++;
	}
	if (!no_nl)
		ft_printf("\n");
	return (0);
}
