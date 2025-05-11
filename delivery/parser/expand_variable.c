/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 18:15:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/05/11 15:23:26 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>              //bool
#include "../libft/libft.h"
#include "parser.h"

bool	is_metachar(char *content)
{
	if (content == NULL)
		return (false);
	if ((*content == '$' && *(content + 1) != ' ') && *(content + 1) != '\0'
		&& *(content + 1) != '&' && *(content + 1) != '|')
		return (true);
	return (false);
}

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

char	*search_var(const char *variable, t_list *var_list, int len)
{
	const char	*tmp;

	tmp = variable + 1;
	while (var_list != NULL)
	{
		if (ft_strncmp(tmp, (const char *)(((t_content_var *)(var_list->content))->var_name), len) == 0)
			return (((t_content_var *)(var_list->content))->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}

int	search_and_expand(t_list *token_list, t_list *var_list)
{
	char	*content;
	int		len;
	char	*var_value_found;

	while (token_list != NULL)
	{
		content = (char *)(token_list->content);
		len = ft_strlen(content);
		if (is_metachar(content) == true)
		{
			var_value_found = search_var((const char *)(content), var_list, (len - 1));
			if (var_value_found != NULL)
			{
				free(token_list->content);
				token_list->content = ft_strdup(var_value_found);
			}
		}
		token_list = token_list->next;
	}
	return (0);
}
