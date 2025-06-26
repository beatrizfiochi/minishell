/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:47 by djunho            #+#    #+#             */
/*   Updated: 2025/06/26 17:11:41 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"		// ft_strlen()
#include "../minishell.h"
#include "../parser/parser.h"

// Search for the environment variable (must contain the = at the end) in
//  the envp
// const char	*get_env(char *env, char **envp)
// {
// 	int	i;

// 	i = -1;
// 	while (envp[++i] != NULL)
// 		if (ft_strncmp(envp[i], env, ft_strlen(env)) == 0)
// 			break ;
// 	if (envp[i] == NULL)
// 		return (NULL);
// 	return (&envp[i][ft_strlen(env)]);
// }

// Search for the environment variable at the var_list
//  env contains the = at the end so we compare the len - 1
const char	*get_env(char *env, t_list *var_list)
{
	t_content_var	*envp_content;

	while (var_list != NULL)
	{
		envp_content = (t_content_var *)(var_list)->content;
		if (ft_strncmp((char *)((envp_content)->var_name), env, ft_strlen(env) - 1) == 0)
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

char	**convert_list_to_envp(t_list *var_list)
{
	int				i;
	char			**envp;
	char			*joined;
	t_list			*tmp;
	t_content_var	*var;

	if (var_list == NULL)
		return (NULL);
	envp = malloc(sizeof(char *) * (ft_lstsize(var_list) + 1));
	if (envp == NULL)
		return (NULL);
	i = 0;
	tmp = var_list;
	while (tmp != NULL)
	{
		var = (t_content_var *)tmp->content;
		joined = ft_strjoin(var->var_name, "=");
		envp[i++] = ft_strjoin_free(joined, var->var_value);
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

