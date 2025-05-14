/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/14 11:08:51 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>              //bool
#include "../libft/libft.h"
#include "parser.h"

t_list	*create_var_node(char *name, char *value)
{
	char			*var_name;
	char			*var_value;
	t_content_var	*content;

	var_name = ft_strdup(name);
	var_value = ft_strdup(value);
	if (var_name == NULL)
		return (NULL);
	if (var_value == NULL)
	{
		free(var_name);
		return (NULL);
	}
	content = malloc(sizeof(t_content_var));
	if (content == NULL)
	{
		free(var_name);
		free(var_value);
		return (NULL);
	}
	content->var_name = var_name;
	content->var_value = var_value;
	return (ft_lstnew(content));
}

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

static char *search_var(const char *variable, t_list *var_list, int len)
{
	t_content_var	*content;
	int				var_name_len;

	while (var_list != NULL)
	{
		content = (t_content_var *)var_list->content;
		var_name_len = (int)ft_strlen(content->var_name);
		if (var_name_len == len && ft_strncmp(variable, content->var_name, len) == 0)
			return (content->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}


static void	expand_variable(t_list *token_list, char *var, char **content, char *var_value_found, int var_len)
{
	char	*new_content;
	int		len;
	
	if (var_value_found != NULL)
	{
		len = ft_strlen(token_list->content) - (var_len + 1) + ((int)ft_strlen(var_value_found) + 1);
		new_content = malloc(sizeof(int) * len);
		if (new_content == NULL)
			return ;
		ft_strlcpy(new_content, (char *)(token_list->content), (var - (char *)(token_list->content)) + 1);
		ft_strlcat(new_content, var_value_found, len);
		ft_strlcat(new_content, *content, len);
		*content = new_content + (var - (char *)(token_list->content)) + (int)ft_strlen(var_value_found);
	}
	else
	{
		len = ft_strlen(token_list->content) - (var_len + 1) + 1;
		new_content = malloc(len);
		if (new_content == NULL)
			return ;
		ft_strlcpy(new_content, (char *)(token_list->content), (var - (char *)(token_list->content)) + 1);
		ft_strlcat(new_content, *content, len);
		*content = new_content + (var - (char *)(token_list->content));
	}
	free(token_list->content);
	token_list->content = new_content;
}

void	search_and_expand(t_list *token_list, t_list *var_list)
{
	char	*cnt;
	char	*var_value_found;
	char	*var;
	int		var_len;

	var_value_found = NULL;
	while (token_list != NULL)
	{
		cnt = (char *)(token_list->content);
		while (*cnt != '\0' && (cnt[0] != '\'' && cnt[(int)ft_strlen(cnt)] != '\''))
		{
			if (*cnt == '$')
			{
				var_len = 0;
				var = cnt;
				cnt++;
				while (*cnt != '\0' && ((*cnt >= 'a' && *cnt <= 'z') || (*cnt >= 'A' && *cnt <= 'Z') || (*cnt >= '0' && *cnt <= '9')))
				{
					cnt++;
					var_len++;
				}
				var_value_found = search_var((const char*)(var + 1), var_list, var_len);
				expand_variable(token_list, var, &cnt, var_value_found, var_len);
			}
			else
				cnt++;
		}
		token_list = token_list->next;
	}
}
