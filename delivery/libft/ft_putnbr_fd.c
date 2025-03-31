/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:06 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:22:29 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>

static void	ft_putnbr_fd_recursive(int fd, int n)
{
	char	c;

	if (n == 0)
		return ;
	ft_putnbr_fd_recursive(fd, n / 10);
	c = '0' + (n % 10);
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n < 0)
	{
		c = '-';
		write(fd, &c, 1);
		ft_putnbr_fd_recursive(fd, (n / 10) * (-1));
		c = '0' + ((n % 10) * (-1));
		write(fd, &c, 1);
	}
	else
	{
		ft_putnbr_fd_recursive(fd, n / 10);
		c = '0' + (n % 10);
		write(fd, &c, 1);
	}
}
