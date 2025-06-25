/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:10:01 by djunho            #+#    #+#             */
/*   Updated: 2025/06/25 16:12:39 by bfiochi-         ###   ########.fr       */
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
bool		is_quote(char c);

t_list		*search_op(t_list *tokens, bool full_expand);

void		free_btree_content(void *_content);

void		*abort_tree_lst(t_btnode *tree, t_list **token_list,
				const char *msg);

t_node_op	op(char *token_str);

char		*go_to_end_quote(char *content);

#endif // AUX_H
