/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:30:30 by djunho            #+#    #+#             */
/*   Updated: 2025/07/31 21:14:28 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_AUX_BONUS_H
# define REDIRECT_AUX_BONUS_H

# include "../btree/btree_bonus.h"
# include "../cmd_bonus.h"
# include "../minishell_bonus.h"

char			*get_redir_filename(t_shell *shell, t_btnode *file_node);
t_node_op		get_next_operation(t_btnode *node);
bool			is_redirect_file_op(t_node_op op);
t_content_node	*get_next_cmd(t_btnode *node);
t_content_node	*get_first_cmd(t_btnode *node);
bool			is_cmd_branch(t_btnode *node);

#endif // REDIRECT_AUX_H
