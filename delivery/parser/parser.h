/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:14:02 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/24 15:33:13 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../btree/btree.h"
# include "../libft/libft.h"
# include "../minishell.h"

typedef struct s_content_var
{
	char		*var_name;
	char		*var_value;
}	t_content_var;

t_btnode	*create_tree(t_list **token_list, t_btnode *parent);

t_list		*tokenization(char *line);

void		free_var_content(void *var_content);
void		search_and_expand(t_list *token_list,
				t_list *var_list, t_shell *shell);

char		*go_next_char(char *line, char c);

int			is_op(char *line);
int			is_valid_name(char *name, int len);
int			is_token_operator(char *token);

bool		clean_token_quotes(t_list *token_list);

#endif
