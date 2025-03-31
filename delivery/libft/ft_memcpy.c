/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:07:42 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:07:42 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	if ((dest == NULL) && (src == NULL))
		return (NULL);
	while (n-- > 0)
	{
		((char *)dest)[n] = ((char *)src)[n];
	}
	return (dest);
}
