/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 14:25:49 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/22 11:58:14 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "parser.h"

char	*search_var(const char *variable, t_list *var_list, int len)
{
	t_content_var	*content;
	int				var_name_len;

	while (var_list != NULL)
	{
		content = (t_content_var *)var_list->content;
		var_name_len = (int)ft_strlen(content->var_name);
		if (var_name_len == len
			&& ft_strncmp(variable, content->var_name, len) == 0)
			return (content->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}

char	*remove_var_name(char *content, int var_pos, char **cnt)
{
	int		len;
	char	*new_content;
	int		var_len;

	var_len = *cnt - (content + var_pos) - 1;
	len = ft_strlen(content) - (var_len + 1) + 1;
	new_content = malloc(len);
	if (new_content == NULL)
		return (NULL);
	ft_strlcpy(new_content, content, var_pos + 1);
	ft_strlcat(new_content, *cnt, len);
	*cnt = new_content + var_pos;
	return (new_content);
}

char	*replace_var_name_by_value(char *content, int var_pos,
	char **cnt, char *var_value)
{
	int		len;
	int		var_len;
	char	*new_content;

	var_len = *cnt - (content + var_pos) - 1;
	len = ft_strlen(content) - (var_len + 1) + (ft_strlen(var_value) + 1);
	new_content = malloc(sizeof(char) * len);
	if (new_content == NULL)
		return (NULL);
	ft_strlcpy(new_content, content, var_pos + 1);
	ft_strlcat(new_content, var_value, len);
	ft_strlcat(new_content, *cnt, len);
	*cnt = new_content + var_pos + ft_strlen(var_value);
	return (new_content);
}

char	*expand_variable(char *content, int var_pos,
	char **cnt, char *var_value)
{
	char	*new_content;

	if (var_value != NULL)
		new_content = replace_var_name_by_value(content, var_pos,
				cnt, var_value);
	else
		new_content = remove_var_name(content, var_pos, cnt);
	free(content);
	return (new_content);
}

void	search_and_expand_content(char **content, t_list *var_list)
{
	char	*var;
	char	*var_value;
	int		var_len;
	char	*cnt;

	cnt = *content;
	while (*cnt != '\0' && (cnt[0] != '\'' && cnt[(int)ft_strlen(cnt)] != '\''))
	{
		if (*cnt == '$')
		{
			var = cnt;
			cnt++;
			while ((*cnt != '\0') && ((*cnt == '_') || ft_isalnum(*cnt)))
				cnt++;
			var_len = ((int)(cnt - var)) - 1;
			var_value = search_var((const char *)(var + 1), var_list, var_len);
			*content = expand_variable(*content, (var - *content), &cnt,
					var_value);
		}
		else
			cnt++;
	}
}
