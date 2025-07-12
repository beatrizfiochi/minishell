/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:45:39 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/12 11:23:20 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../libft/libft.h"
#include "variables.h"
#include "../parser/parser.h"

bool	is_valid_name(char *name, int len)
{
	int	i;

	i = 0;
	if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z')
		|| (name[i] == '_'))
	{
		i++;
		while (i < len)
		{
			if ((ft_isalnum(name[i]) == true) || name[i] == '_')
				i++;
			else
				return (false);
		}
		return (true);
	}
	else
		return (false);
}

bool	name_and_value_split(char *str, char **name, char **value)
{
	*name = str;
	*value = NULL;
	while ((*str != '\0') && (*str != '='))
		str++;
	if (is_valid_name(*name, (str - *name)) == false)
		return (false);
	if (*str == '=')
	{
		*value = (str + 1);
		*str = '\0';
	}
	return (true);
}

enum e_var_exit	handle_var(t_list **var_list, char *var, bool is_exported)
{
	t_list		*new_node;
	char		*var_value;

	if (name_and_value_split(var, &var, &var_value) == false)
		return (VAR_STATUS_NAME_INVALID);
	if (var_value == NULL)
		return (VAR_STATUS_EQUAL_NOT_FOUND);
	if (check_and_replace_var(*var_list, var, var_value) == true)
		return (VAR_STATUS_SUCCESS_UPDATED);
	new_node = create_var_node(var, var_value, is_exported);
	if (new_node == NULL)
		return (VAR_STATUS_GENERIC_ERROR);
	ft_lstadd_back(var_list, new_node);
	return (VAR_STATUS_SUCCESS_CREATED);
}
