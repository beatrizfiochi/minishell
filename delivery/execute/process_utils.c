/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 16:13:37 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/12 11:57:42 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"
#include "../parser/parser.h"
#include "../parser/aux.h"
#include "../variables/variables.h"

// This function processes variable assignment commands into the
//  shell->tmp_var_list.
// It assumes that the var name is well formed
int	handle_var_assign(t_shell *shell, t_btnode *node)
{
	t_content_node	*content;
	t_list			*var;
	enum e_var_exit	var_status;

	content = (t_content_node *)node->content;
	if (content->cmd.tokens == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error: Invalid variable assignment\n");
		return (EXIT_FAILURE);
	}
	while (content->cmd.tokens != NULL)
	{
		var = content->cmd.tokens;
		var_status = handle_var(&shell->tmp_var_list,
				(char *)var->content, false);
		if ((var_status != VAR_STATUS_SUCCESS_CREATED)
			&& (var_status != VAR_STATUS_SUCCESS_UPDATED))
			break ;
		content->cmd.tokens = var->next;
		ft_lstdelone(var, free);
	}
	return (EXIT_SUCCESS);
}

void	join_shell_variable_lists(t_shell *shell)
{
	t_list			*tmp_node;
	t_list			*new_node;
	t_content_var	*tmp_content;

	tmp_node = shell->tmp_var_list;
	while (tmp_node != NULL)
	{
		tmp_content = (t_content_var *)tmp_node->content;
		if (check_and_replace_var(shell->variable_list,
				tmp_content->var_name, tmp_content->var_value, NULL) == false)
		{
			new_node = create_var_node(tmp_content->var_name,
					tmp_content->var_value, tmp_content->is_exported);
			if (new_node == NULL)
				return ;
			ft_lstadd_back(&shell->variable_list, new_node);
		}
		tmp_node = tmp_node->next;
	}
	ft_lstclear(&shell->tmp_var_list, free_var_content);
	shell->tmp_var_list = NULL;
}
