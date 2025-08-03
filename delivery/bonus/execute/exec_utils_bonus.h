/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:08:47 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 18:13:53 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "../cmd_bonus.h"
# include "../btree/btree_bonus.h"
# include "../minishell_bonus.h"

void			free_join(char **args);
bool			create_cmd_path(char *cmd, t_list *var_list, char **path);
char			**convert_list_to_vector(t_list *list, int *size);
t_content_node	*get_last_command_run(t_btnode *parent);

void			close_any_possible_fd(t_shell *shell);
void			close_all_pipes(t_btnode *node);
int				wait_previous_process(t_shell *shell);
int				exec_cmd(t_shell *shell, char **args, char **envp);

#endif // EXEC_UTILS_H
