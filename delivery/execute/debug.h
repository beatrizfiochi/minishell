/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:18:43 by djunho            #+#    #+#             */
/*   Updated: 2025/06/01 14:20:00 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "../btree/btree.h"
# include "../minishell.h"

void	debug_btree_print(t_btnode *node);
void	debug_execution_pos(t_shell *shell);

#endif // DEBUG_H
