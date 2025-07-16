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

static void	handle_special_var(char **cont, char **cnt,
		char *var, t_shell *shell)
{
	char	*v_value;

	v_value = ft_itoa(shell->last_exit_status);
	if (v_value == NULL)
		return ;
	*cont = expand_variable(*cont, var, cnt, v_value);
	free(v_value);
}

static void	handle_normal_var(char **cont, char **cnt,
		char *var, t_list *var_list)
{
	char	*v_value;
	int		var_len;

	while ((**cnt != '\0') && ((**cnt == '_') || ft_isalnum(**cnt)))
		(*cnt)++;
	var_len = ((int)(*cnt - var) - 1);
	v_value = search_var((const char *)(var + 1), var_list, var_len);
	*cont = expand_variable(*cont, var, cnt, v_value);
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

static t_list	*handle_normal_var_with_retoken(t_list *curr, char **cnt,
		char *var, t_list *var_list)
{
	char	*v_value;
	int		var_len;
	char	**split;
	char	*aux;
	t_list	*next;
	int	i;

	while ((**cnt != '\0') && ((**cnt == '_') || ft_isalnum(**cnt)))
		(*cnt)++;
	var_len = ((int)(*cnt - var) - 1);
	v_value = search_var((const char *)(var + 1), var_list, var_len);
	if (v_value == NULL)
	{
		curr->content = expand_variable((char *)(curr->content), var, cnt, NULL);
		return (curr);
	}
	split = ft_split(v_value, ' ');
	i = 0;
	if (v_value[0] != ' ')
	{
		// The first item of the var expansion should be tokenized with the previous token
		curr->content = expand_variable((char *)(curr->content), var, cnt, split[0]);
		free(split[0]);
		i = 1;
	}
	next = curr->next;
	while (split[i] != NULL)
	{
		curr->next = ft_lstnew(split[i]);
		curr = curr->next;
		i++;
	}
	curr->next = next;
	if (i > 1)
	{
		i--;
		aux = ft_strjoin(split[i], *cnt);
		**cnt = '\0';
		*cnt = aux + ft_strlen(split[i]);
		free(curr->content);
		curr->content = aux;
	}
	free(split);
	return (curr);
}

char	*process_var_expansion(t_list **curr, char *cnt, t_list *var_list, t_shell *shell)
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
	char	*aux;
	bool	dquote;

	cnt = (char *)((*curr)->content);
	dquote = false;
	while (*cnt != '\0')
	{
		if ((*cnt == '$') && dquote)
			cnt = handle_possible_var((char **)&((*curr)->content), cnt, var_list, shell);
		else if ((*cnt == '$') && !dquote)
			cnt = process_var_expansion(curr, cnt, var_list, shell);
		else if ((*cnt == '\'') && (dquote == false))
		{
			aux = go_to_end_quote(cnt);
			ft_memmove(aux - 1, aux, ft_strlen(aux) + 1);
			ft_memmove(cnt, cnt + 1, ft_strlen(cnt + 1) + 1);
			cnt = aux - 2;
		}
		else if (is_quote(*cnt) && (*cnt == '"'))
		{
			ft_memmove(cnt, cnt + 1, ft_strlen(cnt + 1) + 1);
			dquote = !dquote;
		}
		else
			cnt++;
	}
}

void	search_and_expand(t_list *token_list, t_list *var_list, t_shell *shell)
{
	t_list	*curr;

	curr = token_list;
	while (curr != NULL)
	{
		expand_variable_token(&curr, var_list, shell);
		curr = curr->next;
	}
	expand_wildcards_token(token_list);
}
