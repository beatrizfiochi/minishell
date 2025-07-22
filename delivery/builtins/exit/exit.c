/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:57:23 by djunho            #+#    #+#             */
/*   Updated: 2025/07/22 18:55:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>            // LONG_MAX
#include <stdbool.h>
#include <string.h>
#include "../../minishell.h"

#define LLONG_MAX_STR "9223372036854775807"
#define LLONG_MIN_STR "9223372036854775808"

char	*skip_spaces(char *str)
{
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	return (str);
}

static bool	check_if_bigger(char *input)
{
	int		len;
	bool	negative;
	char	*digits;

	input = skip_spaces(input);
	negative = false;
	if ((*input == '+') || (*input == '-'))
	{
		if (*input == '-')
			negative = true;
		digits = input + 1;
	}
	else
		digits = input;
	while (*digits == '0')
		digits++;
	len = ft_strlen(digits);
	if (len > 19)
		return (true);
	if (len < 19)
		return (false);
	if (negative == true)
		return (ft_strncmp(digits, LLONG_MIN_STR, 19) > 0);
	else
		return (ft_strncmp(digits, LLONG_MAX_STR, 19) > 0);
}

static bool	check_input(char *input)
{
	int	i;

	i = 0;
	input = skip_spaces(input);
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
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_fprintf(STDERR_FILENO, "exit\n");
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
