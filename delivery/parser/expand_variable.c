/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/24 15:28:15 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expand_variables.h"
#include "../minishell.h"

void	free_var_content(void *var_content)
{
	t_content_var	*content;

	if (var_content == NULL)
		return ;
	content = (t_content_var *)var_content;
	free(content->var_name);
	free(content->var_value);
	free(content);
}

void	search_and_expand(t_list *token_list, t_list *var_list, t_shell *shell)
{
	while (token_list != NULL)
	{
		search_and_expand_content((char **)(&(token_list->content)),
				var_list, shell);
		token_list = token_list->next;
	}
}
