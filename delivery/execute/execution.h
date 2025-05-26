/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 19:49:18 by djunho            #+#    #+#             */
/*   Updated: 2025/05/26 12:01:11 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include <stdbool.h>
# include "../libft/libft.h"
# include "../btree/btree.h"
# include "../minishell.h"

void	free_join(char **args);
bool	create_cmd_path(char *cmd, char **envp, char **path);
int		process(t_shell *shell);
int		execute(t_btnode *tree, t_shell *shell, char *envp[]);
char	**convert_list_to_vector(t_list *list);
void	create_op_list(t_btnode *tree, t_list **op_list);

int		run_child(t_cmd *cmd, t_shell *shell);
int		process_and(t_shell *shell);
int		process_or(t_shell *shell);
int		process_pipe(t_btnode *node);
void	configure_pipe(int pipe[2], int carry_over_fd, bool is_last_pipe);

# ifndef TEST

void	debug_btree_print(t_btnode *node);
# endif

#endif // EXECUTION_H
