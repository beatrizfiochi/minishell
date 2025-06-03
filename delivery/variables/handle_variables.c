/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:17:55 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/02 11:29:13 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../cmd.h"
#include "../minishell.h"

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

static int	check_and_replace_var(t_list *current, t_content_var *existing,
	t_content_node *name, t_content_node *value)
{
	while (current != NULL)
	{
		if (ft_strncmp(existing->var_name,
				(char *)(name->cmd.tokens->content),
			ft_strlen((const char *)(name->cmd.tokens->content))) == 0)
		{
			free(existing->var_value);
			existing->var_value
				= ft_strdup((char *)(value->cmd.tokens->content));
			if (existing->var_value == NULL)
				return (1);
			return (0);
		}
		current = current->next;
	}
	return (0);
}

int	process_var_assign(t_btnode *nd, t_shell *sh)
{
	t_content_node	*name;
	t_content_node	*value;
	t_list			*current;
	t_content_var	*existing;
	t_list			*new_node;

	if ((nd->left == NULL) || (nd->right == NULL))
		return (1);
	name = (t_content_node *)nd->left->content;
	value = (t_content_node *)nd->right->content;
	if ((name == NULL) || (value == NULL))
		return (1);
	current = sh->variable_list;
	if (current != NULL)
	{
		existing = (t_content_var *)current->content;
		check_and_replace_var(current, existing, name, value);
	}
	new_node = create_var_node((char *)(name->cmd.tokens->content),
		(char *)(value->cmd.tokens->content));
	if (new_node == NULL)
		return (1);
	ft_lstadd_back(&sh->variable_list, new_node);
	return (0);
}
