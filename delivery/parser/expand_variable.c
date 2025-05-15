/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/15 14:28:19 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "parser.h"
#include "expand_variables.h"

t_list	*create_var_node(char *name, char *value)
{
	char			*var_name;
	char			*var_value;
	t_content_var	*content;

	var_name = ft_strdup(name);
	var_value = ft_strdup(value);
	if (var_name == NULL)
		return (NULL);
	if (var_value == NULL)
	{
		free(var_name);
		return (NULL);
	}
	content = malloc(sizeof(t_content_var));
	if (content == NULL)
	{
		free(var_name);
		free(var_value);
		return (NULL);
	}
	content->var_name = var_name;
	content->var_value = var_value;
	return (ft_lstnew(content));
}

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

void	search_and_expand(t_list *token_list, t_list *var_list)
{
	while (token_list != NULL)
	{
		search_and_expand_content((char **)(&(token_list->content)), var_list);
		token_list = token_list->next;
	}
}
