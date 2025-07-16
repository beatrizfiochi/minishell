/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/13 18:40:38 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_variables.h"
#include "../minishell.h"
#include "../parser/aux.h"
#include "variables.h"
#include <unistd.h>

void	handle_special_var(char **cont, char **cnt,
		char *var, t_shell *shell)
{
	char	*v_value;

	v_value = ft_itoa(shell->last_exit_status);
	if (v_value == NULL)
		return ;
	*cont = expand_var(*cont, var, cnt, v_value);
	free(v_value);
}

void	handle_normal_var(char **cont, char **cnt,
		char *var, t_list *var_list)
{
	char	*v_value;
	int		var_len;

	var_len = get_variable_name_length(*cnt);
	*cnt = *cnt + var_len;
	v_value = search_var((const char *)(var + 1), var_list, var_len);
	*cont = expand_var(*cont, var, cnt, v_value);
}

t_list	*retokenzine(t_list *curr, char **cnt, char **split, int *i)
{
	t_list	*next;
	char	*aux;

	next = curr->next;
	while (split[*i] != NULL)
	{
		curr->next = ft_lstnew(split[(*i)++]);
		curr = curr->next;
	}
	curr->next = next;
	if ((*i)-- > 1)
	{
		aux = ft_strjoin(split[*i], *cnt);
		**cnt = '\0';
		*cnt = aux + ft_strlen(split[*i]);
		free(curr->content);
		curr->content = aux;
	}
	free(split);
	return (curr);
}

t_list	*handle_normal_var_with_retoken(t_list *curr, char **cnt,
		char *var, t_list *var_list)
{
	char	*v_value;
	int		var_len;
	char	**split;
	int		i;

	var_len = get_variable_name_length(*cnt);
	*cnt = *cnt + var_len;
	v_value = search_var((const char *)(var + 1), var_list, var_len);
	if (v_value == NULL)
	{
		curr->content = expand_var((char *)(curr->content), var, cnt, NULL);
		return (curr);
	}
	split = ft_split(v_value, ' ');
	if (split == NULL)
		ft_fprintf(STDERR_FILENO, "Error on retokinize var expansion\n");
	if (split == NULL)
		return (curr);
	i = 0;
	if (var[0] != ' ')
	{
		curr->content = expand_var((char *)(curr->content), var, cnt, split[i]);
		free(split[i++]);
	}
	return (retokenzine(curr, cnt, split, &i));
}

char	*copy_inside_quotes(char *string)
{
	char	*aux;

	aux = go_to_end_quote(string);
	ft_memmove(aux - 1, aux, ft_strlen(aux) + 1);
	ft_memmove(string, string + 1, ft_strlen(string + 1) + 1);
	string = aux - 2;
	return (string);
}
