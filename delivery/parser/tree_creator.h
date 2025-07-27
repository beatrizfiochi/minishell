/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_creator.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 19:01:19 by djunho            #+#    #+#             */
/*   Updated: 2025/07/27 12:29:43 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TREE_CREATOR_H
# define TREE_CREATOR_H

# include "../btree/btree.h"     // t_btnode
# include "../parser/aux.h"      // enum e_expand_type *expand_type);
# include "../libft/libft.h"     // t_list

t_btnode	*create_basic_tree(t_list **token_list, t_btnode *parent,
				enum e_expand_type expand_type);

bool		is_btnode_different(t_btnode *node1, t_btnode *node2);
t_btnode	*check_tree_syntax(t_btnode *node);
bool		check_enclosure(t_list **token_list,
				enum e_expand_type *expand_type);

t_btnode	*expand_tree_by_type(t_btnode *tree, enum e_expand_type type,
							bool *expanded);

#endif // TREE_CREATOR_H
