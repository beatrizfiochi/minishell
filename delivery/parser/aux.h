/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <djunho@student.42porto.com>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:10:01 by djunho            #+#    #+#             */
/*   Updated: 2025/06/13 16:41:38 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AUX_H
# define AUX_H

# include "../libft/libft.h"
# include "../btree/btree.h"
# include "../cmd.h"

// Find the previous item of the current_node
t_list		*prev_list_item(t_list *first_node, t_list *current_node);

struct s_split_token_list
{
	t_list	*left;
	t_list	*op;
	t_list	*right;
	t_list	*remain;
};

bool		split_token_list(struct s_split_token_list *split, bool need_left,
				bool full_expand);

t_list		*search_op(t_list *tokens, bool full_expand);

void		free_btree_content(void *_content);

void		*abort_tree_lst(t_btnode *tree, t_list **token_list,
				const char *msg);

t_node_op	op(char *token_str);

#endif // AUX_H
