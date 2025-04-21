/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:01:04 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/04/21 17:36:00 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
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

bool	are_closed_single_quotes(char *str)
{
	int	single_quotes;

	single_quotes = 0;
	while (*str != '\0')
	{
		if (*str == '\'')
			single_quotes++;
		str++;
	}
	printf("There are %d single quotes\n", single_quotes);
	if (single_quotes % 2 != 0)
	{
		printf("UNCLOSED SINGLE QUOTES\n");
		return (false);
	}
	printf("CLOSED SINGLE QUOTES\n");
	return (true);
}

bool	are_closed_db_quotes(char *str)
{
	int	db_quotes;

	db_quotes = 0;
	while (*str != '\0')
	{
		if (*str == '"')
			db_quotes++;
		str++;
	}
	printf("There are %d double quotes\n", db_quotes);
	if (db_quotes % 2 != 0)
	{
		printf("UNCLOSED DOUBLE QUOTES\n");
		return (false);
	}
	printf("CLOSED DOUBLE QUOTES\n");
	return (true);
}
