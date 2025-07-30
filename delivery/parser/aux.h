/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:10:01 by djunho            #+#    #+#             */
/*   Updated: 2025/07/30 22:27:10 by bfiochi-         ###   ########.fr       */
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
bool		is_basic_op(t_node_op op);

t_list		*search_any_op(t_list *tokens);
t_list		*search_op(t_list *tokens, enum e_expand_type expand_type);
t_list		*exit_tokenization(t_list *prev_token, t_list *head_token);

void		free_btree_content(void *_content);
void		search_token(char *line, int *len);

t_node_op	op_list(t_list *list);
t_node_op	op(char *token_str);

char		*go_to_end_quote(char *content);

int			check_token(t_list *prev, t_list *curr);
int			check_special_with_reddir(t_list *token);
int			process_tokens(char *line, t_list **head, t_list **prev);
int			check_parentheses_syntax(t_list *token);

#endif // AUX_H
