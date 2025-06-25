/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:51:22 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/23 13:57:44 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>           //write
#include "libft/libft.h"

void	printf_error(const char *error)
{
	int	len;

	if (error == NULL)
		return ;
	len = ft_strlen(error);
	write(2, error, len);
}

char	*go_next_char(char *line, char c)
{
	while (*line != c && *line != '\0')
		line++;
	return (line);
}
