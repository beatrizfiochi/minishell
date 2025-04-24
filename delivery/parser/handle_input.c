/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:01:04 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/24 15:45:51 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../libft/libft.h"
#include <stdio.h>            //printf
#include <stdbool.h>          //booleans

char	*clean_string(char *str)    //APAGAR
{
	char *clean_input;

	if (str == NULL)
		return (NULL);
	clean_input = ft_strtrim(str, " ");
	printf("%s.\n", clean_input);
	return (clean_input);
}

