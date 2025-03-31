/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:56 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:08:57 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>

static bool	is_space(char c)
{
	return ((c == ' ') || (c == '\f') || (c == '\n')
		|| (c == '\r') || (c == '\t') || (c == '\v'));
}

long	ft_atol(const char *nptr)
{
	int		signal;
	long	num;

	while ((*nptr != '\0') && is_space(*nptr))
		nptr++;
	signal = 1;
	if ((*nptr != '\0') && ((*nptr == '-') || (*nptr == '+')))
	{
		if (*nptr == '-')
			signal *= -1;
		nptr++;
	}
	num = 0;
	while ((*nptr != '\0') && ((*nptr >= '0') && (*nptr <= '9')))
	{
		num = (num * 10) + (*nptr - '0');
		nptr++;
	}
	return (num * signal);
}
