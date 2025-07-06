/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 10:59:36 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/06 12:07:02 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft/libft.h"
#include "../../minishell.h"
#include "../../parser/parser.h"

// env built-in without options and arguments
//  this function works similar to printenv
//  this will print all exported variables
int	env(int argc, char *argv[], t_shell *shell)
{
	t_list			*tmp_var_list;
	t_list			*var_list;
	t_content_var	*tmp_content;
	t_content_var	*var_content;

	(void)argc;
	(void)argv;
	tmp_var_list = shell->tmp_var_list;
	var_list = shell->variable_list;
	while(tmp_var_list != NULL)
	{
		tmp_content = (t_content_var *)tmp_var_list->content;
		ft_printf("%s=%s\n", tmp_content->var_name, tmp_content->var_value);
		tmp_var_list = tmp_var_list->next;
	}
	while(var_list != NULL)
	{
		var_content = (t_content_var *)var_list->content;
		if(var_content->is_exported == true)
			ft_printf("%s=%s\n", var_content->var_name, var_content->var_value);
		var_list = var_list->next;
	}
	return (0);
}
