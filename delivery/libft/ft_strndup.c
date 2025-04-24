/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:45:56 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/23 10:51:19 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int len)
{
    char	*dup;

	dup = malloc(len + 1);
    if (dup == NULL)
        return NULL;
    ft_strlcpy(dup, s, len + 1);
    return (dup);
}
