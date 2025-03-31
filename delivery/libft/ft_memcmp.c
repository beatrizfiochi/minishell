/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:07:38 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:13:19 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int					res;
	unsigned int		i;
	const unsigned char	*aux1;
	const unsigned char	*aux2;

	aux1 = (unsigned char *)s1;
	aux2 = (unsigned char *)s2;
	res = 0;
	i = 0;
	while ((res == 0) && (i < n))
	{
		res = aux1[i] - aux2[i];
		i++;
	}
	return (res);
}
