/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:59:36 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/31 21:18:25 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../../libft/libft.h"
#include "../../minishell.h"
#include "../../variables/variables.h"

static void	check_list_and_print(t_list *var_list)
{
	t_content_var	*content;

	content = (t_content_var *)var_list->content;
	ft_printf("%s=%s\n", content->var_name, content->var_value);
}

static int	env_error(void)
{
	ft_fprintf(STDERR_FILENO, "env: Arguments and options not allowed\n");
	return (EXIT_INCORRECT_USAGE);
}

// env built-in without options and arguments
//  this function works similar to printenv
//  this will print all exported variables and yours environment variables
int	env(int argc, char *argv[], t_shell *shell)
{
	t_list			*tmp_var_list;
	t_list			*var_list;
	t_content_var	*var_content;

	(void)argv;
	if (argc != 1)
		return (env_error());
	tmp_var_list = shell->tmp_var_list;
	var_list = shell->variable_list;
	while (tmp_var_list != NULL)
	{
		check_list_and_print(tmp_var_list);
		tmp_var_list = tmp_var_list->next;
	}
	while (var_list != NULL)
	{
		var_content = (t_content_var *)var_list->content;
		if (var_content->is_exported == true && var_content->var_value != NULL)
			ft_printf("%s=%s\n", var_content->var_name, var_content->var_value);
		var_list = var_list->next;
	}
	return (0);
}
