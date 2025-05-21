/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 22:46:44 by djunho            #+#    #+#             */
/*   Updated: 2025/05/18 20:55:06 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <unistd.h>
#include "../libft.h"

// Check size > 2
// Check if there is a + or - char
static bool	is_input_valid(const char *base)
{
	int	len;
	int	i;

	len = ft_strlen(base);
	if (len < 2)
		return (false);
	i = 0;
	while ((base[i] != '\0') && (base[i] != '+') && (base[i] != '-'))
		i++;
	if (base[i] != '\0')
		return (false);
	while (len > 0)
	{
		i = 0;
		while ((base[i] != base[len - 1]))
			i++;
		if ((base[i] == base[len - 1]) && (i != len - 1))
			return (false);
		len--;
	}
	return (true);
}

static int	ft_putnbr_recursive(unsigned long long int nb, const char *base_str,
								int base, int fd)
{
	if ((nb / base) == 0)
	{
		if (fd >= 0)
			ft_putchar_fd(base_str[nb], fd);
		return (1);
	}
	else
	{
		return (ft_putnbr_recursive(nb / base, base_str, base, fd)
			+ ft_putnbr_recursive(nb % base, base_str, base, fd));
	}
}

static int	normalize_input(long long int *up, long long int *down,
							long long int nbr, int len)
{
	int	ret;

	if (nbr < 0)
	{
		*up = (-1) * (nbr / len);
		*down = (-1) * (nbr % len);
		ret = 1;
	}
	else
	{
		*up = (nbr / len);
		*down = (nbr % len);
		ret = 0;
	}
	return (ret);
}

int	ft_putnbr_base_fd(long long int nbr, char *base, int fd)
{
	long long int	up;
	long long int	down;
	int				len;
	int				ret;

	if (!is_input_valid(base))
		return (-1);
	len = ft_strlen(base);
	if ((nbr < 0) && (fd >= 0))
		ft_putchar_fd('-', fd);
	ret = normalize_input(&up, &down, nbr, len);
	if (up == 0)
	{
		if (fd >= 0)
			ft_putchar_fd(base[down], fd);
		ret += 1;
	}
	else
	{
		ret += ft_putnbr_recursive(up, base, len, fd);
		ret += ft_putnbr_recursive(down, base, len, fd);
	}
	return (ret);
}

int	ft_putunbr_base_fd(unsigned long long int nbr, const char *base, int fd)
{
	unsigned long long int	up;
	unsigned long long int	down;
	int						len;
	int						ret;

	if (!is_input_valid(base))
		return (-1);
	len = ft_strlen(base);
	up = (nbr / len);
	down = (nbr % len);
	ret = 0;
	if (up == 0)
	{
		if (fd >= 0)
			ft_putchar_fd(base[down], fd);
		ret += 1;
	}
	else
	{
		ret += ft_putnbr_recursive(up, base, len, fd);
		ret += ft_putnbr_recursive(down, base, len, fd);
	}
	return (ret);
}
