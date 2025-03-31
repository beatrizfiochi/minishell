/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:07:29 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:18:17 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static size_t	get_len(int nb)
{
	size_t	len;

	len = 0;
	if (nb <= 0)
	{
		len++;
	}
	while ((nb != 0))
	{
		nb = nb / 10;
		len++;
	}
	return (len);
}

static size_t	ft_itoa_recursive(char *s, int n)
{
	int	i;

	if (n == 0)
		return (0);
	i = ft_itoa_recursive(s, n / 10);
	s[i] = '0' + (n % 10);
	return (i + 1);
}

char	*ft_itoa(int n)
{
	char	*s;
	size_t	len;
	size_t	i;

	len = get_len(n);
	s = (char *)malloc((len + 1) * sizeof(char));
	if (s == NULL)
		return (NULL);
	s[len] = '\0';
	i = 0;
	if (n < 0)
	{
		s[i++] = '-';
		i += ft_itoa_recursive(&s[i], (n / 10) * (-1));
		s[i] = '0' + ((n % 10) * (-1));
	}
	else
	{
		i += ft_itoa_recursive(&s[i], n / 10);
		s[i] = '0' + (n % 10);
	}
	return (s);
}
