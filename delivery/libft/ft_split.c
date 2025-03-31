/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 13:08:12 by djunho            #+#    #+#             */
/*   Updated: 2024/10/27 13:16:27 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

static int	find_how_many(char const *str, char c)
{
	int		i;

	i = 0;
	while (*str != '\0')
	{
		while ((*str != '\0') && (*str == c))
			str++;
		if (*str != '\0')
			i++;
		while ((*str != '\0') && (*str != c))
			str++;
	}
	return (i);
}

static int	create_array(char ***dst, int len)
{
	len++;
	*dst = (char **)malloc(sizeof(char *) * len);
	if (*dst == NULL)
		return (-1);
	while (len > 0)
		(*dst)[--len] = NULL;
	return (1);
}

static char	**free_array(char **dst, int i)
{
	while (i > 0)
	{
		i--;
		free(dst[i]);
	}
	free(dst);
	return (NULL);
}

static char	*create_substr(char const *s, char c, char **pos)
{
	char	*new;

	*pos = ft_strchr(s, c);
	if (*pos != NULL)
		new = ft_substr(s, 0, *pos - s);
	else
		new = ft_strdup(s);
	return (new);
}

char	**ft_split(char const *s, char c)
{
	char	**dst;
	int		i;
	char	*pos;

	if (create_array(&dst, find_how_many(s, c)) != 1)
		return (NULL);
	i = 0;
	while ((s != NULL) && (*s != '\0'))
	{
		while ((*s != '\0') && (*s == c))
			s++;
		if (*s == '\0')
			break ;
		dst[i] = create_substr(s, c, &pos);
		if (dst[i] == NULL)
			return (free_array(dst, i));
		if ((pos != NULL) && (*pos != '\0'))
			s = pos + 1;
		else
			s += ft_strlen(s);
		i++;
	}
	return (dst);
}
