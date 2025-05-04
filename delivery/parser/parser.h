/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:14:02 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/04 16:12:06 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define	PARSER_H

# include "../btree/btree.h"
# include "../libft/libft.h"
# include "../cmd.h"

t_btnode	*create_node(t_list *token_list);
t_btnode	*create_tree(t_list *token_list);

t_node_op	op(char *token_str);

t_list		*search_op(t_list *tokens);
t_list		*tokenization(char *line);

char		*go_next_char(char *line, char c);

#endif
