/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/24 16:16:09 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "expand_variables.h"
#include "../minishell.h"

void	free_var_content(void *var_content)
{
	t_content_var	*content;

	if (var_content == NULL)
		return ;
	content = (t_content_var *)var_content;
	free(content->var_name);
	free(content->var_value);
	free(content);
}

static void	handle_special_var(char **cont, char **cnt, char *var, t_shell *shell)
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

static void	handle_normal_var(char **cont,  char **cnt, char *var, t_list *var_list)
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

static void	search_and_expand_content(char **cont, t_list *var_list, t_shell *shell)
{
	char	*var;
	char	*cnt;

	cnt = *cont;
	while (*cnt != '\0' && (cnt[0] != '\'' && cnt[(int)ft_strlen(cnt)] != '\''))
	{
		if (*cnt == '$')
		{
			if (*(cnt + 1) == '\0')
				return ;
			var = cnt;
			cnt++;
			if (*cnt == '?')
			{
				cnt++;
				handle_special_var(cont, &cnt, var, shell);
				continue ;
			}
			if ((*cnt != '_') && !ft_isalpha(*cnt))
				continue ;
			handle_normal_var(cont, &cnt, var, var_list);
		}
		else
			cnt++;
	}
}

void	search_and_expand(t_list *token_list, t_list *var_list, t_shell *shell)
{
	while (token_list != NULL)
	{
		search_and_expand_content((char **)(&(token_list->content)),
			var_list, shell);
		token_list = token_list->next;
	}
}
