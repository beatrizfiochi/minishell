/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:34:16 by djunho            #+#    #+#             */
/*   Updated: 2025/05/05 18:39:00 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef DEBUG_H
# define DEBUG_H

# include "libft/libft.h"
# include "btree/btree.h"

void	debug_print_tree(t_btnode *tree);
void	debug_print_read_command(t_list *node, char *line);

#endif // DEBUG_H
