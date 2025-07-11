/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:57:23 by djunho            #+#    #+#             */
/*   Updated: 2025/07/12 00:50:28 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <limits.h>		// LONG_MAX
#include <stdlib.h>
#include <unistd.h>
#include "../../minishell.h"

static bool	check_if_bigger(char *input)
{
	long long	num;
	long long	num2;

	if (ft_strlen(input) > 19)
		return (false);
	if ((*input == '-') || (*input == '+'))
		return (false);
	num = ft_atoll(input);
	num2 = ft_atoll(input + 1);
	if (num2 < num)
		return (false);
	return (true);
}

// hack: Avoid extensive checks by rejecting inputs longer than 19 characters,
// as the maximum value for a long integer is 9223372036854775807
static bool	check_input(char *input)
{
	int	i;

	i = 0;
	if ((input[i] == '-') || (input[i] == '+'))
		i++;
	while ((input[i] != '\0') && ft_isdigit(input[i]))
		i++;
	if (input[i] != '\0')
		return (false);
	return (!check_if_bigger(input));
}

int	sh_exit(int argc, char *argv[], t_shell *shell)
{
	long long	ret;

	ret = 0;
	if (argc >= 2)
	{
		ret = ft_atoll(argv[1]);
		if (!check_input(argv[1]) || (ret > LONG_MAX))
		{
			ret = EXIT_INCORRECT_USAGE;
			ft_fprintf(STDERR_FILENO, "exit: %s: numeric argument required\n",
				argv[1]);
		}
		else if (argc > 2)
		{
			ft_fprintf(STDERR_FILENO, "exit: too many arguments\n");
			return (EXIT_FAILURE);
		}
	}
	clear_minishell(shell);
	free(argv);
	exit(ret % 256);
	return (EXIT_SUCCESS);
}
