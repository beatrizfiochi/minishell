/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:45:39 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/09 22:52:10 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../libft/libft.h"
#include "variables.h"
#include "../parser/parser.h"

int	is_valid_name(char *name, int len)
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
				return (-1);
		}
		return (0);
	}
	else
		return (-1);
}

enum e_var_exit	handle_var(t_list **var_list, char *var, bool is_exported)
{
	t_list		*new_node;
	char		*var_value;

	var_value = var;
	while ((*var_value != '\0') && (*var_value != '='))
		var_value++;
	if (*var_value == '\0')
		return (VAR_STATUS_EQUAL_NOT_FOUND);
	if (is_valid_name(var, var_value - var) != 0)
		return (VAR_STATUS_NAME_INVALID);
	*var_value = '\0';
	var_value++;
	if (check_and_replace_var(*var_list, var, var_value) == true)
		return (VAR_STATUS_SUCCESS_UPDATED);
	new_node = create_var_node(var, var_value, is_exported);
	if (new_node == NULL)
		return (VAR_STATUS_GENERIC_ERROR);
	ft_lstadd_back(var_list, new_node);
	return (VAR_STATUS_SUCCESS_CREATED);
}
