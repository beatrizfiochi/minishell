/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:16:32 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/05 16:02:40 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLES_H
# define VARIABLES_H

# include "../libft/libft.h"
# include "../minishell.h"

t_list	*create_var_node(char *name, char *value, bool is_exported);
t_list	*envp_list(t_shell *shell, char *envp[]);

int		process_var_assign(t_list *name, t_list *op, t_list *value,
			t_shell *sh);

bool	check_and_replace_var(t_list *current, char *name, char *value);

void	update_shell_level(t_list *var_list);

#endif
