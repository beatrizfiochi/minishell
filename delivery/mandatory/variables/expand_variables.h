/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:26:44 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/27 18:52:36 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARIABLES_H
# define EXPAND_VARIABLES_H

# include "../libft/libft.h"
# include "../minishell.h"

# define QUOTE_MARK  0x1D

char	*search_var(const char *variable, t_list *var_list, int len);
char	*remove_var_name(char *content, int var_pos, char **cnt);
char	*replace_var_name_by_value(char *content, int var_pos,
			char **cnt, char *var_value);
char	*expand_var(char *content, char *var_pos, char **cnt, char *var_value);
char	*handle_possible_var(char **cont, char *cnt, t_list *var_list,
			t_shell *shell);

char	*process_var_expansion(t_list **curr, char *cnt, t_list *var_list,
			t_shell *shell);
// Internals functions
void	handle_special_var(char **cont, char **cnt, char *var, t_shell *shell);
void	handle_normal_var(char **cont, char **cnt, char *var, t_list *var_list);
t_list	*retokenize(t_list *curr, char **cnt, char **split, int *i);
t_list	*handle_normal_var_with_retoken(t_list *curr, char **cnt, char *var,
			t_list *var_list);
char	*mark_quotes(char *string);

#endif
