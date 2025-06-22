/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:18:43 by djunho            #+#    #+#             */
/*   Updated: 2025/06/22 20:12:19 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_DEBUG_H
# define EXECUTE_DEBUG_H

# include "../btree/btree.h"
# include "../minishell.h"

void	debug_btree_print(t_btnode *node);
void	debug_execution_pos(t_shell *shell);

#endif // EXECUTE_DEBUG_H
