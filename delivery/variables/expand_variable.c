/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
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

void	expand_variable_token(t_list **curr, t_list *var_list, t_shell *shell)
{
	char	*cnt;
	bool	dquote;

	cnt = (char *)((*curr)->content);
	dquote = false;
	while (*cnt != '\0')
	{
		if ((*cnt == '$') && dquote)
			cnt = handle_possible_var((char **)&((*curr)->content), cnt,
					var_list, shell);
		else if ((*cnt == '$') && !dquote)
			cnt = process_var_expansion(curr, cnt, var_list, shell);
		else if ((*cnt == '\'') && (dquote == false))
			cnt = mark_quotes(cnt);
		else if ((*cnt == '"') || (*cnt == QUOTE_MARK))
		{
			mark_quotes(cnt);
			cnt++;
			dquote = !dquote;
		}
		else
			cnt++;
	}
}

void	search_and_expand(t_list **token_list, t_list *var_list, t_shell *shell)
{
	t_list	*curr;

	curr = *token_list;
	while (curr != NULL)
	{
		expand_variable_token(&curr, var_list, shell);
		curr = curr->next;
	}
	expand_wildcards_token(token_list);
	remove_empty_token(token_list);
	clean_token_quotes(*token_list);
}
