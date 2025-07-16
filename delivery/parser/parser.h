/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:14:02 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/14 11:39:43 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../btree/btree.h"
# include "../libft/libft.h"
# include "../minishell.h"

t_list	*create_token(const char *line, int len);
int		create_tree(t_shell *shell, t_btnode **tree, t_list **token_list,
			t_btnode *parent);

t_list	*tokenization(char *line);

void	free_var_content(void *var_content);
void	expand_wildcards_token(t_list *token);
void	expand_variable_string(char **cont, t_list *var_list, t_shell *shell);
void	search_and_expand(t_list **token_list, t_list *var_list,
			t_shell *shell);
void	remove_empty_token(t_list **token_list);

char	*go_next_char(char *line, char c);

int		is_op(char *line);
int		is_token_operator(char *token);

bool	clean_token_quotes(t_list *token_list);
bool	clean_string_quotes(char **content_ptr);

#endif
