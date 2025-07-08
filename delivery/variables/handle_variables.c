/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:17:55 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/08 18:06:31 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"
#include "../minishell.h"
#include "../parser/parser.h"

static char *value_utils(char *value, char *var_name)
{
	char	*var_value;

	if (value != NULL)
	{
		var_value = ft_strdup(value);
		if (var_value == NULL)
		{
			free(var_name);
			return (NULL);
		}
	}
	else
		var_value = NULL;
	return (var_value);
}

t_list	*create_var_node(char *name, char *value, bool is_exported)
{
	char			*var_name;
	char			*var_value;
	t_content_var	*content;

	var_name = ft_strdup(name);
	if (var_name == NULL)
		return (NULL);
	var_value = value_utils(value, var_name);
	content = malloc(sizeof(t_content_var));
	if (content == NULL)
	{
		free(var_name);
		free(var_value);
		return (NULL);
	}
	content->var_name = var_name;
	content->var_value = var_value;
	content->is_exported = is_exported;
	return (ft_lstnew(content));
}

bool	is_strlen_equals(char *text_1, char *text_2)
{
	int	len_1;
	int	len_2;

	len_1 = ft_strlen(text_1);
	len_2 = ft_strlen(text_2);
	if (len_1 == len_2)
		return (true);
	return (false);
}

// Checks if a variable already exists:
//  - If it exists: replaces its value and returns true
//  - If it doesn't exist: returns false
bool	check_and_replace_var(t_list *current, char *name, char *value)
{
	t_content_var	*current_content;

	while (current != NULL)
	{
		current_content = (t_content_var *)current->content;
		if (is_strlen_equals(current_content->var_name, name))
		{
			if (ft_strncmp(current_content->var_name, name,
					ft_strlen(name)) == 0)
			{
				free(current_content->var_value);
				current_content->var_value = ft_strdup(value);
				if (current_content->var_value == NULL)
					return (false);
				return (true);
			}
		}
		current = current->next;
	}
	return (false);
}

int	process_var_assign(t_list *name, t_list *op, t_list *value, t_shell *sh)
{
	t_list			*current;
	t_list			*new_node;

	if ((name == NULL) || (op == NULL) || (value == NULL))
		return (1);
	current = sh->tmp_var_list;
	if (check_and_replace_var(current, (char *)(name->content),
		(char *)(value->content)) == true)
		return (0);
	new_node = create_var_node((char *)(name->content),
			(char *)(value->content), false);
	if (new_node == NULL)
		return (1);
	ft_lstadd_back(&sh->tmp_var_list, new_node);
	return (0);
}
