/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 15:06:35 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 15:08:09 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include "parser/parser_bonus.h"

void	clear_minishell(t_shell *shell)
{
	btree_clear(&shell->cmds, free_btree_node);
	if (shell->variable_list != NULL)
		ft_lstclear(&shell->variable_list, free_var_content);
	if (shell->tmp_var_list != NULL)
		ft_lstclear(&shell->tmp_var_list, free_var_content);
}

void	reset_minishell(t_shell *shell, int ret)
{
	shell->last_exit_status = ret;
	shell->is_running_redirect = false;
	shell->is_last_redirect = false;
	shell->pipe.carry_over_fd = -1;
	shell->pipe.pipe[0] = -1;
	shell->pipe.pipe[1] = -1;
	shell->last_pid = -1;
}
