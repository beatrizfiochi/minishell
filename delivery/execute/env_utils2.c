/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:47 by djunho            #+#    #+#             */
/*   Updated: 2025/07/13 21:15:09 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"		// ft_strlen()

// Get the size of filled fields of envp
int	get_envp_size(char **envp)
{
	int	i;

	if (envp == NULL)
		return (0);
	i = 0;
	while (*envp != NULL)
	{
		i++;
		envp++;
	}
	return (i);
}

char	**envp_cat(char **envp1, char **envp2)
{
	int		size;
	int		i;
	int		j;
	char	**envp;

	size = get_envp_size(envp1) + get_envp_size(envp2);
	envp = ft_calloc(size + 1, sizeof(char *));
	if (envp == NULL)
	{
		return (NULL);
	}
	i = 0;
	j = 0;
	while (envp1[j] != NULL)
		envp[i++] = envp1[j++];
	j = 0;
	while (envp2[j] != NULL)
		envp[i++] = envp2[j++];
	return (envp);
}
