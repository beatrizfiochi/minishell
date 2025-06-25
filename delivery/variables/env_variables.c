/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variables.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 17:07:52 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/06/25 17:48:24 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "variables.h"
#include "../parser/parser.h"
#include "../cmd.h"
#include "../libft/libft.h"

t_list	*envp_list(t_shell *shell, char *envp[])
{
	char		*sep;
	char		*name;
	char		*value;
	t_list		*new_node;
	int			name_len;

	while (*envp != NULL)
	{
		sep = ft_strchr(*envp, '=');
		name_len = sep - *envp;
		name = ft_substr(*envp, 0, name_len);
		value = ft_strdup(sep + 1);
		if (name == NULL || value == NULL)
		{
			free(name);
			free(value);
			return (NULL);
		}
		if (check_and_replace_var(shell->variable_list, name, value))
		{
			free(name); // valor já foi usado e copiado no replace
			free(value);
			envp++;
			continue;
		}
		new_node = create_var_node(name, value);
		if (!new_node)
		{
			free(name);
			free(value);
			return (NULL);
		}
		ft_lstadd_back(&shell->variable_list, new_node);
		envp++;
	}
	return (shell->variable_list);
}

// t_list	*envp_list(t_shell *shell, char *envp[])
// {
// 	char	*name;
// 	char	*value;
// 	int		i;
// 	int		pos;
// 	t_list	*current;
// 	t_list	*new_node;

// 	if (*envp == NULL)
// 		return (NULL);
// 	name = NULL;
// 	value = NULL;
// 	while (*envp != NULL)
// 	{
// 		i = 0;
// 		pos = -1;
// 		while (*envp[i]!= '\0')
// 		{
// 			while (*envp[i]!= "=" && i != pos)
// 			{
// 				name = *envp[i];
// 				i++;
// 			}
// 			pos = i;
// 			i++;
// 			value = *envp[i];
// 			i++;
// 		}
// 		current = shell->variable_list;
// 		if (check_and_replace_var(current, name, value) == true)
// 			return (0);
// 		new_node = create_var_node(name, value);
// 		if (new_node == NULL)
// 			return (1);
// 		ft_lstadd_back(&shell->variable_list, new_node);
// 		*envp++;
// 	}
// 	return (current);
// }
