/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:10:01 by djunho            #+#    #+#             */
/*   Updated: 2025/07/29 23:19:31 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUX_H
# define AUX_H

# include "../libft/libft.h"
# include "../btree/btree.h"
# include "../cmd.h"

// Find the previous item of the current_node
t_list		*prev_list_item(t_list *first_node, t_list *current_node);

enum e_expand_type
{
	EXP_NORMAL = 0,
	EXP_PIPE,
	EXP_REDIR,
	EXP_PAREN,
	EXP_ASSIGN
};

bool		is_quote(char c);
bool		is_basic_op_list(t_list *list);

t_list		*search_any_op(t_list *tokens);
t_list		*search_op(t_list *tokens, enum e_expand_type expand_type);

void		free_btree_content(void *_content);

t_node_op	op_list(t_list *list);
t_node_op	op(char *token_str);

char		*go_to_end_quote(char *content);

int			check_token(t_list *prev, t_list *curr);

#endif // AUX_H
