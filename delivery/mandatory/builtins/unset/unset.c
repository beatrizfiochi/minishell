/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfiochi- <bfiochi-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 14:21:43 by bfiochi-          #+#    #+#             */
/*   Updated: 2025/07/12 15:19:48 by bfiochi-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../../variables/variables.h"
#include "../../parser/parser.h"

int	unset(int argc, char *argv[], t_shell *shell)
{
	t_list			*curr;
	t_list			*next;
	t_content_var	*var_list;
	int				i;

	i = 0;
	while (++i < argc)
	{
		curr = shell->variable_list;
		while (curr != NULL)
		{
			next = curr->next;
			var_list = (t_content_var *)curr->content;
			if ((is_strlen_equals(var_list->var_name, argv[i]) == true)
				&& (ft_strncmp(var_list->var_name, argv[i],
						ft_strlen(argv[i])) == 0))
			{
				free_var_content(var_list);
				ft_lstremove(&shell->variable_list, curr);
				free(curr);
			}
			curr = next;
		}
	}
	return (0);
}
