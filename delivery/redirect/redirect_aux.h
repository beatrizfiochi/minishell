/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:30:30 by djunho            #+#    #+#             */
/*   Updated: 2025/07/20 22:56:40 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_AUX_H
# define REDIRECT_AUX_H

# include "../btree/btree.h"
# include "../cmd.h"
# include "../minishell.h"

char			*get_redir_filename(t_shell *shell, t_btnode *file_node);
t_node_op		get_next_operation(t_btnode *node);
bool			is_a_redirect_file_op(t_node_op op);
t_content_node	*get_next_cmd(t_btnode *node);
t_content_node	*get_first_cmd(t_btnode *node);

#endif // REDIRECT_AUX_H
