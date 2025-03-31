/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:07:04 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:07:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*arr;
	size_t	i;

	i = nmemb * size;
	if ((nmemb != 0) && ((i / nmemb) != size))
		return (NULL);
	arr = (void *)malloc(i);
	if (!arr)
		return (0);
	ft_bzero(arr, i);
	return (arr);
}
