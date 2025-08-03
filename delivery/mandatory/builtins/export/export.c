/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 14:43:42 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/20 19:59:39 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../parser/parser.h"
#include "../../libft/libft.h"
#include "../../execute/execution.h"
#include "../../variables/variables.h"
#include "../../variables/expand_variables.h"
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

static void	handle_export_var(int *ret, char *str, t_shell *shell)
{
	char	*name;
	char	*value;

	if (name_and_value_split(str, &name, &value) == false)
	{
		*ret = EXIT_FAILURE;
		ft_fprintf(STDERR_FILENO,
			"export: `%s': not a valid identifier\n", str);
		return ;
	}
	if (value == NULL)
		value = search_var(name, shell->tmp_var_list, ft_strlen(name));
	if (value == NULL)
		value = search_var(name, shell->variable_list, ft_strlen(name));
	if (handle_var_name_and_value(&shell->variable_list, name, value, true)
		== VAR_STATUS_GENERIC_ERROR)
		*ret = EXIT_FAILURE;
}

int	export(int argc, char *argv[], t_shell *shell)
{
	int	ret;
	int	i;

	if (1 == argc)
		print_export(shell);
	i = 0;
	ret = EXIT_SUCCESS;
	while (++i < argc)
		handle_export_var(&ret, argv[i], shell);
	return (ret);
}
