/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 21:16:32 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/09 22:13:41 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLES_H
# define VARIABLES_H

# include "../libft/libft.h"
# include "../minishell.h"

typedef struct s_content_var
{
	char		*var_name;
	char		*var_value;
	bool		is_exported;	// Indicates if the variable is exported
}	t_content_var;

t_list	*create_var_node(char *name, char *value, bool is_exported);
t_list	*envp_list(t_shell *shell, char *envp[]);

int		process_var_assign(t_list *name, t_list *op, t_list *value,
			t_shell *sh);
int		is_valid_name(char *name, int len);

enum e_var_exit
{
	VAR_STATUS_GENERIC_ERROR,
	VAR_STATUS_NAME_INVALID,
	VAR_STATUS_EQUAL_NOT_FOUND,
	VAR_STATUS_SUCCESS_UPDATED,
	VAR_STATUS_SUCCESS_CREATED,
};
enum e_var_exit		handle_var(t_list **var_list, char *var, bool is_exported);

bool	is_strlen_equals(char *text_1, char *text_2);
bool	check_and_replace_var(t_list *current, char *name, char *value);

void	update_shell_level(t_list *var_list);

#endif
