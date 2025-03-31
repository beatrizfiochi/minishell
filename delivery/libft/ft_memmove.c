/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:07:51 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 14:29:27 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	aux;

	if (src == dest)
		return (dest);
	else if (src > dest)
		aux = src - dest;
	else
		aux = dest - src;
	if (aux > n)
		ft_memcpy(dest, src, n);
	else
	{
		if (src > dest)
		{
			ft_memcpy(dest, src, aux);
			ft_memmove(dest + aux, src + aux, n - aux);
		}
		else
		{
			ft_memcpy(dest + aux, src + aux, n - aux);
			ft_memcpy(dest, src, aux);
		}
	}
	return (dest);
}
