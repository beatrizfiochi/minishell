/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:49:18 by djunho            #+#    #+#             */
/*   Updated: 2025/07/06 16:17:24 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdbool.h>
# include "../libft/libft.h"
# include "../btree/btree.h"
# include "../minishell.h"
# include "../cmd.h"

int		process(t_shell *shell);
int		execute(t_shell *shell);
int		execute_execve(t_btnode *node, t_shell *shell);

int		get_fork_return(int wstatus);
int		run_child(t_cmd *cmd, t_shell *shell);
int		process_and(t_shell *shell, int previous_ret, bool *should_continue);
int		process_or(t_shell *shell, int previous_ret, bool *should_continue);
int		process_pipe(t_shell *shell, t_btnode *node);

void	configure_pipe(int pipe[2], int carry_over_fd, bool is_last_pipe);
void	create_op_list(t_btnode *tree, t_list **op_list);

int		handle_var_assign(t_shell *shell, t_btnode *node);
void	join_shell_variable_lists(t_shell *shell);

#endif // EXECUTION_H
