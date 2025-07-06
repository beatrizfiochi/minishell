/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:47 by djunho            #+#    #+#             */
/*   Updated: 2025/07/05 15:53:25 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"		// ft_strlen()
#include "../minishell.h"
#include "../parser/parser.h"

// Search for the environment variable at the var_list
//   env doesn't contain the = at the end, we compare with len + 1
//   because we need to ensure that both strings end with '\0'
const char	*get_env(char *env, t_list *var_list)
{
	t_content_var	*envp_content;

	while (var_list != NULL)
	{
		envp_content = (t_content_var *)(var_list)->content;
		if (ft_strncmp((char *)((envp_content)->var_name),
			env, ft_strlen(env) + 1) == 0)
			return (envp_content->var_value);
		var_list = var_list->next;
	}
	return (NULL);
}

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

static void	list_to_envp(t_list *var_list, char **envp, int *i,
						bool ignore_exported)
{
	t_content_var	*var;
	char			*joined;

	while (var_list != NULL)
	{
		var = (t_content_var *)var_list->content;
		if (ignore_exported || var->is_exported)
		{
			joined = ft_strjoin(var->var_name, "=");
			envp[*i] = ft_strjoin_free(joined, var->var_value);
			(*i)++;
		}
		var_list = var_list->next;
	}
}

char	**convert_list_to_envp(t_list *var_list, t_list *tmp_var_list)
{
	int				i;
	char			**envp;
	t_list			*aux;

	if (var_list == NULL)
		return (NULL);
	i = 0;
	aux = var_list;
	while (aux != NULL)
	{
		if (((t_content_var *)(aux->content))->is_exported)
			i++;
		aux = aux->next;
	}
	envp = malloc(sizeof(char *) * (i + ft_lstsize(tmp_var_list) + 1));
	if (envp == NULL)
		return (NULL);
	i = 0;
	list_to_envp(var_list, envp, &i, false);
	list_to_envp(tmp_var_list, envp, &i, true);
	envp[i] = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
