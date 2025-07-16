/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:26:44 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/24 16:09:43 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARIABLES_H
# define EXPAND_VARIABLES_H

# include "../libft/libft.h"
# include "../minishell.h"

char	*search_var(const char *variable, t_list *var_list, int len);
char	*remove_var_name(char *content, int var_pos, char **cnt);
char	*replace_var_name_by_value(char *content, int var_pos,
			char **cnt, char *var_value);
char	*expand_var(char *content, char *var_pos, char **cnt, char *var_value);

#endif
