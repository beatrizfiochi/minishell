/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djunho <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 13:57:47 by djunho            #+#    #+#             */
/*   Updated: 2025/06/23 13:59:04 by djunho           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"		// ft_strlen()

// Search for the environment variable (must contain the = at the end) in
//  the envp
const char	*get_env(char *env, char **envp)
{
	int	i;

	i = -1;
	while (envp[++i] != NULL)
		if (ft_strncmp(envp[i], env, ft_strlen(env)) == 0)
			break ;
	if (envp[i] == NULL)
		return (NULL);
	return (&envp[i][ft_strlen(env)]);
}
