/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:29 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:08:30 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	unsigned int	i;
	unsigned int	len;

	len = 0;
	i = 0;
	if (size != 0)
	{
		while ((dst[len] != '\0') && (len < size - 1))
			len++;
		if ((dst[len] != '\0') && (len == size - 1))
			len++;
	}
	if (len < size)
	{
		while ((len + i < size - 1) && (src[i] != '\0'))
		{
			dst[len + i] = src[i];
			i++;
		}
		dst[len + i] = '\0';
	}
	while (src[i] != '\0')
		i++;
	return (len + i);
}
