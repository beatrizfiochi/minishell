/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/27 18:46:44 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_variables.h"
#include "../minishell.h"
#include "../parser/aux.h"
#include "../parser/parser.h"
#include <unistd.h>

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

void	expand_variable_string(char **cont, t_list *var_list, t_shell *shell)
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
		else if (is_quote(*cnt) && (*cnt == '"'))
		{
			dquote = !dquote;
			cnt++;
		}
		else
			cnt++;
	}
}

char	*process_var_expansion(t_list **curr, char *cnt, t_list *var_list,
							t_shell *shell)
{
	char	*var;

	if (*(cnt + 1) == '\0')
		return (cnt + 1);
	var = cnt;
	cnt++;
	if (*cnt == '?')
	{
		cnt++;
		handle_special_var((char **)&((*curr)->content), &cnt, var, shell);
		return (cnt);
	}
	if ((*cnt != '_') && !ft_isalpha(*cnt))
		return (cnt);
	*curr = handle_normal_var_with_retoken(*curr, &cnt, var, var_list);
	return (cnt);
}

void	search_and_expand(t_list **token_list, t_list *var_list, t_shell *shell)
{
	expand_variable_token(token_list, var_list, shell);
	expand_wildcards_token(token_list);
	remove_empty_token(token_list);
	clean_token_quotes(*token_list);
}
