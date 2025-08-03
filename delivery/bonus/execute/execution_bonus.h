/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:49:18 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 18:48:06 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_BONUS_H
# define EXECUTION_BONUS_H

# include <stdbool.h>
# include "../../libft/libft.h"
# include "../btree/btree_bonus.h"
# include "../minishell_bonus.h"
# include "../cmd_bonus.h"

int		process(t_shell *shell);
int		execute(t_shell *shell);
int		run_cmd(t_shell *shell, t_btnode *node, t_node_op parent_op);

int		get_fork_return(int wstatus);
int		run_child(t_cmd *cmd, t_shell *shell);
int		process_and(t_shell *shell, int previous_ret, bool *should_continue);
int		process_or(t_shell *shell, int previous_ret, bool *should_continue);

void	configure_redir(t_shell *shell, const t_cmd *cmd);
void	create_op_list(t_btnode *tree, t_list **op_list);

void	close_possible_pipe(t_shell *shell);
int		handle_var_assign(t_shell *shell, t_btnode *node);
void	join_shell_variable_lists(t_shell *shell);

bool	is_op_redirect_type(t_node_op op);
bool	is_pipe(t_btnode *node);

int		prepare_parenthesis(t_shell *shell, t_btnode *node,
			bool *should_continue);
int		process_parenthesis(t_shell *shell, t_btnode *node, int ret,
			bool *should_continue);

#endif // EXECUTION_H
