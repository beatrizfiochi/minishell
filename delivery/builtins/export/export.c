/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:43:42 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/08 18:06:53 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../parser/parser.h"
#include "../../libft/libft.h"
#include "../../execute/execution.h"
#include "../../variables/variables.h"

static void	print_export(t_shell *shell)
{
	t_list			*var_list;
	t_content_var	*var_content;

	var_list = shell->variable_list;
	while (var_list != NULL)
	{
		var_content = (t_content_var *)var_list->content;
		if (var_content->is_exported == true && var_content->var_value != NULL)
			ft_printf("declare -x %s=\"%s\"\n",
					var_content->var_name, var_content->var_value);
		else if (var_content->is_exported == true
					&& var_content->var_value == NULL)
			ft_printf("declare -x %s\n", var_content->var_name);
		var_list = var_list->next;
	}
}

static int	export_existent_variable(t_shell *shell, char *variable)
{
	t_list			*tmp_list;
	t_content_var	*var_content;

	tmp_list = shell->variable_list;
	while (tmp_list != NULL)
	{
		var_content = (t_content_var *)tmp_list->content;
		if (is_strlen_equals(variable, var_content->var_name))
		{
			if (ft_strncmp(var_content->var_name, variable,
			ft_strlen(variable)) == 0)
			{
				var_content->is_exported = true;
				return (0);
			}
		}
		tmp_list = tmp_list->next;
	}
	return (1);
}

int	export(int argc, char *argv[], t_shell *shell)
{
	int		i;
	char	*var;
	t_list	*new_node;

	i = 1;
	if (i == argc)
		print_export(shell);
	else if (i < argc)
	{
		while (i < argc)
		{
			var = malloc(sizeof (char) * (ft_strlen(argv[i])) + 1);
			if (var == NULL)
				return (1);
			ft_strlcpy(var, argv[i], (ft_strlen(argv[i]) + 1));
			if (export_existent_variable(shell, var) == 1)
			{
				new_node = create_var_node(var, NULL, true);
				if (new_node == NULL)
					return (1);
				ft_lstadd_back(&shell->variable_list, new_node);
			}
			i++;
		}
	}
	return (0);
}
