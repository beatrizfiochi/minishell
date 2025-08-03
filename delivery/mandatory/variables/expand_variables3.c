/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 18:46:29 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/27 20:34:49 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "../minishell.h"
#include "expand_variables.h"

static void	expand_variable_token_i(t_list **curr, t_list *var_list,
	t_shell *shell)
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

void	expand_variable_token(t_list **token_list, t_list *var_list,
	t_shell *shell)
{
	t_list	*curr;

	curr = *token_list;
	while (curr != NULL)
	{
		expand_variable_token_i(&curr, var_list, shell);
		curr = curr->next;
	}
}
