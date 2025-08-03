/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:34:16 by djunho            #+#    #+#             */
/*   Updated: 2025/08/03 12:51:54 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "../libft/libft.h"
# include "btree/btree.h"
# include "cmd.h"
# include "minishell.h"

void		debug_print_tree(t_btnode *tree);
void		debug_print_read_command(t_list *node, char *line);
const char	*op_str(t_node_op op);
void		debug_env_var_init(t_shell *shell);
void		debug_print_env_var(t_shell *shell);

#endif // DEBUG_H
