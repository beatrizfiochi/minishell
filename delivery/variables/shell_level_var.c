/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:45:35 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/27 20:00:05 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../parser/parser.h"
#include "variables.h"

// Search for the var_name and returns the var_value
//  if don't find, return NULL
static char	*find_var_value(char *var_name, t_list *var_list)
{
	t_content_var	*var;

	while (var_list != NULL)
	{
		var = (t_content_var *)var_list->content;
		if (ft_strncmp(var_name, var->var_name,
				ft_strlen(var->var_name) + 1) == 0)
			return (var->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}

// Update the SHLVL variable -> var_value
void	update_shell_level(t_list *var_list)
{
	char	*shlvl_value;
	char	*new_value;
	int		shlvl;

	shlvl_value = find_var_value("SHLVL", var_list);
	if (shlvl_value == NULL)
		return ;
	shlvl = (ft_atoi(shlvl_value) + 1);
	new_value = ft_itoa(shlvl);
	if (new_value == NULL)
		return ;
	check_and_replace_var(var_list, "SHLVL", new_value);
	free(new_value);
}
