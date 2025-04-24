/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:51:22 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/22 16:46:47 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>           //write
#include <stdlib.h>           //malloc

void	printf_error(const char *error)
{
	int	len;

	if (error == NULL)
		return ;
	len = ft_strlen(error);
	write(2, error, len);
}
