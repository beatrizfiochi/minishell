/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:46 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:18:56 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	unsigned int	j;

	if (ft_strlen(little) == 0)
		return ((char *)big);
	i = 0;
	while ((i < len) && (big[i] != '\0'))
	{
		j = 0;
		while (((i + j) < len) && (little[j] != '\0')
			&& (big[i + j] == little[j]))
			j++;
		if (little[j] == '\0')
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
