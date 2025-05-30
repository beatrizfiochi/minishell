/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:17:55 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/29 21:18:21 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "parser.h"

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
