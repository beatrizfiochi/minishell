/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:07:52 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/05 15:47:31 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "variables.h"
#include "../parser/parser.h"
#include "../cmd.h"
#include "../libft/libft.h"

static void	free_name_value(char *name, char *value)
{
	if (name != NULL)
		free(name);
	if (value != NULL)
		free(value);
}

// Parse env in name_variable and value_variable
//   before '=' -> name_variable
//    after '=' --> value_variable
static bool	parse_env(char *env, char **name, char **value)
{
	char	*sep;
	int		name_len;

	sep = ft_strchr(env, '=');
	if (sep == NULL)
		return (false);
	name_len = sep - env;
	*name = ft_substr(env, 0, name_len);
	*value = ft_strdup(sep + 1);
	if (*name == NULL || *value == NULL)
	{
		free_name_value(*name, *value);
		return (false);
	}
	return (true);
}

// Checks if the variable exists in the list
//  if it exists, replaces its value, frees name and/or value, and returns true
//  if not found, returns false (caller is responsible for managing memory)
static bool	handle_existing_var(t_shell *shell, char *name, char *value)
{
	if (check_and_replace_var(shell->variable_list, name, value) == true)
	{
		free_name_value(name, value);
		return (true);
	}
	return (false);
}

// Creates a variable node and adds it to the list
// Frees name and value if allocation fails. Returns true on success
static bool	create_and_add_var(t_shell *shell, char *name, char *value)
{
	t_list	*new_node;

	new_node = create_var_node(name, value, true);
	if (new_node == NULL)
		return (false);
	ft_lstadd_back(&shell->variable_list, new_node);
	return (true);
}

t_list	*envp_list(t_shell *shell, char *envp[])
{
	char		*name;
	char		*value;

	while (*envp != NULL)
	{
		if (parse_env(*envp, &name, &value) == false)
		{
			envp++;
			continue ;
		}
		if (handle_existing_var(shell, name, value) == true)
		{
			envp++;
			continue ;
		}
		if (create_and_add_var(shell, name, value) == false)
		{
			ft_lstclear(&shell->variable_list, free_var_content);
			return (NULL);
		}
		free_name_value(name, value);
		envp++;
	}
	return (shell->variable_list);
}
