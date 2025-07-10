/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:43:42 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/10 12:12:52 by bfiochi-         ###   ########.fr       */
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
			return (EXIT_FAILURE);
		ft_lstadd_back(&shell->variable_list, new_node);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static bool	handle_var_status(t_shell *sh, char *arg, enum e_var_exit status)
{
	bool	error;

	error = false;
	if (status == VAR_STATUS_SUCCESS_UPDATED)
		export_existent_variable(sh, arg);
	else if ((status == VAR_STATUS_EQUAL_NOT_FOUND)
		&& (export_existent_variable(sh, arg) == EXIT_FAILURE)
		&& (create_empty_variable_node(sh, arg) == EXIT_FAILURE))
		error = true;
	else if (status == VAR_STATUS_NAME_INVALID)
		error = true;
	return (error);
}

int	export(int argc, char *argv[], t_shell *shell)
{
	int				i;
	int				ret;
	enum e_var_exit	var_status;
	bool			error;

	i = 1;
	if (i == argc)
		print_export(shell);
	ret = EXIT_SUCCESS;
	while (i < argc)
	{
		error = false;
		var_status = handle_var(&shell->variable_list, argv[i], true);
		error = handle_var_status(shell, argv[i], var_status);
		if (error)
		{
			ret = EXIT_FAILURE;
			ft_fprintf(STDERR_FILENO,
				"Minishell: export: `%s': not a valid identifier\n", argv[i]);
		}
		i++;
	}
	return (ret);
}
