/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:52 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:09:05 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		l;

	i = 0;
	while (ft_strchr(set, s1[i]) != NULL)
		i++;
	l = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[l]) != NULL)
		l--;
	return (ft_substr(s1, i, l - i + 1));
}
