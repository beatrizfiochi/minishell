/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/11 20:43:04 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "variables.h"
#include "expand_variables.h"
#include "../minishell.h"
#include "../parser/aux.h"

static void	handle_special_var(char **cont, char **cnt,
		char *var, t_shell *shell)
{
	char	*v_value;
	int		var_position;

	v_value = ft_itoa(shell->last_exit_status);
	if (v_value == NULL)
		return ;
	var_position = var - (*cont);
	*cont = expand_variable(*cont, var_position, cnt, v_value);
	free(v_value);
}

static void	handle_normal_var(char **cont, char **cnt,
		char *var, t_list *var_list)
{
	char	*v_value;
	int		var_len;
	int		var_position;

	while ((**cnt != '\0') && ((**cnt == '_') || ft_isalnum(**cnt)))
		(*cnt)++;
	var_len = ((int)(*cnt - var) - 1);
	v_value = search_var((const char *)(var + 1), var_list, var_len);
	var_position = var - (*cont);
	*cont = expand_variable(*cont, var_position, cnt, v_value);
}

char	*handle_possible_var(char **cont, char *cnt,
		t_list *var_list, t_shell *shell)
{
	char	*var;

	if (*(cnt + 1) == '\0')
		return (cnt + 1);
	var = cnt;
	cnt++;
	if (*cnt == '?')
	{
		cnt++;
		handle_special_var(cont, &cnt, var, shell);
		return (cnt);
	}
	if ((*cnt != '_') && !ft_isalpha(*cnt))
		return (cnt);
	handle_normal_var(cont, &cnt, var, var_list);
	return (cnt);
}

void	search_and_expand_string(char **cont, t_list *var_list, t_shell *shell)
{
	char	*cnt;
	bool	dquote;

	cnt = *cont;
	dquote = false;
	while (*cnt != '\0')
	{
		if (*cnt == '$')
			cnt = handle_possible_var(cont, cnt, var_list, shell);
		else if (is_quote(*cnt) && (*cnt == '\'') && (dquote == false))
			cnt = go_to_end_quote(cnt);
		else if (is_quote(*cnt))
		{
			dquote = !dquote;
			cnt++;
		}
		else
			cnt++;
	}
}

void	search_and_expand(t_list *token_list, t_list *var_list, t_shell *shell)
{
	while (token_list != NULL)
	{
		search_and_expand_string((char **)(&(token_list->content)),
			var_list, shell);
		token_list = token_list->next;
	}
}
