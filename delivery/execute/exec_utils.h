/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:08:47 by djunho            #+#    #+#             */
/*   Updated: 2025/06/25 17:29:22 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_UTILS_H
# define EXEC_UTILS_H

# include "../cmd.h"
# include "../btree/btree.h"

void			free_join(char **args);
bool			create_cmd_path(char *cmd, char **envp, char **path);
char			**convert_list_to_vector(t_list *list, int *size);
t_content_node	*get_last_command_run(t_btnode *parent);

#endif // EXEC_UTILS_H
