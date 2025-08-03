/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:22:37 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 15:05:41 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdio.h>
#include "../libft/libft.h"
#include "cmd_bonus.h"
#include "variables/variables_bonus.h"

#ifdef TEST

void	debug_print_env_var(t_shell *shell)
{
	t_list	*variable_iter;

	variable_iter = shell->variable_list;
	while (variable_iter != NULL)
	{
		printf("var_name = %s, var_value = %s\n",
			((t_content_var *)(variable_iter->content))->var_name,
			((t_content_var *)(variable_iter->content))->var_value);
		variable_iter = variable_iter->next;
	}
	variable_iter = shell->tmp_var_list;
	while (variable_iter != NULL)
	{
		printf("var_name = %s, var_value = %s\n",
			((t_content_var *)(variable_iter->content))->var_name,
			((t_content_var *)(variable_iter->content))->var_value);
		variable_iter = variable_iter->next;
	}
}

#else

void	debug_print_env_var(t_shell *shell)
{
	(void)shell;
}

#endif
