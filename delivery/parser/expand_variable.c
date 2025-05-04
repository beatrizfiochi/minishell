/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/04 18:58:30 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdbool.h>              //bool

bool	is_metachar(char *content)
{
	if (content == NULL)
		return (false);
	if ((*content == '$' && *(content + 1) != ' ') && *(content + 1) != '\0'
		&& *(content + 1) != '&' && *(content + 1) != '|')
		return (true);
	return (false);
}

int	search_and_expand(t_list	*token_list)
{
	char	*content;

	while (token_list != NULL)
	{
		content = (char *)(token_list->content);
		if (is_metachar(content) == true)
		{
			if (search_variable(char *variable, char *value) == true)   //TO DO
			{
				SUBSTITUIR O CONTEUDO DESSE NO "$variable" por *value e dou free no content anterior
				retorno sucesso
			}
		}
		token_list = token_list->next;
	}
}
