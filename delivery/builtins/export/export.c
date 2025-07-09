/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:43:42 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/09 20:14:18 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../parser/parser.h"
#include "../../libft/libft.h"
#include "../../execute/execution.h"
#include "../../variables/variables.h"
#include <unistd.h>

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
				return (EXIT_SUCCESS);
			}
		}
		tmp_list = tmp_list->next;
	}
	return (EXIT_FAILURE);
}

static int	create_empty_variable_node(t_shell *shell, char *var)
{
	t_list	*new_node;

	if (is_valid_name(var, ft_strlen(var)) == 0)
	{
		new_node = create_var_node(var, NULL, true);
		if (new_node == NULL)
			return (1);
		ft_lstadd_back(&shell->variable_list, new_node);
	}
	else
	{
		ft_fprintf(STDERR_FILENO, "Minishell: export: `%s': not a valid identifier\n", var);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	assign_or_change_value_variable(t_shell *shell, char *var)
{
	t_list			*new_node;
	char			*var_value;
	int				i;

	var_value = NULL;
	i = 0;
	while (var[i] != '=')
		i++;
	var[i] = '\0';
	var_value = &var[i + 1];
	if (check_and_replace_var(shell->variable_list, var,
		var_value) == true)
		return (EXIT_SUCCESS);
	else if(is_valid_name(var, ft_strlen(var)) == 0)
	{
		if (is_valid_name(var, ft_strlen(var)) == 0)
		{
			new_node = create_var_node(var, var_value, true);
			if (new_node == NULL)
				return (1);
			ft_lstadd_back(&shell->variable_list, new_node);
		}
		else
		{
			ft_fprintf(STDERR_FILENO, "Minishell: export: `%s=%s': not a valid identifier\n", var, var_value);
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

int	export(int argc, char *argv[], t_shell *shell)
{
	int		i;
	int		j;
	int		ret;

	i = 1;
	j = 0;
	ret = EXIT_SUCCESS;
	if (i == argc)
		print_export(shell);
	else if (i < argc)
	{
		while (i < argc)
		{
			while (argv[i][j] != '\0')
			{
				if (argv[i][j] == '=')
				{
					if (assign_or_change_value_variable(shell, argv[i]) == EXIT_FAILURE)
						ret = (EXIT_FAILURE);
					break;
				}
				j++;
			}
			if (export_existent_variable(shell, argv[i]) == EXIT_FAILURE)
				if (create_empty_variable_node(shell, argv[i]) == EXIT_FAILURE)
					ret = EXIT_FAILURE;
			i++;
		}
	}
	return (ret);
}
