/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:14:02 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/06 17:29:55 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../btree/btree.h"
# include "../libft/libft.h"

typedef struct s_content_var
{
	char		*var_name;
	char		*var_value;
}	t_content_var;

t_btnode	*create_tree(t_list *token_list);

t_list		*tokenization(char *line);
t_list		*create_var_node(char *name, char *value);

char		*go_next_char(char *line, char c);

int			search_and_expand(t_list *token_list, t_list *var_list);


#endif
