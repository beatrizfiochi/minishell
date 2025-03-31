/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:42 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:08:43 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				res;
	unsigned int	i;

	res = 0;
	i = 0;
	while ((res == 0) && (i < n) && ((s1[i] != '\0') || (s2[i] != '\0')))
	{
		res = 0;
		if (s1[i] != '\0')
		{
			res = (unsigned char)s1[i];
		}
		if (s2[i] != '\0')
		{
			res = res - (unsigned char)s2[i];
		}
		i++;
	}
	return (res);
}
